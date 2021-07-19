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
    const char* hexCharMap = "0123456789ABCDEF";
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
    print("    | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9| A| B| C| D| E| F|                \n");
    print("----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+----------------\n");
    for (uint64_t index = 0; index < size; index += 512) {
        if (!getSectorOfINode(systemDevice, fileInode, index / 512, buffer)) {
            print("Error while reading the file\n");
            return 1;
        }
        uint64_t subIndex = 0;
        for (; subIndex < 512 && index + subIndex < size; ++subIndex) {
            if (subIndex % 16 == 0) {
                uint16_t address = (uint16_t) ((subIndex + index) / 16);
                write(hexCharMap[((address >> 12) & 0xF)]);
                write(hexCharMap[((address >> 8) & 0xF)]);
                write(hexCharMap[((address >> 4) & 0xF)]);
                write(hexCharMap[(address & 0xF)]);
            }
            write('|');
            write(hexCharMap[(buffer[subIndex] >> 4) & 0xF]);
            write(hexCharMap[buffer[subIndex] & 0xF]);
            if (subIndex % 16 == 15) {
                write('|');
                for (uint64_t i = 0; i < 16; ++i) {
                    uint8_t ch = buffer[((subIndex + index) & (~0xF)) + i];
                    if (ch >= 0x20 && ch <= 0x7E) {
                        write(ch);
                    } else {
                        write('.');
                    }
                }
                write('\n');
            }
        }
        if (subIndex % 16 != 0) {
            for (uint64_t i = subIndex % 16; i < 16; ++i) {
                print("|  ");
            }
            write('|');
            for (uint64_t i = 0; i < 16; ++i) {
                uint8_t ch = buffer[((subIndex + index) & (~0xF)) + i];
                if (ch >= 0x20 && ch <= 0x7E) {
                    write(ch);
                } else {
                    write('.');
                }
            }
            write('\n');
        }
    }
    return 0;
}