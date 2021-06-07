#include "setup.hpp"

void print(const char* str) {
    for(;*str; ++str) {
        write(*str);
    }
}

void printBase(uint64_t number, uint64_t base, int size) {
    const char* digits = "0123456789ABCDEF";
    char buffer[64];
    for(uint64_t i = 0; i < 64; ++i) {
        buffer[i] = 0;
    }
    
    int index;
    for(index = 1; number > 0; ++index) {
        buffer[index] = digits[number % base];
        number /= base;
    }

    for(int i = 0; i < size - index + 1; ++i) {
        write('0');
    }

    buffer[63] = 0; // always terminated pls
    for(char* ptr = buffer+index-1; *ptr; --ptr) {
        write(*ptr);
    }
}

uint64_t t(uint64_t a, uint64_t b) {
    struct FileSizeRequest {
        uint64_t inode;
        uint64_t sizeInBytes;
    };
    FileSizeRequest request;
    request.inode = b;
    request.sizeInBytes = 0;
    asm volatile(R"(
        mov $4, %%rax
        mov $5, %%rbx
        mov %0, %%rcx
        mov %1, %%rdx

        int $0x80
    )"
    :
    :
    "g"(a),
    "g"((uint64_t)&request)
    : "rax", "rbx", "rcx", "rdx");
    return request.sizeInBytes;
}

int main(int argc, const char* argv[]) {
    if(argc < 2) {
        print("Usage: ");
        if(argc < 1) {
            print("program");
        } else {
            print(argv[0]);
        }
        print(" filename\n");
        return 1;
    }
    const char* file = argv[1];
    uint64_t systemDevice = getSystemDevice();
    uint64_t fileInode = getINodeOfPath(systemDevice, file);
    if(fileInode == 0) {
        print("File not found\n");
        return 1;
    }
    uint64_t size = getFileSize(systemDevice, fileInode);
    print("Size of file \"");
    print(file);
    print("\" is ");
    printBase(size, 10, 1);
    print(" bytes\n");
    return 0;
}
