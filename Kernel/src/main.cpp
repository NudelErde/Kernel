#include "start.h"
#include "print.hpp"
#include "serial.hpp"
#include "inout.hpp"
#include "interrupt.hpp"
#include <stddef.h>
#include "interruptExceptionHandler.hpp"
#include "memory.hpp"
#include "PCI.hpp"
#include "ACPI.hpp"
#include "wait.hpp"
#include "KernelOut.hpp"
#include "MemoryManager.hpp"
#include "IDE.hpp"
#include "kernelMem.hpp"
#include "Partition.hpp"
#include "FileSystem.hpp"
#include "checksum.hpp"
#include "elf.hpp"
#include "APIC.hpp"
#include "Scheduler.hpp"
#include "tss.hpp"
#include "debug.hpp"
#include "USB.hpp"
#include "MassStorage.hpp"
#include "AHCI.hpp"
#include "SharedInterrupts.hpp"

//docker run --rm -v "${pwd}:/root/env" myos-buildenv make build-x86_64; qemu-system-x86_64 -cdrom dist/kernel.iso -drive file=image_file,format=raw -L "C:\Program Files\qemu" -m 6G --serial stdio -boot d

using namespace Kernel;

extern uint8_t kernel_start;
extern uint8_t kernel_end;

extern uint8_t gdt64[] asm("gdt64");

USB* usb;

extern "C" void __cxa_pure_virtual() { asm("int $32"); }

void loadPCI(bool withText) {
    LinkedList<PCIDeviceData> pciDevices = PCI::checkAllBuses();
    
    bool cont = true;
    if(withText) kout << "Unused pci devices: \n";
    for(auto iter = pciDevices.getIterator(); iter.valid() && cont; cont = iter.next()) {
        auto d = iter.get();
        PCICommonHeader header;
        PCI::readCommonHeader(header, d->bus, d->device, d->function);
        if(header.classCode == 0x01 && header.subclass == 0x01) {
            if(withText) kout << "Found ATA controller\n";
            ATA::openController(d->bus, d->device, d->function, header);
        } else if(header.classCode == 0x01 && header.subclass == 0x06 && header.progIF == 0x01) {
            if(withText) kout << "Found AHCI controller\n";
            AHCI::openController(d->bus, d->device, d->function, header);
        } else if(header.classCode == 0x0C && header.subclass == 0x03) {
            if(withText) kout << "Found USB controller\n";
            usb = USB::openController(d->bus, d->device, d->function, header);
        } else {
            if(header.classCode == 0x06)
                continue; // skip bridge devices
            if(withText) kout << "Class: " << Hex(header.classCode) << " Subclass: " << Hex(header.subclass)  << " ProgIf: " << Hex(header.progIF) << '\n';
        }
    }
}

void kern_start() {
    crc32c_init();
    Serial serial(0x3F8);
    KernelOut::init();
    Print::setColor(Print::Color::WHITE, Print::Color::BLACK);
    Print::clear();
    kout << setSerial(serial);
    kout << "System start\n";
    get_multiboot_infos();
    Interrupt::init();
    initExceptionHandlers();
    initSleep();
    Debug::init();
    PhysicalMemoryManagment::init();

    setupTss((uint64_t)gdt64, 2);
    initKernelDynamicMemory();
    readACPITables();
    
    IoAPIC* apics = IoAPIC::initIOApics(nullptr);
    apics[0].setMapping(getAPICMapping(0), 240, 0); // map legacy pit interrupt to 240
    LocalAPIC::enable();
    LocalAPIC::setupSleep();
    SharedInterrupt::init();

    Scheduler::init();
    Process::init();

    loadPCI(false);

    uint64_t count = Device::getDeviceCount();

    if(count == 0) {
        kout << "No devices found\n";
        asm("hlt");
    }

    Device* hardDisk = Device::getDevice(Device::getSystemDevice());

    EXT4 ext(hardDisk, 0);
    const char* args = "/startup.elf\0osboot\0";
    loadAndExecute(ext, args, args, 0, false); /*filename is first arg*/
    Scheduler::run();
}