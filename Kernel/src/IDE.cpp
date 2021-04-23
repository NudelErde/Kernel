#include "IDE.hpp"
#include "PCI.hpp"
#include "print.hpp"
#include "inout.hpp"
#include "wait.hpp"
#include "print.hpp"
#include "serial.hpp"
#include "KernelOut.hpp"
#include "MemoryManager.hpp"
#include "util.hpp"
#include "kernelMem.hpp"
#include "process.hpp"

namespace Kernel{

namespace ATA {

struct ATALinkedList{
    Device device;
    ATALinkedList* next;
};

static ATALinkedList* list;
static uint64_t count;

static void poll(Channel& channel) {
    for(uint8_t i = 0; i < 14; ++i) {
        channel.readControl8(0); //wait 420 ns
    }
    while(channel.readControl8(7) & StatusBusy); // wait for bussy to be 0
    return;
}

static bool checkForDevice(Channel& channel, uint8_t isSlave, Device& device) {
    channel.writeIO8(6, 0xA0 | (isSlave << 4)); // 0xA0 for CHS
    sleep(1);
    channel.writeIO8(7, CommandIdentify);
    sleep(1);

    if(channel.readIO8(7) == 0) return false; // status 0 is no device

    uint8_t status;
    uint8_t error;
    uint8_t type = ATA;
    while(true) {
        status = channel.readIO8(7);
        if(status & StatusError) {
            error = 1;
            break; // not ATA
        }
        if( !(status & StatusBusy) && (status & StatusDataRequestReady)) {
            break;
        }
    }

    if(error) {
        uint8_t cl = channel.readIO8(4);
        uint8_t ch = channel.readIO8(5);

        if ((cl == 0x14 && ch == 0xEB) || (cl == 0x69 && ch == 0x96)) {
            type = ATAPI;
        } else {
            return false; // no device
        }

        channel.writeIO8(7, CommandIdentifyPacket);
        sleep(1);
    }

    device.isSlave = isSlave;
    device.isATAPI = type == ATAPI;
    device.channel = channel;
    
    uint8_t buffer[2048 / 4] = {};
    channel.readBuffer(buffer, 512);

    device.signature = *(uint16_t*)(buffer + IdentitfyDeviceType);
    device.capabilities = *(uint16_t*)(buffer + IdentitfyCapabilities);
    device.commandSet = *(uint32_t*)(buffer + IdentitfyCommandsets);

    uint64_t LBA48SectorCount = *(uint64_t*)(buffer + IdentitfyMaxLBAExt);
    uint64_t LBA28SectorCount = *(uint32_t*)(buffer + IdentitfyMaxLBA);

    if(device.commandSet & (1 << 26)) {
        // 48 bit addressing
        device.sectorCount = LBA48SectorCount;
        device.lba48Support = true;
    } else {
        // 28 bit or CHS addressing
        device.sectorCount = (uint64_t)LBA28SectorCount;
        device.lba48Support = false;
    }

    for(uint8_t index = 0; index < 40; index += 2) {
        device.model[index] = buffer[IdentitfyModel + index + 1];
        device.model[index + 1] = buffer[IdentitfyModel + index];
    }
    device.model[40] = 0;

    return true;
}

static void insert(const Device& dev) {
    if(!list) {
        list = new ATALinkedList();
        list->device = dev;
        list->next = nullptr;
    } else {
        ATALinkedList* l = new ATALinkedList();
        l->next = list;
        l->device = dev;
        list = l;
    }
    ++count;
}

static void checkForDevices(Channel& channel) {
    Device master;
    if(checkForDevice(channel, false, master)) {
        insert(master);
    }
    Device slave;
    if(checkForDevice(channel, true, slave)) {
        insert(slave);
    }
}

void openController(uint8_t bus, uint8_t device, uint8_t func, const PCICommonHeader& header) {
    if(header.headerType != 0x00)
        return;
    uint32_t BAR0 = PCI::configReadWord(bus, device, func, 0x10);
    uint32_t BAR1 = PCI::configReadWord(bus, device, func, 0x14);
    uint32_t BAR2 = PCI::configReadWord(bus, device, func, 0x18);
    uint32_t BAR3 = PCI::configReadWord(bus, device, func, 0x1C);
    uint32_t BAR4 = PCI::configReadWord(bus, device, func, 0x20);
    uint8_t interruptPinPrimary = (PCI::configReadWord(bus, device, func, 0x3C) >> 8) & 0xFF;
    uint8_t interruptPinSecondary = interruptPinPrimary;

    if(!(header.progIF & 0b1)) {
        BAR0 = 0x01F0;
        BAR1 = 0x03F6;
        interruptPinPrimary = 14;
    }
    if(!(header.progIF & 0b100)) {
        BAR2 = 0x0170;
        BAR3 = 0x0376;
        interruptPinSecondary = 15;
    }
    if(!(header.progIF & 0b10000000)) {
        BAR4 = 0x0000;
    }

    Channel primary(BAR0, BAR1);
    Channel secondary(BAR2, BAR3);

    primary.writeControl8(0, 0b10);
    secondary.writeControl8(0, 0b10);

    checkForDevices(primary);
    checkForDevices(secondary);
}

Channel::Channel(uint32_t BAR0, uint32_t BAR1): ioBase(BAR0), controlBase(BAR1) {}

void Channel::readBuffer(uint8_t* buffer, uint64_t size) {
    for(uint64_t i = 0; i < size / 2; ++i) {
        while(!(readIO8(7) & StatusDataRequestReady));
        ((uint16_t*)buffer)[i] = in16(ioBase);
    }
}

uint64_t getDeviceCount() {
    return count;
}

Device* getDevice(uint64_t index) {
    ATALinkedList* node = list;
    for(uint64_t i = 0; i < index; ++i) {
        node = node->next;
        if(!node)
            return nullptr;
    }
    return &(node->device);
}

static uint64_t systemDevice;

void setSystemDevice(uint64_t device) {
    systemDevice = device;
}

uint64_t getSystemDevice() {
    return systemDevice;
}

void Device::read(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* dest){
    if (sectorCount == 0)
        return;

    uint8_t LBA[8]{};
    *(uint64_t*)(void*)(LBA) = sectorIndex;
    if (lba48Support) {
        channel.writeIO8(6, 0b11100000 | (isSlave << 4)); //LBA mode
        sleep(1);
        channel.writeIO8(2, (uint8_t)(sectorCount >> 8));
        channel.writeIO8(3, LBA[3]);
        channel.writeIO8(4, LBA[4]);
        channel.writeIO8(5, LBA[5]);

        channel.writeIO8(2, (uint8_t)(sectorCount & 0xFF));
        channel.writeIO8(3, LBA[0]);
        channel.writeIO8(4, LBA[1]);
        channel.writeIO8(5, LBA[2]);
        channel.writeIO8(7, CommandReadPIOExt);
    } else {
        channel.writeIO8(6, 0b11100000 | (isSlave << 4) | ((sectorIndex << 24) & 0xF)); // LBA mode & bits 24-27 from sectorIndex
        sleep(1);
        channel.writeIO8(2, (uint8_t)sectorCount);
        channel.writeIO8(3, (uint8_t)(sectorIndex & 0xFF));
        channel.writeIO8(4, (uint8_t)((sectorIndex >> 8) & 0xFF));
        channel.writeIO8(5, (uint8_t)((sectorIndex >> 16) & 0xFF));
        channel.writeIO8(7, CommandReadPIO);
    }

    uint64_t index;
    for (index = 0; index < sectorCount * 512 / 2; ++index) {
        if(index % (512 / 2) == 0) { // wait for ready in every sector
            uint8_t status;
            do {
                status = channel.readIO8(7);
            } while (!(status & 0b1000)); // wait for data ready
        }
        ((uint16_t *)dest)[index] = channel.readIO16(0);
    }
}
void Device::write(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* src){
    if (sectorCount == 0)
        return;

    uint8_t LBA[8]{};
    *(uint64_t*)(void*)(LBA) = sectorIndex;

    if (lba48Support) {
        channel.writeIO8(6, 0b11100000 | (isSlave << 4)); //LBA mode
        sleep(1);
        channel.writeIO8(2, (uint8_t)(sectorCount >> 8));
        channel.writeIO8(3, LBA[3]);
        channel.writeIO8(4, LBA[4]);
        channel.writeIO8(5, LBA[5]);

        channel.writeIO8(2, (uint8_t)(sectorCount & 0xFF));
        channel.writeIO8(3, LBA[0]);
        channel.writeIO8(4, LBA[1]);
        channel.writeIO8(5, LBA[2]);
        channel.writeIO8(7, CommandWritePIOExt);
    } else {
        channel.writeIO8(6, 0b11100000 | (isSlave << 4) | ((sectorIndex << 24) & 0xF)); // LBA mode & bits 24-27 from sectorIndex
        sleep(1);
        channel.writeIO8(2, (uint8_t)sectorCount);
        channel.writeIO8(3, (uint8_t)(sectorIndex & 0xFF));
        channel.writeIO8(4, (uint8_t)((sectorIndex >> 8) & 0xFF));
        channel.writeIO8(5, (uint8_t)((sectorIndex >> 16) & 0xFF));
        channel.writeIO8(7, CommandWritePIO);
    }
    
    for (uint64_t index = 0; index < sectorCount * 512 / 2; ++index) {
        if(index % (512 / 2) == 0) { // wait for ready in every sector
            uint8_t status;
            do {
                status = channel.readIO8(7);
            } while (!(status & 0b1000)); // wait for data ready
        }
        channel.writeIO16(0, ((uint16_t*)src)[index]);
    }
}

void ATA::Device::flush() {
    channel.writeIO8(6, 0b11100000 | (isSlave << 4)); //LBA mode
    if (lba48Support) {
        channel.writeIO8(7, CommandCacheFlushExt);
    } else {
        channel.writeIO8(7, CommandCacheFlush);
    }
}

}

}