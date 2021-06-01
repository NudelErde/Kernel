#include "USB.hpp"
#include "inout.hpp"
#include "KernelOut.hpp"
#include "wait.hpp"
#include "SharedInterrupts.hpp"
#include "UHCI.hpp"
#include "XHCI.hpp"

namespace Kernel {

USB* USB::openController(PCI* dev, const PCICommonHeader& header) {
    switch (header.progIF) {
    case 0x00:
        //return new UhciUsbController(dev->, header);
        return nullptr;
    case 0x30:
        return new XhciUsbController(dev, header);
    default:
        return nullptr;
    }
}

}