#pragma once

#include "Ethernet.hpp"

namespace Kernel {
class RTL8168 : public Ethernet {
public:
    RTL8168(PCI* dev);

    static bool isSupported(uint64_t vendor, uint64_t device);

private:
    constexpr static uint64_t rxPageCount = 4;
    constexpr static uint64_t txPageCount = 4;

    uint64_t ethernetID;
    uint64_t descriptorPage;
    uint64_t rxBaseDesc;
    uint64_t txBaseDesc;
    uint64_t rxPages[rxPageCount];
    uint64_t txPages[txPageCount];
};

}// namespace Kernel