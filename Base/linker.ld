OUTPUT_FORMAT(elf64-x86-64)
ENTRY(__start)

SECTIONS {
    . = 1536M;
    
    .text : ALIGN(0x1000)
    {
        *(.text)
    }
    
    .data : ALIGN(0x1000)
    {
        *(.data)
    }
    
    .bss : ALIGN(0x1000)
    {
        *(.bss)
    }
    
    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }
}
