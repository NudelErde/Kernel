#pragma once

#include "stdint.h"

namespace Kernel {

class Process;
class Thread;

class Scheduler {
public:
    static void addProcess(Process&& proc);
    static void addThread(Thread&& thread);
    static void removeCurrentThread();
    static void run();
    static void init();
    static uint64_t getCurrentRelativeTime();

    static Process* getProcessById(uint64_t pid);
    static Thread* getThreadById(uint64_t tid);
};

}