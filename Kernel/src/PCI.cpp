#include "PCI.hpp"
#include "stdint.h"
#include "inout.hpp"
#include "print.hpp"
#include "IDE.hpp"
#include "kernelMem.hpp"
#include "units.hpp"
#include "KernelOut.hpp"

namespace Kernel{

static uint64_t lastBarVirtualAddress;

void PCI::BAR::setup(PCI* pci, uint8_t barID) {
    if(lastBarVirtualAddress == 0) {
        lastBarVirtualAddress = 1024Gi;
    }
    uint32_t bar = pci->readConfig(0x10 + barID * 4);
    if(bar == 0) {
        valid = false;
        return;
    }
    valid = true;
    if(bar & 0b1) {
        io = true;
        address = bar & ~0b11;
    } else {
        io = false;
        uint8_t type = (uint8_t)((bar >> 1) & 0b11);
        address = (bar & ~0b1111);
        uint32_t size = 0;
        pci->writeConfig(0x10 + barID * 4, ~0);
        size = pci->readConfig(0x10 + barID * 4);
        pci->writeConfig(0x10 + barID * 4, bar);
        size = ~size;
        size += 1;
        if(type == 0x02) {
            // 64 bit register
            uint64_t addressH = pci->readConfig(0x10 + (barID + 1) * 4);
            address |= (addressH << 32);
        }
        if(address > 1Gi) {
            // memory map address
            uint64_t pageCount = ((size - 1) / pageSize) + 1;
            size = pageCount * pageSize;
            virtualAddress = lastBarVirtualAddress;

            lastBarVirtualAddress += size + pageSize; // extra empty page between two BARs -> interrupt if access outside of bounds
            
            for(uint64_t i = 0; i < pageCount; ++i) {
                uint64_t vaddress = virtualAddress + (i * pageSize);
                uint64_t paddress = address + (i * pageSize);
                MemoryPage page(paddress, true);
                bool success = page.mapTo(vaddress, true, false, true, true);
                if(!success) {
                    kout << "Error while mapping virtual address 0x" << Hex(vaddress) << " to physical address 0x" << Hex(paddress) << '\n';
                    asm("hlt");
                }
            }
        } else {
            virtualAddress = address;
        }
    }
}
void PCI::BAR::write8(uint32_t offset, uint8_t data) {
    if(!valid)
        return;
    if(io) {
        outb(address + offset, data);
    } else {
        *(uint8_t*)(virtualAddress + offset) = data;
    }
}
void PCI::BAR::write16(uint32_t offset, uint16_t data) {
    if(!valid)
        return;
    if(io) {
        out16(address + offset, data);
    } else {
        *(uint16_t*)(virtualAddress + offset) = data;
    }
}
void PCI::BAR::write32(uint32_t offset, uint32_t data) {
    if(!valid)
        return;
    if(io) {
        outl(address + offset, data);
    } else {
        *(uint32_t*)(virtualAddress + offset) = data;
    }
}
uint8_t PCI::BAR::read8(uint32_t offset) {
    if(!valid)
        return 0;
    if(io) {
        return inb(address + offset);
    } else {
        return *(uint8_t*)(virtualAddress + offset);
    }
}
uint16_t PCI::BAR::read16(uint32_t offset) {
    if(!valid)
        return 0;
    if(io) {
        return in16(address + offset);
    } else {
        return *(uint16_t*)(virtualAddress + offset);
    }
}
uint32_t PCI::BAR::read32(uint32_t offset) {
    if(!valid)
        return 0;
    if(io) {
        return inl(address + offset);
    } else {
        uint32_t value = *(uint32_t*)(virtualAddress + offset);
        return value;
    }
}

uint8_t PCI::selfTest() {
    uint32_t word = readConfig(0x0C);
    if(word & (0b1 << 31)) {
        writeConfig(0x0C, word | (0b1 << 30));
        while(readConfig(0x0C) & (0b1 << 30));
        uint8_t result = (uint8_t)(readConfig(0x0C) >> 24);
        result &= 0b1111;
        return result;
    } else {
        return 0;
    }
}

void PCI::writeConfig(uint32_t offset, uint32_t data) {
    PCI::configWriteWord(bus, device, function, (uint8_t)offset, data);
}
uint32_t PCI::readConfig(uint32_t offset) {
    return PCI::configReadWord(bus, device, function, (uint8_t)offset);
}

void PCI::configWriteWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t message) {
    if(offset & 0b11)
        return;
    ConfigAddressRegister addressRegister;
    addressRegister.busNumber = bus;
    addressRegister.deviceNumber = device;
    addressRegister.functionNumber = func;
    addressRegister.registerOffset = offset;
    addressRegister.enableBit = true;
    uint32_t data = *(uint32_t*)&addressRegister;
    outl(PCI_CONFIG_ADDRESS, data);
    outl(PCI_CONFIG_DATA, message);
}

uint32_t PCI::configReadWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
    if(offset & 0b11)
        return 0xFFFFFFFF;
    ConfigAddressRegister addressRegister;
    addressRegister.busNumber = bus;
    addressRegister.deviceNumber = device;
    addressRegister.functionNumber = func;
    addressRegister.registerOffset = offset;
    addressRegister.enableBit = true;
    uint32_t data = *(uint32_t*)&addressRegister;
    outl(PCI_CONFIG_ADDRESS, data);
    return inl(PCI_CONFIG_DATA);
}

void PCI::readCommonHeader(PCICommonHeader& header, uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t data = configReadWord(bus, device, function, 0x00);
    header.vendorID = (uint16_t)(data & 0xFFFF);
    header.deviceID = (uint16_t)((data >> 16) & 0xFFFF);
    data = configReadWord(bus, device, function, 0x04);
    header.command = (uint16_t)(data & 0xFFFF);
    header.status = (uint16_t)((data >> 16) & 0xFFFF);
    data = configReadWord(bus, device, function, 0x08);
    header.revisionID = (uint8_t)(data & 0xFF);
    header.progIF = (uint8_t)((data >> 8) & 0xFF);
    header.subclass = (uint8_t)((data >> 16) & 0xFF);
    header.classCode = (uint8_t)((data >> 24) & 0xFF);
    data = configReadWord(bus, device, function, 0x0C);
    header.cacheLineSize = (uint8_t)(data & 0xFF);
    header.latencyTimer = (uint8_t)((data >> 8) & 0xFF);
    header.headerType = (uint8_t)((data >> 16) & 0xFF);
    header.BIST = (uint8_t)((data >> 24) & 0xFF);
}

void PCI::checkDevice(uint8_t bus, uint8_t device, LinkedList<PCI>& list) {
    PCICommonHeader header;
    readCommonHeader(header, bus, device, 0);
    if(header.vendorID == 0xFFFF)
        return; // this device doesn't exist
    checkFunction(bus, device, 0, header, list);
    if(header.headerType & 0b10000000) {
        for(uint8_t function = 1; function < 8; ++function) {
            readCommonHeader(header, bus, device, function);
            if(header.vendorID == 0xFFFF)
                continue;
            checkFunction(bus, device, function, header, list);
        }
    }
}

void PCI::checkBus(uint8_t bus, LinkedList<PCI>& list) {
    for(uint8_t device = 0; device < 32; ++device) {
        checkDevice(bus, device, list);
    }
}

void PCI::checkFunction(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header, LinkedList<PCI>& list) {
    if(header.classCode == 0x06 && header.subclass == 0x04) {
        uint32_t data = configReadWord(bus, device, function, 0x18);
        uint8_t secondaryBus = (data >> 8) & 0xFF;
        checkBus(secondaryBus, list);
    } else {
        PCI pci;
        pci.bus = bus;
        pci.device = device;
        pci.function = function;
        list.getIterator().insert(pci);
    }
}

void PCI::writeBAR8(uint64_t BAR, uint32_t offset, uint8_t data) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        outb((uint16_t)(BAR + offset), data);
    } else {
        BAR &= ~0b1111;
        *(uint8_t*)(uint64_t)(BAR+offset) = data;
    }
}
void PCI::writeBAR16(uint64_t BAR, uint32_t offset, uint16_t data) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        out16((uint16_t)(BAR + offset), data);
    } else {
        BAR &= ~0b1111;
        *(uint16_t*)(uint64_t)(BAR+offset) = data;
    }
}
void PCI::writeBAR32(uint64_t BAR, uint32_t offset, uint32_t data) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        outl((uint16_t)(BAR + offset), data);
    } else {
        BAR &= ~0b1111;
        *(uint32_t*)(uint64_t)(BAR+offset) = data;
    }
}
uint8_t PCI::readBAR8(uint64_t BAR, uint32_t offset) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        return inb((uint16_t)(BAR + offset));
    } else {
        BAR &= ~0b1111;
        return *(uint8_t*)(uint64_t)(BAR+offset);
    }
}
uint16_t PCI::readBAR16(uint64_t BAR, uint32_t offset) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        return in16((uint16_t)(BAR + offset));
    } else {
        BAR &= ~0b1111;
        return *(uint16_t*)(uint64_t)(BAR+offset);
    }
}
uint32_t PCI::readBAR32(uint64_t BAR, uint32_t offset) {
    if(BAR & 0b1) {
        BAR &= ~0b11;
        return inl((uint16_t)(BAR + offset));
    } else {
        BAR &= ~0b1111;
        return *(uint32_t*)(uint64_t)(BAR+offset);
    }
}

LinkedList<PCI> PCI::checkAllBuses() {
    LinkedList<PCI> list;
    PCICommonHeader header;
    readCommonHeader(header, 0, 0, 0);
    if(!(header.headerType & 0b10000000)) {
        checkBus(0, list);
    } else {
        for(uint8_t function = 0; function < 8; function++) {
            readCommonHeader(header, 0, 0, function);
            checkBus(function, list);
        }
    }
    return list;
}

}
