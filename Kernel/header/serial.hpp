#pragma once

#include <stdint.h>

namespace Kernel {

struct Serial {
public:
    Serial(uint16_t address);

    void start();

    void write(uint8_t);
    uint8_t read();
    void printHex(uint64_t hex, uint8_t size = 0);
    void print(const char*);
    void print(char);
    void printBaseN(uint64_t num, uint8_t n, uint8_t size = 0);

    bool outputBufferEmpty();
    bool inputBufferEmpty();

    uint32_t getError();

private:
    void setError(uint32_t);
    void checkHardwareError();

    uint16_t address{};
    uint32_t error{};
};

}// namespace Kernel