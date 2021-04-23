#include "process.hpp"
#include "interrupt.hpp"
#include "Scheduler.hpp"
#include "KernelOut.hpp"
#include "elf.hpp"
#include "IDE.hpp"
#include "FileSystem.hpp"
#include "SharedMemory.hpp"

namespace Kernel {

    extern void __systemHandler() asm("__systemHandler");
    extern uint8_t __syscallData[] asm("__syscallData");

    static Thread* current;
    static Process* lastProcess;

    void onProcessSystemCall(uint64_t args[4]) {
        switch(args[1]) { // select process command
            case 1: // return and remove from scheduler
                lastProcess->exit(args[2]);
                Scheduler::removeCurrentThread();
                Thread::toKernel();
                return;
            case 2:
                *((uint64_t*)args[2]) = lastProcess->getPID();
                return;
            case 3: {
                    struct SpawnRequest{
                        uint64_t deviceID;
                        const char* path;
                        const char* argumentsArray;
                        uint64_t pid;
                    };
                    SpawnRequest* req = (SpawnRequest*)args[2];
                    const char* processPath = req->path;
                    ATA::Device* device = ATA::getDevice(req->deviceID);
                    EXT4 ext(*device, 0);
                    uint64_t pid = loadAndExecute(ext, processPath, req->argumentsArray);
                    lastProcess->reload();
                    current->reload();
                    req->pid = pid;
                }
                return;
            case 4: 
                current->setWaiting(args[2]); // wait for args micro seconds
                Thread::toKernel();
                return;
            case 5:
                *((uint64_t*)args[3]) = (uint64_t)(new uint8_t[args[2]]);
                return;
            case 6:
                delete (uint8_t*)args[2];
                return;
            case 7: // create shared memory page
                *((uint64_t*)args[2]) = lastProcess->createSharedMemoryPage();
                return;
            case 8: // invite process to memory page
                lastProcess->addProcessToSharedMemoryPage(args[2], args[3]);
                return;
            case 9: // get pointer to shared memory page by id
                *((uint64_t*)args[3]) = lastProcess->getPointerToSharedPage(args[2]);
                return;
            case 10: // free shared memory page
                lastProcess->unuseSharedMemoryPage(args[2]);
                return;
            case 11: // get method id in other proccess
                return;
            case 12: // call process method by id an pid
                return;
            case 13: // register inter process method
                return;
            case 14: // remove inter process method
                return;
            case 15:
                current->waitForPID(args[2]);
                Thread::toKernel();
                *((uint64_t*)args[3]) = current->waitForPIDResult();
                return;
            case 16:
                *((uint64_t*)args[2]) = (uint64_t) lastProcess->getArgumentPointer();
                return;
            default:
                return;
        }
    }

    void onATASystemCall(uint64_t args[4]) {
        switch (args[1]) {
        case 1:
            *((uint64_t*)args[2]) = ATA::getDeviceCount();
            return;
        case 2:
            *((uint64_t*)args[2]) = ATA::getSystemDevice();
            return;
        default:
            return;
        }
    }

    void onBasicIOCall(uint64_t args[4]) {
        switch (args[1]) {
        case 1: {
            Serial serial(0x3F8);
            while(!serial.outputBufferEmpty()) { // todo resource and interrupt stuff
                current->setWaiting(100);
                Thread::toKernel();
            }
            serial.write(args[2]);
        }
            return;
        case 2: {
            Serial serial(0x3F8);
            while(serial.inputBufferEmpty()) { // todo resource and interrupt stuff
                current->setWaiting(100);
                Thread::toKernel();
            }
            *((uint8_t*)args[2]) = serial.read();
        }
            return;
        default:
            return;
        }
    }

    void onEXT4Syscall(uint64_t args[4]) {
        ATA::Device* device = ATA::getDevice(args[2]);
        EXT4 ext(*device, 0);
        switch (args[1]) {
        case 1: {
            struct InodeOfPathRequest{
                const char* path;
                uint64_t inode;
            };
            InodeOfPathRequest* req = (InodeOfPathRequest*)args[3];
            req->inode = ext.findFileINode(req->path);
        }
            return;
        case 2: {
            struct SectorOfInodeRequest{
                uint64_t inode;
                uint64_t sector;
                uint8_t* buffer;
                bool success;
            };
            SectorOfInodeRequest* req = (SectorOfInodeRequest*)args[3];
            auto inodeObj = ext.getINode(req->inode);
            req->success = ext.getSectorInFile(inodeObj, req->buffer, req->sector);
        }
            return;
        case 3: {
            struct FlagsOfInodeRequest {
                uint64_t inode;
                uint64_t flags;
            };
            FlagsOfInodeRequest* req = (FlagsOfInodeRequest*)args[3];
            auto inodeObj = ext.getINode(req->inode);
            req->flags = inodeObj.i_mode;
        }
            return;
        case 4: 
        {
            struct DirectoryEntriesRequest {
                struct DirectoryEntry {
                    uint64_t inode;
                    uint64_t type;
                    char* name;
                };
                
                uint64_t inode;
                uint64_t maxEntries;
                DirectoryEntry* entries;
                uint64_t count;
            };
            DirectoryEntriesRequest* req = (DirectoryEntriesRequest*)args[3];
            req->count = 0;
            auto inodeObj = ext.getINode(req->inode);
            if(ext.getFileType(inodeObj) != EXT4::FileType::DIRECTORY)
                return;
            ext.iterateDirectory(inodeObj, [](void* _r, const EXT4::DirEntry& entry) -> bool{
                DirectoryEntriesRequest* request = (DirectoryEntriesRequest*) _r;
                uint64_t index = request->count;
                if(index < request->maxEntries) {
                    request->entries[index].inode = entry.inode;
                    request->entries[index].type = entry.fileType;
                    request->entries[index].name = new char[entry.nameSize + 1];
                    request->entries[index].name[entry.nameSize] = 0;
                    memcpy(request->entries[index].name, entry.name, entry.nameSize);
                }
                ++request->count;
                return false;
            }, (void*)req);
        }
            return;
        default:
            return;
        }
    }

    extern "C" __attribute__((interrupt)) 
    void onSystemCall(void* stackFrame) {
        uint64_t* data = (uint64_t*)__syscallData; // size = 4
        switch(data[0]) {
            case 1:
                onProcessSystemCall(data);
                break;
            case 2:
                onATASystemCall(data);
                break;
            case 3:
                onBasicIOCall(data);
                break;
            case 4:
                onEXT4Syscall(data);
                break;
            default:
                break;
        }
    }

    static uint64_t nextPID{};

    Process::Process(MemoryPage *programPages, uint64_t count, MemoryManager &&heap, uint64_t sharedPagesLocation) : programPages(programPages), heap((MemoryManager &&) heap) {
        Process::sharedPagesLocation = sharedPagesLocation;
        valid = true;
        Process::count = count;
        pid = ++nextPID;
        for(auto& s : sharedPages) {
            s.sharedPageId = 0;
        }
    }

    Process::Process(Process&& other) noexcept : heap((MemoryManager&&) other.heap) {
        programPages = other.programPages;
        finalReturnValue = other.finalReturnValue;
        finished = other.finished;
        valid = other.valid;
        count = other.count;
        pid = other.pid;
        threads = other.threads;
        argumentPointer = other.argumentPointer;
        sharedPagesLocation = other.sharedPagesLocation;
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(other.sharedPages[i].sharedPageId) {
                sharedPages[i].sharedPageId = other.sharedPages[i].sharedPageId;
                new(&sharedPages[i].pageBuffer.page) MemoryPage((MemoryPage&&)other.sharedPages[i].pageBuffer.page);
                other.sharedPages[i].sharedPageId = 0;
            }
        }

        other.valid = false;
    }
    Process& Process::operator=(Process&& other) noexcept {
        if(valid) {
            this->~Process();
        }

        programPages = other.programPages;
        heap = (MemoryManager&&)other.heap;
        finalReturnValue = other.finalReturnValue;
        finished = other.finished;
        valid = other.valid;
        count = other.count;
        pid = other.pid;
        threads = other.threads;
        argumentPointer = other.argumentPointer;
        sharedPagesLocation = other.sharedPagesLocation;
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(other.sharedPages[i].sharedPageId) {
                sharedPages[i].sharedPageId = other.sharedPages[i].sharedPageId;
                new(&sharedPages[i].pageBuffer.page) MemoryPage((MemoryPage&&)other.sharedPages[i].pageBuffer.page);
                other.sharedPages[i].sharedPageId = 0;
            }
        }

        other.valid = false;

        return *this;
    }
    Process::~Process() {
        if(valid) {
            for(uint64_t i = 0; i < count; ++i) {
                programPages[i].~MemoryPage();
            }
            delete[] (uint8_t*)programPages; // destroy
            for(uint64_t i = 0; i < maxSharedPages; ++i) {
                if(sharedPages[i].sharedPageId) {
                    SharedMemory::pages[sharedPages[i].sharedPageId-1].remove(pid); // remove me from shared page
                    sharedPages[i].pageBuffer.page.~MemoryPage();
                }
            }
        }
    }

    void Process::init() {
        Interrupt::setInterrupt(0x80, __systemHandler);
    }

    Process* Process::getLastLoadedProcess() {
        return lastProcess;
    }

    uint64_t Process::createSharedMemoryPage() {
        for(uint64_t i = 0; i < SharedMemory::maxSharedMemoryPages; ++i) {
            uint64_t j;
            for(j = 0; j < SharedMemory::maxUsers; ++j) {
                if(SharedMemory::pages[i].users[j]) {
                    break;
                }
            }
            if(j == SharedMemory::maxUsers) {
                SharedMemory::pages[i].use(pid);
                useSharedMemoryPage(i + 1);
                return i + 1;
            }
        }
        return 0;
    }

    void Process::useSharedMemoryPage(uint64_t sharedMemoryId) {
        { // check if this process can use the memory page
            uint64_t i;
            for(i = 0; i < SharedMemory::maxUsers; ++i) {
                if(SharedMemory::pages[sharedMemoryId-1].users[i] == pid)
                    break;
            }
            if(i == SharedMemory::maxUsers)
                return; // pid is not in users list
        }
        
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(!sharedPages[i].sharedPageId) {
                sharedPages[i].sharedPageId = sharedMemoryId;
                new(&sharedPages[i].pageBuffer.page) MemoryPage(SharedMemory::pages[sharedMemoryId - 1].physicalAddress, true); // do not auto release page
                sharedPages[i].pageBuffer.page.mapTo(sharedPagesLocation + (i * pageSize), true, true);
                return;
            }
        }
    }

    uint64_t Process::getPointerToSharedPage(uint64_t sharedMemoryId) {
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(sharedPages[i].sharedPageId == sharedMemoryId) {
                return sharedPagesLocation + (i * pageSize);
            }
        }
        return 0;
    }

    void Process::unuseSharedMemoryPage(uint64_t sharedMemoryId) {
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(sharedPages[i].sharedPageId == sharedMemoryId) {
                sharedPages[i].sharedPageId = 0; // unuse internal buffer
                sharedPages[i].pageBuffer.page.~MemoryPage(); // destruct page;
                sharedPages[i].pageBuffer.page.mapTo(sharedPagesLocation + (i * pageSize), true, true);
                SharedMemory::pages[sharedMemoryId - 1].remove(pid);
                return;
            }
        }
    }

    void Process::addProcessToSharedMemoryPage(uint64_t sharedMemoryId, uint64_t otherPid) {
        { // check if this process can use the memory page
            uint64_t i;
            for(i = 0; i < SharedMemory::maxUsers; ++i) {
                if(SharedMemory::pages[sharedMemoryId-1].users[i] == pid)
                    break;
            }
            if(i == SharedMemory::maxUsers)
                return; // pid is not in users list
        }
        for(uint64_t i = 0; i < SharedMemory::maxUsers; ++i) {
            if(!SharedMemory::pages[sharedMemoryId-1].users[i]) {
                SharedMemory::pages[sharedMemoryId-1].users[i] = otherPid;
                return;
            }
        }
    }

    void Process::reload() {
        for(uint64_t i = 0; i < count; ++i) {
            programPages[i].remap();
        }
        for(uint64_t i = 0; i < maxSharedPages; ++i) {
            if(sharedPages[i].sharedPageId) {
                sharedPages[i].pageBuffer.page.remap();
            }
        }
        heap.reload();
        lastProcess = this;
    }

    static volatile bool inProcess;

    Thread::~Thread() {
    }

    Thread::Thread(MemoryPage stack[stackPageCount], uint64_t currentCodeAddress, uint64_t pid) {
        for(uint8_t i = 0; i < stackPageCount; ++i) {
            new(Thread::stack + i)MemoryPage((MemoryPage&&) stack[i]);
        }
        Thread::stackAddress = stack[stackPageCount-1].getVirtualAddress() + pageSize - 1;
        Thread::currentCodeAddress = currentCodeAddress;
        Thread::pid = pid;
    }

    void Thread::setWaiting(uint64_t microseconds) {
        earliestSchedule = microseconds + Scheduler::getCurrentRelativeTime();
    }

    bool Thread::isInProgram() {
        return inProcess;
    }

    Thread* Thread::getCurrent() {
        return current;
    }

    void Thread::reload() {
        for(uint8_t i = 0; i < 8; ++i) {
            stack[i].remap();
        }
    }

    void Thread::toProcess() {
        current = this;
        // gcc asm shit is stupid
        asm(R"(
    mov %0, %%rax
    mov %1, %%rdx
    mov %2, %%rbx
    call __process_toProcess
)"
            : //output
            : //input
            "g"(currentCodeAddress),
            "g"(stackAddress),
            "g"(&inProcess)
            : "rax", "rbx", "rdx");
    }

    void Thread::toKernel() {
        // gcc asm shit is stupid
        inProcess = false;
        Thread* tmpPointer = current;
        current = nullptr;
        asm(R"(
    mov %0, %%rax
    mov %1, %%rdx
    call __process_toKernel
)"
        : //output
        : //input
        "g"(&tmpPointer->currentCodeAddress),
        "g"(&tmpPointer->stackAddress)
        : "rax", "rdx");
    }

}