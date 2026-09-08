

boot:
	nop
	mov     x5, 216
	movk    x5, 0xfff0, lsl 16
	mrs     x6, mpidr_el1
	and     x6, x6, #0x3
spin:
	wfe
	ldr     x4, [x5,x6,lsl #3]
	cbz     x4, spin
	mov     x0, #0x0
	mov     x1, #0x0
	mov     x2, #0x0
	mov     x3, #0x0
	br      x4
