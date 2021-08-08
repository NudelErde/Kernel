#include "setup.hpp"

uint64_t systemDevice;

void printKernelOut(const char* str) {
    for (; *str; ++str) {
        write(*str);
    }
}

void onWrite(char ch) {
}

void startByDirEntry(DirectoryEntry* entry, const char* dirname) {
    if (entry->name[0] == '.')
        return;
    char buffer[512];
    uint64_t index = 0;
    for (const char* ptr = dirname; *ptr; ++ptr) {
        buffer[index++] = *ptr;
    }
    buffer[index++] = '/';
    for (const char* ptr = entry->name; *ptr; ++ptr) {
        buffer[index++] = *ptr;
    }
    //free(entry->name);
    buffer[index++] = 0;
    buffer[index++] = 0;

    createProcess(systemDevice, buffer, buffer, false);
}

int main() {
    systemDevice = getSystemDevice();
    uint64_t shPid = createProcess(systemDevice, "/lib/badsh.elf", "/lib/badsh.elf\0", false);
    uint64_t inode = getINodeOfPath(systemDevice, "/shared");

    //registerInterProcessMethod(1, 1, (uint64_t) &onWrite, false);

    constexpr uint64_t dirBufferSize = 8;
    DirectoryEntry dirBuffer[dirBufferSize];
    uint64_t entryCount = getDirectoryEntriesOfINode(systemDevice, inode, dirBufferSize, dirBuffer);
    uint64_t loadedCount = entryCount < dirBufferSize ? entryCount : dirBufferSize;
    for (uint64_t i = 0; i < loadedCount; ++i) {
        startByDirEntry(&dirBuffer[i], "/shared");
    }
    if (entryCount > dirBufferSize) {
        DirectoryEntry* allocatedBuffer = (DirectoryEntry*) malloc(sizeof(DirectoryEntry) * entryCount);
        entryCount = getDirectoryEntriesOfINode(systemDevice, inode, entryCount, allocatedBuffer);
        for (uint64_t i = dirBufferSize; i < entryCount; ++i) {
            startByDirEntry(&allocatedBuffer[i], "/shared");
        }
        //free(allocatedBuffer);
    }
    printKernelOut("uwu");
    while (true) {
        sleep(1000);
    }
    return 0;
}