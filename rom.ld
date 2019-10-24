OUTPUT_FORMAT("elf32-bigmips")
OUTPUT_ARCH(mips)

ENTRY(_start)

SECTIONS {
	.text 0xFFFFFFFF80000400 : AT (0xFFFFFFFF80000400) {
		crt0.o
		KEEP(*(.init .init.*));
		*(.fini .fini.*)
		*(.text.startup .text.startup.*)
		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.eh_frame)
/*		KEEP(*(.dtors .dtors.*));
*/
		KEEP(*(.ctors .ctors.*));
	}

	.rodata : {
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r.*)
	}

	.data : {
		*(.data .data.*)
		*(.gnu.linkonce.d.*)
	}

	.got.plt : {
		*(.got.plt)
	}

	HIDDEN (_gp = ALIGN(16) + 0x7FF0);

	.got : {
		*(.got)
	}

	.sdata : {
		*(.sdata .sdata.*)
		*(.gnu.linkonce.s.*)
	}

	.lits : {
		*(.lit8 .lit4)
	}

	. = ALIGN(16);
	__bss_start = .;
	.bss : {
		*(.bss .bss.*)
	}
	. = ALIGN(16);
	__bss_end = .;

	/DISCARD/ : {
		*(.MIPS.abiflags)
	}
}
