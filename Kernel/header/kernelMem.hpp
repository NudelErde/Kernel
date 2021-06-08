#pragma once
#include "MemoryManager.hpp"

namespace Kernel {

MemoryManager& getKernelMemoryManager();
void initKernelDynamicMemory();
MemoryManager& getCurrentMemoryManager();

}// namespace Kernel

void* operator new(uint64_t count);
void* operator new[](uint64_t count);

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete(void* ptr, uint64_t size) noexcept;
void operator delete[](void* ptr, uint64_t size) noexcept;