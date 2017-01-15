	.text
	.globl main
main:
	# rotate left 1 bit
	lw $t0, str1
	la $t1, str2

	jal average
	move $4, $6
	
	la $a0, str2
	li $v0, 5
	syscall


average:
	la $a0, str1
	li $v0, 4
	syscall
	j $ra

	.data
str1: .asciiz "hello"

str2: .asciiz "word"