#include "APIC.hpp"
#include "ACPI.hpp"
#include "interrupt.hpp"
#include "KernelOut.hpp"
#include "wait.hpp"

namespace Kernel {

static uint8_t memoryPageBuffer[sizeof(MemoryPage)]{};

static uint32_t readIoApic(uint64_t ioapicaddr, uint32_t reg) {
   uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
   ioapic[0] = (reg & 0xff);
   return ioapic[4];
}
 
static void writeIoApic(uint64_t ioapicaddr, uint32_t reg, uint32_t value) {
   uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
   ioapic[0] = (reg & 0xff);
   ioapic[4] = value;
}

IoAPIC* IoAPIC::initIOApics(uint8_t* countPtr) {
    uint8_t count;
    uint64_t* arr = getIoAPICData(count);
    if(countPtr)
        *countPtr = count;
    IoAPIC* ioApics = (IoAPIC*) new uint8_t[count * sizeof(IoAPIC)]; // do not initialize
    for(uint8_t i = 0; i < count; ++i) {
        new(ioApics + i) IoAPIC(arr[i]); // in place construct
    }
    return ioApics;
}

IoAPIC::IoAPIC(uint64_t address): address(address), page(address, true) {
    page.mapTo(address, true, false);
}

union RedirectionEntry {
    struct {
        uint64_t vector       : 8;
        uint64_t delvMode     : 3;
        uint64_t destMode     : 1;
        uint64_t delvStatus   : 1;
        uint64_t pinPolarity  : 1;
        uint64_t remoteIRR    : 1;
        uint64_t triggerMode  : 1;
        uint64_t mask         : 1;
        uint64_t reserved     : 39;
        uint64_t destination  : 8;
    };
    struct {
        uint32_t lowerDword;
        uint32_t upperDword;
    };
};


void IoAPIC::setMapping(uint8_t interruptNum, uint8_t resultVector, uint8_t cpuId) {
    uint64_t offset = 0x10 + interruptNum * 2;
    RedirectionEntry entry;
    entry.lowerDword = readIoApic(address, offset);
    entry.upperDword = readIoApic(address, offset + 1);

    entry.vector = resultVector;
    entry.delvMode = 0;
    entry.destMode = 0;
    entry.delvStatus = 0;
    entry.pinPolarity = 0;
    entry.triggerMode = 0;
    entry.mask = 0;
    entry.destination = cpuId;

    writeIoApic(address, offset, entry.lowerDword);
    writeIoApic(address, offset + 1, entry.upperDword);
}
static bool localAPICRunning = false;
void LocalAPIC::endInterrupt() {
    if(localAPICRunning)
        *(uint32_t volatile*)(localAPICAddress + 0x0B0) = 0x0; // eof
}

void LocalAPIC::enable() {
    localAPICRunning = true;
    MemoryPage* page = new(memoryPageBuffer) MemoryPage(localAPICAddress, true);
    page->mapTo(localAPICAddress, true, false);
    for(uint8_t i = 0; i < 16; ++i) {
        Interrupt::setHardwareInterruptMask(i, false);
    }

    // set Spurious Interrupt Vector Register
    uint32_t sivr = *((uint32_t volatile *) (localAPICAddress + 0xF0));
    *((uint32_t volatile*) (localAPICAddress + 0xF0)) = (sivr | 0xFF | 0x100); //0xFF is interrupt number //0x100 is apic enable
}

static uint64_t speed;

uint64_t LocalAPIC::getTimeLeft() {
    uint64_t currentTicksLeft = (*(uint32_t volatile*)(localAPICAddress + 0x390));
    return currentTicksLeft * 100000 / ticksIn100ms;
}

void LocalAPIC::setupSleep() {
    Interrupt::setHandler(0x90, [](const Interrupt&){LocalAPIC::endInterrupt();}); // Interrupt handler = ignore
    
    *(uint32_t volatile*)(localAPICAddress + 0x3E0) = 0x3; // divider = 16
    
    *(uint32_t volatile*)(localAPICAddress + 0x320) = 0x90; // set interrupt vector and unmask

    *(uint32_t volatile*)(localAPICAddress + 0x380) = 0xFFFFFFFF; // init count to 0xFFFFFFFF

    sleep(100); // sleep 100 ms

    *(uint32_t volatile*)(localAPICAddress + 0x320) = 0b1 << 16; // mask interrupt

    ticksIn100ms = 0xFFFFFFFF - (*(uint32_t volatile*)(localAPICAddress + 0x390));
    *(uint32_t volatile*)(localAPICAddress + 0x380) = 0x0; // set count to 0x0
}

static void(*localCallback)(); // todo move in cpu specific memory
void LocalAPIC::interruptIn(uint64_t microseconds, void(*callback)()) {
    uint32_t ticks = (uint64_t)ticksIn100ms * microseconds / 100000;
    localCallback = callback;

    Interrupt::setHandler(0x90, [](const Interrupt&){
        if(localCallback)
            localCallback();
        LocalAPIC::endInterrupt();
    }, 0); // Interrupt handler = ignore

    *(uint32_t volatile*)(localAPICAddress + 0x3E0) = 0x3; // divider = 16
    
    *(uint32_t volatile*)(localAPICAddress + 0x320) = 0x90; // set interrupt vector and unmask and oneshot mode

    *(uint32_t volatile*)(localAPICAddress + 0x380) = ticks; // init count to ticks
}

}