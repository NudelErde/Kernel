#include "stdint.h"
#include "setup.hpp"

void print(const char* t) {
    for(const char* ptr = t; *ptr; ++ptr) {
        write(*ptr);
    }
}

bool strequ(const char* a, const char* b, uint64_t size) {
    for(uint64_t i = 0; i < size; ++i) {
        if(a[i] != b[i])
            return false;
    }
    return true;
}

void memcpy(void* dest, const void* src, uint64_t size) {
    for(uint64_t i = 0; i < size; ++i) {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
}

void memset(void* dest, char byte, uint64_t size) {
    for(uint64_t i = 0; i < size; ++i) {
        ((uint8_t*)dest)[i] = byte;
    }
}

uint64_t strlen(const char* str) {
    uint64_t i = 0;
    for(; *str; ++str) {
        ++i;
    }
    return i;
}

void printBase(uint64_t number, uint64_t base, int size) {
    const char* digits = "0123456789ABCDEF";
    char buffer[64]{};
    
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

void simplifyPath(char* dest, const char* src, uint64_t destLength) {
    uint64_t destIndex = 0;
    uint64_t srcIndex = 0;
    uint64_t srcLength = strlen(src);
    for(; src[srcIndex]; ++srcIndex) {
        if(srcIndex + 2 < srcLength && (strequ(src+srcIndex, "/../", 4) || strequ(src+srcIndex, "/..\0", 4))) {
            //seek last directory
            srcIndex+=3;
            for(;destIndex > 0; --destIndex) {
                if(dest[destIndex-1] == '/')
                    break;
            }
        } else if(srcIndex + 2 < srcLength && strequ(src+srcIndex, "/./", 3)) {
            //do nothing?
            ++srcIndex;
        } else if(srcIndex + 1 < srcLength && strequ(src+srcIndex, "//", 2)) {
            // do nothing?
        } else {
            dest[destIndex++] = src[srcIndex];
        }
        if(destIndex >= destLength)
            break;
    }
    dest[destIndex] = 0;
}

static char* bufferToArgs(char* buffer) {
    bool stringMode = false;
    int8_t shiftLeft = 0;
    for(char* str = buffer; *str; ++str) {
        char val = str[0];
        if(str[0] == '\\' && str[1] == '"') {
            val = str[1];
            ++str;
            --shiftLeft;
        } else if(str[0] == '"') {
            stringMode = !stringMode;
            --shiftLeft;
            continue;
        } else if(!stringMode && str[0] == ' ') {
            val = 0;
        }
        str[shiftLeft] = val;
        if(shiftLeft != 0)
            str[0] = 0;
    }
    return buffer;
}

static uint64_t systemDevice;
static uint64_t returnValue;

static bool isFile(uint64_t inode) {
    return (inode != 0 && getFlagsOfINode(systemDevice, inode) & 0x8000);
}


int main(int argc, const char* argv) {
    char buffer[257];
    uint16_t index = 0;
    const char searchPath[] = "/lib/";
    const char cmdPost[] = ".elf";
    print("Shell start\n");
    
    uint64_t myInode = 2;
    systemDevice = getSystemDevice();
    
    char currentPath[256];
    memset(currentPath, 0, 256);
    memcpy(currentPath, "/", 2);

    while(true) {
        memset(buffer, 0, 257);
        index = 0;
        print(currentPath);
        write('>');
        write(' ');
        bool reading = true;
        while(reading) {
            uint8_t ch = read();
            if(ch == '\n' || ch == '\r') {
                write('\n');
                reading = false;
            } else {
                buffer[index++] = ch;
                write(ch);
            }
        }
        if(strequ(buffer, "exit", 5)) {
            if(getpid() == 1) {
                print("Can not shutdown root process. Try 'shutdown'.\n");
            } else {
                return 0;
            }
        } else if(strequ(buffer, "cd ", 3)) {
            if(strlen(buffer) > 3) {
                uint64_t nextINode = getINodeOfPath(systemDevice, buffer + 3);
                if(nextINode != 0 && getFlagsOfINode(systemDevice, nextINode) & 0x4000) {
                    myInode = nextINode;
                    memset(currentPath, 0, 256);
                    memcpy(currentPath, buffer + 3, strlen(buffer) - 3);
                } else {
                    print("Directory not found\n");
                }
            }
        } else if(strequ(buffer, "$ ", 2)) {
            if (strlen(buffer) > 2) {
                uint64_t exeINode = getINodeOfPath(systemDevice, buffer + 2);
                if(isFile(exeINode)) {
                    char* args = bufferToArgs(buffer + 2);
                    uint64_t pid = createProcess(systemDevice, buffer + 2, args, false);
                    returnValue = waitForProcess(pid);
                } else {
                    print("File not found\n");
                }
            }
        } else {
            char* args = bufferToArgs(buffer);
            char commandBuffer[256 + sizeof(searchPath) + sizeof(cmdPost)];
            buffer[256] = 0;
            uint64_t commandNameLen = strlen(buffer);
            memcpy(commandBuffer, searchPath, sizeof(searchPath));
            memcpy(commandBuffer + sizeof(searchPath) - 1 /*remove terminating zero*/, buffer, commandNameLen);
            memcpy(commandBuffer + sizeof(searchPath) - 1 + commandNameLen, cmdPost, sizeof(cmdPost));
            uint64_t cmdINode = getINodeOfPath(systemDevice, commandBuffer);
            if(isFile(cmdINode)) {
                uint64_t pid = createProcess(systemDevice, commandBuffer, args, false);
                returnValue = waitForProcess(pid);
            } else {
                print("Command not found\n");
            }
        }
    }
    return 0;
}