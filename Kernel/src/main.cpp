#include "ACPI.hpp"
#include "AHCI.hpp"
#include "APIC.hpp"
#include "BasicIn.hpp"
#include "Ethernet.hpp"
#include "FileSystem.hpp"
#include "IDE.hpp"
#include "KernelOut.hpp"
#include "MassStorage.hpp"
#include "MemoryManager.hpp"
#include "PCI.hpp"
#include "PS2.hpp"
#include "Partition.hpp"
#include "Scheduler.hpp"
#include "SharedInterrupts.hpp"
#include "USB.hpp"
#include "VGA.hpp"
#include "checksum.hpp"
#include "debug.hpp"
#include "elf.hpp"
#include "inout.hpp"
#include "interrupt.hpp"
#include "interruptExceptionHandler.hpp"
#include "kernelMem.hpp"
#include "memory.hpp"
#include "print.hpp"
#include "serial.hpp"
#include "tss.hpp"
#include "wait.hpp"
#include <stddef.h>

//docker run --rm -v "${pwd}:/root/env" myos-buildenv make build-x86_64; qemu-system-x86_64 -cdrom dist/kernel.iso -drive file=image_file,format=raw -L "C:\Program Files\qemu" -m 6G --serial stdio -boot d

using namespace Kernel;

extern uint8_t kernel_start;
extern uint8_t kernel_end;

extern uint8_t gdt64[] asm("gdt64");

USB* usb;
VGA* vga;
Ethernet* ethernet;

extern "C" void __cxa_pure_virtual() { asm("int $32"); }

void configDevice(const PCICommonHeader& header, PCI* dev) {
    if ((header.headerType & ~0x80) != 0x00)
        return;
    for (uint8_t i = 0; i < 6; ++i) {
        dev->bars[i].setup(dev, i);
    }
}

LinkedList<PCIDriver*> loadPCI(bool withText) {
    usb = nullptr;
    vga = nullptr;
    ethernet = nullptr;
    LinkedList<PCI> pciDevices = PCI::checkAllBuses();
    LinkedList<PCIDriver*> drivers;
    bool cont = true;
    uint64_t driverCount = 0;
    if (withText) kout << "PCI devices: \n";
    for (auto iter = pciDevices.getIterator(); iter.valid() && cont; cont = iter.next()) {
        auto d = iter.get();
        PCICommonHeader header;
        PCI::readCommonHeader(header, d);

        if (withText) kout << "Class: " << Hex(header.classCode, 2) << " Subclass: " << Hex(header.subclass, 2) << " ProgIf: " << Hex(header.progIF, 2) << " Status: " << BitList(header.status)
                           << " Device: " << Hex(header.deviceID, 4) << " Vendor: " << Hex(header.vendorID, 4) << '\n';

        configDevice(header, d);
        if (d->selfTest() != 0) {
            kout << "Invalid self test\n";
            continue;
        }

        PCIDriver* driver = nullptr;

        if (header.classCode == 0x01 && header.subclass == 0x01) {
            ATA::openController(d->bus, d->device, d->function, header);
        } else if (header.classCode == 0x01 && header.subclass == 0x06 && header.progIF == 0x01) {
            AHCI::openController(d, header);
        } else if (header.classCode == 0x0C && header.subclass == 0x03) {
            usb = USB::openController(d, header);
            driver = usb;
        } else if (header.classCode == 0x02 && header.subclass == 0x00) {
            ethernet = Ethernet::openController(d, header);
            driver = ethernet;
        } else if (header.classCode == 0x03 && header.subclass == 0x00 && header.progIF == 0x00) {
            //vga = VGA::openController(d, header);
            driver = vga;
        } else {
            if (header.classCode == 0x06)
                continue;// skip bridge devices
        }

        if (!driver) {
            driver = new PCIDefaultDriver();
        }

        driver->busDeviceFunction = (((uint64_t) d->bus) << 16) | (((uint64_t) d->device) << 8) | (((uint64_t) d->function) << 0);
        driver->device = d;
        drivers.getIterator().insert(driver);
        driverCount++;
    }
    PCI::setDriverCount(driverCount);
    return drivers;
}

void kern_start() {
    crc32c_init();
    KernelOut::init();
    Print::setColor(Print::Color::WHITE, Print::Color::BLACK);
    Print::clear();
    kout << setSerial(Serial(0x3F8)) << "System start\n";
    Input::init(Input::InputMode::SERIAL);
    get_multiboot_infos();
    Interrupt::init();
    initExceptionHandlers();
    initSleep();
    Debug::init();
    PhysicalMemoryManagment::init();

    setupTss((uint64_t) gdt64, 2);
    initKernelDynamicMemory();
    readACPITables();

    IoAPIC* apics = IoAPIC::initIOApics(nullptr);
    apics[0].setMapping(getAPICMapping(0), 240, 0);// map legacy pit interrupt to 240
    apics[0].setMapping(1, 241, 0);
    apics[0].setMapping(12, 252, 0);
    LocalAPIC::enable();
    LocalAPIC::setupSleep();
    SharedInterrupt::init();

    Scheduler::init();
    Process::init();

    PCI::setDrivers(loadPCI(false));
    PS2::init();
    if (vga) {
        vga->clear();
        VideoTextField topText(vga, 0, 0, vga->getWidth(), 16, 1);
        topText.mapColor(0, 7);
        topText.mapColor(1, 0);

        const char* message = "CrackOS";
        char topLine[81]{};
        memset(topLine, ' ', sizeof(topLine));
        for (uint8_t index = 0; index < sizeof(topLine) - 1 && message[index]; ++index) {
            topLine[index] = message[index];
        }
        topLine[sizeof(topLine) - 1] = 0;

        for (const char* str = topLine; *str; ++str) {
            topText.print(*str);
        }

        VideoTextField mainText(vga, 0, 20, vga->getWidth(), vga->getHeight() - 20, 1);
        mainText.mapColor(1, 7);
        KernelOut::getRingBufferCurrent()[0] = 0;
        for (const char* str = KernelOut::getRingBufferMin(); *str; ++str) {
            mainText.print(*str);
        }
        kout << mainText << "640x480 mode\n";
    }

    if (uint8_t ps2 = PS2::getPorts(); ps2) {
        for (uint8_t i = 0; i < 2; ++i) {
            if (ps2 & (0b1 << i)) {
                if (PS2::getType(i) == 0xAB83) {
                    // keyboard!
                    //Input::init(Input::InputMode::PS2);
                    break;
                }
            }
        }
    }

    uint64_t count = Device::getDeviceCount();

    if (count == 0) {
        kout << "No devices found\n";
        for (;;) {
            asm("hlt");
        }
    }

    Device* hardDisk = Device::getDevice(Device::getSystemDevice());

    EXT4 ext(hardDisk, 0);

    uint64_t inodeID = ext.findFileINode("/start");
    if (!inodeID) {
        kout << "'/start' directory not found.\n";
        return;
    }
    auto inode = ext.getINode(inodeID);
    ext.iterateDirectory(
            inode, [](void* extPtr, const EXT4::DirEntry& entry) -> bool {
                EXT4& ext = *(EXT4*) extPtr;
                auto dirInode = ext.getINode(entry.inode);
                if (ext.getFileType(dirInode) == EXT4::FileType::REGULAR_FILE) {
                    char buffer[512];// will be: "[filename]\0boot\0"
                    memcpy(buffer, "/start/", 7);
                    memcpy(buffer + 7, entry.name, entry.nameSize);
                    buffer[7 + entry.nameSize] = 0;
                    memcpy(buffer + 7 + entry.nameSize + 1, "boot", 4);
                    buffer[7 + entry.nameSize + 5] = 0;
                    buffer[7 + entry.nameSize + 6] = 0;
                    loadAndExecute(ext, buffer, buffer, 0, false);
                }
                return false;
            },
            &ext);
    Scheduler::run();
}

extern "C" void __kernel_main() {
    kern_start();
    for (;;)
        asm("hlt");
}