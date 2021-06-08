#pragma once
#include "memory.hpp"
#include "stdint.h"

namespace Kernel {

class MemoryManager {
public:
    MemoryManager(bool inKernel, uint64_t start, uint64_t pageCount);
    ~MemoryManager();

    void* malloc(uint64_t size);
    void free(void* ptr);

    void reload();

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager(MemoryManager&&) noexcept;
    MemoryManager& operator=(MemoryManager&&) noexcept;

    static MemoryManager* getActiveHeap();

private:
    struct PageBuffer;

    struct PageListNode {
        bool exists;
        uint64_t objectCount;
        PageListNode* next;
        PageListNode* prev;
        MemoryPage page;
        uint64_t remainingSizeInPhysical;
        uint8_t index;
        PageBuffer* buffer;
    };

    struct PageBuffer {
        PageListNode buffer[16];
        PageBuffer* next;
        PageBuffer* prev;
    };

    struct PointerData {
        PageListNode* node;
        uint64_t pageCount;
        uint64_t realSize;
    };

    struct VirtualMemoryAvailabilityNode {
        VirtualMemoryAvailabilityNode* next;
        VirtualMemoryAvailabilityNode* prev;
        uint64_t startPagesBaseAddress;
        uint64_t usedPages;// first used then free
        uint64_t freePages;
    };

    uint64_t getFreePhysicalPage();
    void appendPhysicalPage();
    void removePhysicalPage(PageListNode*);
    void removeReferenceToPage(PageListNode*);
    uint64_t findFreeVirtualPages(uint64_t count);
    void removeVirtualPages(uint64_t baseAddress, uint64_t count);
    void deallocVirtualPageInfo(VirtualMemoryAvailabilityNode* node);
    VirtualMemoryAvailabilityNode* allocVirtualPageInfo();

    PageListNode firstPageNode;
    PageListNode* lastPageNode;
    PageBuffer* firstPageBuffer;

    VirtualMemoryAvailabilityNode firstMemoryAvailabilityNode;

    bool inKernel;
    bool valid;
};

}// namespace Kernel