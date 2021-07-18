#include "USB.hpp"
#include "KernelOut.hpp"
#include "SharedInterrupts.hpp"
#include "UHCI.hpp"
#include "XHCI.hpp"
#include "inout.hpp"
#include "wait.hpp"

namespace Kernel {

USB* USB::openController(PCI* dev, const PCICommonHeader& header) {
    switch (header.progIF) {
        case 0x00:
            // not implemented (uhci sucks)
            //return new UhciUsbController(dev->, header);
            return nullptr;
        case 0x30:
            // not implemented (much work :c)
            return new XhciUsbController(dev, header);
        default:
            return nullptr;
    }
}

}// namespace Kernel