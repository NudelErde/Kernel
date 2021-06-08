#pragma once
#include "stdint.h"

namespace Kernel {

class Device {
public:
    virtual ~Device();
    virtual void read(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* dest) = 0;
    virtual void write(uint64_t sectorIndex, uint64_t sectorCount, uint8_t* src) = 0;
    virtual void flush() = 0;

    static uint64_t getDeviceCount();
    static Device* getDevice(uint64_t index);
    static uint64_t getSystemDevice();
    static uint64_t addDevice(Device*);
    static void setSystemDevice(uint64_t device);
};

}// namespace Kernel