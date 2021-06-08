#include "kernelMem.hpp"
#include "KernelOut.hpp"
#include "debug.hpp"
#include "interrupt.hpp"
#include "print.hpp"

namespace Kernel {

static char buffer[sizeof(MemoryManager)];
static MemoryManager* ptr;

void onPageFault(const Interrupt& i) {
    uint64_t mem;
    asm(R"(
        mov %%cr2, %0
        )"
        : "=a"(mem)
        :
        :);
    kout << "Page fault at 0x" << Hex(mem) << '\n';
    kout << "Error code: " << BitList(i.errorCode) << '\n';
    Debug::printDebugInfo(i.stackFrame);

    asm("hlt");
}

void initKernelDynamicMemory() {
    //register page fault handler
    Interrupt::setHandler(14, onPageFault, 1);

    ptr = new ((void*) buffer) MemoryManager(false, 0x80000000, 100);
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
