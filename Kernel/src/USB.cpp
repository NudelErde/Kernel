#include "USB.hpp"
#include "inout.hpp"
#include "KernelOut.hpp"
#include "wait.hpp"

namespace Kernel {

USB* USB::openController(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header) {
    switch (header.progIF) {
    case 0x00:
        return new UhciUsbController(bus, device, function, header);
    default:
        return nullptr;
    }
}


UhciUsbController::UhciUsbController(uint8_t bus, uint8_t device, uint8_t function, const PCICommonHeader& header) {
    if(header.headerType != 0)
        return;

    return;
    kout << "Interrupt line: " << Hex(PCI::configReadWord(bus, device, function, 0x3C) & 0xFF) << '\n';
    kout << "Interrupt pin: " << Hex((PCI::configReadWord(bus, device, function, 0x3C) >> 8) & 0xFF) << '\n';
    uint32_t BAR4 = PCI::configReadWord(bus, device, function, 0x20);
    baseAddress = BAR4;

    frameList = (uint32_t*)PhysicalMemoryManagment::getFreeKernelPage();
    PhysicalMemoryManagment::setUsed((uint64_t)frameList, true);

    queueArea = (uint8_t*)PhysicalMemoryManagment::getFreeKernelPage();
    PhysicalMemoryManagment::setUsed((uint64_t)queueArea, true);

    for(uint8_t i = 0; i < dataPages; ++i) {
        dataArea[i] = (void*)PhysicalMemoryManagment::getFreeKernelPage();
        PhysicalMemoryManagment::setUsed((uint64_t)(dataArea[i]), true);
    }

    memset(dataBitmap, 0, sizeof(dataBitmap));

    nextQueueElement = 0;
    memset(queueAreaBitmap, 0, sizeof(queueAreaBitmap));

    interruptQueue = (QueueHead*)(uint64_t)allocateNewQueueData();
    controlQueue = (QueueHead*)(uint64_t)allocateNewQueueData();
    bulkQueue = (QueueHead*)(uint64_t)allocateNewQueueData();

    reset();
}

void UhciUsbController::reset() {
    do {
        out16(baseAddress + 0x00, (0b1 << 7)); // stop
        sleep(1);
    } while(!(in16(baseAddress + 0x02) & (0b1 << 5))); // stop

    outl(baseAddress + 0x08, (uint32_t)(uint64_t)frameList); //FRBASEADD
    outb(baseAddress + 0x0C, 0x40); //SOFMOD
    out16(baseAddress + 0x04, 0b1111); // USBINTR

    nextFrameListIndex = 0;

    interruptQueue->nextQueueDescriptor = (uint64_t)controlQueue;
    controlQueue->nextQueueDescriptor = (uint64_t)bulkQueue;
    bulkQueue->nextQueueDescriptor = 0b1;
    
    for(uint64_t i = 0; i < 1024; ++i) {
        frameList[i] = 0b11; //entry is invalid and points to a QH
        frameList[i] |= (uint64_t)interruptQueue; // the first queue head is the interrupt queue
    }

    out16(baseAddress + 0x06, 0x00);
}

bool UhciUsbController::checkIfValid(uint32_t pd) {
    return (pd >= (uint32_t)(uint64_t)queueArea) && (pd < ((uint32_t)(uint64_t)queueArea) + pageSize);
}

uint32_t UhciUsbController::addPacket(PacketType type, PacketIdentification pid, uint8_t address, uint8_t function) {
    uint32_t ptr = allocateNewQueueData();
    TransferDescriptor* td = (TransferDescriptor*)td;
    td->hcData = (0b1 << 24);
    if(type == PacketType::Isochronous) {
        td->hcData |= 0b1 << 25;
    }
    td->controlData = 0;
    td->controlData |= (uint32_t)pid;
    td->controlData |= (address & 0b1111111) << 8;
    td->controlData |= (function & 0xF) << 15;

    if(type == PacketType::Isochronous) {
        nextFrameListIndex %= 1024;
        if(in16(baseAddress + 0x06) == nextFrameListIndex) { // do not insert in current element
            nextFrameListIndex += 1023; // -1
            nextFrameListIndex %= 1024;
        }
        td->nextLink = frameList[nextFrameListIndex];
        frameList[nextFrameListIndex] = (uint32_t)(uint64_t)td;
        nextFrameListIndex++;
    } else {
        QueueHead* const queueHeads[] {interruptQueue, controlQueue, bulkQueue};
        QueueHead* head = queueHeads[(int)type];
        td->nextLink = head->firstElementDescriptor;
        head->firstElementDescriptor = (uint32_t)(uint64_t)td;
    }

    return ptr;
}


void UhciUsbController::setPacketData(uint32_t pd, uint32_t size, void* data) {
    if(!checkIfValid(pd))
        return;
    if(size > 1024)
        return;
    
    TransferDescriptor* td = (TransferDescriptor*)(uint64_t)pd;
    if(size == 0) {
        td->controlData |= (0x7FF << 21);
    } else {
        uint32_t address = allocateNewData(size);
        if(!address)
            return;
        memcpy((void*)(uint64_t)address, data, size);
        td->bufferPointer = address;
        td->controlData |= (size-1) << 21;
    }
}
void UhciUsbController::activatePacket(uint32_t pd) {
    if(!checkIfValid(pd))
        return;
    TransferDescriptor* td = (TransferDescriptor*)(uint64_t)pd;
    td->hcData |= 0b1 << 23;
}

uint32_t UhciUsbController::allocateNewData(uint32_t size) {
    uint8_t chunkCount = (size + chunkSize - 1) / chunkSize;
    for(uint8_t i = 0; i < dataPages; ++i) {
        uint8_t a = 0;
        for(uint8_t index = 0; index < pageSize / chunkSize; ++index) {
            uint8_t positionInBitmap = i * (pageSize / chunkSize) + index;
            if(dataBitmap[positionInBitmap / 8] & (0b1 << positionInBitmap % 8)) {
                a = 0;
            } else {
                ++a;
            }
            if(a == chunkCount) {
                dataBitmap[positionInBitmap / 8] |= (0b1 << positionInBitmap % 8);
                return ((uint32_t)(uint64_t)dataArea[i]) + index * chunkSize;
            }
        }
    }
    return 0;
}
void UhciUsbController::deallocateData(uint32_t address, uint32_t size) {
    for(uint8_t i = 0; i < dataPages; ++i) {
        if(address >= (uint32_t)(uint64_t)dataArea[i] && address < (uint32_t)(uint64_t)dataArea[i] + pageSize) {
            address -= (uint32_t)(uint64_t)dataArea[i];
            address /= chunkSize;
            uint8_t chunkCount = (size + chunkSize - 1) / chunkSize;
            uint8_t positionInBitmap = i * (pageSize / chunkSize) + address;
            for(uint8_t j = 0; j < chunkCount; ++j, ++positionInBitmap) {
                dataBitmap[positionInBitmap / 8] &= ~(0b1 << positionInBitmap % 8);
            }
            return;
        }
    }
}

bool UhciUsbController::isQueueAreaUnused(uint32_t address) {
    address = address - (uint32_t)(uint64_t)queueArea; // address relative to start
    uint32_t index = address / 16;
    uint32_t byte = index / 8;
    uint8_t bit = index % 8;
    return !(queueAreaBitmap[byte] & (0b1 << bit));
}

void UhciUsbController::deallocateQueueData(uint32_t address) {
    address = address - (uint32_t)(uint64_t)queueArea; // address relative to start
    uint32_t index = address / 16;
    uint32_t byte = index / 8;
    uint8_t bit = index % 8;
    queueAreaBitmap[byte] &= ~(0b1 << bit);
}

uint32_t UhciUsbController::allocateNewQueueData() {
    nextQueueElement %= pageSize;
    uint32_t retVal = nextQueueElement;
    do {
        if(isQueueAreaUnused(retVal + (uint32_t)(uint64_t)queueArea)) {
            uint32_t index = retVal / 16;
            uint32_t byte = index / 8;
            uint8_t bit = index % 8;
            queueAreaBitmap[byte] |= 0b1 << bit;
            break;
        }
        retVal += 16;
        retVal %= pageSize;
    } while(retVal != nextQueueElement);
    retVal += 16;
    retVal %= pageSize;
    nextQueueElement = retVal;
    return retVal;
}

}