	.text
	.globl main
main:
# swap values $t0 and $t1 (clobbers $t2)
	lw $t0 , str1
	lw $t1 , str2
	move $t2, $t0
	move $t0, $t1
	move $t1, $t2

str1:
	.word  0xabcdef11
str2:
	.word  0xabcdef22