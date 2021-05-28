#pragma once

#include "stdint.h"

namespace Kernel {
    class Debug{
    public:
        enum class Condition {
            Execute = 0b00, Write = 0b01, ReadWrite = 0b11
        };

        static void setBreakpoint(uint8_t debugNum, uint64_t address, Condition c, bool(*callback)());
        static void removeBreakpoint(uint8_t debugNum);
        static void printDebugInfo(void* stackPointer);
        static void init();
    };
}