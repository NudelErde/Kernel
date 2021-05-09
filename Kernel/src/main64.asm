global long_mode_start
global asmInterruptTable
extern gdt64.code_segment
extern gdt64.task_state_segment
extern __kernel_main

section .text
bits 64
long_mode_start:
    ; load null into all data segment registers
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

	call __kernel_main
    hlt

section .bss
align 4096
asmInterruptTable:
    resb 4096