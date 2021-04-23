#include "wait.hpp"
#include "interrupt.hpp"
#include "inout.hpp"
#include "print.hpp"
#include "APIC.hpp"

namespace Kernel {

static volatile bool sleepDone = false;
static volatile bool pitSleepIsUsed = false;

void onPIT(const Interrupt& interrupt) {
    sleepDone = true;
    Interrupt::sendHardwareEOI(0);
    LocalAPIC::endInterrupt();
}

void initSleep() {
    Interrupt::setHandler(240, &onPIT);
    Interrupt::setHardwareInterruptMask(0, true);
}

static void sleep1MS() {
    uint64_t flags;
    asm volatile(
              "pushf ; pop %0"
              : "=rm" (flags)
              : /* no input */
              : "memory");
    asm("sti");
    
    sleepDone = false;
    outb(0x43, 0b00110000); // channel 0 | lobyte/hibyte | mode 0 | binary mode
    outb(0x40, 0xA9); // counter to 1193 (0x4A9)
    outb(0x40, 0x04);

    while (!sleepDone);
    if(!(flags & 0b1 << 9)) {
        asm("cli");
    }
}

static void pitSleep(uint64_t ms) {
    pitSleepIsUsed = true;
    for(; ms > 0; --ms) {
        sleep1MS();
    }
    pitSleepIsUsed = false;
}

void sleep(uint64_t ms) {
    // todo: check if local apic sleep is available
    if(Thread::isInProgram()) {
        Thread* ptr = Thread::getCurrent();
        if(ptr) {
            ptr->setWaiting(ms * 1000);
            Thread::toKernel(); // jump back to kernel / scheduler -> toProcess should jump here
        } else {
            // error
        }
    } else if (!pitSleepIsUsed) { // todo: this is a race condition! replace with mutex
        pitSleep(ms);
    }
}

}