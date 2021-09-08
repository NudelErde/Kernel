#include "tss.hpp"
#include "KernelOut.hpp"
#include "interrupt.hpp"
#include "memory.hpp"
#include "process.hpp"

namespace Kernel {

TSS tss;
TSS* tssPtr = 0;
uint8_t interruptStack[stackPageCount * pageSize];
uint8_t panicStack[stackPageCount * pageSize];
uint8_t kernelPriv0Stack[stackPageCount * pageSize];

TSS* getTss() {
    return tssPtr;
}

uint8_t getCPL() {
    uint32_t cs;
    asm(R"(
        mov %%cs, %0
        )"
        : "=a"(cs));
    return cs & 0b11;
}

void setupTss(uint64_t gdtAddress, uint64_t num) {
    struct gdtEntry {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_middle;
        uint8_t access;
        uint8_t flags;// flags contains 4 bits of limit
        uint8_t base_high;
        uint32_t base_higher;
        uint32_t reserved;
    } __attribute__((packed));
    static_assert(sizeof(gdtEntry) == 16);
    gdtEntry* tssEntry = (gdtEntry*) (gdtAddress + num * 8);

    uint64_t base = (uint64_t) &tss;
    uint64_t limit = sizeof(TSS);

    tssEntry->base_low = base & 0xFFFF;
    tssEntry->base_middle = (base >> 16) & 0xFF;
    tssEntry->base_high = (base >> 24) & 0xFF;
    tssEntry->base_higher = (base >> 32) & 0xFFFFFFFF;

    tssEntry->limit_low = (limit & 0xFFFF);
    tssEntry->flags = ((limit >> 16) & 0x0F);

    tssEntry->access = 0b10001001;
    tssEntry->flags |= 0b00000000;

    memset(&tss, 0, sizeof(TSS));

    tss.ist1 = ((uint64_t) interruptStack) + sizeof(interruptStack) - 1;
    tss.ist2 = ((uint64_t) panicStack) + sizeof(panicStack) - 1;
    tss.rsp0 = ((uint64_t) kernelPriv0Stack) + sizeof(kernelPriv0Stack) - 1;

    uint16_t selector = (num << 3) | 0b000;// load tss
    asm(R"(
            mov %0, %%ax
            ltr %%ax
        )" ::"m"(selector)
        : "rax");
    Interrupt::updateIST();
    tssPtr = &tss;
}

}// namespace Kernel