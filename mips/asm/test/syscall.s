	.text
	.globl main
main:
	li $a0, 12
	li $v0, 5 # read integer
	syscall
	mul $t0, $v0, 42 # compute and save result
	li $v0, 4 # print string
	la $a0, the_result_is
	syscall
	li $v0, 1 # print integer
	move $a0, $t0
	syscall
	li $v0, 10 # exit
	syscall
	.data
the_result_is:
	.asciiz "The result is "