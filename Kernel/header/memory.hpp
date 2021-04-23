#pragma once

#include "serial.hpp"
#include "stdint.h"
#include "stddef.h"

namespace Kernel {

constexpr uint64_t pageSize = 0x1000;

uint64_t getRSDTPointer();
uint64_t getXSDTPointer();

struct MemoryInfo {
    uint64_t start;
    uint64_t length;
    uint64_t pageIndexStart;
    uint64_t pageIndexEnd;

    MemoryInfo* nextNode;
};

union PageTableElement{
    struct{
        volatile bool present: 1;
        volatile bool writeEnable: 1;
        volatile bool allowUserAccess: 1;
        volatile uint16_t zero: 9;
    } flags;
    volatile uint64_t address;
}__attribute__((__packed__));

struct PageTable{
    volatile PageTableElement elements[512];
}__attribute__((__packed__));

void get_multiboot_infos();
MemoryInfo* getMemoryInfoPtr();

class PhysicalMemoryManagment {
public:
    static void init();
    static bool isValidMemory(uint64_t);
    static bool isUsed(uint64_t);
    static void setUsed(uint64_t, bool);
    static uint64_t getPageIndex(uint64_t);
    static uint64_t getTotalMemorySize();

    static uint64_t getFreeUserPage();
    static uint64_t getFreeKernelPage();
};

class MemoryPage {
public:
    explicit MemoryPage(uint64_t physicalAddress);
    MemoryPage(uint64_t physicalAddress, bool unsafe);
    MemoryPage() = default;
    ~MemoryPage();

    MemoryPage(const MemoryPage&) = delete;
    MemoryPage& operator=(const MemoryPage&) = delete;
    MemoryPage(MemoryPage&&) noexcept;
    MemoryPage& operator=(MemoryPage&&) noexcept;

    bool mapTo(uint64_t virtualAddress, bool writeable, bool userAccess);
    void remap();
    void unmap();
    void softmap(uint64_t virtualAddress, bool writeable, bool userAccess);

    bool isValid();
    uint64_t getVirtualAddress();

    static uint64_t createNewPageTable();
private:

    uint64_t physicalAddress{};
    uint64_t virtualAddress{};
    volatile PageTableElement* entry{};
    bool valid{};
    bool destruct{};

    bool writeable;
    bool userAccess;
};

void memcpy(void* dest, const void* src, uint64_t count);
void memset(void* dest, uint8_t value, uint64_t count);
bool memequ(const void* a, const void* b, uint64_t count);

}

extern Kernel::PageTable page_table_l4;
extern Kernel::PageTable page_table_l3;
extern Kernel::PageTable page_table_l2;

void* operator new (size_t count, void* ptr );
void* operator new[] (size_t count, void* ptr );