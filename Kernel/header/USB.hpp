#pragma once
#include "PCI.hpp"

namespace Kernel {

class USB : public PCIDriver {
public:
    static USB* openController(PCI* dev, const Kernel::PCICommonHeader& header);
};

}// namespace Kernel