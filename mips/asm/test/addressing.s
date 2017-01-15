	.text
	.globl main
main:
# length n of byte sequence - 1
	li $t0, 5
copy:
	lb $t1, src($t0) # pseudo! (src: 32 bits wide)
	sb $t1, dst($t0)
	sub $t0, $t0, 1
	bgez $t0, copy
.data
	src: .byte 0x11, 0x22, 0x33, 0x44, 0x55, 0x66
	dst: .space 6