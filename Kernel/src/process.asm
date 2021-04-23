global __process_toProcess
global __process_toKernel
global __systemHandler
global __syscallData
extern onSystemCall

section .text:
bits 64
__systemHandler:
    mov qword[__syscallData], rax
    mov qword[__syscallData + 8], rbx
    mov qword[__syscallData + 16], rcx
    mov qword[__syscallData + 24], rdx

    jmp onSystemCall ; the c interrupt handler function will return from interrupt

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
    mov rsp, rdx ; load process stack pointer
    mov byte[rbx], $1 ; rbx is pointer to inProcess ; set in process after loading process stack pointer
    jmp rax ; jump to process code

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

section .bss:
__kernel_stack_pointer:
    resb 8
__syscallData:
    resb 32