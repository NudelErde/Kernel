#pragma once

#include "Ethernet.hpp"

namespace Kernel {
class RTL8168 : public Ethernet {
public:
    RTL8168(PCI* dev, const PCICommonHeader& header);

    void onInterrupt();

    virtual void setOnLinkChange(void (*)(uint64_t), uint64_t data);
    virtual void setOnReceive(void (*)(uint64_t, bool), uint64_t data);
    virtual void setOnTransmit(void (*)(uint64_t, bool), uint64_t data);

    virtual bool transmitPacket(uint8_t* buffer, uint64_t size);
    virtual uint64_t getReceivedPacket(uint8_t* buffer, uint64_t size);
    virtual uint8_t getConnectionStatus();

    static bool isSupported(uint64_t vendor, uint64_t device);

private:
    constexpr static uint64_t rxPageCount = 4;
    constexpr static uint64_t txPageCount = 4;

    void (*linkChangeHandler)(uint64_t);
    uint64_t linkChangeHandlerData;
    void (*rxHandler)(uint64_t, bool);
    uint64_t rxHandlerData;
    void (*txHandler)(uint64_t, bool);
    uint64_t txHandlerData;

    uint64_t ethernetID;
    uint64_t descriptorPage;
    uint64_t rxBaseDesc;
    uint64_t txBaseDesc;
    uint64_t rxPages[rxPageCount];
    uint64_t txPages[txPageCount];
    PCI* dev;
};

}// namespace Kernel