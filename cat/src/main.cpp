#include "setup.hpp"

static uint64_t systemDevice;

static void print(const char* str) {
    for (; *str; ++str) {
        write(*str);
    }
}

static bool isFile(uint64_t inode) {
    return (inode != 0 && getFlagsOfINode(systemDevice, inode) & 0x8000);
}

static uint8_t buffer[512];

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        print("Usage: ");
        if (argc < 1) {
            print("program");
        } else {
            print(argv[0]);
        }
        print(" filename\n");
        return 1;
    }
    const char* file = argv[1];
    systemDevice = getSystemDevice();
    uint64_t fileInode = getINodeOfPath(systemDevice, file);
    if (fileInode == 0) {
        print("File not found\n");
        return 1;
    }
    if (!isFile(fileInode)) {
        print("Not a file\n");
        return 1;
    }
    uint64_t size = getFileSize(systemDevice, fileInode);
    for (uint64_t index = 0; index < size; index += 512) {
        if (!getSectorOfINode(systemDevice, fileInode, index / 512, buffer)) {
            print("Error while reading the file\n");
            return 1;
        }
        for (uint64_t subIndex = 0; subIndex < 512 && index + subIndex < size; ++subIndex) {
            write(buffer[subIndex]);
        }
    }
    return 0;
}