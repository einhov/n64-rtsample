
	.text
	.align 2

// Appparently nop before start to trick GDB?
//	nop

	.globl _start
	.ent _start
_start:
// From libn64
// Tell PIF to not throw NMI
	addiu $v0, $zero, 0x8
	lui $v1, 0xbfc0
	sw $v0, 0x07fc($v1)
// (Get available RAM)
// Set initial status register
	lui $v0, 0x2000
	addiu $v0, $v0, 0x400
	mtc0 $v0, $12

// From newlib
// Clear Cause register?	
	mtc0 $zero, $13
// Set global data pointer
	la $gp, _gp
// Zero out BSS section
	la $v0, __bss_start
	la $v1, __bss_end
1:
	addiu $v0, $v0, 8
.set gp=64
	sd $zero, -8($v0)
.set gp=32
	bne $v0, $v1, 1b
// Set up stack
	li $sp, 0x80200000

	jal _init # Global constructors
	jal main  # Call C main
	j .

	.end _start
