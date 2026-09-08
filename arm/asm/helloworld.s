
helloworld.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <main>:
   0:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
   4:	910003fd 	mov	x29, sp
   8:	b9001fe0 	str	w0, [sp, #28]
   c:	f9000be1 	str	x1, [sp, #16]
  10:	90000000 	adrp	x0, 0 <main>
  14:	91000000 	add	x0, x0, #0x0
  18:	94000000 	bl	0 <puts>
  1c:	52800000 	mov	w0, #0x0                   	// #0
  20:	a8c27bfd 	ldp	x29, x30, [sp], #32
  24:	d65f03c0 	ret

Disassembly of section .rodata:

0000000000000000 <.rodata>:
   0:	6c6c6568 	ldnp	d8, d25, [x11, #-320]
   4:	6f77206f 	umlal2	v15.4s, v3.8h, v7.h[3]
   8:	Address 0x8 is out of bounds.


Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	43434700 	.inst	0x43434700 ; undefined
   4:	5528203a 	.inst	0x5528203a ; undefined
   8:	746e7562 	.inst	0x746e7562 ; undefined
   c:	33312075 	.inst	0x33312075 ; undefined
  10:	302e332e 	adr	x14, 5c675 <main+0x5c675>
  14:	6275362d 	.inst	0x6275362d ; undefined
  18:	75746e75 	.inst	0x75746e75 ; undefined
  1c:	34327e32 	cbz	w18, 64fe0 <main+0x64fe0>
  20:	2934302e 	stp	w14, w12, [x1, #-96]
  24:	2e333120 	usubw	v0.8h, v9.8h, v19.8b
  28:	00302e33 	.inst	0x00302e33 ; NYI

Disassembly of section .eh_frame:

0000000000000000 <.eh_frame>:
   0:	00000010 	udf	#16
   4:	00000000 	udf	#0
   8:	00527a01 	.inst	0x00527a01 ; undefined
   c:	011e7804 	.inst	0x011e7804 ; undefined
  10:	001f0c1b 	.inst	0x001f0c1b ; undefined
  14:	00000020 	udf	#32
  18:	00000018 	udf	#24
  1c:	00000000 	udf	#0
  20:	00000028 	udf	#40
  24:	200e4100 	.inst	0x200e4100 ; undefined
  28:	039e049d 	.inst	0x039e049d ; undefined
  2c:	0eddde48 	.inst	0x0eddde48 ; undefined
	...
