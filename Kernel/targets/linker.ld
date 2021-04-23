ENTRY(start)

SECTIONS
{
	. = 1M;

	kernel_start = .;
	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		*(.text)
	}

	.bss :
	{
		*(.bss)
	}

	.rodata : 
	{
		*(.rodata)
	}
	kernel_end = .;
}