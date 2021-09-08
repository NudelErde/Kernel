#include "setup.hpp"

uint64_t systemDevice;

uint8_t getCPL() {
    uint32_t cs;
    asm(R"(
        mov %%cs, %0
        )"
        : "=a"(cs));
    return cs & 0b11;
}

void printBase(uint64_t number, uint64_t base, int size) {
    const char* digits = "0123456789ABCDEF";
    char buffer[64]{};

    int index;
    for (index = 1; number > 0; ++index) {
        buffer[index] = digits[number % base];
        number /= base;
    }

    for (int i = 0; i < size - index + 1; ++i) {
        write('0');
    }

    buffer[63] = 0;// always terminated pls
    for (char* ptr = buffer + index - 1; *ptr; --ptr) {
        write(*ptr);
    }
}

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

    printKernelOut("[BOOT] Start new process\n");
    //createProcess(systemDevice, buffer, buffer, false);
    printKernelOut("[BOOT] Started process ");
    printKernelOut(buffer);
    printKernelOut("!\n");
}

int main() {
    systemDevice = getSystemDevice();

    printKernelOut("[BOOT]: CPL: ");
    printBase(getCPL(), 10, 1);
    printKernelOut("\n");

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
        free(allocatedBuffer);
    }
    while (true) {
        sleep(10000);
    }
    return 0;
}