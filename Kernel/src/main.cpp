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

//docker run --rm -v "${pwd}:/root/env" myos-buildenv make build-x86_64; qemu-system-x86_64 -cdrom dist/kernel.iso -drive file=image_file,format=raw -L "C:\Program Files\qemu" -m 6G --serial stdio -boot d

using namespace Kernel;

extern uint8_t kernel_start;
extern uint8_t kernel_end;

extern uint8_t gdt64[] asm("gdt64");

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
    PhysicalMemoryManagment::init();

    setupTss((uint64_t)gdt64, 2);
    initKernelDynamicMemory();
    readACPITables();
    
    IoAPIC* apics = IoAPIC::initIOApics(nullptr);
    apics[0].setMapping(2, 240, 0); // map legacy pit interrupt to 240
    LocalAPIC::enable();
    LocalAPIC::setupSleep();

    Scheduler::init();
    Process::init();

    PCI::checkAllBuses();

    uint64_t count = ATA::getDeviceCount();

    ATA::Device* hardDisk;

    for(uint64_t index = 0; index < count; ++index) {
        ATA::Device* device = ATA::getDevice(index);
        if(device->sectorCount > 0 && !device->isATAPI) {
            hardDisk = device;
            ATA::setSystemDevice(index);
        }
    }

    EXT4 ext(*hardDisk, 0);
    const char* args = "/startup.elf\0osboot\0";
    loadAndExecute(ext, args, args, 0); /*filename is first arg*/
    Scheduler::run();
}