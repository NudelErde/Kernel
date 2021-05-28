#pragma once

#include "PCI.hpp"
#include "MassStorage.hpp"

namespace Kernel {

class AHCI{
public:
    
    struct CommandSlot;
    struct Port;
    struct PrdtEntry;
    struct CommandTable;
    struct ReceivedFIS;

    class AHCIDevice: public Device {
    private:
        enum class DeviceType {
            SATA, SATAPI, SEMB, PM, UNKNOWN
        };
        DeviceType type;
        volatile Port* port;
        AHCI* controller;
        CommandSlot* commandList;
        CommandTable* commandTables;
        ReceivedFIS* receivedFIS;

        uint64_t sectorCount;
        bool hasLBA48;
        
        void setup();
        void start();
        void stop();
        void sendIdentify();
        uint64_t findSlot();
        bool waitForFinish();
        bool waitForTask(uint32_t slot);
        void onInterrupt();
        void writeH2DCommand(uint32_t slot, uint8_t command, uint64_t sectorIndex, uint64_t sectorCount);
        friend class AHCI;
    public:
        AHCIDevice(void* port, AHCI* controller);

        void read(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* dest) override;
        void write(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* src) override;
        void flush() override;
    };

    void onInterrupt();
    void setup();
    void reset();
    void tryDevice(uint8_t device);
    static void openController(uint8_t bus, uint8_t device, uint8_t func, const PCICommonHeader& header);

private:
    uint8_t bus;
    uint8_t device;
    uint8_t func;
    volatile uint32_t* ABAR;
    uint32_t capabilities;
    uint32_t capabilities2;
    uint32_t portsAvailable;
    uint32_t version;
    AHCIDevice* devices[32];
};

}