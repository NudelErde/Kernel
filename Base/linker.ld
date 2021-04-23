OUTPUT_FORMAT(elf64-x86-64)
ENTRY(__start)

SECTIONS {
    . = 1536M;
    
    .text : 
    {
        *(.text)
    }
    
    .data : 
    {
        *(.data)
    }
    
    .bss : 
    {
        *(.bss)
    }
    
    .rodata : 
    {
        *(.rodata)
    }
}
