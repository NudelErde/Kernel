#pragma once
#include "stdint.h"

void exit(uint64_t exitCode);
uint64_t getpid();
uint64_t createProcess(uint64_t device, const char* path, const char* args, bool withDebug);
void sleep(uint64_t microseconds);
void* malloc(uint64_t size);
void free(void* ptr);
uint64_t createSharedPage();
void inviteProcessToSharedPage(uint64_t pageID, uint64_t pid);
void* getPointerToSharedPage(uint64_t pageID);
void freeSharedMemoryPage(uint64_t pageID);
uint64_t waitForProcess(uint64_t pid);
const char* getArguments();
uint64_t getParentPid();

struct InterProcessMethodInfo {
    uint8_t argCount;
    bool createThread;
};
InterProcessMethodInfo getInterProcessMethodInfo(uint64_t pid, uint8_t ipmid);
uint64_t callInterProcessMethod(uint64_t pid, uint8_t ipmid, uint64_t* argPointer);
void registerInterProcessMethod(uint8_t ipmid, uint8_t argCount, uint64_t functionPointer, bool thread);
void removeInterProcessMethod(uint8_t ipmid);

template<typename ArgT>
constexpr uint64_t convertTo64Bit(ArgT val) {
    static_assert(sizeof(ArgT) <= sizeof(uint64_t));
    uint64_t result = *reinterpret_cast<uint64_t*>(&val);
    if constexpr (sizeof(ArgT) == sizeof(uint64_t)) {
        return result;
    } else {
        return result & (0x1ull << (sizeof(ArgT) * 8u)) - 1u;
    }
}

template<typename... ArgT>
uint64_t callIPM(uint64_t pid, uint64_t ipmid, ArgT... argV) {
    if constexpr (sizeof...(argV) == 0) {
        uint64_t args[1]{};
        return callInterProcessMethod(pid, ipmid, args);
    } else {
        uint64_t args[sizeof...(argV)]{};
        uint64_t i = 0;
        ((args[i++] = convertTo64Bit(argV)), ...);

        return callInterProcessMethod(pid, ipmid, args);
    }
}

uint64_t getDeviceCount();
uint64_t getSystemDevice();

void write(uint8_t value);
uint8_t read();

struct DirectoryEntry {
    uint64_t inode;
    uint64_t type;
    char* name;
};
uint64_t getINodeOfPath(uint64_t device, const char* path);
bool getSectorOfINode(uint64_t device, uint64_t inode, uint64_t sector, uint8_t* buffer);
uint64_t getFlagsOfINode(uint64_t device, uint64_t inode);
uint64_t getDirectoryEntriesOfINode(uint64_t device, uint64_t inode, uint64_t maxCount, DirectoryEntry* entries);
uint64_t getFileSize(uint64_t device, uint64_t inode);

uint64_t getPciDeviceCount();
void getPciDeviceList(uint64_t* buffer, uint64_t count);
void getPciDeviceHeader(uint64_t busDeviceFuntion, uint8_t* buffer);
void allocateMemoryPage(uint64_t virtualAddress);
void freeMemoryPage(uint64_t virtualAddress);
uint64_t virtualToPhysicalAddress(uint64_t virtualAddress);
void mapVirtualToPhysicalAddress(uint64_t vAddress, uint64_t pAddress);

uint64_t getDriverStatus(uint64_t busDeviceFunction);
void driverCall(uint64_t busDeviceFunction, uint8_t num, void* arg);