#include "process.hpp"
#include "BasicIn.hpp"
#include "FileSystem.hpp"
#include "IDE.hpp"
#include "KernelOut.hpp"
#include "Scheduler.hpp"
#include "SharedMemory.hpp"
#include "debug.hpp"
#include "elf.hpp"
#include "interrupt.hpp"
#include "tss.hpp"
#include "units.hpp"

namespace Kernel {

extern void __systemHandler() asm("__systemHandler");

static Thread* current;
static Process* lastProcess;

void onProcessSystemCall(uint64_t b, uint64_t c, uint64_t d) {
    switch (b) {  // select process command
        case 0x01:// return and remove from scheduler
            lastProcess->exit(c);
            Scheduler::removeCurrentThread();
            Thread::toKernel();
            return;
        case 0x02:
            *((uint64_t*) c) = lastProcess->getPID();
            return;
        case 0x03: {
            struct SpawnRequest {
                uint64_t fileSystemID;
                const char* path;
                const char* argumentsArray;
                uint64_t pid;
                bool loadWithDebug;
            };
            SpawnRequest* req = (SpawnRequest*) c;
            const char* processPath = req->path;
            uint64_t deviceId = getDeviceForFileSystem(req->fileSystemID);
            uint64_t partitionId = getPartitionForFileSystem(req->fileSystemID);
            Device* device = Device::getDevice(deviceId);
            EXT4 ext(device, partitionId);
            uint64_t thisTid = current->getTID();
            uint64_t pid = loadAndExecute(ext, processPath, req->argumentsArray, lastProcess->getPID(), req->loadWithDebug);
            req->pid = pid;
            Thread::toKernel();
        }
            return;
        case 0x04:
            current->setWaiting(c);// wait for args micro seconds
            Thread::toKernel();
            return;
        case 0x05:
            *((uint64_t*) d) = (uint64_t) (lastProcess->getHeap().malloc(c));
            return;
        case 0x06:
            delete (uint8_t*) c;
            return;
        case 0x07:// create shared memory page
            *((uint64_t*) c) = lastProcess->createSharedMemoryPage();
            return;
        case 0x08:// invite process to memory page
            lastProcess->addProcessToSharedMemoryPage(c, d);
            return;
        case 0x09:// get pointer to shared memory page by id
            *((uint64_t*) d) = lastProcess->getPointerToSharedPage(c);
            return;
        case 0x0A:// free shared memory page
            lastProcess->unuseSharedMemoryPage(c);
            return;
        case 0x0B: {
            Process* proc = Scheduler::getProcessById(c);
            if (proc == 0) {
                return;
            }// process no longer exists
            uint8_t lockID = current->makeLock();
            proc->setFinishLock(current->getTID(), lockID);
            Thread::toKernel();
            *((uint64_t*) d) = current->getWaitForPIDResult();
        }
            return;
        case 0x0C:
            *((uint64_t*) c) = (uint64_t) lastProcess->getArgumentPointer();
            return;
        case 0x0D:
            *(uint64_t*) c = lastProcess->getParentPID();
            return;
        case 0x10: {// get method id in other proccess
            struct InterProcessMethodInformationRequest {
                uint64_t pid;
                uint8_t ipmid;
                uint8_t argCount;
                bool threadCreating;
            };
            InterProcessMethodInformationRequest* request = (InterProcessMethodInformationRequest*) c;
            Process* proc = Scheduler::getProcessById(request->pid);
            if (!proc)// not a process
                return;
            if (request->ipmid >= Process::maxInterProcessMethods)// out of bounds
                return;
            InterProcessMethod* ipm = &proc->getInterPorcessMethods()[request->ipmid];
            if (!ipm->address)// invalid ipmid
                return;

            request->argCount = ipm->argCount;
            request->threadCreating = ipm->createThread;
        }
            return;
        case 0x11: {// call process method by id and pid
            struct InterProcessMethodCallTargetDescriptor {
                uint64_t pid;
                uint64_t* argPointer;// must be on stack
                uint8_t ipmid;
                uint64_t result;
            };
            InterProcessMethodCallTargetDescriptor* request = (InterProcessMethodCallTargetDescriptor*) c;
            Process* proc = Scheduler::getProcessById(request->pid);
            if (!proc)// not a process
                return;
            if (request->ipmid >= Process::maxInterProcessMethods)// out of bounds
                return;
            InterProcessMethod* ipm = &proc->getInterPorcessMethods()[request->ipmid];
            if (ipm->address == 0)// invalid ipmid
                return;
            if (ipm->createThread)// thread creation not supported yet
                return;
            request->result = ipm->switchTo(request->argPointer, request->pid);
        }
            return;
        case 0x12: {// register inter process method
            struct InterProcessMethodRegistration {
                uint8_t ipmid;
                uint8_t argCount;
                uint64_t functionPointer;
                bool createThread;
            };
            InterProcessMethodRegistration* request = (InterProcessMethodRegistration*) c;
            if (request->ipmid >= Process::maxInterProcessMethods)// out of bounds
                return;
            InterProcessMethod* ipm = &lastProcess->getInterPorcessMethods()[request->ipmid];
            if (ipm->address)// already used
                return;
            ipm->address = request->functionPointer;
            ipm->argCount = request->argCount;
            ipm->createThread = request->createThread;
        }
            return;
        case 0x13:                                   // remove inter process method
            if (c >= Process::maxInterProcessMethods)// out of bounds
                return;
            lastProcess->getInterPorcessMethods()[c].address = 0;
            return;

        default:
            return;
    }
}

void onMassStorageSystemCall(uint64_t b, uint64_t c, uint64_t d) {
    switch (b) {
        case 1:
            *((uint64_t*) c) = getFileSystemCount();
            return;
        case 2:
            *((uint64_t*) c) = getSystemFileSystem();
            return;
        default:
            return;
    }
}

void onBasicIOCall(uint64_t b, uint64_t c, uint64_t d) {
    switch (b) {
        case 1: {
            kout << (char) c;
        }
            return;
        case 2: {
            while (Input::isEmpty()) {// todo resource and interrupt stuff
                current->setWaiting(1000);
                Thread::toKernel();
            }
            *((uint8_t*) c) = Input::readBlocking();
        }
            return;
        default:
            return;
    }
}

void onEXT4Syscall(uint64_t b, uint64_t c, uint64_t d) {
    uint64_t deviceId = getDeviceForFileSystem(c);
    uint64_t partitionId = getPartitionForFileSystem(c);
    Device* device = Device::getDevice(deviceId);
    EXT4 ext(device, partitionId);
    switch (b) {
        case 1: {
            struct InodeOfPathRequest {
                const char* path;
                uint64_t inode;
            };
            InodeOfPathRequest* req = (InodeOfPathRequest*) d;
            req->inode = ext.findFileINode(req->path);
        }
            return;
        case 2: {
            struct SectorOfInodeRequest {
                uint64_t inode;
                uint64_t sector;
                uint8_t* buffer;
                bool success;
            };
            SectorOfInodeRequest* req = (SectorOfInodeRequest*) d;
            auto inodeObj = ext.getINode(req->inode);
            req->success = ext.getSectorInFile(inodeObj, req->buffer, req->sector);
        }
            return;
        case 3: {
            struct FlagsOfInodeRequest {
                uint64_t inode;
                uint64_t flags;
            };
            FlagsOfInodeRequest* req = (FlagsOfInodeRequest*) d;
            auto inodeObj = ext.getINode(req->inode);
            req->flags = inodeObj.i_mode;
        }
            return;
        case 4: {
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
            DirectoryEntriesRequest* req = (DirectoryEntriesRequest*) d;
            req->count = 0;
            auto inodeObj = ext.getINode(req->inode);
            if (ext.getFileType(inodeObj) != EXT4::FileType::DIRECTORY)
                return;
            ext.iterateDirectory(
                    inodeObj, [](void* _r, const EXT4::DirEntry& entry) -> bool {
                        DirectoryEntriesRequest* request = (DirectoryEntriesRequest*) _r;
                        uint64_t index = request->count;
                        if (index < request->maxEntries) {
                            request->entries[index].inode = entry.inode;
                            request->entries[index].type = entry.fileType;
                            request->entries[index].name = new char[entry.nameSize + 1];
                            request->entries[index].name[entry.nameSize] = 0;
                            memcpy(request->entries[index].name, entry.name, entry.nameSize);
                        }
                        ++request->count;
                        return false;
                    },
                    (void*) req);
        }
            return;
        case 5: {
            struct FileSizeRequest {
                uint64_t inode;
                uint64_t sizeInBytes;
            } __attribute__((packed));
            FileSizeRequest* req = (FileSizeRequest*) d;
            req->sizeInBytes = ext.getFileSize(ext.getINode(req->inode));
        }
            return;
        default:
            return;
    }
}

void onPCIDeviceCall(uint64_t b, uint64_t c, uint64_t d) {
    switch (b) {
        case 2: {
            if (c == 0) {
                *(uint64_t*) d = PCI::getDriverCount();
                return;
            }
            uint64_t* buffer = (uint64_t*) d;
            auto iter = PCI::getDrivers().getIterator();
            for (uint64_t i = 0; i < c; ++i) {
                if (iter.valid()) {
                    buffer[i] = (*iter.get())->busDeviceFunction;
                    if (!iter.next())
                        return;
                } else {
                    return;
                }
            }
            return;
        }
        case 3: {
            bool cont = true;
            for (auto iter = PCI::getDrivers().getIterator(); iter.valid() && cont; cont = iter.next()) {
                auto dev = *iter.get();
                if (dev->busDeviceFunction == c) {
                    uint32_t header[16];
                    for (uint8_t i = 0; i < 16; ++i) {
                        header[i] = dev->device->readConfig(i * 4);
                    }
                    memcpy((void*) d, header, sizeof(header));
                    return;
                }
            }
            return;
        }
        case 4: {
            c &= ~(pageSize - 1);
            if (c <= 1Gi)
                return;
            uint64_t page = PhysicalMemoryManagment::getFreeUserPage();
            PhysicalMemoryManagment::setUsed(page, true);
            auto node = new Process::UnsafeMapNode();
            node->map.cacheDisable = true;
            node->map.write = true;
            node->map.pAddress = page;
            node->map.vAddress = c;
            node->next = lastProcess->unsafeMapping;
            lastProcess->unsafeMapping = node;
            lastProcess->reload();
            return;
        }
        case 5: {
            c &= ~(pageSize - 1);
            if (c <= 1Gi)
                return;
            if (lastProcess->unsafeMapping && lastProcess->unsafeMapping->map.vAddress == c) {
                lastProcess->unsafeMapping = nullptr;
            }
            for (auto node = lastProcess->unsafeMapping; node->next; node = node->next) {
                if (node->next->map.vAddress == c) {
                    node = node->next->next;
                }
            }
            lastProcess->unload();
            lastProcess->reload();
            return;
        }
        case 6: {
            *(uint64_t*) d = MemoryPage::getPhysicalAddressFromVirtual(c);
            return;
        }
        case 7: {
            c &= ~(pageSize - 1);
            d &= ~(pageSize - 1);
            if (c <= 1Gi)
                return;
            auto node = new Process::UnsafeMapNode();
            node->map.cacheDisable = true;
            node->map.write = true;
            node->map.vAddress = c;
            node->map.pAddress = d;
            node->next = lastProcess->unsafeMapping;
            lastProcess->unsafeMapping = node;
            lastProcess->reload();
            return;
        }
        case 0x10: {
            bool cont = true;
            for (auto iter = PCI::getDrivers().getIterator(); iter.valid() && cont; cont = iter.next()) {
                auto dev = *iter.get();
                if (dev->busDeviceFunction == c) {
                    *(uint64_t*) d = dev->getStatus();
                }
            }
            return;
        }
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F: {
            uint64_t id = b - 0x10;
            bool cont = true;
            for (auto iter = PCI::getDrivers().getIterator(); iter.valid() && cont; cont = iter.next()) {
                auto dev = *iter.get();
                if (dev->busDeviceFunction == c) {
                    uint64_t argSize = dev->getArgSize(b);
                    void* ptr = nullptr;

                    getKernelMemoryManager().reload();
                    if (argSize != 0) {// copy from process in kernel
                        ptr = getKernelMemoryManager().malloc(argSize);
                        uint8_t buffer[256];
                        for (uint64_t i = 0; i < argSize; i += sizeof(buffer)) {
                            lastProcess->reload();
                            current->reload();
                            uint64_t cpSize = (argSize - i);
                            if (cpSize > sizeof(buffer))
                                cpSize = sizeof(buffer);
                            memcpy(buffer, (void*) d, cpSize);
                            getKernelMemoryManager().reload();
                            memcpy(ptr, buffer, cpSize);
                        }
                    }

                    dev->handleDriverCall(id, ptr);

                    if (argSize != 0) {// copy from kernel to process
                        uint8_t buffer[256];
                        for (uint64_t i = 0; i < argSize; i += sizeof(buffer)) {
                            uint64_t cpSize = (argSize - i);
                            if (cpSize > sizeof(buffer))
                                cpSize = sizeof(buffer);

                            getKernelMemoryManager().reload();

                            memcpy(buffer, ptr, cpSize);

                            lastProcess->reload();
                            current->reload();

                            memcpy((void*) d, buffer, cpSize);
                        }
                        getKernelMemoryManager().reload();
                        getKernelMemoryManager().free(ptr);
                    }
                    lastProcess->reload();
                    current->reload();
                }
            }
            return;
        }
    }
}

extern "C" void onSystemCall(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    constexpr void (*functions[])(uint64_t, uint64_t, uint64_t){onProcessSystemCall, onMassStorageSystemCall, onBasicIOCall, onEXT4Syscall, onPCIDeviceCall};
    if (a <= sizeof(functions) / sizeof(*functions) && a >= 1) {
        functions[a - 1](b, c, d);
    }
}

asm(R"(
    .globl jmpAndChangePrivilegeLevel
jmpAndChangePrivilegeLevel:
    mov %rdx, %ds
    mov %rdx, %es
    mov %rdx, %fs
    mov %rdx, %gs

    push %rdx
    push %rdi
    push %r8
    push %rcx
    push %rsi
    iretq
)");

static uint64_t nextPID{};

Process::Process(MemoryPage* programPages, uint64_t count, MemoryManager&& heap, uint64_t sharedPagesLocation, uint64_t parentPid) : programPages(programPages), heap((MemoryManager &&) heap) {
    Process::sharedPagesLocation = sharedPagesLocation;
    valid = true;
    Process::count = count;
    pid = ++nextPID;
    Process::parentPid = parentPid;
    for (auto& s : sharedPages) {
        s.sharedPageId = 0;
    }
}

Process::Process(Process&& other) noexcept : heap((MemoryManager &&) other.heap) {
    programPages = other.programPages;
    finalReturnValue = other.finalReturnValue;
    finished = other.finished;
    valid = other.valid;
    count = other.count;
    pid = other.pid;
    threads = other.threads;
    parentPid = other.parentPid;
    argumentPointer = other.argumentPointer;
    sharedPagesLocation = other.sharedPagesLocation;
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (other.sharedPages[i].sharedPageId) {
            sharedPages[i].sharedPageId = other.sharedPages[i].sharedPageId;
            new (&sharedPages[i].pageBuffer.page) MemoryPage((MemoryPage &&) other.sharedPages[i].pageBuffer.page);
            other.sharedPages[i].sharedPageId = 0;
        }
    }
    memcpy(&interProcessMethods, &other.interProcessMethods, sizeof(interProcessMethods));

    other.valid = false;
}
Process& Process::operator=(Process&& other) noexcept {
    if (valid) {
        this->~Process();
    }

    programPages = other.programPages;
    heap = (MemoryManager &&) other.heap;
    finalReturnValue = other.finalReturnValue;
    finished = other.finished;
    valid = other.valid;
    count = other.count;
    pid = other.pid;
    threads = other.threads;
    parentPid = other.parentPid;
    argumentPointer = other.argumentPointer;
    sharedPagesLocation = other.sharedPagesLocation;
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (other.sharedPages[i].sharedPageId) {
            sharedPages[i].sharedPageId = other.sharedPages[i].sharedPageId;
            new (&sharedPages[i].pageBuffer.page) MemoryPage((MemoryPage &&) other.sharedPages[i].pageBuffer.page);
            other.sharedPages[i].sharedPageId = 0;
        }
    }
    memcpy(&interProcessMethods, &other.interProcessMethods, sizeof(interProcessMethods));

    other.valid = false;

    return *this;
}
Process::~Process() {
    if (valid) {
        for (uint64_t i = 0; i < count; ++i) {
            programPages[i].~MemoryPage();
        }
        delete[](uint8_t*) programPages;// destroy
        for (uint64_t i = 0; i < maxSharedPages; ++i) {
            if (sharedPages[i].sharedPageId) {
                SharedMemory::pages[sharedPages[i].sharedPageId - 1].remove(pid);// remove me from shared page
                sharedPages[i].pageBuffer.page.~MemoryPage();
            }
        }
    }
}

void Process::init() {
    Interrupt::setInterrupt(0x80, __systemHandler, 0);
}

Process* Process::getLastLoadedProcess() {
    return lastProcess;
}

uint64_t Process::createSharedMemoryPage() {
    for (uint64_t i = 0; i < SharedMemory::maxSharedMemoryPages; ++i) {
        uint64_t j;
        for (j = 0; j < SharedMemory::maxUsers; ++j) {
            if (SharedMemory::pages[i].users[j]) {
                break;
            }
        }
        if (j == SharedMemory::maxUsers) {
            SharedMemory::pages[i].use(pid);
            useSharedMemoryPage(i + 1);
            return i + 1;
        }
    }
    return 0;
}

void Process::useSharedMemoryPage(uint64_t sharedMemoryId) {
    {// check if this process can use the memory page
        uint64_t i;
        for (i = 0; i < SharedMemory::maxUsers; ++i) {
            if (SharedMemory::pages[sharedMemoryId - 1].users[i] == pid)
                break;
        }
        if (i == SharedMemory::maxUsers)
            return;// pid is not in users list
    }

    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (!sharedPages[i].sharedPageId) {
            sharedPages[i].sharedPageId = sharedMemoryId;
            new (&sharedPages[i].pageBuffer.page) MemoryPage(SharedMemory::pages[sharedMemoryId - 1].physicalAddress, true);// do not auto release page
            sharedPages[i].pageBuffer.page.mapTo(sharedPagesLocation + (i * pageSize), true, true);
            return;
        }
    }
}

uint64_t Process::getPointerToSharedPage(uint64_t sharedMemoryId) {
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (sharedPages[i].sharedPageId == sharedMemoryId) {
            return sharedPagesLocation + (i * pageSize);
        }
    }
    useSharedMemoryPage(sharedMemoryId);
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (sharedPages[i].sharedPageId == sharedMemoryId) {
            return sharedPagesLocation + (i * pageSize);
        }
    }
    return 0;
}

void Process::unuseSharedMemoryPage(uint64_t sharedMemoryId) {
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (sharedPages[i].sharedPageId == sharedMemoryId) {
            sharedPages[i].sharedPageId = 0;             // unuse internal buffer
            sharedPages[i].pageBuffer.page.~MemoryPage();// destruct page;
            sharedPages[i].pageBuffer.page.mapTo(sharedPagesLocation + (i * pageSize), true, true);
            SharedMemory::pages[sharedMemoryId - 1].remove(pid);
            return;
        }
    }
}

void Process::addProcessToSharedMemoryPage(uint64_t sharedMemoryId, uint64_t otherPid) {
    {// check if this process can use the memory page
        uint64_t i;
        for (i = 0; i < SharedMemory::maxUsers; ++i) {
            if (SharedMemory::pages[sharedMemoryId - 1].users[i] == pid)
                break;
        }
        if (i == SharedMemory::maxUsers)
            return;// pid is not in users list
    }
    for (uint64_t i = 0; i < SharedMemory::maxUsers; ++i) {
        if (!SharedMemory::pages[sharedMemoryId - 1].users[i]) {
            SharedMemory::pages[sharedMemoryId - 1].users[i] = otherPid;
            return;
        }
    }
}

void Process::unload() {
    for (uint64_t i = 0; i < count; ++i) {
        if (programPages[i].checkIfMapped()) {
            programPages[i].unmap();
        }
    }
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (sharedPages[i].sharedPageId && sharedPages[i].pageBuffer.page.checkIfMapped()) {
            sharedPages[i].pageBuffer.page.unmap();
        }
    }
    for (auto ptr = unsafeMapping; ptr; ptr = ptr->next) {
        if (MemoryPage::getPhysicalAddressFromVirtual(ptr->map.vAddress) == ptr->map.pAddress) {
            MemoryPage page(ptr->map.pAddress, true);
            page.mapTo(ptr->map.vAddress, false, false);
            page.unmap();
        }
    }
    heap.unmap();
}

void Process::reload() {
    for (uint64_t i = 0; i < count; ++i) {
        programPages[i].remap();
    }
    for (uint64_t i = 0; i < maxSharedPages; ++i) {
        if (sharedPages[i].sharedPageId) {
            sharedPages[i].pageBuffer.page.remap();
        }
    }
    heap.reload();
    for (auto ptr = unsafeMapping; ptr; ptr = ptr->next) {
        MemoryPage page(ptr->map.pAddress, true);
        page.mapTo(ptr->map.vAddress, ptr->map.write, true, ptr->map.cacheDisable, ptr->map.cacheDisable);
    }
    lastProcess = this;
}

static volatile bool inProcess;

Thread::~Thread() {
}

static uint64_t nextTid;

Thread::Thread(MemoryPage stack[stackPageCount], uint64_t currentCodeAddress, uint64_t pid, bool debugOnStart) {
    Thread::debugOnStart = debugOnStart;
    for (uint8_t i = 0; i < stackPageCount; ++i) {
        new (Thread::stack + i) MemoryPage((MemoryPage &&) stack[i]);
    }
    Thread::stackAddress = Thread::stack[stackPageCount - 1].getVirtualAddress() + pageSize - 8;
    Thread::currentCodeAddress = currentCodeAddress;
    Thread::pid = pid;
    Thread::tid = ++nextTid;
    Thread::ring = 3;
    memset(Thread::locks, 0, 32);
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
    for (uint8_t i = 0; i < 8; ++i) {
        stack[i].remap();
    }
}

extern "C" void __process_toProcess(uint64_t rip, uint64_t rsp, uint64_t inProcessPointer, uint64_t targetStackSegment, uint64_t targetCodeSegment);

void Thread::toProcess() {
    current = this;

    // gcc asm shit is stupid
    if (debugOnStart) {
        debugOnStart = false;
        KTODO("process debugging");
    }
    uint64_t targetSS = (ring == 0 ? (0) : ((3 << 3) | 3));
    uint64_t targetCS = (ring == 0 ? (1 << 3) : ((2 << 3) | 3));
    __process_toProcess(currentCodeAddress, stackAddress, (uint64_t) &inProcess, targetSS, targetCS);
}

void Thread::toKernel() {
    // gcc asm shit is stupid
    inProcess = false;
    Thread* tmpPointer = current;
    current->ring = getCPL();
    current = nullptr;
    asm(R"(
    mov %0, %%rax
    mov %1, %%rdx
    call __process_toKernel
)"
        ://output
        ://input
        "g"(&tmpPointer->currentCodeAddress),
        "g"(&tmpPointer->stackAddress)
        : "rax", "rdx");
}

extern "C" __attribute__((interrupt)) void onSystemCallExit(void* stackframe) {
}

}// namespace Kernel