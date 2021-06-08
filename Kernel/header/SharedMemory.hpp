#pragma once
#include "stdint.h"

namespace Kernel {

class SharedMemory {
public:
    static constexpr uint64_t maxUsers = 16;
    static constexpr uint64_t maxSharedMemoryPages = 128;

    uint64_t users[maxUsers];// owner is user 0
    uint64_t physicalAddress;

    void use(uint64_t owner);
    void remove(uint64_t pid);

    static SharedMemory pages[maxSharedMemoryPages];
};

}// namespace Kernel