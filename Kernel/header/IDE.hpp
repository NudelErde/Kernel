#pragma once
#include "stdint.h"
#include "PCI.hpp"
#include "inout.hpp"
#include "MassStorage.hpp"

namespace Kernel {

namespace ATA {

constexpr uint8_t StatusBusy = 0x80;
constexpr uint8_t StatusDriveReady = 0x40;
constexpr uint8_t StatusWriteFault = 0x20;
constexpr uint8_t StatusSeekComplete = 0x10;
constexpr uint8_t StatusDataRequestReady = 0x08;
constexpr uint8_t StatusCorrectedData = 0x04;
constexpr uint8_t StatusIndex = 0x02;
constexpr uint8_t StatusError = 0x01;

constexpr uint8_t ErrorBadBlock = 0x80;
constexpr uint8_t ErrorUncorrectableData = 0x40;
constexpr uint8_t ErrorMediaChanged = 0x20;
constexpr uint8_t ErrorIDMarkNotFound = 0x10;
constexpr uint8_t ErrorMediaChangeRequest = 0x08;
constexpr uint8_t ErrorCommandAborted = 0x04;
constexpr uint8_t ErrorTrack0NotFound = 0x02;
constexpr uint8_t ErrorNoAddressMark = 0x01;

constexpr uint8_t CommandReadPIO = 0x20;
constexpr uint8_t CommandReadPIOExt = 0x24;
constexpr uint8_t CommandReadDMA = 0xC8;
constexpr uint8_t CommandReadDMAExt = 0x25;
constexpr uint8_t CommandWritePIO = 0x30;
constexpr uint8_t CommandWritePIOExt = 0x34;
constexpr uint8_t CommandWriteDMA = 0xCA;
constexpr uint8_t CommandWriteDMAExt = 0x35;
constexpr uint8_t CommandCacheFlush = 0xE7;
constexpr uint8_t CommandCacheFlushExt = 0xEA;
constexpr uint8_t CommandPacket = 0xA0;
constexpr uint8_t CommandIdentifyPacket = 0xA1;
constexpr uint8_t CommandIdentify = 0xEC;

constexpr uint8_t ATAPICommandRead = 0xA8;
constexpr uint8_t ATAPICommandEject = 0x1B;

constexpr uint8_t IdentitfyDeviceType = 0;
constexpr uint8_t IdentitfyCylinders = 2;
constexpr uint8_t IdentitfyHeads = 6;
constexpr uint8_t IdentitfySector = 12;
constexpr uint8_t IdentitfySerial = 20;
constexpr uint8_t IdentitfyModel = 54;
constexpr uint8_t IdentitfyCapabilities = 98;
constexpr uint8_t IdentitfyFieldValid = 106;
constexpr uint8_t IdentitfyMaxLBA = 120;
constexpr uint8_t IdentitfyCommandsets = 164;
constexpr uint8_t IdentitfyMaxLBAExt = 200;

constexpr uint8_t ATA = 0x00;
constexpr uint8_t ATAPI = 0x01;

constexpr uint8_t MASTER = 0x00;
constexpr uint8_t SLAVE = 0x01;

class Channel {
public:
    Channel() = default;
    Channel(uint32_t BAR0, uint32_t BAR1);

    void readBuffer(uint8_t* buffer, uint64_t size);

    uint16_t readIO16(uint8_t offset) {
        return PCI::readBAR16(ioBase, offset);
    }
    uint16_t readControl16(uint8_t offset) {
        return PCI::readBAR16(controlBase, offset);
    }
    uint8_t readIO8(uint8_t offset) {
        return PCI::readBAR8(ioBase, offset);
    }
    uint8_t readControl8(uint8_t offset) {
        return PCI::readBAR8(controlBase, offset);
    }

    inline void writeIO16(uint8_t offset, uint16_t data) {
        PCI::writeBAR16(ioBase, offset, data);
    }
    inline void writeControl16(uint8_t offset, uint16_t data) {
        PCI::writeBAR16(controlBase, offset, data);
    }
    inline void writeIO8(uint8_t offset, uint8_t data) {
        PCI::writeBAR8(ioBase, offset, data);
    }
    inline void writeControl8(uint8_t offset, uint8_t data) {
        PCI::writeBAR8(controlBase, offset, data);
    }

private:

    uint32_t ioBase;
    uint32_t controlBase;
};

class ATADevice : public Device{
public:
    virtual ~ATADevice();
    uint16_t signature;
    uint16_t capabilities;
    uint32_t commandSet;
    uint8_t model[41];
    uint64_t sectorCount;
    bool isATAPI;
    Channel channel;
    bool isSlave;
    bool lba48Support;
    
    void read(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* dest) override;
    void write(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* src) override;
    void flush() override;
};

void openController(uint8_t bus, uint8_t device, uint8_t func, const PCICommonHeader& header);

}
    
}