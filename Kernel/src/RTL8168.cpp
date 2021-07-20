#include "RTL8168.hpp"
#include "KernelOut.hpp"
#include "SharedInterrupts.hpp"
#include "units.hpp"

namespace Kernel {

struct RxDescriptor {
    uint32_t command;
    uint32_t vlan;
    uint32_t bufferLow;
    uint32_t bufferHigh;
};

bool RTL8168::isSupported(uint64_t vendor, uint64_t device) {
    uint64_t vendors[]{0x10ec, 0x10ec, 0x10ec, 0x1259, 0x1737, 0x16ec};
    uint64_t devices[]{0x8161, 0x8168, 0x8169, 0xc107, 0x1032, 0x0116};
    static_assert(sizeof(vendors) == sizeof(devices));
    for (uint64_t i = 0; i < sizeof(vendors) / sizeof(uint64_t); ++i) {
        if (vendor == vendors[i] && device == devices[i])
            return true;
    }
    return false;
}

static void onSharedInterrupt(void* me) {
    ((RTL8168*) me)->onInterrupt();
}

void RTL8168::onInterrupt() {
    uint16_t status = dev->bars[0].read16(0x3E);
    kout << "Ethernet interrupt status: " << BitList(status) << '\n';
    dev->bars[0].write16(0x3E, 0xFFFF);// enable all interrupts?
}

RTL8168::RTL8168(PCI* dev, const PCICommonHeader& header) {
    // reset

    if (!SharedInterrupt::configInterrupt(dev, header))
        return;

    RTL8168::dev = dev;
    dev->bars[0].write8(0x37, 0b1 << 4);
    dev->interrupt->setData(this);
    dev->interrupt->setInterruptFunction(onSharedInterrupt);
    while (dev->bars[0].read8(0x37) & (0b1 << 4)) {}

    ethernetID = 0;
    for (uint8_t i = 0; i < 6; ++i) {
        ((uint8_t*) &ethernetID)[i] = dev->bars[0].read8(i);
    }
    kout << "EthernetID: " << Hex(ethernetID) << '\n';

    uint8_t status = dev->bars[0].read8(0x6C);
    if (status & 0b1 << 1) {
        kout << "Ethernet connected with: ";
        if (status & 0b1 << 2) {
            kout << "10Mbps\n";
        } else if (status & 0b1 << 3) {
            kout << "100Mbps\n";
        } else if (status & 0b1 << 4) {
            kout << "1000Mbps\n";
        } else {
            kout << "Unknown speed\n";
        }
    } else {
        kout << "Ethernet not connected\n";
    }
    kout << "Ethernet status: " << BitList(status) << '\n';

    // setup rx pages
    for (uint8_t i = 0; i < rxPageCount; ++i) {
        uint64_t page = PhysicalMemoryManagment::getAnyPage();
        PhysicalMemoryManagment::setUsed(page, true);
        rxPages[i] = page;
    }

    // setup tx pages
    for (uint8_t i = 0; i < txPageCount; ++i) {
        uint64_t page = PhysicalMemoryManagment::getAnyPage();
        PhysicalMemoryManagment::setUsed(page, true);
        txPages[i] = page;
    }

    descriptorPage = PhysicalMemoryManagment::getAnyPage();
    PhysicalMemoryManagment::setUsed(descriptorPage, true);

    MemoryPage descPage(descriptorPage, true);
    descPage.mapTo(4Ti, true, false);
    RxDescriptor* descBase = (RxDescriptor*) (4Ti);

    //rx descriptor
    for (uint8_t i = 0; i < rxPageCount * 2; ++i) {
        uint64_t address = rxPages[i / 2] + (i % 2) * (pageSize / 2);
        descBase[i].bufferLow = (uint32_t) address;
        descBase[i].bufferHigh = (uint32_t) (address >> 32);
        descBase[i].vlan = 0;
        descBase[i].command = 0b1 << 31;
        descBase[i].command |= pageSize / 2;
        if (i == rxPageCount * 2 - 1) {
            descBase[i].command |= 0b1 << 30;
        }
    }
    rxBaseDesc = descriptorPage;

    //tx descriptor

    //256 byte alignment
    uint64_t txBaseOffsetIndex = rxPageCount * 2;
    uint64_t txBaseByteOffset = txBaseOffsetIndex * sizeof(RxDescriptor);
    if (txBaseByteOffset & 0xFF) {
        txBaseByteOffset = (txBaseByteOffset & 0xFF) + 0x100;
    }
    txBaseDesc = descriptorPage + txBaseByteOffset;
    RxDescriptor* txDescBase = (RxDescriptor*) (((uint8_t*) descBase) + txBaseByteOffset);
    for (uint8_t i = 0; i < txPageCount * 2; ++i) {
        uint64_t address = txPages[i / 2] + (i % 2) * (pageSize / 2);
        txDescBase[i].bufferLow = (uint32_t) address;
        txDescBase[i].bufferHigh = (uint32_t) (address >> 32);
        txDescBase[i].vlan = 0;
        txDescBase[i].command = 0;
        txDescBase[i].command |= pageSize / 2;
        if (i == txPageCount * 2 - 1) {
            txDescBase[i].command |= 0b1 << 30;
        }
    }

    dev->bars[0].write8(0x50, 0xC0);
    dev->bars[0].write32(0x44, 0x0000E70F);// unlimited rx and dma burst & accept all packets
    dev->bars[0].write8(0x37, 0x04);       // enable tx
    dev->bars[0].write32(0x40, 0x03000700);// tx in normal timing & unlimited dma burst
    dev->bars[0].write16(0xDA, 0x800);     // max rx packet size = 2Ki
    dev->bars[0].write8(0xEC, 0x10);       // max tx packet size = 2Ki
    dev->bars[0].write32(0x20, (uint32_t) txBaseDesc);
    dev->bars[0].write32(0x24, (uint32_t) (txBaseDesc >> 32));

    dev->bars[0].write32(0xE4, (uint32_t) rxBaseDesc);
    dev->bars[0].write32(0xE8, (uint32_t) (rxBaseDesc >> 32));

    dev->bars[0].write16(0x3C, 0b100101111);// enable interrupts for link change and tx/rx
    dev->bars[0].write16(0x3E, 0xFFFF);

    dev->bars[0].write8(0x37, 0b1100);// enable tx/rx
    dev->bars[0].write8(0x50, 0x00);  // lock config and start operation

    kout << "Ethernet started\n";
}

}// namespace Kernel