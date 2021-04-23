#pragma once
#include "stdint.h"

namespace Kernel{
    constexpr uint64_t PCI_CONFIG_ADDRESS = 0xCF8;
    constexpr uint64_t PCI_CONFIG_DATA = 0xCFC;

    struct ConfigAddressRegister{
        uint8_t registerOffset: 8;
        uint8_t functionNumber: 3;
        uint8_t deviceNumber: 5;
        uint8_t busNumber: 8;
        uint8_t reserved: 7;
        uint8_t enableBit: 1;
    }__attribute__((__packed__));

    struct PCICommonHeader{
        uint16_t vendorID;
        uint16_t deviceID;
        uint16_t command;
        uint16_t status;
        uint8_t revisionID;
        uint8_t progIF;
        uint8_t subclass;
        uint8_t classCode;
        uint8_t cacheLineSize;
        uint8_t latencyTimer;
        uint8_t headerType;
        uint8_t BIST;
    };

    class PCI {
    public:
        static uint32_t configReadWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
        static void readCommonHeader(PCICommonHeader& header, uint8_t bus, uint8_t device, uint8_t function);
        static void checkDevice(uint8_t bus, uint8_t device);
        static void checkFunction(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header);
        static void checkBus(uint8_t bus);
        static void checkAllBuses();
    };
}