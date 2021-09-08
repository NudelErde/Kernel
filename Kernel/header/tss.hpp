#pragma once
#include "stdint.h"

namespace Kernel {

struct TSS {
    uint32_t unused0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t unused1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t unused2;
    uint16_t unused3;
    uint16_t iomapbase;
} __attribute__((packed));
static_assert(sizeof(TSS) == 104);

void setupTss(uint64_t gdtAddress, uint64_t num);
TSS* getTss();
uint8_t getCPL();
}// namespace Kernel