#include "PCI.hpp"
#include "stdint.h"
#include "inout.hpp"
#include "print.hpp"
#include "IDE.hpp"

namespace Kernel{

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

void PCI::checkDevice(uint8_t bus, uint8_t device) {
    PCICommonHeader header;
    readCommonHeader(header, bus, device, 0);
    if(header.vendorID == 0xFFFF)
        return; // this device doesn't exist
    checkFunction(bus, device, 0, header);
    if(header.headerType & 0b10000000) {
        for(uint8_t function = 1; function < 8; ++function) {
            readCommonHeader(header, bus, device, function);
            if(header.vendorID == 0xFFFF)
                continue;
            checkFunction(bus, device, function, header);
        }
    }
}

void PCI::checkBus(uint8_t bus) {
    for(uint8_t device = 0; device < 32; ++device) {
        checkDevice(bus, device);
    }
}

void PCI::checkFunction(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header) {
    if(header.classCode == 0x06 && header.subclass == 0x04) {
        uint32_t data = configReadWord(bus, device, function, 0x18);
        uint8_t secondaryBus = (data >> 8) & 0xFF;
        checkBus(secondaryBus);
    } else if(header.classCode == 0x01 && header.subclass == 0x01) {
        ATA::openController(bus, device, function, header);
    } else {
    }
}

void PCI::checkAllBuses() {
    PCICommonHeader header;
    readCommonHeader(header, 0, 0, 0);
    if(!(header.headerType & 0b10000000)) {
        checkBus(0);
    } else {
        for(uint8_t function = 0; function < 8; function++) {
            readCommonHeader(header, 0, 0, function);
            checkBus(function);
        }
    }
}

}
