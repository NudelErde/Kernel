#include "Ethernet.hpp"
#include "RTL8168.hpp"

namespace Kernel {

Ethernet* Ethernet::openController(PCI* dev, const PCICommonHeader& header) {
    if (RTL8168::isSupported(header.vendorID, header.deviceID)) {
        return new RTL8168(dev);
    }
    return nullptr;
}

}// namespace Kernel