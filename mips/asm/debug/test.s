	.file	1 "test.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=32
	.module	oddspreg
	.abicalls
	.option	pic0
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-8
	sw	$fp,4($sp)
	move	$fp,$sp
#APP
 # 12 "test.c" 1
	ssnop
	
 # 0 "" 2
 # 13 "test.c" 1
	ssnop
	
 # 0 "" 2
 # 15 "test.c" 1
	.set push		    
	.set noreorder		
	.word   0x40028000 
	.word   0x3c030000 
	.word   0x2463fff8 
	.word   0x00431024 
	.word   0x34420003 
	.word   0x40828000 
	.word   0x3c020800 
	.word   0x40826800 
	.word   0x40804800 
	.word   0x3c021000 
	.word   0x40826000 
	.word   0x3c1d8067 
	.word   0x27bd2448 
	.word   0x8fbd0000 
	.word   0x3c1c8067 
	.word   0x279c244c 
	.word   0x8f9c0000 
	.word   0x3c1f8049 
	.word   0x27ffb748 
	.word   0x03e00008 
	.word   0x00000000 
	.word   0x00000000 
	.word   0x00000000 
	.word   0x00000000 
	.set    pop		
	
 # 0 "" 2
 # 44 "test.c" 1
	ssnop
	
 # 0 "" 2
 # 45 "test.c" 1
	ssnop
	
 # 0 "" 2
#NO_APP
	move	$2,$0
	move	$sp,$fp
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	j	$31
	.end	main
	.size	main, .-main
	.ident	"GCC: (Ingenic r3.2.0-gcc520 2017.08-24) 5.2.0"
