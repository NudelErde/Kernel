global __process_toProcess
global __process_toKernel
global __systemHandler
extern onSystemCall
extern onSystemCallExit
extern jmpAndChangePrivilegeLevel

section .text
bits 64
__systemHandler:
    push rbp
    mov  rbp, rsp
    add  rsp, -16
    mov  rdi, rax
    mov  rsi, rbx
    xchg rdx, rcx
    call onSystemCall
    leave

    jmp onSystemCallExit ; the c interrupt handler function will return from interrupt

;extern "C" void __process_toProcess(uint64_t rip, uint64_t rsp, uint64_t inProcessPointer, uint64_t targetStackSegment, uint64_t targetCodeSegment)
;                                             rdi           rsi            rdx                                      rcx                          r8 r9
__process_toProcess:
    push rax ; save registers
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    pushfq


    mov qword[__kernel_stack_pointer], rsp ; store kernel stack pointer
    
    xchg rdi, rsi ; rip -> targetCode & rsp -> targetStack
    mov byte[rdx], $1
    mov rdx, rcx
    mov rcx, r8
    mov r8, qword[rsp] ; flags -> targetFlags
                                   ;                  rdi                  rsi                          rdx                         rcx                    r8 r9
    jmp jmpAndChangePrivilegeLevel;(uint64_t targetStack, uint64_t targetCode, uint64_t targetStackSegment, uint64_t targetCodeSegment, uint64_t targetFlags)

__process_toProcess_return_to_kernel: ; process executed return to kernel stack
    mov rsp, qword[__kernel_stack_pointer] ; load kernel stack pointer
    popfq
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret

__process_toKernel:
    mov qword[rax], .continue ; save continue address in Process::currentCodeAddress
    push rax ; save registers
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    pushfq

    mov qword[rdx], rsp ; store process stack pointer in Process::stackAddress
    jmp __process_toProcess_return_to_kernel ; jump to next instruction of kernel program thing code stuff

.continue:
    popfq
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret

section .bss
__kernel_stack_pointer:
    resb 8