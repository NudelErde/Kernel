#pragma once
#include "PCI.hpp"
#include "interrupt.hpp"

namespace Kernel {

class SharedInterrupt {
private:
    uint8_t vector;
    void* data;
    void (*interruptFunction)(void*);

public:
    SharedInterrupt(uint8_t vector);

    void setData(void*);
    uint8_t getVector();
    uint8_t getProcessor();
    void setInterruptFunction(void interruptFunction(void*));

    static SharedInterrupt* findInterrupt();
    static void init();

    static bool configInterrupt(PCI* dev, const PCICommonHeader& header);

    void check();
};

}// namespace Kernel