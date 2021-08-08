#include "setup.hpp"
#include "stdint.h"

inline void syscall(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    asm volatile(R"(
        mov %0, %%rax
        mov %1, %%rbx
        mov %2, %%rcx
        mov %3, %%rdx

        int $0x80
    )"
                 :
                 : "g"(arg0),
                   "g"(arg1),
                   "g"(arg2),
                   "g"(arg3)
                 : "rax", "rbx", "rcx", "rdx");
}

static uint64_t myPid;

void exit(uint64_t exitCode) {
    syscall(1, 0x01, exitCode, 0);
}

uint64_t getpid() {
    uint64_t pid;
    syscall(1, 0x02, (uint64_t) &pid, 0);
    return pid;
}

uint64_t createProcess(uint64_t device, const char* path, const char* argumentsArray, bool withDebug) {
    struct SpawnRequest {
        uint64_t deviceID;
        const char* path;
        const char* argumentsArray;
        uint64_t pid;
        bool withDebug;
    };
    SpawnRequest request;
    request.deviceID = device;
    request.path = path;
    request.argumentsArray = argumentsArray;
    request.withDebug = withDebug;
    syscall(1, 0x03, (uint64_t) &request, 0);
    return request.pid;
}

void sleep(uint64_t microseconds) {
    syscall(1, 0x04, microseconds, 0);
}

void* malloc(uint64_t size) {
    uint64_t value;
    syscall(1, 0x05, size, (uint64_t) &value);
    return (void*) value;
}

void free(void* ptr) {
    syscall(1, 0x06, (uint64_t) ptr, 0);
}

uint64_t createSharedPage() {
    uint64_t pageID;
    syscall(1, 0x07, (uint64_t) &pageID, 0);
    return pageID;
}

void inviteProcessToSharedPage(uint64_t pageID, uint64_t pid) {
    syscall(1, 0x08, pageID, pid);
}

void* getPointerToSharedPage(uint64_t pageID) {
    uint64_t pointer;
    syscall(1, 0x09, pageID, (uint64_t) &pointer);
    return (void*) pointer;
}

void freeSharedMemoryPage(uint64_t pageID) {
    syscall(1, 0x0A, pageID, 0);
}

uint64_t waitForProcess(uint64_t pid) {
    uint64_t result;
    syscall(1, 0x0B, pid, (uint64_t) &result);
    return result;
}

const char* getArguments() {
    const char* result;
    syscall(1, 0x0C, (uint64_t) &result, 0);
    return result;
}

uint64_t getParentPid() {
    uint64_t parentPid;
    syscall(1, 0x0D, (uint64_t) &parentPid, 0);
    return parentPid;
}

InterProcessMethodInfo getInterProcessMethodInfo(uint64_t pid, uint8_t ipmid) {
    struct InterProcessMethodInformationRequest {
        uint64_t pid;
        uint8_t ipmid;
        uint8_t argCount;
        bool threadCreating;
    };
    InterProcessMethodInformationRequest request;
    request.pid = pid;
    request.ipmid = ipmid;
    syscall(1, 0x10, (uint64_t) &request, 0);
    InterProcessMethodInfo res;
    res.argCount = request.argCount;
    res.createThread = request.threadCreating;
    return res;
}

uint64_t callInterProcessMethod(uint64_t pid, uint8_t ipmid, uint64_t* argPointer) {
    struct InterProcessMethodCallTargetDescriptor {
        uint64_t pid;
        uint64_t* argPointer;// must be on stack
        uint8_t ipmid;
        uint64_t result;
    };
    InterProcessMethodCallTargetDescriptor desc;
    desc.pid = pid;
    desc.ipmid = ipmid;
    desc.argPointer = argPointer;
    syscall(1, 0x11, (uint64_t) &desc, 0);
    return desc.result;
}

void registerInterProcessMethod(uint8_t ipmid, uint8_t argCount, uint64_t functionPointer, bool thread) {
    struct InterProcessMethodRegistration {
        uint8_t ipmid;
        uint8_t argCount;
        uint64_t functionPointer;
        bool createThread;
    };
    InterProcessMethodRegistration request;
    request.ipmid = ipmid;
    request.argCount = argCount;
    request.functionPointer = functionPointer;
    request.createThread = thread;
    syscall(1, 0x12, (uint64_t) &request, 0);
}

void removeInterProcessMethod(uint8_t ipmid) {
    syscall(1, 0x13, ipmid, 0);
}

uint64_t getDeviceCount() {
    uint64_t value;
    syscall(2, 1, (uint64_t) &value, 0);
    return value;
}

uint64_t getSystemDevice() {
    uint64_t value;
    syscall(2, 2, (uint64_t) &value, 0);
    return value;
}

void write(uint8_t value) {
    syscall(3, 1, (uint64_t) value, 0);
}

uint8_t read() {
    uint8_t value;
    syscall(3, 2, (uint64_t) &value, 0);
    return value;
}

uint64_t getINodeOfPath(uint64_t device, const char* path) {
    struct InodeOfPathRequest {
        const char* path;
        uint64_t inode;
    };
    InodeOfPathRequest request;
    request.path = path;
    syscall(4, 1, device, (uint64_t) &request);
    return request.inode;
}

bool getSectorOfINode(uint64_t device, uint64_t inode, uint64_t sector, uint8_t* buffer) {
    struct SectorOfInodeRequest {
        uint64_t inode;
        uint64_t sector;
        uint8_t* buffer;
        bool success;
    };
    SectorOfInodeRequest request;
    request.inode = inode;
    request.sector = sector;
    request.buffer = buffer;
    syscall(4, 2, device, (uint64_t) &request);
    return request.success;
}

uint64_t getFlagsOfINode(uint64_t device, uint64_t inode) {
    struct FlagsOfInodeRequest {
        uint64_t inode;
        uint64_t flags;
    };
    FlagsOfInodeRequest request;
    request.inode = inode;
    syscall(4, 3, device, (uint64_t) &request);
    return request.flags;
}

uint64_t getDirectoryEntriesOfINode(uint64_t device, uint64_t inode, uint64_t maxCount, DirectoryEntry* entries) {
    struct DirectoryEntriesRequest {
        uint64_t inode;
        uint64_t maxEntries;
        DirectoryEntry* entries;
        uint64_t count;
    };
    DirectoryEntriesRequest request;
    request.inode = inode;
    request.maxEntries = maxCount;
    request.entries = entries;
    syscall(4, 4, device, (uint64_t) &request);
    return request.count;
}

uint64_t getFileSize(uint64_t device, uint64_t inode) {
    struct FileSizeRequest {
        uint64_t inode;
        uint64_t sizeInBytes;
    } __attribute__((packed));
    FileSizeRequest request;
    request.inode = inode;
    request.sizeInBytes = 0;
    syscall(4, 5, device, (uint64_t) &request);
    return request.sizeInBytes;
}

uint64_t getPciDeviceCount() {
    uint64_t count = 0;
    syscall(5, 0x02, 0, (uint64_t) &count);
    return count;
}

void getPciDeviceList(uint64_t* buffer, uint64_t count) {
    syscall(5, 0x02, count, (uint64_t) buffer);
}
void getPciDeviceHeader(uint64_t busDeviceFuntion, uint8_t* buffer) {
    syscall(5, 0x03, busDeviceFuntion, (uint64_t) buffer);
}
void allocateMemoryPage(uint64_t virtualAddress) {
    syscall(5, 0x04, virtualAddress, 0);
}
void freeMemoryPage(uint64_t virtualAddress) {
    syscall(5, 0x05, virtualAddress, 0);
}
uint64_t virtualToPhysicalAddress(uint64_t virtualAddress) {
    uint64_t physicalAddressBuffer = 0;
    syscall(5, 0x06, virtualAddress, (uint64_t) &physicalAddressBuffer);
    return physicalAddressBuffer;
}
void mapVirtualToPhysicalAddress(uint64_t vAddress, uint64_t pAddress) {
    syscall(5, 0x07, vAddress, pAddress);
}

uint64_t getDriverStatus(uint64_t busDeviceFunction) {
    uint64_t result = 0;
    syscall(5, 0x10, busDeviceFunction, (uint64_t) &result);
    return result;
}
void driverCall(uint64_t busDeviceFunction, uint8_t num, void* arg) {
    if (!(num >= 0x1 && num <= 0xF))
        return;
    syscall(5, 0x10 + num, busDeviceFunction, (uint64_t) arg);
}

int countArgc(const char* args) {
    int argc = 0;
    if (args == 0 || args[0] == 0) {
        argc = 0;
    } else {
        while (true) {
            if (args[0] == 0)
                ++argc;
            if (args[0] == 0 && args[1] == 0)
                break;

            ++args;
        }
    }
    return argc;
}

const char** buildArgv(int argc, const char* args) {
    const char** argv = (const char**) malloc(sizeof(uintptr_t) * (argc + 1));
    argv[argc] = 0;
    if (argc > 0) {
        uint64_t index = 0;
        do {
            argv[index] = args;
            ++index;
            for (; *args; ++args)
                ;  // to end of string
            ++args;// start of next string
        } while (*args);
    }
    return argv;
}

extern "C" int main(int argc, const char** argv);

extern "C" void __start1() {
    myPid = getpid();
    const char* args = getArguments();
    int argc = countArgc(args);
    const char** argv = buildArgv(argc, args);
    int value = main(argc, argv);
    exit(value);
}

asm(R"(
    .globl __start
__start:
    mov %rsp, %rbp
    call __start1
    int3
)");