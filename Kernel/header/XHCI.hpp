#pragma once
#include "USB.hpp"

namespace Kernel {

class XhciUsbController : public USB {
private:
    class Ring {
    private:
        struct LinkElement {
            uint64_t segmentPointer;
            uint32_t status;
            uint32_t control;
        };

    private:
        uint64_t enqueueIndex{};
        uint64_t dequeueIndex{};
        uint64_t entryCount{};
        uint64_t queueStart{};
        bool valid{};
        bool CCS{};

    public:
        Ring();
        Ring(uint64_t* segmentPointerArray, uint64_t* segmentSizeArray, uint64_t segmentArraySize, bool CCS);
        void insert(uint64_t parameter, uint32_t status, uint32_t control);
        void run();
        bool isFull();

        [[nodiscard]] inline bool isValid() const { return valid; }

    private:
        uint8_t* pointerOfIndex(uint64_t index);
    };

public:
    XhciUsbController(PCI* dev, const Kernel::PCICommonHeader& header);
    void onInterrupt();

    uint64_t getStatus();
    uint64_t getArgSize(uint8_t argNum);
    void handleDriverCall(uint8_t argNum, void* arg);

private:
    uint32_t structural1;
    uint32_t structural2;
    uint32_t structural3;
    uint32_t capability1;
    uint32_t capability2;

    uint32_t operationalBase;
    uint32_t runtimeBase;
    uint32_t doorbellBase;

    PCI* dev;
    uint64_t deviceContextBaseAddressArrayPointer;
    Ring commandRing;
};

}// namespace Kernel