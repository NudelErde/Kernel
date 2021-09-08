#pragma once

#include "serial.hpp"
#include "stddef.h"
#include "stdint.h"

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

union PageTableElement {
    struct {
        volatile bool present : 1;
        volatile bool writeEnable : 1;
        volatile bool allowUserAccess : 1;
        volatile bool writeThrough : 1;
        volatile bool cacheDisable : 1;
        volatile uint16_t zero : 7;
    } flags;
    volatile uint64_t address;
} __attribute__((__packed__));

struct PageTable {
    volatile PageTableElement elements[512];
} __attribute__((__packed__));

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
    static uint64_t getAnyPage();
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

    bool mapTo(uint64_t virtualAddress, bool writeable, bool userAccess, bool cacheDisable = false, bool writeThrough = false);
    void remap();
    void unmap();
    void softmap(uint64_t virtualAddress, bool writeable, bool userAccess, bool cacheDisable = false, bool writeThrough = false);

    [[nodiscard]] inline bool isValid() const { return valid; }
    [[nodiscard]] inline uint64_t getVirtualAddress() const { return virtualAddress; }
    [[nodiscard]] inline uint64_t getPhysicalAddress() const { return physicalAddress; }
    [[nodiscard]] inline uint64_t checkIfMapped() const { return getPhysicalAddressFromVirtual(virtualAddress) == physicalAddress; }

    static uint64_t createNewPageTable();
    static uint64_t getPhysicalAddressFromVirtual(uint64_t address);
    static uint64_t getFlagsForVirtualPage(uint64_t address);

    [[nodiscard]] inline bool isWriteable() const { return writeable; }
    [[nodiscard]] inline bool isUserAccess() const { return userAccess; }
    [[nodiscard]] inline bool isCacheDisable() const { return cacheDisable; }
    [[nodiscard]] inline bool isWriteThrough() const { return writeThrough; }

private:
    uint64_t physicalAddress{};
    uint64_t virtualAddress{};
    volatile PageTableElement* entry{};
    bool valid{};
    bool destruct{};

    bool writeable;
    bool userAccess;
    bool cacheDisable;
    bool writeThrough;
};

void memcpy(void* dest, const void* src, uint64_t count);
void memset(void* dest, uint8_t value, uint64_t count);
bool memequ(const void* a, const void* b, uint64_t count);

}// namespace Kernel

extern Kernel::PageTable page_table_l4;
extern Kernel::PageTable page_table_l3;
extern Kernel::PageTable page_table_l2;

void* operator new(size_t count, void* ptr);
void* operator new[](size_t count, void* ptr);