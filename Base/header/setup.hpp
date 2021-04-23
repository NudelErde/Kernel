#pragma once
#include "stdint.h"

void exit(uint64_t exitCode);
uint64_t getpid();
uint64_t createProcess(uint64_t device, const char* path, const char* args);
void sleep(uint64_t microseconds);
void* malloc(uint64_t size);
void free(void* ptr);
uint64_t createSharedPage();
void inviteProcessToSharedPage(uint64_t pageID, uint64_t pid);
void* getPointerToSharedPage(uint64_t pageID);
void freeSharedMemoryPage(uint64_t pageID);
uint64_t waitForProcess(uint64_t pid);
const char* getArguments();

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
