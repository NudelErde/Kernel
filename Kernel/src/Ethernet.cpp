#include "Ethernet.hpp"
#include "RTL8168.hpp"

namespace Kernel {

Ethernet* Ethernet::openController(PCI* dev, const PCICommonHeader& header) {
    if (header.deviceID == 0x8168 && header.vendorID == 0x10ec) {
        return new RTL8168(dev);
    }
}

}// namespace Kernel