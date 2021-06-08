#include "Partition.hpp"
#include "endian.hpp"

#include "KernelOut.hpp"

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
void setPartition(Device& device, uint8_t partitionID, uint64_t start, uint64_t end, uint8_t type, bool bootable) {
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

    MasterBootRecordSector[index + 4] = type;

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

Partition getPartition(Device& device, uint8_t partitionID) {
    Partition part;
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
    return part;
}
}// namespace Kernel