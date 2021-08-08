#include "Scheduler.hpp"
#include "APIC.hpp"
#include "KernelOut.hpp"
#include "List.hpp"
#include "debug.hpp"
#include "interrupt.hpp"
#include "kernelMem.hpp"
#include "process.hpp"
#include "units.hpp"
#include "wait.hpp"

namespace Kernel {

static constexpr uint64_t maxProcessTime = 300000;
static constexpr uint64_t maxProcessCount = 16;

static uint64_t currentRelativeTime;
static uint8_t staticProcessBuffer[sizeof(Process)];

template<typename T>
struct ListNode {
    T* value;
    ListNode* next;
};

ListNode<Process>* procFirstNode;
ListNode<Thread>* threadFirstNode;

static bool removed;

void Scheduler::forceKillProcess(uint64_t pid) {
    if (Thread::isInProgram())
        getKernelMemoryManager().reload();

    if (procFirstNode->value->getPID() == pid) {
        procFirstNode = procFirstNode->next;
    }

    for (auto ptr = procFirstNode; ptr->next; ptr = ptr->next) {
        if (ptr->next->value->getPID() == pid) {
            auto removed = ptr->next;
            ptr->next = removed->next;
            delete removed->value;
            removed->value = 0;
            delete removed;
            break;
        }
    }

    if (Thread::isInProgram())
        Process::getLastLoadedProcess()->reload();
}

void Scheduler::forceKillThread(uint64_t tid) {
    if (Thread::isInProgram())
        getKernelMemoryManager().reload();

    if (threadFirstNode->value->getTID() == tid) {
        threadFirstNode = threadFirstNode->next;
    }

    for (auto ptr = threadFirstNode; ptr->next; ptr = ptr->next) {
        if (ptr->next->value->getTID() == tid) {
            auto removed = ptr->next;
            ptr->next = removed->next;
            delete removed->value;
            removed->value = 0;
            delete removed;
            break;
        }
    }

    if (Thread::isInProgram())
        Process::getLastLoadedProcess()->reload();
}

Process* Scheduler::getProcessById(uint64_t pid) {
    if (Thread::isInProgram())
        getKernelMemoryManager().reload();

    Process* proc = nullptr;

    for (auto ptr = procFirstNode; ptr; ptr = ptr->next) {
        if (ptr->value->getPID() == pid) {
            proc = ptr->value;
            break;
        }
    }

    if (Thread::isInProgram())
        Process::getLastLoadedProcess()->reload();
    return proc;
}

Thread* Scheduler::getThreadById(uint64_t tid) {
    if (Thread::isInProgram())
        getKernelMemoryManager().reload();

    Thread* thread = nullptr;

    for (auto ptr = threadFirstNode; ptr; ptr = ptr->next) {
        if (ptr->value->getTID() == tid) {
            thread = ptr->value;
            break;
        }
    }

    if (Thread::isInProgram())
        Process::getLastLoadedProcess()->reload();
    return thread;
}

uint8_t memoryPageBuffer[sizeof(MemoryPage)];

void Scheduler::addProcess(Process&& proc) {
    if (Thread::isInProgram()) {
        // move dynamic memory from process to kernel heap
        MemoryPage* pagesInCurrentProcess = proc.programPages;
        MemoryPage* pagesInKernel = (MemoryPage*) getKernelMemoryManager().malloc(sizeof(MemoryPage) * proc.count);
        for (uint64_t i = 0; i < proc.count; ++i) {
            memset(memoryPageBuffer, 0, sizeof(MemoryPage));
            ((Process*) staticProcessBuffer)->reload();
            new (memoryPageBuffer) MemoryPage((MemoryPage &&) pagesInCurrentProcess[i]);// in place construct in tmp
            getKernelMemoryManager().reload();
            new (pagesInKernel + i) MemoryPage((MemoryPage &&) * memoryPageBuffer);// in place construct from tmp to
        }
        proc.programPages = pagesInKernel;
        ((Process*) staticProcessBuffer)->reload();
        delete (uint8_t*) pagesInCurrentProcess;
        //getKernelMemoryManager().reload();
    }
    getKernelMemoryManager().reload();
    auto tmp = procFirstNode;
    procFirstNode = new ListNode<Process>();
    procFirstNode->value = new Process((Process &&) proc);
    procFirstNode->next = tmp;
}

void Scheduler::addThread(Thread&& thread) {
    if (Thread::isInProgram())
        getKernelMemoryManager().reload();

    auto tmp = threadFirstNode;
    threadFirstNode = new ListNode<Thread>();
    threadFirstNode->value = new Thread((Thread &&) thread);
    threadFirstNode->next = tmp;

    if (Thread::isInProgram())
        Process::getLastLoadedProcess()->reload();
}

void Scheduler::removeCurrentThread() {
    removed = true;// destruct in scheduler loop
}

void schedule(void (*finish)()) {
    currentRelativeTime += maxProcessTime;
    if (Thread::isInProgram()) {
        finish();
        Thread::toKernel();
        return;
    }
    finish();
}

uint64_t Scheduler::getCurrentRelativeTime() {
    return currentRelativeTime + maxProcessTime - LocalAPIC::getTimeLeft();
}

void Scheduler::run() {
    ListNode<Thread>* tnode = threadFirstNode;
    while (true) {
        if (tnode == nullptr) {
            tnode = threadFirstNode;
            continue;
        }
        Thread* currentThread = tnode->value;

        tnode = tnode->next;

        bool isLocked = false;
        for (uint8_t i = 0; i < 32; ++i) {
            if (currentThread->locks[i] != 0) {
                isLocked = true;
                break;
            }
        }
        if (isLocked) {
            continue;
        }

        bool cont = true;
        Process* proc = getProcessById(currentThread->getPID());

        if (proc != nullptr) {
            if (!currentThread->markedProcess) {
                ++(proc->threads);
                currentThread->markedProcess = true;
            }
            if (currentThread->enterIPM) {
                ++(proc->threads);
                currentThread->enterIPM = false;
            }
            currentRelativeTime += maxProcessTime - LocalAPIC::getTimeLeft();// add time left in timer before starting the new timer
            LocalAPIC::interruptIn(maxProcessTime, schedule);
            if (currentThread->getEarliestSchedule() > currentRelativeTime) {
                continue;// continue doesn't stop the timer -> next iteration should have different relative time
            }
            //Process* procPtr = new (staticProcessBuffer) Process((Process &&) * proc);
            proc->reload();
            currentThread->reload();
            currentThread->toProcess();
            proc->unload();
            //new (proc) Process((Process &&) * procPtr);
            if (currentThread->exitIPM) {
                --(proc->threads);
                currentThread->exitIPM = false;
            }
        } else {
            removed = true;// thread has no process -> remove;
        }
        getKernelMemoryManager().reload();

        if (removed && proc) {// thread finished
            --(proc->threads);
        }
        if (proc->threads == 0) {// process finished
            uint64_t val = proc->getReturnValue();
            for (uint8_t i = 0; i < 32; ++i) {
                if (proc->finishLocks[i].tid) {
                    getThreadById(proc->finishLocks[i].tid)->clearLock(proc->finishLocks[i].tid);
                }
            }
            forceKillProcess(proc->pid);
            getKernelMemoryManager().reload();
        }
        if (removed) {
            forceKillThread(currentThread->tid);
        }

        removed = false;
    }
}

void Scheduler::init() {
    removed = false;
    currentRelativeTime = 100;
}
}// namespace Kernel