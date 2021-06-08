#pragma once
#include "USB.hpp"

namespace Kernel {

class UhciUsbController : public USB {
private:
    struct TransferDescriptor {
        uint32_t nextLink;
        uint32_t hcData;
        uint32_t controlData;
        uint32_t bufferPointer;
        uint64_t softwareProcessID;
        uint32_t softwareLockID;
        uint32_t softwareData3;
    } __attribute__((packed));

    struct QueueHead {
        uint32_t nextQueueDescriptor;
        uint32_t firstElementDescriptor;
    } __attribute__((packed));

    enum class PacketType {
        Isochronous = 3,
        Interrupt = 0,
        Control = 1,
        Bulk = 2
    };

    enum class PacketIdentification : uint8_t {
        IN = 0x69,
        OUT = 0xE1,
        SETUP = 0x2D
    };

    static_assert(sizeof(QueueHead) == sizeof(uint32_t) * 2);
    static_assert(sizeof(TransferDescriptor) == 32);

    uint32_t baseAddress;
    uint32_t* frameList;
    uint32_t nextFrameListIndex;
    uint8_t* queueArea;
    QueueHead* interruptQueue;
    QueueHead* controlQueue;
    QueueHead* bulkQueue;
    uint8_t queueAreaBitmap[(pageSize / 16) / 8];
    uint32_t nextQueueElement;

    bool isQueueAreaUnused(uint32_t address);
    uint32_t allocateNewQueueData();
    void deallocateQueueData(uint32_t address);

    static constexpr uint8_t dataPages = 8;
    static constexpr uint8_t chunkSize = 128;
    void* dataArea[dataPages];
    uint8_t dataBitmap[(dataPages * pageSize / chunkSize) / 8];
    uint32_t allocateNewData(uint32_t size);
    void deallocateData(uint32_t address, uint32_t size);

    bool checkIfValid(uint32_t pd);

public:
    UhciUsbController(uint8_t bus, uint8_t device, uint8_t func, const Kernel::PCICommonHeader& header);

    uint32_t addPacket(PacketType, PacketIdentification, uint8_t address, uint8_t function);
    void setPacketData(uint32_t pd, uint32_t size, void* data);
    void activatePacket(uint32_t pd);

    void reset();

    void onInterrupt();
};

}// namespace Kernel