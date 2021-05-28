#include "elf.hpp"
#include "IDE.hpp"
#include "memory.hpp"
#include "kernelMem.hpp"
#include "FileSystem.hpp"
#include "Partition.hpp"
#include "units.hpp"
#include "KernelOut.hpp"
#include "process.hpp"
#include "Scheduler.hpp"

namespace Kernel {

struct FileIterator{
    EXT4& ext;
    uint64_t inodeNum;
    EXT4::INode inode;
    uint64_t size;
    uint64_t current;
    bool needSectorLoad;
    uint8_t sector[512];

    FileIterator(EXT4& ext, uint64_t inodeNum): ext(ext), inodeNum(inodeNum) {
        inode = ext.getINode(inodeNum);
        size = ext.getFileSize(inode);
        current = 0;
        needSectorLoad = 0;
    }

    bool hasData() {
        return current < size;
    }

    uint8_t get() {
        if(current >= size)
            return 0;
        if(current % 512 == 0) {
            needSectorLoad = true;
        }
        if(needSectorLoad) {
            ext.getSectorInFile(inode, sector, current / 512);
            needSectorLoad = false;
        }
        return sector[(current++) % 512]; // return value and increment data pointer
    }

    uint64_t currentAddress() {
        return current;
    }

    void setNextAddress(uint64_t nextCurrent) {
        current = nextCurrent;
        needSectorLoad = true;
    }
};

struct ELFHeader{
    uint8_t magicNumber[4];              // 0x7F | 'E' | 'L' | 'F'
    uint8_t bitMode;                     // 2 = 64bit
    uint8_t endianness;                  // 1 = little endian
    uint8_t headerVersion;               //
    uint8_t padding0[9];                 //
    uint16_t type;                       // 2 = executeable
    uint16_t instructionSet;             // should be 0x3E for x86_&4
    uint32_t elfVersion;                 //
    uint64_t entryPoint;                 //
    uint64_t programTableHeaderPosition; //
    uint64_t sectionTableHeaderPosition; //
    uint32_t flags;                      //
    uint16_t headerSize;                 //
    uint16_t programTableEntrySize;      //
    uint16_t programTableEntryCount;     //
    uint16_t sectorTableEntrySize;       //
    uint16_t sectorTableEntryCount;      //
    uint16_t sectorWithSectorNames;      //
}__attribute__((packed));

static_assert(sizeof(ELFHeader) == 64);

struct ProgramEntry {
    uint32_t type;
    uint32_t flags;
    uint64_t fileOffset;
    uint64_t memoryOffset;
    uint64_t padding0;
    uint64_t fileSize;
    uint64_t memorySize;
    uint64_t alignment;
}__attribute__((packed));

static_assert(sizeof(ProgramEntry) == 56);

uint64_t loadAndExecute(EXT4& ext, const char* str, const char* arguments, uint64_t parentPid, bool loadWithDebug) {
    uint64_t inodeNum = ext.findFileINode(str);
    if(!inodeNum) {
        return false;
    }

    FileIterator fi(ext, inodeNum);
    ELFHeader header;
    for(uint64_t i = 0; i < sizeof(header); ++i) {
        ((uint8_t*)&header)[i] = fi.get();
    }
    if(header.magicNumber[0] != 0x7F || header.magicNumber[1] != 'E' || header.magicNumber[2] != 'L' || header.magicNumber[3] != 'F')
        return false; // not an elf file
    
    if(header.bitMode != 2 || header.endianness != 1 || header.type != 2 || header.instructionSet != 0x3E)
        return false; // invalid elf file

    fi.setNextAddress(header.programTableHeaderPosition);
    MemoryPage* programPages;
    uint64_t count = 0;
    for(decltype(header.programTableEntryCount) i = 0; i < header.programTableEntryCount; ++i) {
        ProgramEntry entry;
        for(uint64_t j = 0; j < sizeof(entry); ++j) {
            ((uint8_t*)&entry)[j] = fi.get();
        }
        if(entry.type != 1) {
            continue;
        }

        if(entry.memoryOffset <= 1Gi) {
            return false; // low memory is used by kernel
        }

        count += (entry.memorySize / pageSize) + 1;
    }
    programPages = new MemoryPage[count];
    uint8_t* programPagesPermissions = new uint8_t[count];
    memset(programPagesPermissions, 0, count);

    uint64_t pagesIndex = 0;
    uint64_t highestAddress = 0;

    uint64_t programTableAddress = header.programTableHeaderPosition;
    for(decltype(header.programTableEntryCount) i = 0; i < header.programTableEntryCount; ++i) {
        fi.setNextAddress(programTableAddress);
        ProgramEntry entry;
        for(uint64_t j = 0; j < sizeof(entry); ++j) {
            ((uint8_t*)&entry)[j] = fi.get();
        }
        programTableAddress = fi.currentAddress();
        if(entry.type != 1) {
            continue;
        }

        if(entry.memoryOffset <= 1Gi) {
            return false; // low memory is used by kernel
        }

        uint64_t k = 0;
        if(entry.memoryOffset % pageSize) { // not page aligned
            bool pageAlreadyExists = false;

            for(uint64_t iter = 0; iter < pagesIndex; ++iter) {
                uint64_t start = programPages[iter].getVirtualAddress()-1024Gi;
                if(entry.memoryOffset > start && entry.memoryOffset <= start+pageSize) {
                    pageAlreadyExists = true;
                    break;
                }
            }

            if(!pageAlreadyExists) {
                //create new page
                uint64_t base = entry.memoryOffset & ~(pageSize - 1);
                new(programPages + pagesIndex)MemoryPage(PhysicalMemoryManagment::getFreeUserPage()); // in place construct
                programPagesPermissions[pagesIndex] |= entry.flags;
                programPages[pagesIndex++].mapTo(base + 1024Gi, true, true); // offset by 1TiB in virtual memory to avoid collision with stack
            }
            k = pageSize - entry.memoryOffset % pageSize;
        }
        for(; k < entry.memorySize; k += pageSize) {
            new(programPages + pagesIndex)MemoryPage(PhysicalMemoryManagment::getFreeUserPage()); // in place construct
            programPagesPermissions[pagesIndex] |= entry.flags;
            programPages[pagesIndex++].mapTo(k + entry.memoryOffset + 1024Gi, true, true); // offset by 1TiB in virtual memory to avoid collision with stack
        }
        
        if(highestAddress < k + entry.memoryOffset) {
            highestAddress = k + entry.memoryOffset;
        }
        
        k = entry.memoryOffset + 1024Gi;
        fi.setNextAddress(entry.fileOffset);
        for(uint64_t l = 0; l < entry.fileSize; ++l, ++k) {
            uint8_t data = fi.get();
            *((uint8_t*)k) = data; // load data from file
        }
        if(entry.fileSize < entry.memorySize) {
            memset((uint8_t*)k, 0, entry.memorySize - entry.fileSize); // fill the rest with 0
        }
    }
    for(uint64_t iter = 0; iter < pagesIndex; ++iter) {
        uint8_t flag = programPagesPermissions[pagesIndex];
        bool pageExecuteable = flag & 0x1;
        bool pageWriteable = flag & 0x2;
        bool pageReadable = flag & 0x4;
        programPages[iter].unmap();
        programPages[iter].softmap(programPages[iter].getVirtualAddress()-1024Gi, pageWriteable, true); // todo: set permission
    }
    delete[] programPagesPermissions;
    MemoryPage stack[8];
    for(uint8_t i = 0; i < 8; ++i) {
        new(stack+i) MemoryPage(PhysicalMemoryManagment::getFreeUserPage());
        stack[i].softmap(highestAddress + ((1 + i) * pageSize), true, true);
    }

    if(Thread::isInProgram()) {
        Process::getLastLoadedProcess()->reload();
        Thread::getCurrent()->reload();
    } else {
        getKernelMemoryManager().reload();
    }
    uint64_t argsSize = 0;
    for(const char* args = arguments; 1; ++args) {
        ++argsSize;
        if(*args == 0 && *(args + 1) == 0) {
            break;
        }
    }
    ++argsSize;

    Process proc(programPages, count, MemoryManager(false, highestAddress + ((2 + 8/*stack*/) * pageSize), 1Gi / pageSize), highestAddress + (2 + 8 + 1) * pageSize + 1Gi /*shared pages after heap*/, parentPid);
    Thread thread(stack, header.entryPoint, proc.getPID(), loadWithDebug);
    char* argumentOnHeap = (char*)proc.getHeap().malloc(argsSize);

    char buffer[128];
    uint64_t copyIndex = 0;
    
    while(argsSize) { // copy from source to process memory
        if(Thread::isInProgram()) {
            Process::getLastLoadedProcess()->reload();
            Thread::getCurrent()->reload();
        } else {
            getKernelMemoryManager().reload();
        }
        uint64_t copyCount = argsSize;
        if(copyCount > 128) {
            copyCount = 128;
        }
        argsSize -= copyCount;
        memcpy(buffer, arguments + copyIndex, copyCount);

        proc.getHeap().reload();
        memcpy(argumentOnHeap + copyIndex, buffer, copyCount);

        copyIndex += copyCount; // increment buffer index
    }
    proc.setArgumentPointer(argumentOnHeap);
    Scheduler::addThread((Thread&&) thread);
    Scheduler::addProcess((Process&&) proc);

    return proc.getPID();
}

}