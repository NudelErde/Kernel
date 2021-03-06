#pragma once

#include "MassStorage.hpp"
#include "PCI.hpp"

namespace Kernel {

class AHCI {
public:
    inline explicit AHCI(PCI::BAR& bar) : ABAR(bar) {}

    struct CommandSlot;
    struct Port;
    struct PrdtEntry;
    struct CommandTable;
    struct ReceivedFIS;

    class AHCIDevice : public Device {
    private:
        enum class DeviceType {
            SATA,
            SATAPI,
            SEMB,
            PM,
            UNKNOWN
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
        bool sendIdentify();
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
        uint64_t getSectorCount() override;
    };

    void onInterrupt();
    void setup();
    void reset();
    void tryDevice(uint8_t device);
    static void openController(PCI* device, const PCICommonHeader& header);

private:
    PCI* dev;
    PCI::BAR& ABAR;
    uint32_t capabilities;
    uint32_t capabilities2;
    uint32_t portsAvailable;
    uint32_t version;
    AHCIDevice* devices[32];
};

}// namespace Kernel