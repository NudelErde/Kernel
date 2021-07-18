#include "XHCI.hpp"
#include "KernelOut.hpp"
#include "SharedInterrupts.hpp"
#include "inout.hpp"
#include "units.hpp"
#include "wait.hpp"

namespace Kernel {

static void onSharedInterrupt(void* me) {
    XhciUsbController* self = (XhciUsbController*) me;
    self->onInterrupt();
}

void XhciUsbController::onInterrupt() {
    kout << "COCK\n";
    asm("hlt");
}

XhciUsbController::XhciUsbController(PCI* dev, const Kernel::PCICommonHeader& header) {
    return;
    if ((header.headerType & (~0x80)) != 0x00)
        return;
    if (!SharedInterrupt::configInterrupt(dev, header)) {
        return;
    }
    dev->load();
    dev->interrupt->setData(this);
    dev->interrupt->setInterruptFunction(onSharedInterrupt);

    XhciUsbController::dev = dev;

    structural1 = dev->bars[0].read32(0x04);
    structural2 = dev->bars[0].read32(0x08);
    structural3 = dev->bars[0].read32(0x0C);
    capability1 = dev->bars[0].read32(0x10);
    capability2 = dev->bars[0].read32(0x1C);

    operationalBase = dev->bars[0].read8(0);
    doorbellBase = dev->bars[0].read32(0x14);
    runtimeBase = dev->bars[0].read32(0x18);

    if (!(capability1 & 0b1)) {
        kout << "[XHCI] No 64 bit mode\n";
        return;
    }

    // wait for Controller Ready
    constexpr uint32_t controlNotReady = (0x1 << 11);
    while (dev->bars[0].read32(operationalBase + 0x04) & controlNotReady) {}

    // set max device slots enabled
    uint32_t configRegisterValue = dev->bars[0].read32(operationalBase + 0x38);
    uint8_t maxDeviceSlotsEnabled = structural1 & 0xFF;
    //kout << "[XHCI] MaxSlots: 0x" << Hex(maxDeviceSlotsEnabled) << '\n';
    dev->bars[0].write32(operationalBase + 0x38, (configRegisterValue & ~(0xFF)) | maxDeviceSlotsEnabled);

    //allocate device context base address array
    uint64_t page = PhysicalMemoryManagment::getAnyPage();
    PhysicalMemoryManagment::setUsed(page, true);
    deviceContextBaseAddressArrayPointer = page;
    MemoryPage tmp(deviceContextBaseAddressArrayPointer, true);
    tmp.mapTo(deviceContextBaseAddressArrayPointer + 2Ti, true, false);
    memset((void*) tmp.getVirtualAddress(), 0, 2Ki);
    dev->bars[0].write64(operationalBase + 0x30, deviceContextBaseAddressArrayPointer);

    //setup Scratchpad buffer
    uint64_t scratchpadSize = ((structural2 >> 27) & 0x1F) | ((structural2 >> 21) & 0x1F) << 5;
    if (scratchpadSize > 0) {
        kout << "[XHCI] Scratchpad is not supported\n";
        return;
    }

    // setup command Ring
    bool CCS = 0;

    uint64_t commandRingSegmentPointers[1]{};
    commandRingSegmentPointers[0] = deviceContextBaseAddressArrayPointer + 2Ki;
    uint64_t commandRingSegmentSizes[1]{};
    commandRingSegmentSizes[0] = 2Ki;
    new (&commandRing) Ring(commandRingSegmentPointers, commandRingSegmentSizes, 1, !CCS);
    if (!commandRing.isValid()) {
        kout << "[XHCI] Error initializing the command ring";
        return;
    }
    // register command ring
    dev->bars[0].write64(operationalBase + 0x18, 0b10);
    while (dev->bars[0].read64(operationalBase + 0x18) & 0b1000) {}// wait for finish
    dev->bars[0].write64(operationalBase + 0x18, (commandRingSegmentPointers[0] & ~0b1) | CCS);

    uint64_t maxInterrupter = (structural1 >> 8) & ((0b1 << 10) - 1);

    // event ring stuff
    for (uint64_t i = 0; i < maxInterrupter; ++i) {
        // disable interrupters
        dev->bars[0].write64(runtimeBase + 0x20 + 32 * i, 0b00);
    }

    for (uint64_t i = 0; i < maxInterrupter; ++i) {
        // disable interrupters
        dev->bars[0].write64(runtimeBase + 0x20 + 32 * i, 0b00);
    }

    // init event ring 0
    uint64_t eventRingPage = PhysicalMemoryManagment::getAnyPage();
    PhysicalMemoryManagment::setUsed(eventRingPage, true);
    MemoryPage tmpEventRingPageObject(eventRingPage, true);
    tmpEventRingPageObject.mapTo(eventRingPage + 2Ti, true, false);
    memset((void*) tmpEventRingPageObject.getVirtualAddress(), 0, pageSize);
    uint64_t eventRingSegmentTable = eventRingPage - 16 + pageSize;
    *(uint64_t*) (eventRingSegmentTable + 2Ti) = eventRingPage;
    *(uint16_t*) (eventRingSegmentTable + 2Ti + 8) = (pageSize - 16) / 16;

    dev->bars[0].write32(runtimeBase + 0x28, 0x1);
    dev->bars[0].write64(runtimeBase + 0x38, eventRingPage);
    dev->bars[0].write64(runtimeBase + 0x30, eventRingSegmentTable);

    dev->bars[0].write32(operationalBase, dev->bars[0].read32(operationalBase) | 0b101);// start

    commandRing.insert(0, 0, (23 << 10));
    // ring doorbell
    dev->bars[0].write32(doorbellBase, 0);
    commandRing.run();
}

XhciUsbController::Ring::Ring() {
    valid = false;
}

XhciUsbController::Ring::Ring(uint64_t* pointers, uint64_t* sizes, uint64_t count, bool CCS) {
    Ring::CCS = CCS;
    queueStart = pointers[0] + 2 * 1024Gi;
    for (uint64_t i = 0; i < count; ++i) {
        uint64_t address = pointers[i];
        uint64_t size = sizes[i];
        entryCount += (size / 16) - 1;

        uint64_t next;
        if (i + 1 == count) {
            next = pointers[0];
        } else {
            next = pointers[i + 1];
        }

        if (next & 0xF) {
            valid = false;
            return;// invalid pointer
        }
        // map in tmp ram
        MemoryPage tmp(address, true);
        tmp.mapTo(address + 2 * 1024Gi, true, false);
        uint8_t* pointer = (uint8_t*) tmp.getVirtualAddress();
        memset(pointer, 0, size);

        uint8_t* lastEntry = pointer + size - 16;
        LinkElement* link = (LinkElement*) lastEntry;
        link->segmentPointer = next;
        link->status = 0;               // interrupt target 0
        link->control = (6 << 10) | CCS;// CCS -> xHC owns this entry
    }
    valid = true;
}

void XhciUsbController::Ring::insert(uint64_t parameter, uint32_t status, uint32_t control) {
    uint32_t* pointer = (uint32_t*) pointerOfIndex(enqueueIndex);
    *(uint64_t*) pointer = parameter;
    pointer[2] = status;
    pointer[3] = (control & ~0b1) | CCS;
    ++enqueueIndex;
}

void XhciUsbController::Ring::run() {
    dequeueIndex = enqueueIndex;
}

bool XhciUsbController::Ring::isFull() {
    return (enqueueIndex + 1) == dequeueIndex;
}

uint8_t* XhciUsbController::Ring::pointerOfIndex(uint64_t index) {
    uint8_t* ptr = (uint8_t*) queueStart;
    for (uint64_t i = 0; i < index; ++i) {
        if (((LinkElement*) ptr)->control & (6 << 10)) {// is link
            ptr = (uint8_t*) ((LinkElement*) ptr)->segmentPointer;
        } else {
            ptr = ptr + 16;
        }
    }
    return ptr;
}

}// namespace Kernel