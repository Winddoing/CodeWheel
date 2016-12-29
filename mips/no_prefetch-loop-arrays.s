
bb.o:     file format elf32-tradlittlemips


Disassembly of section .text:

00000000 <func>:
   0:	00000040 	ssnop
   4:	3c060000 	lui	a2,0x0
   8:	00001021 	move	v0,zero
   c:	24c60000 	addiu	a2,a2,0
  10:	24050400 	li	a1,1024
  14:	00402021 	move	a0,v0
  18:	00c21821 	addu	v1,a2,v0
  1c:	24420001 	addiu	v0,v0,1
  20:	1445fffc 	bne	v0,a1,14 <func+0x14>
  24:	a0640000 	sb	a0,0(v1)
  28:	00000040 	ssnop
  2c:	03e00008 	jr	ra
  30:	00001021 	move	v0,zero
	...
