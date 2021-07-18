#pragma once

#include "PCI.hpp"

namespace Kernel {
class Ethernet {
public:
    static Ethernet* openController(PCI* dev, const PCICommonHeader& header);
};
}// namespace Kernel