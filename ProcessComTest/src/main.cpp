#include "setup.hpp"

void memset(void* dest, char byte, uint64_t size) {
    for(uint64_t i = 0; i < size; ++i) {
        ((uint8_t*)dest)[i] = byte;
    }
}

void memcpy(void* dest, const void* src, uint64_t size) {
    for(uint64_t i = 0; i < size; ++i) {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
}

uint64_t strlen(const char* str) {
    uint64_t i = 0;
    for(; *str; ++str) {
        ++i;
    }
    return i;
}

void print(const char* str) {
    for(; *str; ++str) {
        write(*str);
    }
}

uint64_t countArguments(const char* args) {
    uint64_t val = 0;
    for(; *args; ++args) {
        for(; *args; ++args); // skip value
        ++val;
    }
    return val;
}

void getArgv(const char* args, const char** argv) {
    uint64_t index = 0;
    for(; *args; ++args) {
        argv[index] = args;
        for(; *args; ++args); // skip value
        ++index;
    }
}

static void printBase(uint64_t number, uint64_t base, int size) {
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
    print(buffer+index-1);
}

void sprintHex(char* str, uint64_t num) {
    const char* digits = "0123456789ABCDEF";
    for(uint64_t i = 0; i < 64/4; ++i) {
        *str = digits[(num >> 60) & 0xF];
        ++str;
        num << 4;
    }
}

uint64_t parseHex(const char* str) {
    uint64_t result;
    for(; *str; ++str) {
        result <<= 4;
        result |= ((*str <= '9' && *str >= '0')?
                        (*str - '0'):
                        (*str - 'A'));
    }
    return result;
}

void printPidFromExtern(uint64_t arg) {
    print("Process id is: ");
    printBase(getpid(), 10, 1);
    print(" arg: ");
    printBase(arg, 10, 1);
    print("\n");
}

uint64_t printStringFrom(uint64_t pageid) {
    char* page = (char*)getPointerToSharedPage(pageid);
    print(page);
    return strlen(page);
}

int main() {
    const char* args = getArguments();
    if(*args) {
        print("[First] Started\n");
        uint64_t myPid = getpid();
        print("[First] my pid: ");
        printBase(myPid, 10, 1);
        print("\n");
        registerInterProcessMethod(0, 1, (uint64_t) &printPidFromExtern, false);
        registerInterProcessMethod(1, 1, (uint64_t) &printStringFrom, false);
        
        uint64_t pid = createProcess(getSystemDevice(), args /*arg0 is file name*/, "\0\0" /* no args*/, false);
        print("[First] child pid: ");
        printBase(pid, 10, 1);
        print("\n");
        waitForProcess(pid);
        print("[First] close\n");
    } else {
        print("[Second] Started\n");
        print("[Second] my pid: ");
        printBase(getpid(), 10, 1);
        print("\n");
        print("[Second] parent pid: ");
        printBase(getParentPid(), 10, 1);
        print("\n");

        uint64_t pageid = createSharedPage();
        inviteProcessToSharedPage(pageid, getParentPid());
        char* page = (char*)getPointerToSharedPage(pageid);
        memcpy(page, "Hello world text\n", sizeof("Hello world text\n"));

        uint64_t argPointer[4]{};
        argPointer[0] = pageid;
        uint64_t length = callInterProcessMethod(getParentPid(), 1, argPointer);
        print("Length: ");
        printBase(length, 10, 1);
        print("\nShould be: ");
        printBase(strlen(page), 10, 1);
        print("\n");
        print("[Second] close\n");
    }
    return 0;
}