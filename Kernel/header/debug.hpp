#pragma once

#include "stdint.h"

namespace Kernel {

class Debug {
public:
    enum class Condition {
        Execute = 0b00,
        Write = 0b01,
        ReadWrite = 0b11
    };

    static void setBreakpoint(uint8_t debugNum, uint64_t address, Condition c, bool (*callback)(uint64_t));
    static void setCallback(uint8_t debugNum, bool (*callback)(uint64_t));
    static void removeBreakpoint(uint8_t debugNum);
    static void printDebugInfo(void* stackPointer);
    static uint64_t getRSP(void* interruptStack);
    static uint64_t getRIP(void* interruptStack);
    static void printStackTrace();
    static void printStackTrace(uint64_t rbp);
    static void init();
    static void setSectionArray(void*);
    static void setSymbolSection(void*);
    struct Symbol {
        uint64_t value;
        uint64_t size;
        uint64_t flags;
        const char* name;
    };
    static Symbol getSymbol(void* ptr);
    static bool demangleSymbol(const char* source, char* buffer, uint64_t bufferSize);
};

}// namespace Kernel