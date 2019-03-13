
helloworld.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <main>:
   0:	a9be7bfd 	stp	x29, x30, [sp,#-32]!
   4:	910003fd 	mov	x29, sp
   8:	b9001fa0 	str	w0, [x29,#28]
   c:	f9000ba1 	str	x1, [x29,#16]
  10:	90000000 	adrp	x0, 0 <main>
  14:	91000000 	add	x0, x0, #0x0
  18:	94000000 	bl	0 <puts>
  1c:	52800000 	mov	w0, #0x0                   	// #0
  20:	a8c27bfd 	ldp	x29, x30, [sp],#32
  24:	d65f03c0 	ret

Disassembly of section .rodata:

0000000000000000 <.rodata>:
   0:	6c6c6568 	.word	0x6c6c6568
   4:	6f77206f 	.word	0x6f77206f
   8:	00006472 	.word	0x00006472
   c:	00000000 	.word	0x00000000

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	43434700 	.inst	0x43434700 ; undefined
   4:	4228203a 	.inst	0x4228203a ; undefined
   8:	646c6975 	.inst	0x646c6975 ; undefined
   c:	746f6f72 	.inst	0x746f6f72 ; undefined
  10:	31303220 	adds	w0, w17, #0xc0c
  14:	38302e36 	.inst	0x38302e36 ; undefined
  18:	302d312e 	adr	x14, 5a63d <main+0x5a63d>
  1c:	33363230 	.inst	0x33363230 ; undefined
  20:	3436672d 	cbz	w13, 6cd04 <main+0x6cd04>
  24:	65636562 	.inst	0x65636562 ; undefined
  28:	35202938 	cbnz	w24, 4054c <main+0x4054c>
  2c:	302e342e 	adr	x14, 5c6b1 <main+0x5c6b1>
	...
