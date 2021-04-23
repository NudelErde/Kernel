#pragma once

#include "IDE.hpp"

namespace Kernel {
    struct Partition{
        uint32_t LBAStart;
        uint32_t sectorCount;
        bool bootable;
        uint8_t type;
    };
    
    void createMBR(ATA::Device&);
    void setPartition(ATA::Device& device, uint8_t partitionID, uint64_t start, uint64_t end, uint8_t type, bool bootable);
    Partition getPartition(ATA::Device& device, uint8_t partitionID);
}