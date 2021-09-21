#pragma once

#include "MassStorage.hpp"

namespace Kernel {
struct Partition {
    uint32_t LBAStart;
    uint32_t sectorCount;
    bool bootable;
    bool gpt;
    union {
        uint16_t type;
        uint64_t gptType[2];
    };
    uint16_t index;
};

void createMBR(Device&);
void setPartition(Device& device, uint16_t partitionID, uint64_t start, uint64_t end, uint64_t type[2], bool bootable);
Partition getPartition(Device& device, uint16_t partitionID);
uint64_t getPartitionCount(Device& device);

void scanDevicePartitions();
uint64_t getSystemFileSystem();
uint64_t getFileSystemCount();
uint64_t getDeviceForFileSystem(uint64_t id);
uint64_t getPartitionForFileSystem(uint64_t id);
}// namespace Kernel