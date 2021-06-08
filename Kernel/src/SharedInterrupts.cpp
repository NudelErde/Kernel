#include "SharedInterrupts.hpp"
#include "PCI.hpp"
#include "units.hpp"

namespace Kernel {

constexpr uint8_t firstSharedInterrupt = 224;

SharedInterrupt::SharedInterrupt(uint8_t vector) : vector(vector), interruptFunction(nullptr) {}

void SharedInterrupt::setData(void* ndata) {
    data = ndata;
}

uint8_t SharedInterrupt::getVector() {
    return vector;
}

uint8_t SharedInterrupt::getProcessor() {
    return 0;
}

void SharedInterrupt::setInterruptFunction(void ninterruptFunction(void*)) {
    interruptFunction = ninterruptFunction;
}

void SharedInterrupt::check() {
    if (interruptFunction) {
        interruptFunction(data);
    }
}

struct SharedLink {
    SharedLink(uint8_t vector) : interrupt(vector) {}
    SharedLink* next;
    SharedInterrupt interrupt;
};

static SharedLink* links[16];
static uint8_t nextInsert = 0;

void sharedInterrupt(const Interrupt& inter) {
    SharedLink* link = links[inter.interruptNumber - firstSharedInterrupt];
    for (; link != nullptr; link = link->next) {
        link->interrupt.check();
    }
}

void SharedInterrupt::init() {
    for (uint8_t i = 0; i < 16; ++i) {
        Interrupt::setHandler(i + firstSharedInterrupt, sharedInterrupt);
    }
}

SharedInterrupt* SharedInterrupt::findInterrupt() {
    SharedLink** link = &links[nextInsert];

    for (; *link != nullptr; link = &(*link)->next) {}
    *link = new SharedLink(nextInsert + firstSharedInterrupt);

    nextInsert = (nextInsert + 1) % 16;

    return &(*link)->interrupt;
}

bool SharedInterrupt::configInterrupt(PCI* dev, const PCICommonHeader& header) {
    if (!(header.status & (0b1 << 4))) {
        return false;
    }
    uint8_t ptr = (uint8_t) (dev->readConfig(0x34) & ~0b11);

    uint64_t capaBaseMSI = 0;
    uint64_t capaBaseMSI_X = 0;

    while (ptr) {
        uint32_t capaDWord = dev->readConfig(ptr) & 0xFFFF;

        uint8_t capability = (uint8_t) (capaDWord & 0xFF);
        if (capability == 0x05) {
            // setup msi
            capaBaseMSI = ptr;
        } else if (capability == 0x11) {
            capaBaseMSI_X = ptr;
        }
        ptr = (uint8_t) ((capaDWord >> 8) & 0xFF);
    }

    if (capaBaseMSI_X) {
        constexpr uint16_t tableSizeMask = (0b1 << 11) - 1;

        uint16_t messageControl = (uint16_t) ((dev->readConfig(capaBaseMSI_X + 0x00) >> 16));
        uint16_t entryCount = (messageControl & tableSizeMask) + 1;

        uint32_t table = dev->readConfig(capaBaseMSI_X + 0x04);
        uint8_t tableBIR = table & 0b111;
        uint32_t tableOffset = table & ~0b111;

        uint32_t pending = dev->readConfig(capaBaseMSI_X + 0x08);
        uint8_t pendingBIR = pending & 0b111;
        uint32_t pendingOffset = pending & ~0b111;

        PCI::BAR& bar = dev->bars[tableBIR];
        if (!bar.valid) {
            kout << "Invalid BAR\n";
            asm("hlt");
        }

        dev->interrupt = SharedInterrupt::findInterrupt();
        uint16_t data = dev->interrupt->getVector() & 0xFF;
        uint64_t address = (0xFEE00000 | (dev->interrupt->getProcessor() << 12));
        dev->writeConfig(0x04, dev->readConfig(0x04) | 0b111);// enable BAR
        for (uint64_t i = 0; i < entryCount; ++i) {
            bar.write32(tableOffset + (0x10 * i) + 0x0, (uint32_t) address);
            bar.write32(tableOffset + (0x10 * i) + 0x4, (uint32_t) (address >> 32));
            bar.write32(tableOffset + (0x10 * i) + 0x8, (uint32_t) data);
            uint32_t vectorControl = bar.read32(tableOffset + (0x10 * i) + 0xC);
            bar.write32(tableOffset + (0x10 * i) + 0xC, vectorControl & ~0b1);
        }
        uint32_t value = dev->readConfig(capaBaseMSI_X + 0x00);
        dev->writeConfig(capaBaseMSI_X + 0x00, value | (0b1 << 31));
        return true;
    } else if (capaBaseMSI) {
        dev->interrupt = SharedInterrupt::findInterrupt();
        uint16_t data = dev->interrupt->getVector() & 0xFF;
        uint64_t address = (0xFEE00000 | (dev->interrupt->getProcessor() << 12));
        dev->writeConfig(ptr + 0x00, capaBaseMSI | (0b1 << 23));
        dev->writeConfig(ptr + 0x04, (uint32_t) address);
        dev->writeConfig(ptr + 0x08, (uint32_t) (address >> 32));

        uint32_t currentData = dev->readConfig(ptr + 0x0C) & 0xFFFF0000;
        dev->writeConfig(ptr + 0x0C, data | currentData);
        uint32_t messageControl = dev->readConfig(ptr + 0x00);
        dev->writeConfig(ptr + 0x00, messageControl | (0b1 << 23) | (0b1 << 16));
        return true;
    }

    return false;
}

}// namespace Kernel