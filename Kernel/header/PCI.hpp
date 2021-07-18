#pragma once
#include "List.hpp"
#include "stdint.h"

namespace Kernel {
class SharedInterrupt;

constexpr uint64_t PCI_CONFIG_ADDRESS = 0xCF8;
constexpr uint64_t PCI_CONFIG_DATA = 0xCFC;

struct ConfigAddressRegister {
    uint8_t registerOffset : 8;
    uint8_t functionNumber : 3;
    uint8_t deviceNumber : 5;
    uint8_t busNumber : 8;
    uint8_t reserved : 7;
    uint8_t enableBit : 1;
} __attribute__((__packed__));

struct PCICommonHeader {
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

struct PCIDeviceData {
    uint8_t bus;
    uint8_t device;
    uint8_t function;
};

class PCI {
public:
    struct BAR {
        struct BARAccess {
            BAR* bar;
            uint64_t offset;

            inline uint32_t operator=(uint32_t data) {
                bar->write32((uint32_t) offset, data);
                return data;
            }

            inline operator uint32_t() {
                return bar->read32((uint32_t) offset);
            }
        };
        bool valid;
        bool io;
        uint8_t type;
        uint64_t size;
        uint64_t address;
        uint64_t virtualAddress;
        void setup(PCI* pci, uint8_t barID);
        void write8(uint32_t offset, uint8_t data);
        void write16(uint32_t offset, uint16_t data);
        void write32(uint32_t offset, uint32_t data);
        void write64(uint32_t offset, uint64_t data);
        uint8_t read8(uint32_t offset);
        uint16_t read16(uint32_t offset);
        uint32_t read32(uint32_t offset);
        uint64_t read64(uint32_t offset);

        inline BARAccess operator[](uint64_t index) {
            return {this, index * 4};
        }
    };
    void load();
    void writeConfig(uint32_t offset, uint32_t data);
    uint32_t readConfig(uint32_t offset);

    uint8_t selfTest();

    uint8_t bus;
    uint8_t device;
    uint8_t function;
    SharedInterrupt* interrupt;
    BAR bars[6];

public:
    static uint32_t configReadWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
    static void configWriteWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t data);
    static void readCommonHeader(PCICommonHeader& header, uint8_t bus, uint8_t device, uint8_t function);
    inline static void readCommonHeader(PCICommonHeader& header, PCI* data) { readCommonHeader(header, data->bus, data->device, data->function); }
    static void checkDevice(uint8_t bus, uint8_t device, LinkedList<PCI>&);
    static void checkFunction(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header, LinkedList<PCI>&);
    static void checkBus(uint8_t bus, LinkedList<PCI>&);
    static LinkedList<PCI> checkAllBuses();
    static void writeBAR8(uint64_t BAR, uint32_t offset, uint8_t data);
    static void writeBAR16(uint64_t BAR, uint32_t offset, uint16_t data);
    static void writeBAR32(uint64_t BAR, uint32_t offset, uint32_t data);
    static uint8_t readBAR8(uint64_t BAR, uint32_t offset);
    static uint16_t readBAR16(uint64_t BAR, uint32_t offset);
    static uint32_t readBAR32(uint64_t BAR, uint32_t offset);
};
}// namespace Kernel