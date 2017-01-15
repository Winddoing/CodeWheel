	.text
	.globl main
main:
# load with sign extension
	lw $t0, memory
	lh $t1, memory
	lb $t2, memory
	# load without sign extension
	lhu $t3, memory
	lbu $t4, memory
	.data
memory:
	.word 0xabcdea78 # little endian: 80E0CDA