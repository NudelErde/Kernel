#pragma once

#include "interruptExceptionHandler.hpp"
#include "process.hpp"
#include "stdint.h"

namespace Kernel {

struct SegmentSelector {
    uint8_t requestedPrivilegeLevel : 2;
    bool inLocalTable : 1;
    uint16_t index : 13;
};
static_assert(sizeof(SegmentSelector) == sizeof(uint16_t));
struct InterruptGate {
    uint16_t lowAddress;
    SegmentSelector segmentSelector;
    uint8_t interruptStack : 3;
    uint8_t zero0 : 5;
    uint8_t type : 4;
    uint8_t zero1 : 1;
    uint8_t descriptorPrivilegeLevel : 2;
    bool present : 1;
    uint16_t middleAddress;
    uint32_t highAddress;
    uint32_t reserved;
};
static_assert(sizeof(InterruptGate) == sizeof(uint8_t) * 16);

struct Interrupt {
public:
    typedef void (*Handler)(const Interrupt&);

    uint64_t errorCode;
    bool hasErrorCode;
    bool isHardwareInterrupt;
    uint8_t interruptNumber;
    void* stackFrame;

    static void setHandler(uint8_t interruptNumber, Handler handler, uint8_t ist = 0);
    static Handler getHandler(uint8_t interruptNumber);

    static void setDefaultHandler(Handler handler);
    static Handler getDefaultHandler();

    static void init();

    static void setHardwareInterruptMask(uint8_t irq, bool enable);
    static void sendHardwareEOI(uint8_t irq);

    static void updateIST();

private:
    static void setInterrupt(uint8_t interruptID, void (*func)(), uint8_t ist);

    friend void Kernel::initExceptionHandlers();
    friend void Kernel::Process::init();
};
}// namespace Kernel