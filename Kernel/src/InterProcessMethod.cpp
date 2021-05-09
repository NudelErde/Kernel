#include "InterProcessMethod.hpp"
#include "process.hpp"

namespace Kernel {

    extern "C" void __forceReloadEnter() {
        Thread::getCurrent()->setEnterIPM();
        Thread::toKernel();
    }

    extern "C" void __forceReloadExit() {
        Thread::getCurrent()->setExitIPM();
        Thread::toKernel();
    }

    extern "C" uint64_t __switchToImpl(uint64_t* threadPidPointer, uint64_t targetPid, uint64_t* argPointer, uint64_t argCount, uint64_t functionAddress);
    
    uint64_t InterProcessMethod::switchTo(uint64_t* argPointer, uint64_t targetPid) {
        return __switchToImpl(&Thread::getCurrent()->pid, targetPid, argPointer, (uint64_t)argCount, address);
    }

asm(R"(
__switchToImpl:
    pushq %rbp
    movq %rsp, %rbp
    pushq %rbx #move base pointer

    subq $72, %rsp

    movq %rdi, -40(%rbp) # -40 is threadPidPointer
    movq %rdx, -48(%rbp) # -48 is argPointer
    movq %rcx, -56(%rbp) # -56 is argCount
    movq %r8, -64(%rbp) # -64 is functionAddress

    movq (%rdi), %rax    # load old pid
    movq %rax, -72(%rbp) # -72 is old pid
    movq %rsi, (%rdi)    # load new pid
    
    call __forceReloadEnter # return to kernel -> kernel will resume with correct process loaded

    mov -56(%rbp), %rax  # load count in rax
    mov -48(%rbp), %rbx  # load arg pointer in rbx

.loop:
    cmp $6, %rax        # if (6 > rax) {
    jle .register_load

    movq (%rbx), %rcx
    push %rcx

    sub $1, %rax
    add $8, %rbx
    jmp .loop
                        # }
.register_load:

    cmp $6, %rax # skip count everything except 6
    jne .no_r9
    movq (%rbx), %r9
    sub $1, %rax
    add $8, %rbx
.no_r9:

    cmp $5, %rax # skip count everything except 5
    jne .no_r8
    movq (%rbx), %r8
    sub $1, %rax
    add $8, %rbx
.no_r8:

    cmp $4, %rax # sskip count everything except 4
    jne .no_rcx
    movq (%rbx), %rcx
    sub $1, %rax
    add $8, %rbx
.no_rcx:

    cmp $3, %rax # skip count everything except 3
    jne .no_rdx
    movq (%rbx), %rdx
    sub $1, %rax
    add $8, %rbx
.no_rdx:

    cmp $2, %rax # skip count everything except 2
    jne .no_rsi
    movq (%rbx), %rsi
    sub $1, %rax
    add $8, %rbx
.no_rsi:

    cmp $1, %rax # skip count everything except 1
    jne .no_rdi
    movq (%rbx), %rdi
    sub $1, %rax
    add $8, %rbx
.no_rdi:

    movq -64(%rbp), %rax
    call *%rax # %rax has return value
    movq %rax, -64(%rbp) # overrite function address with result

    mov -56(%rbp), %rbx # load argCount in rbx

    cmp $6, %rbx        # if (6 > rax) {
    jbe .simple_cleanup
    lea -48(%rsp, %rbx, 8), %rsp
.simple_cleanup:        # }

    # move value in -72 in (-40)
    mov -40(%rbp), %rax
    mov -72(%rbp), %rbx
    mov %rbx, (%rax)

    call __forceReloadExit
    
    movq -8(%rbp), %rbx
    movq -64(%rbp), %rax # load result from function address field
    leave
    ret
)");

}