#pragma once
#include <stdint.h>

namespace Kernel {

inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

inline void outl(uint16_t port, uint32_t val) {
    asm volatile("outl %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}

inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile("inl %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

inline uint16_t in16(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0"
                 : "=a"(ret)
                 : "Nd"(port));
    return ret;
}

inline void out16(uint16_t port, uint16_t val) {
    asm volatile("outw %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}

static inline void io_wait(void) {
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

}// namespace Kernel