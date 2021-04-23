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
  syscall(1, 1, exitCode, 0);
}

uint64_t getpid() {
  uint64_t value;
  syscall(1, 2, (uint64_t)&value, 0);
  return value;
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
  syscall(1, 3, (uint64_t)&request, 0);
  return request.pid;
}

void sleep(uint64_t microseconds) {
  syscall(1, 4, microseconds, 0);
}

void* malloc(uint64_t size) {
  uint64_t value;
  syscall(1, 5, size, (uint64_t)&value);
  return (void*)value;
}

void free(void* ptr) {
  syscall(1, 6, (uint64_t)ptr, 0);
}

uint64_t createSharedPage() {
  uint64_t pageID;
  syscall(1, 7, (uint64_t)&pageID, 0);
  return pageID;
}

void inviteProcessToSharedPage(uint64_t pageID, uint64_t pid) {
  syscall(1, 8, pageID, pid);
}

void* getPointerToSharedPage(uint64_t pageID) {
  uint64_t pointer;
  syscall(1, 9, pageID, (uint64_t)&pointer);
  return (void*)pointer;
}

void freeSharedMemoryPage(uint64_t pageID) {
  syscall(1, 10, pageID, 0);
}

uint64_t waitForProcess(uint64_t pid) {
  uint64_t result;
  syscall(1, 15, pid, (uint64_t) &result);
  return result;
}

const char* getArguments() {
  const char* result;
  syscall(1, 16, (uint64_t) &result, 0);
  return result;
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

extern "C" {

void __start() {
  int retValue = main();
  exit(retValue);
  asm("int $0x81");
}

}