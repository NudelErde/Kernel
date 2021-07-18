#include "memory.hpp"
#include "KernelOut.hpp"
#include "VGA.hpp"
#include "debug.hpp"
#include "print.hpp"
#include "serial.hpp"
#include "stddef.h"
#include "stdint.h"
#include "units.hpp"

extern uint32_t multiboot_information_structure asm("multiboot_information_structure");

static uint32_t realign(uint32_t i) {
    --i;
    return (i & -8) + 8;
}

static bool basicStrEq(const char* a, const char* b, uint64_t len) {
    for (uint64_t index = 0; index < len; ++index) {
        if (a[index] != b[index])
            return false;
    }
    return true;
}

static void reloadPageTables(uint64_t address) {
    asm volatile("invlpg (%0)" ::"r"(address)
                 : "memory");
}

extern uint8_t kernel_start;
extern uint8_t kernel_end;

namespace Kernel {

struct RSDPDescriptor {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;

    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct RSDPDescriptor20 {
    RSDPDescriptor firstPart;

    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

static MemoryInfo* info = nullptr;
static uint64_t totalMemorySize = 0;
static uint8_t* physicalPageUsageBitmap;
static uint64_t physicalPageUsageBitmapSize;
static uint64_t maxMemoryAddress;
static uint64_t rsdtPointer = 0;
static uint64_t xsdtPointer = 0;

uint64_t getRSDTPointer() {
    return rsdtPointer;
}

uint64_t getXSDTPointer() {
    return xsdtPointer;
}

static void fillMemoryInfo(MemoryInfo* mem, uint64_t& lastPageID, uint64_t base_addr, uint64_t length) {
    mem->start = base_addr;
    mem->length = length;
    mem->nextNode = nullptr;
    mem->pageIndexStart = base_addr;
    lastPageID += length / pageSize;
    mem->pageIndexStart = lastPageID;
}

struct BiosBootDevice {
    uint32_t biosdevice;
    uint32_t partition;
    uint32_t subpartition;
};
BiosBootDevice biosBootDevice;
uint32_t imageBaseAddress;

void get_multiboot_infos() {
    rsdtPointer = 0;
    xsdtPointer = 0;
    struct TagStructure {
        uint32_t total_size;
        uint32_t reserved;
    } __attribute__((__packed__));
    struct BasicTag {
        uint32_t type;
        uint32_t size;
    } __attribute__((__packed__));
    uint32_t tagsPointer = multiboot_information_structure;

    MemoryInfo bufferInCaseTheFirstHitIsKernelCode{};
    bool memoryInfoBufferIsUsed = false;
    uint64_t lastPageID = 0;

    TagStructure head = *(TagStructure*) (void*) (size_t) tagsPointer;
    tagsPointer += sizeof(head);

    kout << "Tags: ";

    for (uint32_t remainingSize = head.total_size; remainingSize;) {
        BasicTag* tag = (BasicTag*) (void*) (size_t) tagsPointer;
        tagsPointer += realign(tag->size);
        remainingSize -= realign(tag->size);
        kout << Hex(tag->type) << ", ";
        if (tag->type == 0xE) {
            RSDPDescriptor* desc = (RSDPDescriptor*) (((uint32_t*) tag) + 2);
            if (basicStrEq(desc->Signature, "RSD PTR ", 8)) {
                rsdtPointer = (uint64_t) (desc->RsdtAddress & 0xFFFFFFFF);
            } else {
                kout << "Error while reading RSDPDescriptor\n";
            }
        } else if (tag->type == 0xF) {
            RSDPDescriptor20* desc = (RSDPDescriptor20*) (((uint32_t*) tag) + 2);
            if (basicStrEq(desc->firstPart.Signature, "RSD PTR ", 8)) {
                rsdtPointer = (uint64_t) (desc->firstPart.RsdtAddress & 0xFFFFFFFF);
                xsdtPointer = desc->XsdtAddress;
            } else {
                kout << "Error while reading XSDPDescriptor\n";
            }

        } else if (tag->type == 0x6) {
            uint32_t tagSize = tag->size;
            uint32_t* data = ((uint32_t*) tag) + 2;
            uint32_t entrySize = data[0];
            uint32_t entryVersion = data[1];
            data += 2;
            tagSize -= 16;

            for (; tagSize > 0;) {
                uint32_t entrySizeLeft = entrySize;
                uint64_t base_addr = *(uint64_t*) data;
                entrySizeLeft -= 8;

                data += 2;
                uint64_t length = *(uint64_t*) data;
                entrySizeLeft -= 8;
                data += 2;
                uint32_t type = *data;
                entrySizeLeft -= 4;
                data += 1;

                uint32_t reserved = *data;
                entrySizeLeft -= 4;
                data += 1;

                uint8_t* bytes = (uint8_t*) data;
                bytes += entrySizeLeft;
                tagSize -= entrySize;

                if (type == 1) {
                    totalMemorySize += length;
                    maxMemoryAddress = (maxMemoryAddress < base_addr + length) ? base_addr + length : maxMemoryAddress;
                    if (info == nullptr) {
                        if (base_addr >= (uint64_t) &kernel_start && base_addr <= (uint64_t) &kernel_end) {
                            // boi this is kernel code DO NOT USE THIS TO STORE RANDOM INFORMATION
                            fillMemoryInfo(&bufferInCaseTheFirstHitIsKernelCode, lastPageID, base_addr, length);
                            memoryInfoBufferIsUsed = true;
                        } else {
                            if (sizeof(MemoryInfo) < length) {
                                info = (MemoryInfo*) base_addr;
                                if (info == nullptr)
                                    info += 1;// do not start at nullptr

                                info = new (info + 1) MemoryInfo();
                                fillMemoryInfo(info, lastPageID, base_addr, length);

                                if (memoryInfoBufferIsUsed) {
                                    memoryInfoBufferIsUsed = false;
                                    info->nextNode = new (info + 1) MemoryInfo(bufferInCaseTheFirstHitIsKernelCode);
                                }
                            } else {
                                Print::println("Error while loading memory");// todo kernel panic because memory is weird
                            }
                        }
                    } else {
                        MemoryInfo* lastNode = info;
                        for (; lastNode->nextNode; lastNode = lastNode->nextNode)
                            ;
                        lastNode->nextNode = new (lastNode + 1) MemoryInfo();
                        fillMemoryInfo(lastNode->nextNode, lastPageID, base_addr, length);
                    }
                }
            }
        } else if (tag->type == 0x8) {
            VGA::readFrameBufferInfo((uint8_t*) tag);
        } else if (tag->type == 0x5) {
            biosBootDevice.biosdevice = ((uint32_t*) tag)[2];
            biosBootDevice.partition = ((uint32_t*) tag)[3];
            biosBootDevice.subpartition = ((uint32_t*) tag)[4];
        } else if (tag->type == 0x4) {
            //ignore basic memory information
        } else if (tag->type == 0x9) {
            //TODO: advanced debug information with elf symbols
        } else if (tag->type == 0xA) {
            // APM only useable in 32 bit mode
        } else if (tag->type == 0x2) {
            kout << "Using bootloader: " << ((char*) tag) + 8 << '\n';
        } else if (tag->type == 0x1) {
            kout << "Boot command: " << ((char*) tag) + 8 << '\n';
        } else if (tag->type == 0x11) {
            //efi memory map
        } else if (tag->type == 0x15) {
            imageBaseAddress = ((uint32_t*) tag)[2];
        } else if (tag->type != 0) {
            kout << "Unknown Multiboot information structure type: \n";
            kout << Hex(tag->type) << '\n';
        }

        if (tag->type == 0)
            break;
    }
    kout << '\n';
}

MemoryInfo* getMemoryInfoPtr() {
    return info;
}

bool PhysicalMemoryManagment::isValidMemory(uint64_t physical) {
    for (MemoryInfo* ptr = info; ptr != nullptr; ptr = ptr->nextNode) {
        if (physical >= ptr->start && physical < ptr->start + ptr->length) {
            return true;
        }
    }
    return false;
}

bool PhysicalMemoryManagment::isUsed(uint64_t physical) {
    uint64_t pageIndex = getPageIndex(physical);
    return (physicalPageUsageBitmap[pageIndex / 8] >> (pageIndex % 8)) & 0b1u;
}

uint64_t PhysicalMemoryManagment::getPageIndex(uint64_t physical) {
    for (MemoryInfo* ptr = info; ptr != nullptr; ptr = ptr->nextNode) {
        if (physical >= ptr->start && physical < ptr->start + ptr->length) {
            uint64_t offset = physical - ptr->start;
            return ptr->pageIndexStart + (offset / pageSize);
        }
    }
    return 0;
}

void PhysicalMemoryManagment::setUsed(uint64_t physical, bool val) {
    if (val) {
        uint64_t pageIndex = getPageIndex(physical);
        physicalPageUsageBitmap[pageIndex / 8] |= 0b1 << (pageIndex % 8);
    } else {
        uint64_t pageIndex = getPageIndex(physical);
        physicalPageUsageBitmap[pageIndex / 8] &= ~(0b1 << (pageIndex % 8));
    }
}

uint64_t PhysicalMemoryManagment::getTotalMemorySize() {
    return totalMemorySize;
}

void PhysicalMemoryManagment::init() {
    const MemoryInfo* last = info;
    for (; last->nextNode; last = last->nextNode)
        ;

    if ((((uint64_t) info) + (totalMemorySize / pageSize) / 8) < info->start + info->length) {
        physicalPageUsageBitmap = (uint8_t*) (last + 1);
        physicalPageUsageBitmapSize = (totalMemorySize / pageSize) / 8;

        for (uint64_t i = 0; i < physicalPageUsageBitmapSize; ++i) {
            physicalPageUsageBitmap[i] = 0x00;
        }

        uint64_t endOfMemoryInfo = (uint64_t) physicalPageUsageBitmap + physicalPageUsageBitmapSize;
        uint64_t startOfMemoryInfo = (uint64_t) info;
        for (uint64_t index = startOfMemoryInfo; index < endOfMemoryInfo; index += pageSize) {
            setUsed(index, true);
        }

        uint64_t kernelStart = (uint64_t) &kernel_start;
        uint64_t kernelEnd = (uint64_t) &kernel_end;

        for (uint64_t index = kernelStart; index < kernelEnd; index += pageSize) {
            setUsed(index, true);
        }

    } else {
        kout << "PANIC\n";
    }
}

static uint64_t lastUserPage = 0;//1GiB
static constexpr uint64_t lastUserPageStart = 1Gi;

uint64_t PhysicalMemoryManagment::getFreeUserPage() {
    if (lastUserPage) {
        lastUserPage = lastUserPageStart;
    }
    uint64_t searchStart = lastUserPage;
    lastUserPage += pageSize;
    for (; lastUserPage != searchStart; lastUserPage += pageSize) {
        if (lastUserPage >= maxMemoryAddress) {
            lastUserPage = 0x40000000;// if memory end reached start again at user space start
        }
        if (isValidMemory(lastUserPage) && !isUsed(lastUserPage)) {
            return lastUserPage;
        }
    }
    return 0;
}

static uint64_t lastKernelPage = 0x00;
uint64_t PhysicalMemoryManagment::getFreeKernelPage() {
    uint64_t searchStart = lastKernelPage;
    lastKernelPage += pageSize;
    for (; lastKernelPage != searchStart; lastKernelPage += pageSize) {
        if (lastKernelPage >= 1Gi) {
            lastKernelPage = 0;// if kernel end reached start again at Kernel space start
        }
        if (isValidMemory(lastKernelPage) && !isUsed(lastKernelPage)) {
            return lastKernelPage;
        }
    }
    return 0;
}

static bool lastArea = 0;
uint64_t PhysicalMemoryManagment::getAnyPage() {
    lastArea != lastArea;
    uint64_t result;
    if (lastArea) {
        result = getFreeUserPage();
        if (result == 0)
            result = getFreeKernelPage();
    } else {
        result = getFreeKernelPage();
        if (result == 0)
            result = getFreeUserPage();
    }
    return result;
}

uint64_t MemoryPage::getPhysicalAddressFromVirtual(uint64_t address) {
    if (address < 1Gi)
        return address;// low memory is identity mapped
    uint64_t offset = address & (pageSize - 1);
    address &= ~(pageSize - 1);
    uint64_t l4Index = ((address >> 39) & 0x1FF);// to 9 bit
    uint64_t l3Index = ((address >> 30) & 0x1FF);// to 9 bit
    uint64_t l2Index = ((address >> 21) & 0x1FF);// to 9 bit
    uint64_t l1Index = ((address >> 12) & 0x1FF);// to 9 bit
    page_table_l4.elements[l4Index];
    volatile PageTableElement* l4Element = &page_table_l4.elements[l4Index];
    if (!l4Element->flags.present)
        return 0;
    PageTable* l3Table = (PageTable*) (l4Element->address & ~0xFFF);
    volatile PageTableElement* l3Element = &l3Table->elements[l3Index];
    if (!l3Element->flags.present)
        return 0;
    PageTable* l2Table = (PageTable*) (l3Element->address & ~0xFFF);
    volatile PageTableElement* l2Element = &l2Table->elements[l2Index];
    if (!l2Element->flags.present)
        return 0;
    PageTable* l1Table = (PageTable*) (l2Element->address & ~0xFFF);
    volatile PageTableElement* l1Element = &l1Table->elements[l1Index];
    if (!l1Element->flags.present)
        return 0;
    return (l1Element->address & ~0xFFF) | offset;
}

MemoryPage::MemoryPage(uint64_t physicalAddress) : MemoryPage(physicalAddress, false) {}

MemoryPage::MemoryPage(uint64_t physicalAddress, bool unsafe) {
    physicalAddress &= ~(pageSize - 1);
    if (unsafe) {
        MemoryPage::physicalAddress = physicalAddress;
        MemoryPage::virtualAddress = 0;
        MemoryPage::valid = true;
        MemoryPage::destruct = false;
        return;
    }
    if ((PhysicalMemoryManagment::isValidMemory(physicalAddress) && !PhysicalMemoryManagment::isUsed(physicalAddress))) {
        PhysicalMemoryManagment::setUsed(physicalAddress, true);
        MemoryPage::physicalAddress = physicalAddress;
        MemoryPage::virtualAddress = 0;
        MemoryPage::valid = true;
        MemoryPage::destruct = true;
    } else {
        MemoryPage::valid = false;
    }
}

MemoryPage::~MemoryPage() {
    if (destruct) {
        if (checkIfMapped()) {// only destruct if mapped
            unmap();
            PhysicalMemoryManagment::setUsed(physicalAddress, false);
        }
        destruct = false;
    }
}

void MemoryPage::softmap(uint64_t virtualAddress, bool writeable, bool userAccess, bool cacheDisable, bool writeThrough) {
    MemoryPage::virtualAddress = virtualAddress;
    MemoryPage::writeable = writeable;
    MemoryPage::userAccess = userAccess;
    MemoryPage::cacheDisable = cacheDisable;
    MemoryPage::writeThrough = writeThrough;
}

bool MemoryPage::mapTo(uint64_t virtualAddress, bool writeable, bool userAccess, bool cacheDisable, bool writeThrough) {
    virtualAddress &= 0xFFFFFFFFF000;                   // 48 bit virtual address max and page start
    uint64_t l4Index = ((virtualAddress >> 39) & 0x1FF);// to 9 bit
    uint64_t l3Index = ((virtualAddress >> 30) & 0x1FF);// to 9 bit
    uint64_t l2Index = ((virtualAddress >> 21) & 0x1FF);// to 9 bit
    uint64_t l1Index = ((virtualAddress >> 12) & 0x1FF);// to 9 bit

    volatile PageTableElement* l4Element = &page_table_l4.elements[l4Index];
    if (!l4Element->flags.present) {
        uint64_t newPageTable = createNewPageTable();// create L3 table
        if (newPageTable == 0)
            return false;
        l4Element->address = newPageTable;
        l4Element->flags.present = true;
        l4Element->flags.writeEnable = writeable;
        l4Element->flags.allowUserAccess = userAccess;
        l4Element->flags.writeThrough = false;
        l4Element->flags.cacheDisable = false;
        l4Element->flags.zero = 0;
        reloadPageTables((uint64_t) l4Element);
    }
    PageTable* l3Table = (PageTable*) (l4Element->address & ~0xFFF);
    volatile PageTableElement* l3Element = &l3Table->elements[l3Index];
    if (!l3Element->flags.present) {
        uint64_t newPageTable = createNewPageTable();// create L2 table
        if (newPageTable == 0)
            return false;
        l3Element->address = newPageTable;
        l3Element->flags.present = true;
        l3Element->flags.writeEnable = writeable;
        l3Element->flags.allowUserAccess = userAccess;
        l3Element->flags.writeThrough = false;
        l3Element->flags.cacheDisable = false;
        l3Element->flags.zero = 0;
        reloadPageTables((uint64_t) l3Element);
    }
    PageTable* l2Table = (PageTable*) (l3Element->address & ~0xFFF);
    volatile PageTableElement* l2Element = &l2Table->elements[l2Index];
    if (!l2Element->flags.present) {
        uint64_t newPageTable = createNewPageTable();// create L1 table
        if (newPageTable == 0)
            return false;
        l2Element->address = newPageTable;
        l2Element->flags.present = true;
        l2Element->flags.writeEnable = writeable;
        l2Element->flags.allowUserAccess = userAccess;
        l2Element->flags.writeThrough = false;
        l2Element->flags.cacheDisable = false;
        l2Element->flags.zero = 0;
        reloadPageTables((uint64_t) l2Element);
    }

    if (l2Element->address & 0b10000000) {// this is 2MB page do not use!
        return false;
    }

    PageTable* l1Table = (PageTable*) (l2Element->address & ~0xFFF);
    volatile PageTableElement* l1Element = &l1Table->elements[l1Index];

    l1Element->address = physicalAddress;
    l1Element->flags.present = true;
    l1Element->flags.writeEnable = writeable;
    l1Element->flags.allowUserAccess = userAccess;
    l1Element->flags.cacheDisable = cacheDisable;
    l1Element->flags.writeThrough = writeThrough;
    l1Element->flags.zero = 0;
    reloadPageTables((uint64_t) l1Element);
    MemoryPage::entry = l1Element;
    MemoryPage::virtualAddress = virtualAddress;
    MemoryPage::writeable = writeable;
    MemoryPage::userAccess = userAccess;
    return true;
}

void MemoryPage::remap() {
    mapTo(virtualAddress, writeable, userAccess, cacheDisable, writeThrough);
}

void MemoryPage::unmap() {
    entry->flags.present = false;
    reloadPageTables((uint64_t) entry);
}

uint64_t MemoryPage::createNewPageTable() {
    uint64_t address = PhysicalMemoryManagment::getFreeKernelPage();
    if (PhysicalMemoryManagment::isValidMemory(address) && !PhysicalMemoryManagment::isUsed(address)) {
        PhysicalMemoryManagment::setUsed(address, true);
        uint8_t* page = (uint8_t*) address;
        for (uint64_t j = 0; j < pageSize; ++j) {
            page[j] = 0;
        }
        return address;
    }
    return 0;
}

MemoryPage::MemoryPage(MemoryPage&& other) noexcept {
    physicalAddress = other.physicalAddress;
    virtualAddress = other.virtualAddress;
    entry = other.entry;
    valid = other.valid;
    destruct = other.destruct;

    other.destruct = false;
}
MemoryPage& MemoryPage::operator=(MemoryPage&& other) noexcept {
    if (this == &other)
        return *this;
    this->~MemoryPage();

    physicalAddress = other.physicalAddress;
    virtualAddress = other.virtualAddress;
    entry = other.entry;
    valid = other.valid;
    destruct = other.destruct;

    other.destruct = false;

    return *this;
}

void memcpy(void* dest, const void* src, uint64_t count) {
    for (uint64_t i = 0; i < count; ++i) {
        ((uint8_t*) dest)[i] = ((const uint8_t*) src)[i];
    }
}

void memset(void* dest, uint8_t value, uint64_t count) {
    for (uint64_t i = 0; i < count; ++i) {
        ((uint8_t*) dest)[i] = value;
    }
}

bool memequ(const void* a, const void* b, uint64_t count) {
    if (a == b)
        return true;
    for (uint64_t i = 0; i < count; i++) {
        if (((uint8_t*) a)[i] != ((uint8_t*) b)[i])
            return false;
    }
    return true;
}

}// namespace Kernel

void* operator new(size_t count, void* ptr) {
    return ptr;
}

void* operator new[](size_t count, void* ptr) {
    return ptr;
}
