#include "XHCI.hpp"
#include "KernelOut.hpp"
#include "SharedInterrupts.hpp"
#include "inout.hpp"
#include "wait.hpp"

namespace Kernel {

static void onSharedInterrupt(void* me) {
    XhciUsbController* self = (XhciUsbController*) me;
    self->onInterrupt();
}

void XhciUsbController::onInterrupt() {
    kout << "COCK\n";
}

XhciUsbController::XhciUsbController(PCI* dev, const Kernel::PCICommonHeader& header) {
    if ((header.headerType & (~0x80)) != 0x00)
        return;
    if (!SharedInterrupt::configInterrupt(dev, header)) {
        return;
    }
    dev->writeConfig(0x04, dev->readConfig(0x04) | 0b111);// enable BAR
    return;
    dev->interrupt->setData(this);
    dev->interrupt->setInterruptFunction(onSharedInterrupt);
}

}// namespace Kernel