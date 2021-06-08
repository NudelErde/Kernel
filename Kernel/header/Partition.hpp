#pragma once

#include "MassStorage.hpp"

namespace Kernel {
struct Partition {
    uint32_t LBAStart;
    uint32_t sectorCount;
    bool bootable;
    uint8_t type;
};

void createMBR(Device&);
void setPartition(Device& device, uint8_t partitionID, uint64_t start, uint64_t end, uint8_t type, bool bootable);
Partition getPartition(Device& device, uint8_t partitionID);
}// namespace Kernel