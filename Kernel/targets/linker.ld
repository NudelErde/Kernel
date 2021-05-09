ENTRY(start)

SECTIONS
{
	. = 1M;

	kernel_start = .;
	.boot :
	{
		KEEP(*(.multiboot_header))
	}
	boot_end = .;
	.text :
	{
		*(.text)
	}
	code_end = .;

	.bss :
	{
		*(.bss)
	}

	.rodata : 
	{
		*(.rodata)
	}

	.data : 
	{
		*(.data)
	}

	kernel_end = .;
}