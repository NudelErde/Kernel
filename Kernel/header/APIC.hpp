#pragma once

#include "stdint.h"
#include "memory.hpp"

namespace Kernel {

class LocalAPIC {
public:
    static void enable();

    static void setupSleep();

    static void endInterrupt();

    static void interruptIn(uint64_t microseconds, void(*callback)(void(*)()));
    static uint64_t getTimeLeft();

    inline static uint64_t localAPICAddress;

    inline static uint32_t ticksIn100ms;
};

class IoAPIC {
public:
    static IoAPIC* initIOApics(uint8_t* count);

    void setMapping(uint8_t interruptNum, uint8_t resultVector, uint8_t cpuId);
private:
    IoAPIC(uint64_t address);

    uint64_t address;
    MemoryPage page;
};

}