#include "kernelMem.hpp"
#include "KernelOut.hpp"
#include "interrupt.hpp"
#include "print.hpp"
#include "KernelOut.hpp"

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
    kout
        << "Page fault.\n"
        << "Access to " << Hex(mem) << '\n'
        << "Error code 0b" << Bin(i.errorCode) << '\n'
        << setDisplay()
        << "Page fault.\n"
        << "Access to " << Hex(mem) << '\n'
        << "Error code 0b" << Bin(i.errorCode) << '\n'
        ;
    asm("hlt");
}

void initKernelDynamicMemory() {
    //register page fault handler
    Interrupt::setHandler(14, onPageFault);

    ptr = new ((void*)buffer) MemoryManager(false, 0x80000000, 100);
    ptr->reload(); // set as active heap
}

MemoryManager& getKernelMemoryManager() {
    return *ptr;
}

MemoryManager& getCurrentMemoryManager() {
    return *MemoryManager::getActiveHeap();
}

}

void* operator new(uint64_t count){
    return Kernel::getCurrentMemoryManager().malloc(count);
}

void* operator new[](uint64_t count){
    return Kernel::getCurrentMemoryManager().malloc(count);
}

void operator delete(void* ptr) noexcept{
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete[](void* ptr) noexcept{
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete(void* ptr, uint64_t size) noexcept{
    Kernel::getCurrentMemoryManager().free(ptr);
}

void operator delete[](void* ptr, uint64_t size) noexcept{
    Kernel::getCurrentMemoryManager().free(ptr);
}
