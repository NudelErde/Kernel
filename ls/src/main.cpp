#include "setup.hpp"

void print(const char* t) {
    for (const char* ptr = t; *ptr; ++ptr) {
        write(*ptr);
    }
}

static void printBase(uint64_t number, uint64_t base, int size) {
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

int main(int argc, const char* argv[]) {
    uint64_t systemDevice = getSystemDevice();
    const char* requestedPath = "/";
    if (argc > 1) {
        requestedPath = argv[1];
    }
    uint64_t inode = getINodeOfPath(systemDevice, requestedPath);
    if (!inode || getFlagsOfINode(systemDevice, inode) & 0x4000 == 0) {
        print("Directory not found\n");
        return 1;
    }
    DirectoryEntry entries[8];
    uint64_t count = getDirectoryEntriesOfINode(systemDevice, inode, 8, entries);
    uint64_t elements = count < 8 ? count : 8;
    for (uint64_t i = 0; i < elements; ++i) {
        print(entries[i].name);
        write('\t');
        //free(entries[i].name); // program ends soon -> do not free, because heap is deleted at program end
    }
    if (count > 8) {
        DirectoryEntry* entries = (DirectoryEntry*) malloc(sizeof(DirectoryEntry) * count);
        count = getDirectoryEntriesOfINode(systemDevice, inode, count, entries);
        for (uint64_t i = 0; i < count; ++i) {
            if (i >= 8) {
                if (i % 8 == 0)
                    write('\n');
                print(entries[i].name);
                write('\t');
            }
            //free(entries[i].name); // program ends soon -> do not free, because heap is deleted at program end
        }
    }
    write('\n');
    return 0;
}