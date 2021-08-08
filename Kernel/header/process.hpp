#pragma once

#include "InterProcessMethod.hpp"
#include "KernelOut.hpp"
#include "Scheduler.hpp"
#include "kernelMem.hpp"
#include "memory.hpp"
#include "stdint.h"

namespace Kernel {

constexpr uint64_t stackPageCount = 8;

class Thread {
    MemoryPage stack[stackPageCount];
    uint64_t currentCodeAddress;
    uint64_t stackAddress;
    uint64_t pid;
    uint64_t tid;
    uint64_t earliestSchedule{};
    uint64_t waitingForExitValue{};
    uint8_t locks[32];
    bool markedProcess{};
    bool enterIPM{};
    bool exitIPM{};
    bool debugOnStart{};

    friend class Scheduler;
    friend class InterProcessMethod;

public:
    Thread(MemoryPage stack[stackPageCount], uint64_t currentCodeAddress, uint64_t pid, bool debugOnStart = false);
    Thread(Thread&& other) noexcept = default;
    Thread& operator=(Thread&& other) noexcept = default;
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    ~Thread();

    void setWaiting(uint64_t microseconds);

    inline uint64_t getCurrentCodeAddress() { return currentCodeAddress; }
    inline uint64_t getStackAddress() { return stackAddress; }
    inline uint64_t getEarliestSchedule() { return earliestSchedule; }
    inline uint64_t getPID() { return pid; }
    inline uint64_t getTID() { return tid; }
    inline uint64_t getStackBaseAddress() { return stack[0].getVirtualAddress(); }

    inline void setEnterIPM() { enterIPM = true; }
    inline void setExitIPM() { exitIPM = true; }

    inline uint64_t getWaitForPIDResult() { return waitingForExitValue; }
    inline uint8_t makeLock() {
        for (uint8_t i = 0; i < 32; ++i) {
            if ((i == 0 && locks[i] != 0xFE) || locks[i] != 0xFF) {
                for (uint8_t j = 0; j < 8; ++j) {
                    if (i == 0 && j == 0)
                        continue;
                    if (!(locks[i] & (0b1 << j))) {
                        locks[i] |= (0b1 << j);
                        return i * 8 + j;
                    }
                }
            }
        }
        return 0x00;
    }
    inline void clearLock(uint8_t n) {
        locks[n / 8] &= ~(0b1 << (n % 8));
    }

    static Thread* getCurrent();

    void reload();
    void toProcess();
    static void toKernel();
    static bool isInProgram();
};

class Process {
public:
    struct Lock {
        uint64_t tid;
        uint8_t lock;
    };
    static constexpr uint64_t maxSharedPages = 8;
    static constexpr uint64_t maxInterProcessMethods = 16;

    struct UnsafeMap {
        uint64_t vAddress;
        uint64_t pAddress;
        bool write;
        bool cacheDisable;
    };
    struct UnsafeMapNode {
        UnsafeMap map;
        UnsafeMapNode* next;
    };
    UnsafeMapNode* unsafeMapping;

private:
    MemoryPage* programPages;
    MemoryManager heap;
    struct SharedMemoryPageInfo {
        uint64_t sharedPageId;
        union PageBuffer {
            MemoryPage page;
            uint8_t buffer[sizeof(MemoryPage)];
            PageBuffer() : buffer{} {}
            ~PageBuffer() {}
        } pageBuffer;
    } sharedPages[maxSharedPages];
    InterProcessMethod interProcessMethods[maxInterProcessMethods]{};// todo dynamic allocated

    uint64_t finalReturnValue{};
    uint64_t count;
    uint64_t pid;
    uint64_t parentPid;
    uint64_t threads{};
    uint64_t sharedPagesLocation;
    const char* argumentPointer;
    bool finished{};
    bool valid{};

    Lock finishLocks[32];

    friend class Scheduler;
    friend class Debug;

public:
    Process(MemoryPage* programPages, uint64_t count, MemoryManager&& heap, uint64_t sharedPagesLocation, uint64_t parentPid);
    Process(Process&& other) noexcept;
    Process& operator=(Process&& other) noexcept;
    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;
    ~Process();

    void reload();
    void unload();

    uint64_t createSharedMemoryPage();
    void useSharedMemoryPage(uint64_t sharedMemoryId);
    uint64_t getPointerToSharedPage(uint64_t sharedMemoryId);
    void unuseSharedMemoryPage(uint64_t sharedMemoryId);
    void addProcessToSharedMemoryPage(uint64_t sharedMemoryId, uint64_t pid);

    inline const char* getArgumentPointer() { return argumentPointer; }
    inline void setArgumentPointer(const char* str) { argumentPointer = str; }
    inline uint64_t getPID() { return pid; }
    inline uint64_t getParentPID() { return parentPid; }
    inline uint64_t getThreadCount() { return threads; }
    inline InterProcessMethod* getInterPorcessMethods() { return interProcessMethods; }
    inline void setFinishLock(uint64_t tid, uint8_t lock) {
        for (uint8_t i = 0; i < 32; ++i) {
            if (finishLocks[i].tid == 0) {
                finishLocks[i].tid = tid;
                finishLocks[i].lock = lock;
                return;
            }
        }
    }

    static void init();
    static Process* getLastLoadedProcess();

    inline MemoryManager& getHeap() { return heap; }

    inline void exit(uint64_t returnValue) {
        finished = true;
        finalReturnValue = returnValue;
    }

    inline bool isRunning() { return !finished; }
    inline uint64_t getReturnValue() { return finalReturnValue; }
};

}// namespace Kernel