global __check_cpu_random_support
global __cpu_random_support_var
global __cpu_random_value
global __cpu_random_value_success
global __cpu_random_generate

section .text
bits 64
__check_cpu_random_support:
    push rax
    push rcx
    mov rax, 1
    mov rcx, 0
    cpuid
    shr rcx, 30
    and rcx, 1
    mov byte[__cpu_random_support_var], cl
    pop rcx
    pop rax
    ret

__cpu_random_generate:
    push rax
    push rcx
    mov rcx, 100
.retry:
    rdrand rax
    jc .done
    loop .retry
.fail:
    mov rax, 0
    mov byte[__cpu_random_value_success], al
    pop rcx
    pop rax
    ret
.done:
    mov qword[__cpu_random_value], rax
    mov rax, 1
    mov byte[__cpu_random_value_success], al
    pop rcx
    pop rax
    ret

section .bss
__cpu_random_support_var:
    resb 1
__cpu_random_value:
    resb 8
__cpu_random_value_success:
    resb 1