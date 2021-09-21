#include "Partition.hpp"
#include "GPT.hpp"
#include "KernelOut.hpp"
#include "endian.hpp"

namespace Kernel {
static void LBA2CHS(uint64_t LBA, uint8_t* result) {
    //source: https://www.viralpatel.net/taj/tutorial/chs_translation.php

    uint64_t cylinder = LBA / (256 * 63);
    uint64_t temp = LBA % (256 * 63);
    uint64_t head = temp / 63;
    uint64_t sector = temp % 63 + 1;

    //source: https://upload.wikimedia.org/wikipedia/commons/1/1e/Chs.svg

    result[0] = (uint8_t) (head & 0xFF);
    result[1] = (uint8_t) (sector & 0b00111111);
    result[1] |= (uint8_t) ((cylinder & 0b1100000000) >> 2);
    result[2] = (uint8_t) (cylinder & 0xFF);
}
void createMBR(Device& device) {
    char MasterBootRecordSector[512]{};
    MasterBootRecordSector[510] = 0x55;
    MasterBootRecordSector[511] = 0xAA;
    device.write(0, 1, (uint8_t*) MasterBootRecordSector);
    device.flush();
}
uint64_t getPartitionCount(Device& device) {
    if (checkIfGPT(device)) {
        return getGPTPartitionCount(device);
    }
    return 4;
}
void setPartition(Device& device, uint16_t partitionID, uint64_t start, uint64_t end, uint64_t type[2], bool bootable) {
    if (checkIfGPT(device)) {
        setGPTPartition(device, partitionID, start, end, type);
        return;
    }
    uint8_t MasterBootRecordSector[512]{};
    device.read(0, 1, (uint8_t*) MasterBootRecordSector);
    if (!(MasterBootRecordSector[510] == 0x55 && MasterBootRecordSector[511] == 0xAA)) {
        return;
    }
    uint16_t index = 0x01BE;
    index += partitionID * 0xF;

    MasterBootRecordSector[index] = (bootable ? 0x80 : 0x00);
    if (start >= 8455716864llu) {//max size for CHS
        MasterBootRecordSector[index + 1] = 0xFF;
        MasterBootRecordSector[index + 2] = 0xFF;
        MasterBootRecordSector[index + 3] = 0xFF;
    } else {
        LBA2CHS(start, (MasterBootRecordSector + 1));
    }

    MasterBootRecordSector[index + 4] = (uint8_t) type[0];

    if (end >= 8455716864llu) {//max size for CHS
        MasterBootRecordSector[index + 5] = 0xFF;
        MasterBootRecordSector[index + 6] = 0xFF;
        MasterBootRecordSector[index + 7] = 0xFF;
    } else {
        LBA2CHS(end, (MasterBootRecordSector + 5));
    }

    MasterBootRecordSector[index + 8] = (uint8_t) (start & 0xFF);
    MasterBootRecordSector[index + 9] = (uint8_t) ((start >> 8) & 0xFF);
    MasterBootRecordSector[index + 10] = (uint8_t) ((start >> 16) & 0xFF);
    MasterBootRecordSector[index + 11] = (uint8_t) ((start >> 24) & 0xFF);

    uint64_t partitionSize = end - start;

    MasterBootRecordSector[index + 12] = (uint8_t) (partitionSize & 0xFF);
    MasterBootRecordSector[index + 13] = (uint8_t) ((partitionSize >> 8) & 0xFF);
    MasterBootRecordSector[index + 14] = (uint8_t) ((partitionSize >> 16) & 0xFF);
    MasterBootRecordSector[index + 15] = (uint8_t) ((partitionSize >> 24) & 0xFF);

    device.write(0, 1, MasterBootRecordSector);
    device.flush();
}

Partition getPartition(Device& device, uint16_t partitionID) {
    if (checkIfGPT(device)) {
        return getGPTPartition(device, partitionID);
    }
    Partition part;
    part.index = partitionID;
    uint8_t MasterBootRecordSector[512]{};
    device.read(0, 1, MasterBootRecordSector);
    if (!(MasterBootRecordSector[510] == 0x55 && MasterBootRecordSector[511] == 0xAA)) {
        return {};
    }
    uint16_t index = 0x01BE;
    index += partitionID * 0xF;
    part.bootable = (MasterBootRecordSector[index + 0] == 0x80);
    part.type = MasterBootRecordSector[index + 4];
    part.LBAStart = fromLittleEndian(*(uint32_t*) (MasterBootRecordSector + index + 0x8));
    part.sectorCount = fromLittleEndian(*(uint32_t*) (MasterBootRecordSector + index + 0xC));
    part.gpt = false;
    return part;
}

struct FileSystemId {
    uint64_t deviceId;
    uint64_t partitionId;
};
static FileSystemId fileSystemList[128];
static uint64_t systemFileSystem;
static uint64_t fileSystemCount;

void scanDevicePartitions() {
    fileSystemCount = 0;
    systemFileSystem = 0;
    uint64_t deviceCount = Device::getDeviceCount();
    for (uint64_t deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex) {
        Device* device = Device::getDevice(deviceIndex);
        if (device->getSectorCount() > 0) {
            uint64_t partitionCount = getPartitionCount(*device);
            for (uint64_t partitionIndex = 0; partitionIndex < partitionCount; ++partitionIndex) {
                Partition p = getPartition(*device, partitionIndex);
                if (!p.gpt && p.type == 0) {
                    continue;
                }
                if (p.gpt && getGUID(p.gptType).key == GUID_KEY::UNKNOWN) {
                    if ((p.gptType[0] != 0 || p.gptType[1] != 0)) {
                        kout << Hex(deviceIndex) << ':' << Hex(partitionIndex) << " UNKNOWN ";
                        for (uint8_t i = 0; i < 16; ++i) {
                            kout << Hex(((uint8_t*) p.gptType)[i], 2);
                        }
                        kout << '\n';
                    }
                    continue;
                }
                fileSystemList[fileSystemCount].deviceId = deviceIndex;
                fileSystemList[fileSystemCount].partitionId = partitionIndex;
                if (p.gpt) {
                    kout << Hex(deviceIndex) << ':' << Hex(partitionIndex) << ' ' << getGUID(p.gptType).name << '\n';
                }
                if ((!p.gpt && p.type == 83) || (p.gpt && getGUID(p.gptType).key == GUID_KEY::LINUX_NATIVE)) {
                    systemFileSystem = fileSystemCount;
                }
                fileSystemCount++;
            }
        }
    }
}
uint64_t getSystemFileSystem() {
    return systemFileSystem;
}
uint64_t getFileSystemCount() {
    return fileSystemCount;
}
uint64_t getDeviceForFileSystem(uint64_t id) {
    return fileSystemList[id].deviceId;
}
uint64_t getPartitionForFileSystem(uint64_t id) {
    return fileSystemList[id].partitionId;
}

}// namespace Kernel