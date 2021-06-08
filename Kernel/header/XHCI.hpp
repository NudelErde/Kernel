#pragma once
#include "USB.hpp"

namespace Kernel {

class XhciUsbController : public USB {
public:
    XhciUsbController(PCI* dev, const Kernel::PCICommonHeader& header);
    void onInterrupt();
};

}// namespace Kernel