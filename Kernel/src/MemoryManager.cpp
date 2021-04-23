#include "MemoryManager.hpp"
#include "KernelOut.hpp"
#include "process.hpp"

namespace Kernel{

MemoryManager* activeHeap;

MemoryManager* MemoryManager::getActiveHeap() {
    return activeHeap;
}

MemoryManager::MemoryManager(bool inKernel, uint64_t start, uint64_t pageCount): inKernel(inKernel) {
    firstMemoryAvailabilityNode.next = nullptr;
    firstMemoryAvailabilityNode.prev = nullptr;
    firstMemoryAvailabilityNode.startPagesBaseAddress = start & ~0xFFFu;
    firstMemoryAvailabilityNode.usedPages = 0;
    firstMemoryAvailabilityNode.freePages = pageCount;

    firstPageNode.prev = nullptr;
    firstPageNode.next = nullptr;
    firstPageNode.page = MemoryPage(getFreePhysicalPage());
    uint64_t virtualPage = findFreeVirtualPages(1);
    firstPageNode.page.mapTo(virtualPage, true, inKernel);
    firstPageNode.exists = true;
    firstPageNode.remainingSizeInPhysical = pageSize;
    firstPageNode.index = 0;
    firstPageNode.buffer == nullptr;
    firstPageNode.objectCount = 0;
    lastPageNode = &firstPageNode;

    firstPageBuffer = (PageBuffer*)malloc(sizeof(PageBuffer));
    firstPageBuffer->next = nullptr;
    firstPageBuffer->prev = nullptr;
    for(uint8_t i = 0; i < 16; ++i) {
        firstPageBuffer->buffer[i].exists = false;
    }
    valid = true;
}

MemoryManager::~MemoryManager() {
    if(!valid)
        return;
    for(PageListNode* node = lastPageNode; node; node = node -> prev) {
        node->page.unmap();
    }
}

void MemoryManager::reload() {
    activeHeap = this;
    for(PageListNode* ptr = &firstPageNode; ptr; ptr = ptr->next) {
        ptr->page.remap();
    }
}

MemoryManager::MemoryManager(MemoryManager&& other) noexcept {
    firstPageNode = (PageListNode&&)other.firstPageNode;

    if(other.lastPageNode == &other.firstPageNode) { // if pointer in local => recalc pointer
        lastPageNode = &firstPageNode;
    } else {
        lastPageNode = other.lastPageNode;
    }

    firstMemoryAvailabilityNode = other.firstMemoryAvailabilityNode;
    firstPageBuffer = other.firstPageBuffer;
    inKernel = other.inKernel;
    valid = other.valid;
    other.valid = false;
}
MemoryManager& MemoryManager::operator=(MemoryManager&& other) noexcept {
    if(valid) {
        for(PageListNode* node = lastPageNode; node; node = node -> prev) {
            node->page.unmap();
        }
    }
    
    firstPageNode = (PageListNode&&)other.firstPageNode;

    if(other.lastPageNode == &other.firstPageNode) { // if pointer in local => recalc pointer
        lastPageNode = &firstPageNode;
    } else {
        lastPageNode = other.lastPageNode;
    }

    firstMemoryAvailabilityNode = other.firstMemoryAvailabilityNode;
    firstPageBuffer = other.firstPageBuffer;
    inKernel = other.inKernel;

    other.lastPageNode = nullptr;
    other.firstPageBuffer = nullptr;
    other.inKernel = false;
    valid = other.valid;
    other.valid = false;
    return *this;
}

uint64_t MemoryManager::getFreePhysicalPage() {
    return inKernel ? PhysicalMemoryManagment::getFreeKernelPage() : PhysicalMemoryManagment::getFreeUserPage();
}

void* MemoryManager::malloc(uint64_t size) {
    uint64_t realSize = sizeof(PointerData) + size;
    for(PageListNode* node = &firstPageNode; node; node = node->next) {
        if(node->remainingSizeInPhysical >= realSize) { // pack in existing node
            uint64_t baseAddress = node->page.getVirtualAddress();
            baseAddress += pageSize;
            baseAddress -= node->remainingSizeInPhysical;

            PointerData* ptrData = (PointerData*) baseAddress;
            ptrData->node = node;
            ptrData->realSize = realSize;
            ptrData->pageCount = 1;

            baseAddress += sizeof(PointerData);
            node->objectCount += 1;

            node->remainingSizeInPhysical -= realSize;

            return (void*)baseAddress;
        }
    }
    uint64_t memoryPageCount = (realSize / pageSize) + 1;
    uint64_t virtualBaseAddress = findFreeVirtualPages(memoryPageCount);
    uint64_t returnAddress = virtualBaseAddress + sizeof(PointerData);

    for(uint64_t index = 0; index < memoryPageCount; ++index) {
        appendPhysicalPage();

        lastPageNode->objectCount += 1;
        if(memoryPageCount == index + 1) {
            lastPageNode->remainingSizeInPhysical -= realSize % pageSize;
        } else {
            lastPageNode->remainingSizeInPhysical = 0;
        }
        lastPageNode->page.mapTo(virtualBaseAddress + pageSize * index, true, !inKernel);

        if(index == 0) {
            PointerData* ptrData = (PointerData*) virtualBaseAddress;
            ptrData->node = lastPageNode; // in loop because of lastPageNode
            ptrData->realSize = realSize;
            ptrData->pageCount = memoryPageCount;
        }
    }
    return (void*)returnAddress;
}

uint64_t MemoryManager::findFreeVirtualPages(uint64_t count) {
    for(VirtualMemoryAvailabilityNode* node = &firstMemoryAvailabilityNode; node; node = node->next) {
        if(node->freePages >= count) {
            uint64_t offset = node->usedPages * pageSize;
            node->freePages -= count;
            node->usedPages += count;

            //check if mergeable with next
            while((node->freePages == 0) && (node->next)) {
                //move next node in this
                VirtualMemoryAvailabilityNode* next = node->next;
                node->next = next->next;
                if(node->next) {
                    node->next->prev = node;
                }
                node->freePages = next->freePages;
                node->usedPages += next->usedPages;

                next->next = nullptr;
                next->prev = nullptr;
                deallocVirtualPageInfo(next);
            }
            return node->startPagesBaseAddress + offset;
        }
    }
    //TODO: error could not find new pages
    return 0;
}

void MemoryManager::removeVirtualPages(uint64_t base, uint64_t count) {
    base &= ~0xFFF;
    uint64_t endAddress = base + count * pageSize;
    for(VirtualMemoryAvailabilityNode* node = &firstMemoryAvailabilityNode; node; node = node->next) {
        uint64_t nodeEnd = node->startPagesBaseAddress + node->usedPages * pageSize;
        if(endAddress == nodeEnd) { // remove at end of used section
            // *****_____
            // ***_______
            node->usedPages -= count;
            node->freePages += count;
            // check for merge with prev
            if(node->usedPages == 0 && node->prev) {
                // _______
                node->prev->freePages += node->freePages;
                //remove node
                VirtualMemoryAvailabilityNode* next = node->next;
                VirtualMemoryAvailabilityNode* prev = node->prev;
                if(next) {
                    next->prev = prev;
                }
                if(prev) {
                    prev->next = next;
                }
                node->next = nullptr;
                node->prev = nullptr;
                deallocVirtualPageInfo(node);
            }
            return;
        } else if(base == node->startPagesBaseAddress && endAddress < nodeEnd && node->prev) {
            // ++++ ********________ 
            // ++++ __******________ 
            // ++++ __ ******________ 
            // ++++__ ******________ 
            node->prev->freePages += count;
            node->usedPages -= count;
            node->startPagesBaseAddress += count * pageSize;
            return;
        } else if(base >= node->startPagesBaseAddress && endAddress < nodeEnd) {
            // ********________ 
            // ***__***________ 
            // ***__ ***________ 
            // |     ***   __    |     ***   ________ 
            // base1 used1 free1 base2 used2 free2
            uint64_t base1 = node->startPagesBaseAddress;
            uint64_t used1 = (base - node->startPagesBaseAddress) / pageSize;
            uint64_t free1 = count;

            uint64_t base2 = (used1 + free1) * pageSize + base1;
            uint64_t used2 = node->usedPages - (used1 + free1);
            uint64_t free2 = node->freePages;

            VirtualMemoryAvailabilityNode* newNode = allocVirtualPageInfo();
            newNode->startPagesBaseAddress = base2;
            newNode->usedPages = used2;
            newNode->freePages = free2;

            node->startPagesBaseAddress = base1;
            node->usedPages = used1;
            node->freePages = free1;

            //insert newNode

            //links from newNode
            newNode->next = node->next;
            newNode->prev = node;

            //links to newNode
            if(node->next) {
                node->next->prev = newNode;
            }
            node->next = newNode;
            return;
        }
    }
    //TODO: error could not remove pages
}

void MemoryManager::appendPhysicalPage() {
    PageListNode* result;

    bool done = false;
    uint8_t index;
    PageBuffer* buffer;
    for(buffer = firstPageBuffer; buffer && !done; buffer = buffer->next) {
        for(index = 0; index < 16; ++index) {
            if(!buffer->buffer[index].exists) {
                result = &buffer->buffer[index];
                done = true;
                break;
            }
        }
    }

    //init result
    result->buffer = buffer;
    result->exists = true;
    result->index = index;
    result->objectCount = 0;
    uint64_t physicalPage = getFreePhysicalPage();
    result->page = MemoryPage(physicalPage);
    result->remainingSizeInPhysical = pageSize;

    if(index == 14 && buffer->next == nullptr) {
        //create next buffer
        PageListNode* fillPage = &(buffer->buffer[15]);
        
        //init fillPage
        fillPage->buffer = buffer;
        fillPage->exists = true;
        fillPage->index = 15;
        fillPage->objectCount = 0;
        physicalPage = getFreePhysicalPage();
        fillPage->page = MemoryPage(physicalPage);
        fillPage->remainingSizeInPhysical = pageSize;

        //insert fillPage
        fillPage->prev = lastPageNode;
        lastPageNode->next = fillPage;
        lastPageNode->next = nullptr;
        lastPageNode = fillPage;

        PageBuffer* nBuffer = (PageBuffer*)malloc(sizeof(PageBuffer));
        
        for(uint8_t i = 0; i < 16; ++i) {
            nBuffer->buffer[i].exists = false;
        }

        nBuffer->next = nullptr;
        nBuffer->prev = buffer;

        buffer->next = nBuffer;
    }

    //insert result
    result->prev = lastPageNode;
    lastPageNode->next = result;
    result->next = nullptr;
    lastPageNode = result;
}
void MemoryManager::removePhysicalPage(PageListNode* node) {
    node->exists = false;
    node->page.~MemoryPage();

    if(&firstPageNode == node) {
        return; // do not delete initial page
    }
    PageBuffer* buffer = node->buffer;
    for(uint8_t i = 0; i < 16; ++i) {
        if(buffer->buffer[i].exists)
            return;
    }
    PageBuffer* next = buffer->next;
    PageBuffer* prev = buffer->prev;

    if(next) {
        next->prev = prev;
    }
    if(prev) {
        prev->next = next;
    }
    buffer->next = nullptr;
    buffer->prev = nullptr;
    free(buffer);
}

uint64_t i = 3;
void MemoryManager::removeReferenceToPage(PageListNode* node) {
    if(node == &firstPageNode) {
        node->remainingSizeInPhysical == pageSize; // reuse init page
        return;
    }

    removeVirtualPages(node->page.getVirtualAddress(), 1);

    PageListNode* next = node->next;
    PageListNode* prev = node->prev;

    if(node == lastPageNode) {
        lastPageNode = node->prev;
    }

    node->next = nullptr;
    node->prev = nullptr;
    if(next) {
        next->prev = prev;
    }
    if(prev) {
        prev->next = next;
    }
    removePhysicalPage(node);
}

void MemoryManager::free(void* ptr){
    if(ptr == nullptr)
        return;
    PointerData* ptrData = (PointerData*)(((uint8_t*)(ptr)) - sizeof(PointerData));
    if(!ptrData->node->exists) {
        return; //invalid pointer
    }
    if(ptrData->pageCount == 1) {
        --ptrData->node->objectCount;
        if(ptrData->node->objectCount == 0) {
            removeReferenceToPage(ptrData->node);
        }
    } else {
        if(((uint64_t)ptrData) & 0xFFF) {
            return;
        } else {
            PageListNode* node = ptrData->node;
            uint64_t count = ptrData->pageCount;
            for(count++; count > 0; --count) {
                if(!node)
                    return;
                --node->objectCount;
                PageListNode* tmp = node;
                node = node->next;
                if(tmp->objectCount == 0) {
                    removeReferenceToPage(tmp);
                }
                
            }
        }
    }
}

void MemoryManager::deallocVirtualPageInfo(VirtualMemoryAvailabilityNode* node){
    free(node); //pls
}
MemoryManager::VirtualMemoryAvailabilityNode* MemoryManager::allocVirtualPageInfo(){
    return (VirtualMemoryAvailabilityNode*) malloc(sizeof(VirtualMemoryAvailabilityNode)); //pls
}

}