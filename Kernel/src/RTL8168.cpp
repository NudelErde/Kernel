#include "RTL8168.hpp"
#include "KernelOut.hpp"
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

RTL8168::RTL8168(PCI* dev) {
    kout << "Ethernet\n";
    ethernetID = 0;
    for (uint8_t i = 0; i < 6; ++i) {
        ((uint8_t*) &ethernetID)[i] = dev->bars[0].read8(i);
    }
    kout << "EthernetID: " << Hex(ethernetID) << '\n';
    dev->bars[0].write8(0x37, 0b1 << 4);
    while (dev->bars[0].read8(0x37) & (0b1 << 4)) {}

    uint8_t status = dev->bars[0].read8(0x6C);
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

    //tx descriptor
    for (uint8_t i = 0; i < txPageCount * 2; ++i) {
        uint64_t address = txPages[i / 2] + (i % 2) * (pageSize / 2);
        descBase[i + rxPageCount * 2].bufferLow = (uint32_t) address;
        descBase[i + rxPageCount * 2].bufferHigh = (uint32_t) (address >> 32);
        descBase[i + rxPageCount * 2].vlan = 0;
        descBase[i + rxPageCount * 2].command = 0;
        descBase[i + rxPageCount * 2].command |= pageSize / 2;
        if (i == rxPageCount * 2 - 1) {
            descBase[i + rxPageCount * 2].command |= 0b1 << 30;
        }
    }
}

}// namespace Kernel