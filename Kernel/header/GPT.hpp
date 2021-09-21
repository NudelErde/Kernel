#pragma once

#include "MassStorage.hpp"
#include "Partition.hpp"

namespace Kernel {

enum class GUID_KEY {
    GRUB_BIOS_BOOT,
    LINUX_NATIVE,
    UNKNOWN
};

struct GUID {
    const char* name;
    const char* bytes;
    GUID_KEY key;
};

GUID getGUID(uint64_t bytes[2]);
bool checkIfGPT(Device& device);
void setGPTPartition(Device& device, uint16_t partitionID, uint64_t start, uint64_t end, uint64_t type[2]);
Partition getGPTPartition(Device& device, uint16_t partitionID);

void readGPTHeader(Device& device, uint8_t* buffer, uint64_t bufferSize);
void writeGPTHeader(Device& device, uint8_t* buffer, uint64_t bufferSize);

void getPartitionName(Device& device, uint16_t partitionID, char16_t* buffer);
void setPartitionName(Device& device, uint16_t partitionID, char16_t* buffer);

void getUniqueId(Device& device, uint16_t partitionID, uint8_t* buffer);
void setUniqueId(Device& device, uint16_t partitionID, uint8_t* buffer);

uint64_t getGPTPartitionCount(Device& device);

}// namespace Kernel