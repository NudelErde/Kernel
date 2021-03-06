global start
global multiboot_information_structure
global page_table_l4
global page_table_l3
global page_table_l2
global gdt64
extern long_mode_start

section .text
bits 32
start:
	mov esp, stack_top

	call check_multiboot

	mov dword[multiboot_information_structure], ebx

	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
	jmp 0b1000:long_mode_start

	hlt

check_multiboot:
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret
.no_multiboot:
	mov edx, .no_multiboot_string
	jmp error
.no_multiboot_string:
	db "Not started by multiboot", 0  

check_cpuid:
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	cmp eax, ecx
	je .no_cpuid
	ret
.no_cpuid:
	mov edx, .no_cpuid_string
	jmp error
.no_cpuid_string:
	db "No cpuid string", 0  

check_long_mode:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_long_mode

	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	
	ret
.no_long_mode:
	mov edx, .no_long_mode_string
	jmp error
.no_long_mode_string:
	db "No long mode supported", 0  

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; present, writable
	mov [page_table_l4], eax
	
	mov eax, page_table_l2
	or eax, 0b11 ; present, writable
	mov [page_table_l3], eax

	mov ecx, 0 ; counter
.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_l4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

error:
	mov ESI, edx ; clone edx to esi
	mov EDI, 0xB8000 ; output to edi

.loop:
	mov al, [ESI] ; current char in eax
	
	mov byte [EDI], al ; char in output address
	mov byte [EDI+1], 4 ; color red

	inc ESI
	inc EDI
	inc EDI

	cmp al, 0

	je .error_end
	jmp .loop

.error_end:
	hlt

section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4
stack_top:
multiboot_information_structure:
	resb 4
section .rodata
gdt64:
	dq 0 ; zero entry
.code_segment: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; code segment
.code_segment_ring3: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 45) | (1 << 46) | (1 << 47) | (1 << 53) ; ring 3 code segment
.data_segment_ring3: equ $ - gdt64
	dq (1 << 41) | (1 << 44) | (1 << 45) | (1 << 46) | (1 << 47)
.task_state_segment: equ $ - gdt64
	dq 0
	dq 0
.pointer:
	dw $ - gdt64 - 1 ; length
	dq gdt64 ; address
