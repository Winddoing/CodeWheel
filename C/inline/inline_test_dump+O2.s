
inline_test+O2:     file format elf32-tradlittlemips


Disassembly of section .init:

004003b0 <_init>:
  4003b0:	3c1c0002 	lui	gp,0x2
  4003b4:	279c83f0 	addiu	gp,gp,-31760
  4003b8:	0399e021 	addu	gp,gp,t9
  4003bc:	27bdffe0 	addiu	sp,sp,-32
  4003c0:	afbc0010 	sw	gp,16(sp)
  4003c4:	afbf001c 	sw	ra,28(sp)
  4003c8:	8f828024 	lw	v0,-32732(gp)
  4003cc:	10400004 	beqz	v0,4003e0 <_init+0x30>
  4003d0:	00000000 	nop
  4003d4:	8f998024 	lw	t9,-32732(gp)
  4003d8:	0320f809 	jalr	t9
  4003dc:	00000000 	nop
  4003e0:	04110001 	bal	4003e8 <_init+0x38>
  4003e4:	00000000 	nop
  4003e8:	0c100166 	jal	400598 <frame_dummy>
  4003ec:	00000000 	nop
  4003f0:	04110001 	bal	4003f8 <_init+0x48>
  4003f4:	00000000 	nop
  4003f8:	0c1001a8 	jal	4006a0 <__do_global_ctors_aux>
  4003fc:	00000000 	nop
  400400:	8fbf001c 	lw	ra,28(sp)
  400404:	03e00008 	jr	ra
  400408:	27bd0020 	addiu	sp,sp,32

Disassembly of section .text:

00400410 <__start>:
  400410:	3c1c0042 	lui	gp,0x42
  400414:	279c87a0 	addiu	gp,gp,-30816
  400418:	0000f821 	move	ra,zero
  40041c:	3c040040 	lui	a0,0x40
  400420:	248405e8 	addiu	a0,a0,1512
  400424:	8fa50000 	lw	a1,0(sp)
  400428:	27a60004 	addiu	a2,sp,4
  40042c:	2401fff8 	li	at,-8
  400430:	03a1e824 	and	sp,sp,at
  400434:	27bdffe0 	addiu	sp,sp,-32
  400438:	3c070040 	lui	a3,0x40
  40043c:	24e705f0 	addiu	a3,a3,1520
  400440:	3c080040 	lui	t0,0x40
  400444:	25080698 	addiu	t0,t0,1688
  400448:	afa80010 	sw	t0,16(sp)
  40044c:	afa20014 	sw	v0,20(sp)
  400450:	afbd0018 	sw	sp,24(sp)
  400454:	0c1001d8 	jal	400760 <__libc_start_main@plt>
  400458:	00000000 	nop

0040045c <hlt>:
  40045c:	1000ffff 	b	40045c <hlt>
  400460:	00000000 	nop
	...

00400470 <deregister_tm_clones>:
  400470:	3c040041 	lui	a0,0x41
  400474:	3c020041 	lui	v0,0x41
  400478:	248407a4 	addiu	a0,a0,1956
  40047c:	244207a7 	addiu	v0,v0,1959
  400480:	00441023 	subu	v0,v0,a0
  400484:	2c420007 	sltiu	v0,v0,7
  400488:	10400003 	beqz	v0,400498 <deregister_tm_clones+0x28>
  40048c:	3c190000 	lui	t9,0x0
  400490:	03e00008 	jr	ra
  400494:	00000000 	nop
  400498:	27390000 	addiu	t9,t9,0
  40049c:	1320fffc 	beqz	t9,400490 <deregister_tm_clones+0x20>
  4004a0:	00000000 	nop
  4004a4:	03200008 	jr	t9
  4004a8:	00000000 	nop

004004ac <register_tm_clones>:
  4004ac:	3c040041 	lui	a0,0x41
  4004b0:	3c020041 	lui	v0,0x41
  4004b4:	248407a4 	addiu	a0,a0,1956
  4004b8:	244207a4 	addiu	v0,v0,1956
  4004bc:	00441023 	subu	v0,v0,a0
  4004c0:	00021083 	sra	v0,v0,0x2
  4004c4:	00021fc2 	srl	v1,v0,0x1f
  4004c8:	00621021 	addu	v0,v1,v0
  4004cc:	00022843 	sra	a1,v0,0x1
  4004d0:	14a00003 	bnez	a1,4004e0 <register_tm_clones+0x34>
  4004d4:	3c190000 	lui	t9,0x0
  4004d8:	03e00008 	jr	ra
  4004dc:	00000000 	nop
  4004e0:	27390000 	addiu	t9,t9,0
  4004e4:	1320fffc 	beqz	t9,4004d8 <register_tm_clones+0x2c>
  4004e8:	00000000 	nop
  4004ec:	03200008 	jr	t9
  4004f0:	00000000 	nop

004004f4 <__do_global_dtors_aux>:
  4004f4:	27bdffd0 	addiu	sp,sp,-48
  4004f8:	afb30028 	sw	s3,40(sp)
  4004fc:	3c130041 	lui	s3,0x41
  400500:	926207d0 	lbu	v0,2000(s3)
  400504:	afbf002c 	sw	ra,44(sp)
  400508:	afb20024 	sw	s2,36(sp)
  40050c:	afb10020 	sw	s1,32(sp)
  400510:	1440001a 	bnez	v0,40057c <__do_global_dtors_aux+0x88>
  400514:	afb0001c 	sw	s0,28(sp)
  400518:	3c120041 	lui	s2,0x41
  40051c:	3c110041 	lui	s1,0x41
  400520:	3c100041 	lui	s0,0x41
  400524:	26520778 	addiu	s2,s2,1912
  400528:	2631077c 	addiu	s1,s1,1916
  40052c:	8e0207d4 	lw	v0,2004(s0)
  400530:	02328823 	subu	s1,s1,s2
  400534:	00118883 	sra	s1,s1,0x2
  400538:	2631ffff 	addiu	s1,s1,-1
  40053c:	0051182b 	sltu	v1,v0,s1
  400540:	1060000a 	beqz	v1,40056c <__do_global_dtors_aux+0x78>
  400544:	24420001 	addiu	v0,v0,1
  400548:	00021880 	sll	v1,v0,0x2
  40054c:	02431821 	addu	v1,s2,v1
  400550:	8c790000 	lw	t9,0(v1)
  400554:	0320f809 	jalr	t9
  400558:	ae0207d4 	sw	v0,2004(s0)
  40055c:	8e0207d4 	lw	v0,2004(s0)
  400560:	0051182b 	sltu	v1,v0,s1
  400564:	1460fff8 	bnez	v1,400548 <__do_global_dtors_aux+0x54>
  400568:	24420001 	addiu	v0,v0,1
  40056c:	0c10011c 	jal	400470 <deregister_tm_clones>
  400570:	00000000 	nop
  400574:	24020001 	li	v0,1
  400578:	a26207d0 	sb	v0,2000(s3)
  40057c:	8fbf002c 	lw	ra,44(sp)
  400580:	8fb30028 	lw	s3,40(sp)
  400584:	8fb20024 	lw	s2,36(sp)
  400588:	8fb10020 	lw	s1,32(sp)
  40058c:	8fb0001c 	lw	s0,28(sp)
  400590:	03e00008 	jr	ra
  400594:	27bd0030 	addiu	sp,sp,48

00400598 <frame_dummy>:
  400598:	3c040041 	lui	a0,0x41
  40059c:	8c820780 	lw	v0,1920(a0)
  4005a0:	1040000b 	beqz	v0,4005d0 <frame_dummy+0x38>
  4005a4:	3c190000 	lui	t9,0x0
  4005a8:	27390000 	addiu	t9,t9,0
  4005ac:	13200008 	beqz	t9,4005d0 <frame_dummy+0x38>
  4005b0:	00000000 	nop
  4005b4:	27bdffe0 	addiu	sp,sp,-32
  4005b8:	afbf001c 	sw	ra,28(sp)
  4005bc:	0320f809 	jalr	t9
  4005c0:	24840780 	addiu	a0,a0,1920
  4005c4:	8fbf001c 	lw	ra,28(sp)
  4005c8:	0810012b 	j	4004ac <register_tm_clones>
  4005cc:	27bd0020 	addiu	sp,sp,32
  4005d0:	0810012b 	j	4004ac <register_tm_clones>
  4005d4:	00000000 	nop
	...

004005e0 <test1>:
  4005e0:	03e00008 	jr	ra
  4005e4:	00a41021 	addu	v0,a1,a0

004005e8 <main>:
  4005e8:	03e00008 	jr	ra
  4005ec:	00001021 	move	v0,zero

004005f0 <__libc_csu_init>:
  4005f0:	3c1c0002 	lui	gp,0x2
  4005f4:	279c81b0 	addiu	gp,gp,-32336
  4005f8:	0399e021 	addu	gp,gp,t9
  4005fc:	8f998018 	lw	t9,-32744(gp)
  400600:	27bdffc8 	addiu	sp,sp,-56
  400604:	afbc0010 	sw	gp,16(sp)
  400608:	afb50030 	sw	s5,48(sp)
  40060c:	afb4002c 	sw	s4,44(sp)
  400610:	afb30028 	sw	s3,40(sp)
  400614:	afb20024 	sw	s2,36(sp)
  400618:	afb10020 	sw	s1,32(sp)
  40061c:	afb0001c 	sw	s0,28(sp)
  400620:	afbf0034 	sw	ra,52(sp)
  400624:	0080a821 	move	s5,a0
  400628:	00a0a021 	move	s4,a1
  40062c:	0411ff60 	bal	4003b0 <_init>
  400630:	00c09821 	move	s3,a2
  400634:	8fbc0010 	lw	gp,16(sp)
  400638:	00008821 	move	s1,zero
  40063c:	8f82801c 	lw	v0,-32740(gp)
  400640:	8f92801c 	lw	s2,-32740(gp)
  400644:	02429023 	subu	s2,s2,v0
  400648:	00129083 	sra	s2,s2,0x2
  40064c:	12400009 	beqz	s2,400674 <__libc_csu_init+0x84>
  400650:	00408021 	move	s0,v0
  400654:	8e190000 	lw	t9,0(s0)
  400658:	02a02021 	move	a0,s5
  40065c:	02802821 	move	a1,s4
  400660:	02603021 	move	a2,s3
  400664:	0320f809 	jalr	t9
  400668:	26310001 	addiu	s1,s1,1
  40066c:	1632fff9 	bne	s1,s2,400654 <__libc_csu_init+0x64>
  400670:	26100004 	addiu	s0,s0,4
  400674:	8fbf0034 	lw	ra,52(sp)
  400678:	8fb50030 	lw	s5,48(sp)
  40067c:	8fb4002c 	lw	s4,44(sp)
  400680:	8fb30028 	lw	s3,40(sp)
  400684:	8fb20024 	lw	s2,36(sp)
  400688:	8fb10020 	lw	s1,32(sp)
  40068c:	8fb0001c 	lw	s0,28(sp)
  400690:	03e00008 	jr	ra
  400694:	27bd0038 	addiu	sp,sp,56

00400698 <__libc_csu_fini>:
  400698:	03e00008 	jr	ra
  40069c:	00000000 	nop

004006a0 <__do_global_ctors_aux>:
  4006a0:	3c020041 	lui	v0,0x41
  4006a4:	8c590770 	lw	t9,1904(v0)
  4006a8:	2402ffff 	li	v0,-1
  4006ac:	13220010 	beq	t9,v0,4006f0 <__do_global_ctors_aux+0x50>
  4006b0:	00000000 	nop
  4006b4:	27bdffd8 	addiu	sp,sp,-40
  4006b8:	afb0001c 	sw	s0,28(sp)
  4006bc:	3c100041 	lui	s0,0x41
  4006c0:	afb10020 	sw	s1,32(sp)
  4006c4:	afbf0024 	sw	ra,36(sp)
  4006c8:	2610076c 	addiu	s0,s0,1900
  4006cc:	2411ffff 	li	s1,-1
  4006d0:	0320f809 	jalr	t9
  4006d4:	2610fffc 	addiu	s0,s0,-4
  4006d8:	8e190004 	lw	t9,4(s0)
  4006dc:	1731fffc 	bne	t9,s1,4006d0 <__do_global_ctors_aux+0x30>
  4006e0:	8fbf0024 	lw	ra,36(sp)
  4006e4:	8fb10020 	lw	s1,32(sp)
  4006e8:	8fb0001c 	lw	s0,28(sp)
  4006ec:	27bd0028 	addiu	sp,sp,40
  4006f0:	03e00008 	jr	ra
  4006f4:	00000000 	nop
	...

Disassembly of section .fini:

00400700 <_fini>:
  400700:	3c1c0002 	lui	gp,0x2
  400704:	279c80a0 	addiu	gp,gp,-32608
  400708:	0399e021 	addu	gp,gp,t9
  40070c:	27bdffe0 	addiu	sp,sp,-32
  400710:	afbc0010 	sw	gp,16(sp)
  400714:	afbf001c 	sw	ra,28(sp)
  400718:	04110001 	bal	400720 <_fini+0x20>
  40071c:	00000000 	nop
  400720:	0c10013d 	jal	4004f4 <__do_global_dtors_aux>
  400724:	00000000 	nop
  400728:	8fbf001c 	lw	ra,28(sp)
  40072c:	03e00008 	jr	ra
  400730:	27bd0020 	addiu	sp,sp,32

Disassembly of section .plt:

00400740 <_PROCEDURE_LINKAGE_TABLE_>:
  400740:	3c1c0041 	lui	gp,0x41
  400744:	8f990784 	lw	t9,1924(gp)
  400748:	279c0784 	addiu	gp,gp,1924
  40074c:	031cc023 	subu	t8,t8,gp
  400750:	03e07821 	move	t7,ra
  400754:	0018c082 	srl	t8,t8,0x2
  400758:	0320f809 	jalr	t9
  40075c:	2718fffe 	addiu	t8,t8,-2

00400760 <__libc_start_main@plt>:
  400760:	3c0f0041 	lui	t7,0x41
  400764:	8df9078c 	lw	t9,1932(t7)
  400768:	03200008 	jr	t9
  40076c:	25f8078c 	addiu	t8,t7,1932
