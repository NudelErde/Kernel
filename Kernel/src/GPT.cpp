#include "GPT.hpp"
#include "KernelOut.hpp"
#include "endian.hpp"
#include "memory.hpp"

namespace Kernel {

static GUID guidList[]{
        {"GRUB_BIOS_BOOT", "Hah!IdontNeedEFI", GUID_KEY::GRUB_BIOS_BOOT},
        {"LINUX_NATIVE", "\xAF\x3D\xC6\x0F\x83\x84\x72\x47\x8E\x79\xE4\x7D\x47\xD8\x69\x3D", GUID_KEY::LINUX_NATIVE}};

GUID getGUID(uint64_t bytes[2]) {
    uint8_t* data = (uint8_t*) bytes;
    GUID unknown{"unknown", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", GUID_KEY::UNKNOWN};
    for (uint64_t i = 0; i < sizeof(guidList) / sizeof(guidList[0]); ++i) {
        if ([&]() -> bool {
                for (uint64_t j = 0; j < 16; ++j) {
                    if (data[j] != guidList[i].bytes[j]) {
                        return false;
                    }
                }
                return true;
            }()) {
            return guidList[i];
        }
    }
    return unknown;
}

static inline bool checkGPTMagicNumber(uint8_t* start) {
    const char* magicNumber = "EFI PART";
    for (uint8_t i = 0; i < 8; ++i) {
        if (start[i] != magicNumber[i]) {
            return false;
        }
    }
    return true;
}

bool checkIfGPT(Device& device) {
    uint8_t header[512];
    readGPTHeader(device, header, sizeof(header));
    return checkGPTMagicNumber(header);
}

void setGPTPartition(Device& device, uint16_t partitionID, uint64_t start, uint64_t end, uint64_t type[2]) {
    kout << "TODO setGPTPartition\n";
}

Partition getGPTPartition(Device& device, uint16_t partitionID) {
    uint8_t header[512];
    readGPTHeader(device, header, sizeof(header));
    if (!checkGPTMagicNumber(header)) {//EFI PART
        return {};
    }
    uint32_t entrySize = *(uint32_t*) (header + 0x54);
    uint64_t index = entrySize * partitionID + 512 * 2;
    uint64_t sectorIndex = index / 512;
    uint64_t offset = index % 512;
    uint8_t sector[512];
    device.read(sectorIndex, 1, sector);
    uint8_t* entry = sector + offset;

    Partition partition{};
    partition.bootable = entry[0x30] & 0b100;
    memcpy(partition.gptType, entry, 16);
    partition.index = partitionID;
    partition.LBAStart = *(uint64_t*) (entry + 0x20);
    uint64_t end = *(uint64_t*) (entry + 0x28);
    partition.sectorCount = end - partition.LBAStart + 1;
    partition.gpt = true;
    return partition;
}


void readGPTHeader(Device& device, uint8_t* buffer, uint64_t size) {
    if (size >= 512) {
        device.read(1, 1, buffer);
    } else {
        uint8_t tmpBuffer[512];
        device.read(1, 1, tmpBuffer);
        memcpy(buffer, tmpBuffer, size);
    }
}

void writeGPTHeader(Device& device, uint8_t* buffer, uint64_t size) {
    if (size >= 512) {
        device.write(1, 1, buffer);
    } else {
        uint8_t tmpBuffer[512]{};
        memcpy(tmpBuffer, buffer, size);
        device.write(1, 1, tmpBuffer);
    }
}


void setPartitionName(Device& device, uint16_t partitionID, char16_t* buffer) {
    kout << "TODO setPartitionName\n";
}
void getPartitionName(Device& device, uint16_t partitionID, char16_t* buffer) {
    uint8_t header[512];
    readGPTHeader(device, header, sizeof(header));
    if (!checkGPTMagicNumber(header)) {//EFI PART
        return;
    }
    uint32_t entrySize = *(uint32_t*) (header + 0x54);
    uint64_t index = entrySize * partitionID + 512 * 2;
    uint64_t sectorIndex = index / 512;
    uint64_t offset = index % 512;
    uint8_t sector[512];
    device.read(sectorIndex, 1, sector);
    uint8_t* entry = sector + offset;

    memcpy(buffer, entry + 0x38, 72);
}

void getUniqueId(Device& device, uint16_t partitionID, uint8_t* buffer) {
    uint8_t header[512];
    readGPTHeader(device, header, sizeof(header));
    if (!checkGPTMagicNumber(header)) {//EFI PART
        return;
    }
    uint32_t entrySize = *(uint32_t*) (header + 0x54);
    uint64_t index = entrySize * partitionID + 512 * 2;
    uint64_t sectorIndex = index / 512;
    uint64_t offset = index % 512;
    uint8_t sector[512];
    device.read(sectorIndex, 1, sector);
    uint8_t* entry = sector + offset;

    memcpy(buffer, entry + 0x10, 16);
}
void setUniqueId(Device& device, uint16_t partitionID, uint8_t* buffer) {
    kout << "TODO setPartitionName\n";
}

uint64_t getGPTPartitionCount(Device& device) {
    uint8_t header[512];
    readGPTHeader(device, header, sizeof(header));
    if (!checkGPTMagicNumber(header)) {//EFI PART
        return 0;
    }
    return (uint64_t) (*(uint32_t*) (header + 0x50));
}

}// namespace Kernel