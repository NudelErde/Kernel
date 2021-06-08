#include "SharedMemory.hpp"
#include "memory.hpp"

namespace Kernel {

SharedMemory SharedMemory::pages[maxSharedMemoryPages];

void SharedMemory::use(uint64_t owner) {
    users[0] = owner;
    physicalAddress = PhysicalMemoryManagment::getFreeUserPage();
    PhysicalMemoryManagment::setUsed(physicalAddress, true);
}

void SharedMemory::remove(uint64_t pid) {
    bool inUse = false;
    for (uint64_t i = 0; i < maxUsers; ++i) {
        if (users[i] == pid) {
            users[i] = 0;
        }
        if (users[i]) {
            inUse = true;
        }
    }
    if (!inUse) {
        PhysicalMemoryManagment::setUsed(physicalAddress, false);
    }
}

}// namespace Kernel