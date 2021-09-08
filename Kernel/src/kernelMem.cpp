#include "kernelMem.hpp"
#include "KernelOut.hpp"
#include "debug.hpp"
#include "interrupt.hpp"
#include "print.hpp"
#include "units.hpp"

namespace Kernel {

static char buffer[sizeof(MemoryManager)];
static MemoryManager* ptr;

void printLPageFlagsAsText(uint64_t flags) {
    const char* text[]{"present", "writeEnable", "userAccess", "write-through", "cache-disable", "accessed", "ignored", "big-page", "ignore", "ignore", "ignore", "ignore"};
    uint64_t index = 0;
    bool first = true;
    for (; flags != 0; flags >>= 1) {
        if (flags & 0b1) {
            if (!first) {
                kout << ", ";
            } else {
                kout << "   ";
            }
            kout << text[index];
            first = false;
        }
        index++;
    }
}

void onPageFault(const Interrupt& i) {
    uint64_t mem;
    asm(R"(
        movq %%cr2, %0
        )"
        : "=a"(mem)
        :
        :);
    kout << "Page fault at 0x" << Hex(mem) << '\n';
    uint64_t physicalAddress = MemoryPage::getPhysicalAddressFromVirtual(mem);
    kout << "Physical: " << Hex(physicalAddress) << '\n';
    uint64_t flags = MemoryPage::getFlagsForVirtualPage(mem);

    kout << "Paging flags: \n";
    kout << "  CR3: " << BitList((flags >> 48) & 0xFFF) << '\n';
    kout << "  L4:  " << BitList((flags >> 36) & 0xFFF);
    printLPageFlagsAsText((flags >> 36) & 0xFFF);
    kout << '\n';
    kout << "  L3:  " << BitList((flags >> 24) & 0xFFF);
    printLPageFlagsAsText((flags >> 24) & 0xFFF);
    kout << '\n';
    kout << "  L2:  " << BitList((flags >> 12) & 0xFFF);
    printLPageFlagsAsText((flags >> 12) & 0xFFF);
    kout << '\n';
    kout << "  L1:  " << BitList((flags >> 0) & 0xFFF);
    printLPageFlagsAsText((flags >> 0) & 0xFFF);
    kout << '\n';

    kout << "Error code: " << BitList(i.errorCode);

    const char* errorCodeTranslation[]{"present", "write", "user", "reserved bit", "instruction fetch", "protection key", "", "", "", "", "", "", "", "", "", "SGX"};
    uint64_t index = 0;
    bool first = true;
    for (uint64_t error = i.errorCode; error != 0; error >>= 1) {
        if (error & 0b1) {
            if (!first) {
                kout << ", ";
            } else {
                kout << "   ";
            }
            kout << errorCodeTranslation[index];
            first = false;
        }
        index++;
    }

    kout << '\n';

    Debug::printDebugInfo(i.stackFrame);

    asm("hlt");
}

void initKernelDynamicMemory() {
    //register page fault handler
    Interrupt::setHandler(14, onPageFault, 1);

    ptr = new ((void*) buffer) MemoryManager(false, 64Ti, 100);
    ptr->reload();// set as active heap
}

MemoryManager& getKernelMemoryManager() {
    return *ptr;
}

MemoryManager& getCurrentMemoryManager() {
    return *MemoryManager::getActiveHeap();
}

}// namespace Kernel

void* operator new(uint64_t count) {
    return Kernel::getCurrentMemoryManager().malloc(count);
}

void* operator new[](uint64_t count) {
    return Kernel::getCurrentMemoryManager().malloc(count);
}

void operator delete(void* ptr) noexcept {
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete[](void* ptr) noexcept {
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete(void* ptr, uint64_t size) noexcept {
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete[](void* ptr, uint64_t size) noexcept {
    Kernel::getCurrentMemoryManager().free(ptr);
}
