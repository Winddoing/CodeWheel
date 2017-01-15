	.text
	.globl main
main:
	# rotate left 1 bit
	li $t0, 0x80010004
	rol $t1, $t0, 1
	# rotate right 3 bits
	li $t0, 0x80010004
	ror $t1, $t0, 3
