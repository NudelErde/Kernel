#include "elf.hpp"
#include "FileSystem.hpp"
#include "IDE.hpp"
#include "KernelOut.hpp"
#include "Partition.hpp"
#include "Scheduler.hpp"
#include "kernelMem.hpp"
#include "memory.hpp"
#include "process.hpp"
#include "units.hpp"

namespace Kernel {

struct FileIterator {
    EXT4& ext;
    uint64_t inodeNum;
    EXT4::INode inode;
    uint64_t size;
    uint64_t current;
    bool needSectorLoad;
    uint8_t sector[512];

    FileIterator(EXT4& ext, uint64_t inodeNum) : ext(ext), inodeNum(inodeNum) {
        inode = ext.getINode(inodeNum);
        size = ext.getFileSize(inode);
        current = 0;
        needSectorLoad = 0;
    }

    bool hasData() {
        return current < size;
    }

    uint8_t get() {
        if (current >= size)
            return 0;
        if (current % 512 == 0) {
            needSectorLoad = true;
        }
        if (needSectorLoad) {
            ext.getSectorInFile(inode, sector, current / 512);
            needSectorLoad = false;
        }
        return sector[(current++) % 512];// return value and increment data pointer
    }

    uint64_t currentAddress() {
        return current;
    }

    void setNextAddress(uint64_t nextCurrent) {
        current = nextCurrent;
        needSectorLoad = true;
    }
};

struct ELFHeader {
    uint8_t magicNumber[4];             // 0x7F | 'E' | 'L' | 'F'
    uint8_t bitMode;                    // 2 = 64bit
    uint8_t endianness;                 // 1 = little endian
    uint8_t headerVersion;              //
    uint8_t padding0[9];                //
    uint16_t type;                      // 2 = executeable
    uint16_t instructionSet;            // should be 0x3E for x86_&4
    uint32_t elfVersion;                //
    uint64_t entryPoint;                //
    uint64_t programTableHeaderPosition;//
    uint64_t sectionTableHeaderPosition;//
    uint32_t flags;                     //
    uint16_t headerSize;                //
    uint16_t programTableEntrySize;     //
    uint16_t programTableEntryCount;    //
    uint16_t sectorTableEntrySize;      //
    uint16_t sectorTableEntryCount;     //
    uint16_t sectorWithSectorNames;     //
} __attribute__((packed));

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
} __attribute__((packed));

static_assert(sizeof(ProgramEntry) == 56);

constexpr uint64_t tempMap = 0Ti;

uint64_t loadAndExecute(EXT4& ext, const char* str, const char* arguments, uint64_t parentPid, bool loadWithDebug) {
    uint64_t inodeNum = ext.findFileINode(str);
    if (!inodeNum) {
        return false;
    }

    FileIterator fi(ext, inodeNum);
    ELFHeader header;
    for (uint64_t i = 0; i < sizeof(header); ++i) {
        ((uint8_t*) &header)[i] = fi.get();
    }
    if (header.magicNumber[0] != 0x7F || header.magicNumber[1] != 'E' || header.magicNumber[2] != 'L' || header.magicNumber[3] != 'F') {
        return false;// not an elf file
    }

    if (header.bitMode != 2 || header.endianness != 1 || header.type != 2 || header.instructionSet != 0x3E) {
        return false;// invalid elf file
    }

    // switch to kernel environment
    getKernelMemoryManager().reload();

    fi.setNextAddress(header.programTableHeaderPosition);
    MemoryPage* programPages;
    uint64_t count = 0;
    for (decltype(header.programTableEntryCount) i = 0; i < header.programTableEntryCount; ++i) {
        ProgramEntry entry;
        for (uint64_t j = 0; j < sizeof(entry); ++j) {
            ((uint8_t*) &entry)[j] = fi.get();
        }
        if (entry.type != 1) {
            continue;
        }

        if (entry.memoryOffset <= 1Gi) {
            kout << "Invalid memory used\n";
            return false;// low memory is used by kernel
        }

        count += (entry.memorySize / pageSize) + 1;
    }
    programPages = new MemoryPage[count];
    uint8_t* programPagesPermissions = new uint8_t[count];
    memset(programPagesPermissions, 0, count);

    uint64_t pagesIndex = 0;
    uint64_t highestAddress = 0;

    uint64_t programTableAddress = header.programTableHeaderPosition;
    for (decltype(header.programTableEntryCount) i = 0; i < header.programTableEntryCount; ++i) {
        fi.setNextAddress(programTableAddress);
        ProgramEntry entry;
        for (uint64_t j = 0; j < sizeof(entry); ++j) {
            ((uint8_t*) &entry)[j] = fi.get();
        }
        programTableAddress = fi.currentAddress();
        if (entry.type != 1) {
            continue;
        }

        if (entry.memoryOffset <= 1Gi) {
            kout << "Invalid memory used\n";
            return false;// low memory is used by kernel
        }

        uint64_t currentTargetMemoryAddress = 0;
        if (entry.memoryOffset % pageSize) {// not page aligned
            bool pageAlreadyExists = false;

            for (uint64_t iter = 0; iter < pagesIndex; ++iter) {
                uint64_t start = programPages[iter].getVirtualAddress() - tempMap;
                if (entry.memoryOffset > start && entry.memoryOffset <= start + pageSize) {
                    pageAlreadyExists = true;
                    break;
                }
            }

            if (!pageAlreadyExists) {
                //create new page
                uint64_t base = entry.memoryOffset & ~(pageSize - 1);
                new (programPages + pagesIndex) MemoryPage(PhysicalMemoryManagment::getFreeUserPage());// in place construct
                programPagesPermissions[pagesIndex] |= entry.flags;
                programPages[pagesIndex++].mapTo(base + tempMap, true, true);// offset by 1TiB in virtual memory to avoid collision with stack
            }
            currentTargetMemoryAddress = pageSize - entry.memoryOffset % pageSize;
        }
        for (; currentTargetMemoryAddress < entry.memorySize; currentTargetMemoryAddress += pageSize) {
            new (programPages + pagesIndex) MemoryPage(PhysicalMemoryManagment::getFreeUserPage());// in place construct
            programPagesPermissions[pagesIndex] |= entry.flags;
            programPages[pagesIndex++].mapTo(currentTargetMemoryAddress + entry.memoryOffset + tempMap, true, true);// offset by [tempMap] in virtual memory to avoid collision with stack
        }

        if (highestAddress < currentTargetMemoryAddress + entry.memoryOffset) {
            highestAddress = currentTargetMemoryAddress + entry.memoryOffset;
        }

        currentTargetMemoryAddress = entry.memoryOffset + tempMap;
        fi.setNextAddress(entry.fileOffset);
        for (uint64_t l = 0; l < entry.fileSize; ++l, ++currentTargetMemoryAddress) {
            uint8_t data = fi.get();
            *((volatile uint8_t*) currentTargetMemoryAddress) = data;// load data from file
        }
        if (entry.fileSize < entry.memorySize) {
            memset((uint8_t*) currentTargetMemoryAddress, 0, entry.memorySize - entry.fileSize);// fill the rest with 0
        }
    }
    for (uint64_t iter = 0; iter < pagesIndex; ++iter) {
        uint8_t flag = programPagesPermissions[iter];
        bool pageExecuteable = flag & (0b1 << 0);
        bool pageWriteable = flag & (0b1 << 1);
        bool pageReadable = flag & (0b1 << 2);
        programPages[iter].unmap();
        programPages[iter].softmap(programPages[iter].getVirtualAddress() - tempMap, pageWriteable, true);// todo: set permission
    }
    delete[] programPagesPermissions;

    // build target stack

    uint64_t stackBase = 1Ti - ((stackPageCount + 2) * pageSize);
    MemoryPage stack[stackPageCount];
    for (uint8_t i = 0; i < stackPageCount; ++i) {
        uint64_t vstack = stackBase + (i * pageSize);
        new (stack + i) MemoryPage(PhysicalMemoryManagment::getFreeUserPage());
        stack[i].softmap(vstack, true, true);
    }

    // build target heap
    uint64_t heapSize = 1Gi;
    uint64_t heapBase = highestAddress + pageSize;
    MemoryManager targetHeap(false, heapBase, heapSize / pageSize);

    // switch to source environment
    if (Thread::isInProgram()) {
        Process::getLastLoadedProcess()->reload();
        Thread::getCurrent()->reload();
    } else {
        getKernelMemoryManager().reload();
    }
    uint64_t argsSize = 0;
    for (const char* args = arguments; 1; ++args) {
        ++argsSize;
        if (*args == 0 && *(args + 1) == 0) {
            break;
        }
    }
    ++argsSize;

    // switch to target environment
    Process proc(programPages, count, (MemoryManager &&) targetHeap, heapBase + 2Gi /*shared pages after heap*/, parentPid);
    Thread thread(stack, header.entryPoint, proc.getPID(), loadWithDebug);
    char* argumentOnHeap = (char*) proc.getHeap().malloc(argsSize);

    constexpr uint64_t copyBufferSize = 128;
    char buffer[copyBufferSize];
    uint64_t copyIndex = 0;

    while (argsSize) {// copy from source to process memory
        //load source env
        if (Thread::isInProgram()) {
            Process::getLastLoadedProcess()->reload();
            Thread::getCurrent()->reload();
        } else {
            getKernelMemoryManager().reload();
        }
        uint64_t copyCount = argsSize;
        if (copyCount > copyBufferSize) {
            copyCount = copyBufferSize;
        }
        argsSize -= copyCount;
        memcpy(buffer, arguments + copyIndex, copyCount);

        proc.getHeap().reload();
        memcpy(argumentOnHeap + copyIndex, buffer, copyCount);

        copyIndex += copyCount;// increment buffer index
    }
    if (Thread::isInProgram()) {
        Process::getLastLoadedProcess()->reload();
        Thread::getCurrent()->reload();
    } else {
        getKernelMemoryManager().reload();
    }
    proc.setArgumentPointer(argumentOnHeap);

    uint64_t pid = proc.getPID();

    Scheduler::addThread((Thread &&) thread);
    Scheduler::addProcess((Process &&) proc);

    return pid;
}

}// namespace Kernel