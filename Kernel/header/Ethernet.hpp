#pragma once

#include "PCI.hpp"
#include "units.hpp"

namespace Kernel {
class Ethernet : public PCIDriver {
private:
    struct EthernetFrameStructProcInterface {
        uint64_t frameSize;
        uint8_t buffer[2Ki];
    };

public:
    static Ethernet* openController(PCI* dev, const PCICommonHeader& header);

    virtual void setOnLinkChange(void (*)(uint64_t), uint64_t data) = 0;
    virtual void setOnReceive(void (*)(uint64_t, bool), uint64_t data) = 0;
    virtual void setOnTransmit(void (*)(uint64_t, bool), uint64_t data) = 0;

    virtual bool transmitPacket(uint8_t* buffer, uint64_t size) = 0;
    virtual uint64_t getReceivedPacket(uint8_t* buffer, uint64_t size) = 0;
    virtual uint8_t getConnectionStatus() = 0;

    inline uint64_t getStatus() override {
        return getConnectionStatus() | (1llu << 63);
    }
    inline uint64_t getArgSize(uint8_t argNum) override {
        constexpr uint64_t argSizes[]{0, 1, sizeof(EthernetFrameStructProcInterface), sizeof(EthernetFrameStructProcInterface)};
        return argNum < (sizeof(argSizes) / sizeof(uint64_t)) ? argSizes[argNum] : 0;
    }
    inline void handleDriverCall(uint8_t argNum, void* arg) override {
        switch (argNum) {
            case 1:
                *(uint64_t*) arg = (uint64_t) getConnectionStatus();
                return;
            case 2: {
                EthernetFrameStructProcInterface* interface = (EthernetFrameStructProcInterface*) arg;
                interface->frameSize = getReceivedPacket(interface->buffer, 2Ki);
                return;
            }
            case 3: {
                EthernetFrameStructProcInterface* interface = (EthernetFrameStructProcInterface*) arg;
                interface->frameSize = (uint64_t) transmitPacket(interface->buffer, 2Ki);
                return;
            }
        }
    }
};
}// namespace Kernel