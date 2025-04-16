/* jprx */
ENTRY(demo_start);

/* iBoot puts us somewhere random, just pretend it's 0 */
APPLESI_LOAD_BASE = 0x00000000;

/* iBoot requires 16K alignment */
PAGE_SIZE = 16K;

SECTIONS {
	. = APPLESI_LOAD_BASE;

	.text.start ALIGN(PAGE_SIZE) : {
		*(.text.start);
	}

	.text ALIGN(PAGE_SIZE) : {
		*(.text);
		*(.text.*);
		*(.got.plt);
	}

	.bss ALIGN(PAGE_SIZE) : {
		PROVIDE(_bss_start = .);
		*(.bss);
		*(.bss.*);
		PROVIDE(_bss_end = .);
	}

	.data ALIGN(PAGE_SIZE) : {
		*(.data);
		*(.data.*);
		*(.rodata);
		*(.rodata.*);

		. = ALIGN(8);
		*(.got);

		. = ALIGN(PAGE_SIZE);
		PROVIDE(_stack_top = .);
		. += 16K;
		PROVIDE(_stack_bottom = .);

		/* iBoot won't load us unless we are 16K page aligned in size */
		. = ALIGN(PAGE_SIZE);

		/* Use rest of RAM for page tables */
		PROVIDE(_page_tables = .);
	}

	/DISCARD/ : { * }
}
