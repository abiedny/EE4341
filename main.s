	.file	1 "main.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text,code
	.align	2
	.globl	gcd
	.set	nomips16
	.set	nomicromips
	.ent	gcd
	.type	gcd, @function
gcd:
	.frame	$fp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
# End mchp_output_function_prologue
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp
	sw	$4,24($fp)
	sw	$5,28($fp)
	lw	$3,24($fp)
	lw	$2,28($fp)
	bne	$3,$2,.L2
	nop

	lw	$2,24($fp)
	j	.L3
	nop

.L2:
	lw	$3,24($fp)
	lw	$2,28($fp)
	slt	$2,$2,$3
	beq	$2,$0,.L4
	nop

	lw	$3,24($fp)
	lw	$2,28($fp)
	subu	$2,$3,$2
	move	$4,$2
	lw	$5,28($fp)
	jal	gcd
	nop

	j	.L3
	nop

.L4:
	lw	$3,28($fp)
	lw	$2,24($fp)
	subu	$2,$3,$2
	lw	$4,24($fp)
	move	$5,$2
	jal	gcd
	nop

.L3:
	move	$sp,$fp
	lw	$31,20($sp)
	lw	$fp,16($sp)
	addiu	$sp,$sp,24
	j	$31
	nop

	.set	macro
	.set	reorder
# Begin mchp_output_function_epilogue
# End mchp_output_function_epilogue
	.end	gcd
	.size	gcd, .-gcd
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
# End mchp_output_function_prologue
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$fp,16($sp)
	move	$fp,$sp
.L6:
	li	$4,18			# 0x12
	li	$5,12			# 0xc
	jal	gcd
	nop

	j	.L6
	nop

	.set	macro
	.set	reorder
# Begin mchp_output_function_epilogue
# End mchp_output_function_epilogue
	.end	main
	.size	main, .-main
	.ident	"GCC: (Microchip Technology) 4.8.3 MPLAB XC32 Compiler v2.50"
# Begin MCHP vector dispatch table
# End MCHP vector dispatch table
# Microchip Technology PIC32 MCU configuration words
