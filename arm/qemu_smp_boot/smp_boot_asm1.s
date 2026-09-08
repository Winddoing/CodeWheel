

boot:
	nop
	mov     x5, 216
	movk    x5, 0xfff0, lsl 16
spin:
	wfe
	ldr     x4, [x5]
	cbz     x4, spin
	mov     x0, #0x0
	mov     x1, #0x0
	mov     x2, #0x0
	mov     x3, #0x0
	br      x4
