#pragma once
#include "stdint.h"

namespace Kernel {

class PS2 {
public:
    static void init();
    static void disable();
    static uint8_t getPorts();
    static uint16_t getType(uint8_t port);
    static bool isEmpty(uint8_t port);
    static uint64_t getError();
    static uint8_t read(uint8_t port);
    static void write(uint8_t port, uint8_t data);
    static void setInterruptOverride(uint8_t port, void (*fn)(uint8_t));
};

}// namespace Kernel