.data 0x10010010
msg1:	.asciiz "Please input first num:\n"
msg2:	.asciiz "Please input second num:\n"
msg3:	.asciiz "The result is:\n"
mask:	.word 0x00000001
		.text
		.globl main
main:
		lw $s0, mask # $s0=1 (the mask)
		addi $t0, $zero, 31 # $t0=31 (The counter)
		move $s2, $zero # $s2=sum, s2=0
		
		la $a0, msg1
		li $v0, 4
		syscall
		li $v0, 5
		syscall
		move $s1, $v0 # $s1=FirstNum
		
		la $a0, msg2
		li $v0, 4
		syscall
		li $v0, 5
		syscall
		move $t1, $v0 # $t1=SecondNum
		
loop:	and $t2, $s0, $t1	# t2= s0 & t1
		beq $t2, $zero, noth
		add $s2, $s2, $s1
noth:	sll $s1, $s1, 1
		srl $t1, $t1, 1
		addi $t0, $t0, -1 # counter--
		beq $t0, $zero, stop # if counter==0, break loop
		j loop
			
stop:	la $a0, msg3
		li $v0, 4
		syscall
		move $a0, $s2
		li $v0, 1
		syscall
		li $v0, 10
		syscall