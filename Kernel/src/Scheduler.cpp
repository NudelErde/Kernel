#include "Scheduler.hpp"
#include "APIC.hpp"
#include "kernelMem.hpp"
#include "KernelOut.hpp"
#include "wait.hpp"
#include "process.hpp"
#include "List.hpp"
#include "interrupt.hpp"
#include "debug.hpp"

namespace Kernel{

    static constexpr uint64_t maxProcessTime = 3000;
    static constexpr uint64_t maxProcessCount = 16;

    static uint64_t currentRelativeTime;
    static uint8_t staticProcessBuffer[sizeof(Process)];

    static uint8_t processListBuffer[sizeof(LinkedList<Process>)];
    static uint8_t threadListBuffer[sizeof(LinkedList<Thread>)];

    LinkedList<Process>* processList;
    LinkedList<Thread>* threadList;

    static bool removed;

    void onInvalidOpcode(const Interrupt& inter) {
        if(inter.hasErrorCode) {
            kout << "Error code: " << inter.errorCode << '\n';
        }
        kout << "Invalid Opcode\n";
        printDebugInfo(inter.stackFrame);
        asm("hlt");
    }

    void onDebug3(const Interrupt& inter) {
        kout << "Breakpoint\n";
        printDebugInfo(inter.stackFrame);
    }

    void onDebug(const Interrupt& inter) {
        debugHandler(inter.stackFrame);
    }

    Process* Scheduler::getProcessById(uint64_t pid) {
        if(Thread::isInProgram())
            getKernelMemoryManager().reload();
        
        Process* proc = nullptr;

        bool searching = true;
        for(auto iter = processList->getIterator() ; iter.valid() && searching; searching = iter.next()) {
            if(iter.get()->getPID() == pid) {
                proc = iter.get();
                break;
            }
        }

        if(Thread::isInProgram())
            Process::getLastLoadedProcess()->reload();
        return proc;
    }

    uint8_t memoryPageBuffer[sizeof(MemoryPage)];

    void Scheduler::addProcess(Process&& proc) {
        if(Thread::isInProgram()) {
            // move dynamic memory from process to kernel heap
            MemoryPage* pagesInCurrentProcess = proc.programPages;
            MemoryPage* pagesInKernel = (MemoryPage*)getKernelMemoryManager().malloc(sizeof(MemoryPage) * proc.count);
            for(uint64_t i = 0; i < proc.count; ++i) {
                memset(memoryPageBuffer, 0, sizeof(MemoryPage));
                ((Process*)staticProcessBuffer)->reload();
                new (memoryPageBuffer) MemoryPage((MemoryPage&&)pagesInCurrentProcess[i]); // in place construct in tmp
                getKernelMemoryManager().reload();
                new (pagesInKernel+i) MemoryPage((MemoryPage&&)*memoryPageBuffer); // in place construct from tmp to 
            }
            proc.programPages = pagesInKernel;
            ((Process*)staticProcessBuffer)->reload();
            delete (uint8_t*)pagesInCurrentProcess;
            getKernelMemoryManager().reload();
        }
        processList->getIterator().insert((Process&&)proc);
    }

    void Scheduler::addThread(Thread&& thread) {
        getKernelMemoryManager().reload();
        threadList->getIterator().insert((Thread&&) thread);
        if(Thread::isInProgram())
            ((Process*)staticProcessBuffer)->reload();
    }

    void Scheduler::removeCurrentThread() {
        removed = true; // destruct in scheduler loop
    }

    void schedule() {
        currentRelativeTime += maxProcessTime;
        if(Thread::isInProgram()) {
            Thread::toKernel();
        }
    }

    uint64_t Scheduler::getCurrentRelativeTime() {
        return currentRelativeTime + maxProcessTime - LocalAPIC::getTimeLeft();
    }

    void Scheduler::run() {
        auto iter = threadList->getIterator();
        while(true) {
            if(!iter.next()) {
                iter = threadList->getIterator();
            }
            Thread* currentThread = iter.get();
            currentRelativeTime += maxProcessTime - LocalAPIC::getTimeLeft(); // add time left in timer before starting the new timer
            LocalAPIC::interruptIn(maxProcessTime, schedule);
            if(currentThread->getEarliestSchedule() > currentRelativeTime) {
                continue; // continue doesn't stop the timer -> next iteration should have different relative time
            }
            if(currentThread->waitingForPID != 0) {
                continue;
            }
            bool cont = true;
            Process* proc = nullptr;;
            LinkedList<Process>::Iterator procIter = processList->getIterator();
            for(; procIter.valid() && cont; cont = procIter.next()) {
                if(procIter.get()->getPID() == currentThread->getPID()) {
                    proc = procIter.get();
                    break;
                }
            }
            if(proc != nullptr) {
                if(!currentThread->markedProcess) {
                    ++(proc->threads);
                    currentThread->markedProcess = true;
                }
                if(currentThread->enterIPM) {
                    ++(proc->threads);
                    currentThread->enterIPM = false;
                }
                Process* procPtr = new(staticProcessBuffer)Process((Process&&)*proc);
                procPtr->reload();
                currentThread->reload();
                currentThread->toProcess();
                new(proc)Process((Process&&)*procPtr);
                if(currentThread->exitIPM) {
                    --(proc->threads);
                    currentThread->exitIPM = false;
                }
            } else {
                removed = true; // thread has no process -> remove;
            }
            getKernelMemoryManager().reload();

            if(removed && proc) { // thread finished
                --(proc->threads);
            }
            if(proc->threads == 0) { // process finished
                uint64_t val = proc->getReturnValue();
                uint64_t thePid = proc->getPID();
                procIter.remove();
                getKernelMemoryManager().reload();
                auto iter = threadList->getIterator();
                do {
                    auto obj = iter.get();

                    if(obj->waitingForPID == thePid) {
                        obj->waitingForPID = 0;
                        obj->waitingForExitValue = val;
                    }
                } while(iter.next());
            }
            if(removed) {
                iter.remove();
            }

            removed = false;
        }
    }

    void Scheduler::init() {
        threadList = (LinkedList<Thread>*)threadListBuffer;
        processList = (LinkedList<Process>*)processListBuffer;
        removed = false;
        currentRelativeTime = 100;
        Interrupt::setHandler(6, onInvalidOpcode);
        Interrupt::setHandler(3, onDebug3);
        Interrupt::setHandler(1, onDebug);
    }
}