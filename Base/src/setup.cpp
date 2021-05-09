#include "stdint.h"
#include "setup.hpp"

extern int main();

void syscall(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    asm volatile(R"(
        mov %0, %%rax
        mov %1, %%rbx
        mov %2, %%rcx
        mov %3, %%rdx

        int $0x80
    )"
    :
    :
    "g"(arg0),
    "g"(arg1),
    "g"(arg2),
    "g"(arg3)
    : "rax", "rbx", "rcx", "rdx");
}

void exit(uint64_t exitCode) {
  syscall(1, 0x01, exitCode, 0);
}

uint64_t getpid() {
  uint64_t pid;
  syscall(1, 0x02, (uint64_t)&pid, 0);
  return pid;
}

uint64_t createProcess(uint64_t device, const char* path, const char* argumentsArray) {
  struct SpawnRequest{
    uint64_t deviceID;
    const char* path;
    const char* argumentsArray;
    uint64_t pid;
  };
  SpawnRequest request;
  request.deviceID = device;
  request.path = path;
  request.argumentsArray = argumentsArray;
  syscall(1, 0x03, (uint64_t)&request, 0);
  return request.pid;
}

void sleep(uint64_t microseconds) {
  syscall(1, 0x04, microseconds, 0);
}

void* malloc(uint64_t size) {
  uint64_t value;
  syscall(1, 0x05, size, (uint64_t)&value);
  return (void*)value;
}

void free(void* ptr) {
  syscall(1, 0x06, (uint64_t)ptr, 0);
}

uint64_t createSharedPage() {
  uint64_t pageID;
  syscall(1, 0x07, (uint64_t)&pageID, 0);
  return pageID;
}

void inviteProcessToSharedPage(uint64_t pageID, uint64_t pid) {
  syscall(1, 0x08, pageID, pid);
}

void* getPointerToSharedPage(uint64_t pageID) {
  uint64_t pointer;
  syscall(1, 0x09, pageID, (uint64_t)&pointer);
  return (void*)pointer;
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
  syscall(1, 0x0D, (uint64_t)&parentPid, 0);
  return parentPid;
}

InterProcessMethodInfo getInterProcessMethodInfo(uint64_t pid, uint8_t ipmid) {
  struct InterProcessMethodInformationRequest{
    uint64_t pid;
    uint8_t ipmid;
    uint8_t argCount;
    bool threadCreating;
  };
  InterProcessMethodInformationRequest request;
  request.pid = pid;
  request.ipmid = ipmid;
  syscall(1, 0x10, (uint64_t)&request, 0);
  InterProcessMethodInfo res;
  res.argCount = request.argCount;
  res.createThread = request.threadCreating;
  return res;
}

uint64_t callInterProcessMethod(uint64_t pid, uint8_t ipmid, uint64_t* argPointer) {
  struct InterProcessMethodCallTargetDescriptor{
    uint64_t pid;
    uint64_t* argPointer; // must be on stack
    uint8_t ipmid;
    uint64_t result;
  };
  InterProcessMethodCallTargetDescriptor desc;
  desc.pid = pid;
  desc.ipmid = ipmid;
  desc.argPointer = argPointer;
  syscall(1, 0x11, (uint64_t)&desc, 0);
  return desc.result;
}

void registerInterProcessMethod(uint8_t ipmid, uint8_t argCount, uint64_t functionPointer, bool thread) {
  struct InterProcessMethodRegistration{
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
  syscall(1, 0x12, (uint64_t)&request, 0);
}

void removeInterProcessMethod(uint8_t ipmid) {
  syscall(1, 0x13, ipmid, 0);
}

uint64_t getDeviceCount() {
  uint64_t value;
  syscall(2, 1, (uint64_t)&value, 0);
  return value;
}

uint64_t getSystemDevice() {
  uint64_t value;
  syscall(2, 2, (uint64_t)&value, 0);
  return value;
}

void write(uint8_t value) {
  syscall(3, 1, (uint64_t)value, 0);
}

uint8_t read() {
  uint8_t value;
  syscall(3, 2, (uint64_t)&value, 0);
  return value;
}

uint64_t getINodeOfPath(uint64_t device, const char* path) {
  struct InodeOfPathRequest{
    const char* path;
    uint64_t inode;
  };
  InodeOfPathRequest request;
  request.path = path;
  syscall(4, 1, device, (uint64_t)&request);
  return request.inode;
}

bool getSectorOfINode(uint64_t device, uint64_t inode, uint64_t sector, uint8_t* buffer) {
  struct SectorOfInodeRequest{
    uint64_t inode;
    uint64_t sector;
    uint8_t* buffer;
    bool success;
  };
  SectorOfInodeRequest request;
  request.inode = inode;
  request.sector = sector;
  request.buffer = buffer;
  syscall(4, 2, device, (uint64_t)&request);
  return request.success;
}

uint64_t getFlagsOfINode(uint64_t device, uint64_t inode) {
  struct FlagsOfInodeRequest {
    uint64_t inode;
    uint64_t flags;
  };
  FlagsOfInodeRequest request;
  request.inode = inode;
  syscall(4, 3, device, (uint64_t)&request);
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
  syscall(4, 4, device, (uint64_t)&request);
  return request.count;
}

asm(R"(
callMain:
        push    %rbp
        mov     %rsp, %rbp
        sub     $16, %rsp
        mov     %rbx, -8(%rbp)

        mov     %rdi, %rax

        mov     %rsp, %rdx
        mov     $0, %rdi
        
        cmpb    $0, (%rax)
        je      callMain.preCall
        push    %rax
        jmp     callMain.loop

callMain.preLoop:
        addq   $1, %rax
callMain.loop:
        cmpb    $0, (%rax)
        jne     callMain.preLoop
        
        add     $1, %rdi
        mov     %rax, %rbx
        add     $1, %rbx
        push    %rbx

        cmpb    $0, 1(%rax)
        jne     callMain.preLoop

callMain.preCall:
        movq    $0, (%rsp)

        mov     %rsp, %rsi
        mov     %rsp, %rax
        
        cmpq    %rdx, %rax
        ja      callMain.call
callMain.reverse:
        movq    (%rdx), %rbx
        xchg    (%rax), %rbx
        mov     %rbx, (%rdx)
        addq    $8, %rax
        subq    $8, %rdx
        cmpq    %rdx, %rax
        jbe     callMain.reverse

callMain.call:
        addq    $8, %rsi
        call    main

        mov     -8(%rbp), %rbx
        leave
        ret
)");

extern "C" int callMain(const char* args);

extern "C" {

void __start() {
  const char* args = getArguments();
  int retValue = callMain(args);
  exit(retValue);
  asm("int $0x81");
}

}