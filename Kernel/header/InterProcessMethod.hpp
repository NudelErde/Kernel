#pragma once
#include "stdint.h"

namespace Kernel {

struct InterProcessMethod {
    uint64_t address;
    uint8_t argCount;
    bool createThread;
    uint64_t switchTo(uint64_t* argPointer, uint64_t targetPid);
};

}