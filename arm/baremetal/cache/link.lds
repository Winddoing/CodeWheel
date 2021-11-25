OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(_start)
SECTIONS {
	. = 0x00000000;
	image_copy_start = .;
	.text : {
		start.o (.text*)
        	*(.text*)
	}
	. = ALIGN(8);
	rodata_start = .;
	.rodata : {
		*(.rodata*)
	}
	. = ALIGN(8);
	rodata_end = .;

	. = ALIGN(8);
	data_start = .;
	.data : {
		*(.data*)
	}
	. = ALIGN(8);
	data_end = .;
	image_copy_end = .;

	. = ALIGN(8);
	.bss : {
		bss_start = .;
		*(.bss*)
		. = ALIGN(8);
		bss_end = .;
	}
}
