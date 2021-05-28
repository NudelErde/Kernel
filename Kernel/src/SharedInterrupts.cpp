#include "SharedInterrupts.hpp"

namespace Kernel {

constexpr uint8_t firstSharedInterrupt = 224;

SharedInterrupt::SharedInterrupt(uint8_t vector): vector(vector), interruptFunction(nullptr) {}

void SharedInterrupt::setData(void* ndata) {
    data = ndata;
}

uint8_t SharedInterrupt::getVector() {
    return vector;
}

uint8_t SharedInterrupt::getProcessor() {
    return 0;
}

void SharedInterrupt::setInterruptFunction(void ninterruptFunction(void*)) {
    interruptFunction = ninterruptFunction;
}

void SharedInterrupt::check() {
    if(interruptFunction) {
        interruptFunction(data);
    }
}

struct SharedLink {
    SharedLink(uint8_t vector): interrupt(vector) {}
    SharedLink* next;
    SharedInterrupt interrupt;
};

static SharedLink* links[16];
static uint8_t nextInsert = 0;

void sharedInterrupt(const Interrupt& inter) {
    SharedLink* link = links[inter.interruptNumber - firstSharedInterrupt];
    for(; link != nullptr; link = link->next) {
        link->interrupt.check();
    }
}

void SharedInterrupt::init() {
    for(uint8_t i = 0; i < 16; ++i) {
        Interrupt::setHandler(i + firstSharedInterrupt, sharedInterrupt);
    }
}

SharedInterrupt* SharedInterrupt::findInterrupt() {
    SharedLink** link = &links[nextInsert];
    
    for(; *link != nullptr; link = &(*link)->next) {}
    *link = new SharedLink(nextInsert + firstSharedInterrupt);

    nextInsert = (nextInsert + 1) % 16;

    return &(*link)->interrupt;
}


}