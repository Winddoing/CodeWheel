
prefetch:     file format elf64-littleaarch64


Disassembly of section .init:

00000000004003d0 <_init>:
  4003d0:	a9bf7bfd 	stp	x29, x30, [sp,#-16]!
  4003d4:	910003fd 	mov	x29, sp
  4003d8:	94000034 	bl	4004a8 <call_weak_fn>
  4003dc:	a8c17bfd 	ldp	x29, x30, [sp],#16
  4003e0:	d65f03c0 	ret

Disassembly of section .plt:

00000000004003f0 <clock@plt-0x20>:
  4003f0:	a9bf7bf0 	stp	x16, x30, [sp,#-16]!
  4003f4:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  4003f8:	f941c611 	ldr	x17, [x16,#904]
  4003fc:	910e2210 	add	x16, x16, #0x388
  400400:	d61f0220 	br	x17
  400404:	d503201f 	nop
  400408:	d503201f 	nop
  40040c:	d503201f 	nop

0000000000400410 <clock@plt>:
  400410:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  400414:	f941ca11 	ldr	x17, [x16,#912]
  400418:	910e4210 	add	x16, x16, #0x390
  40041c:	d61f0220 	br	x17

0000000000400420 <__libc_start_main@plt>:
  400420:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  400424:	f941ce11 	ldr	x17, [x16,#920]
  400428:	910e6210 	add	x16, x16, #0x398
  40042c:	d61f0220 	br	x17

0000000000400430 <__gmon_start__@plt>:
  400430:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  400434:	f941d211 	ldr	x17, [x16,#928]
  400438:	910e8210 	add	x16, x16, #0x3a0
  40043c:	d61f0220 	br	x17

0000000000400440 <abort@plt>:
  400440:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  400444:	f941d611 	ldr	x17, [x16,#936]
  400448:	910ea210 	add	x16, x16, #0x3a8
  40044c:	d61f0220 	br	x17

0000000000400450 <printf@plt>:
  400450:	b0000090 	adrp	x16, 411000 <__FRAME_END__+0xfe88>
  400454:	f941da11 	ldr	x17, [x16,#944]
  400458:	910ec210 	add	x16, x16, #0x3b0
  40045c:	d61f0220 	br	x17

Disassembly of section .text:

0000000000400460 <_start>:
  400460:	d280001d 	mov	x29, #0x0                   	// #0
  400464:	d280001e 	mov	x30, #0x0                   	// #0
  400468:	aa0003e5 	mov	x5, x0
  40046c:	f94003e1 	ldr	x1, [sp]
  400470:	910023e2 	add	x2, sp, #0x8
  400474:	910003e6 	mov	x6, sp
  400478:	580000c0 	ldr	x0, 400490 <_start+0x30>
  40047c:	580000e3 	ldr	x3, 400498 <_start+0x38>
  400480:	58000104 	ldr	x4, 4004a0 <_start+0x40>
  400484:	97ffffe7 	bl	400420 <__libc_start_main@plt>
  400488:	97ffffee 	bl	400440 <abort@plt>
  40048c:	00000000 	.word	0x00000000
  400490:	004005a0 	.word	0x004005a0
  400494:	00000000 	.word	0x00000000
  400498:	004010a0 	.word	0x004010a0
  40049c:	00000000 	.word	0x00000000
  4004a0:	00401118 	.word	0x00401118
  4004a4:	00000000 	.word	0x00000000

00000000004004a8 <call_weak_fn>:
  4004a8:	b0000080 	adrp	x0, 411000 <__FRAME_END__+0xfe88>
  4004ac:	f941b800 	ldr	x0, [x0,#880]
  4004b0:	b4000040 	cbz	x0, 4004b8 <call_weak_fn+0x10>
  4004b4:	17ffffdf 	b	400430 <__gmon_start__@plt>
  4004b8:	d65f03c0 	ret
  4004bc:	00000000 	.inst	0x00000000 ; undefined

00000000004004c0 <deregister_tm_clones>:
  4004c0:	b0000081 	adrp	x1, 411000 <__FRAME_END__+0xfe88>
  4004c4:	b0000080 	adrp	x0, 411000 <__FRAME_END__+0xfe88>
  4004c8:	910f2021 	add	x1, x1, #0x3c8
  4004cc:	910f2000 	add	x0, x0, #0x3c8
  4004d0:	91001c21 	add	x1, x1, #0x7
  4004d4:	cb000021 	sub	x1, x1, x0
  4004d8:	f100383f 	cmp	x1, #0xe
  4004dc:	54000089 	b.ls	4004ec <deregister_tm_clones+0x2c>
  4004e0:	58000081 	ldr	x1, 4004f0 <deregister_tm_clones+0x30>
  4004e4:	b4000041 	cbz	x1, 4004ec <deregister_tm_clones+0x2c>
  4004e8:	d61f0020 	br	x1
  4004ec:	d65f03c0 	ret
	...

00000000004004f8 <register_tm_clones>:
  4004f8:	b0000080 	adrp	x0, 411000 <__FRAME_END__+0xfe88>
  4004fc:	b0000081 	adrp	x1, 411000 <__FRAME_END__+0xfe88>
  400500:	910f2000 	add	x0, x0, #0x3c8
  400504:	910f2021 	add	x1, x1, #0x3c8
  400508:	cb000021 	sub	x1, x1, x0
  40050c:	d2800042 	mov	x2, #0x2                   	// #2
  400510:	9343fc21 	asr	x1, x1, #3
  400514:	9ac20c21 	sdiv	x1, x1, x2
  400518:	b4000081 	cbz	x1, 400528 <register_tm_clones+0x30>
  40051c:	580000a2 	ldr	x2, 400530 <register_tm_clones+0x38>
  400520:	b4000042 	cbz	x2, 400528 <register_tm_clones+0x30>
  400524:	d61f0040 	br	x2
  400528:	d65f03c0 	ret
  40052c:	d503201f 	nop
	...

0000000000400538 <__do_global_dtors_aux>:
  400538:	a9be7bfd 	stp	x29, x30, [sp,#-32]!
  40053c:	910003fd 	mov	x29, sp
  400540:	f9000bf3 	str	x19, [sp,#16]
  400544:	b0000093 	adrp	x19, 411000 <__FRAME_END__+0xfe88>
  400548:	394f2260 	ldrb	w0, [x19,#968]
  40054c:	35000080 	cbnz	w0, 40055c <__do_global_dtors_aux+0x24>
  400550:	97ffffdc 	bl	4004c0 <deregister_tm_clones>
  400554:	52800020 	mov	w0, #0x1                   	// #1
  400558:	390f2260 	strb	w0, [x19,#968]
  40055c:	f9400bf3 	ldr	x19, [sp,#16]
  400560:	a8c27bfd 	ldp	x29, x30, [sp],#32
  400564:	d65f03c0 	ret

0000000000400568 <frame_dummy>:
  400568:	b0000080 	adrp	x0, 411000 <__FRAME_END__+0xfe88>
  40056c:	91064000 	add	x0, x0, #0x190
  400570:	f9400001 	ldr	x1, [x0]
  400574:	b40000e1 	cbz	x1, 400590 <frame_dummy+0x28>
  400578:	58000101 	ldr	x1, 400598 <frame_dummy+0x30>
  40057c:	b40000a1 	cbz	x1, 400590 <frame_dummy+0x28>
  400580:	a9bf7bfd 	stp	x29, x30, [sp,#-16]!
  400584:	910003fd 	mov	x29, sp
  400588:	d63f0020 	blr	x1
  40058c:	a8c17bfd 	ldp	x29, x30, [sp],#16
  400590:	17ffffda 	b	4004f8 <register_tm_clones>
  400594:	d503201f 	nop
	...

00000000004005a0 <main>:
  4005a0:	d109c3ff 	sub	sp, sp, #0x270
  4005a4:	a9b97bfd 	stp	x29, x30, [sp,#-112]!
  4005a8:	910003fd 	mov	x29, sp
  4005ac:	a90153f3 	stp	x19, x20, [sp,#16]
  4005b0:	a9025bf5 	stp	x21, x22, [sp,#32]
  4005b4:	a90363f7 	stp	x23, x24, [sp,#48]
  4005b8:	a9046bf9 	stp	x25, x26, [sp,#64]
  4005bc:	a90573fb 	stp	x27, x28, [sp,#80]
  4005c0:	fd0033e8 	str	d8, [sp,#96]
  4005c4:	b9023fa0 	str	w0, [x29,#572]
  4005c8:	f9011ba1 	str	x1, [x29,#560]
  4005cc:	910003e0 	mov	x0, sp
  4005d0:	f9003fa0 	str	x0, [x29,#120]
  4005d4:	52805780 	mov	w0, #0x2bc                 	// #700
  4005d8:	b902c7a0 	str	w0, [x29,#708]
  4005dc:	b942c7bc 	ldr	w28, [x29,#708]
  4005e0:	b942c7be 	ldr	w30, [x29,#708]
  4005e4:	93407f80 	sxtw	x0, w28
  4005e8:	d1000400 	sub	x0, x0, #0x1
  4005ec:	f9015fa0 	str	x0, [x29,#696]
  4005f0:	93407f80 	sxtw	x0, w28
  4005f4:	aa0003e4 	mov	x4, x0
  4005f8:	d2800005 	mov	x5, #0x0                   	// #0
  4005fc:	d37bfc80 	lsr	x0, x4, #59
  400600:	d37be8a3 	lsl	x3, x5, #5
  400604:	aa030003 	orr	x3, x0, x3
  400608:	d37be882 	lsl	x2, x4, #5
  40060c:	93407f80 	sxtw	x0, w28
  400610:	9e670000 	fmov	d0, x0
  400614:	5f425408 	shl	d8, d0, #2
  400618:	93407fc0 	sxtw	x0, w30
  40061c:	d1000400 	sub	x0, x0, #0x1
  400620:	f9015ba0 	str	x0, [x29,#688]
  400624:	93407f80 	sxtw	x0, w28
  400628:	aa0003e8 	mov	x8, x0
  40062c:	d2800009 	mov	x9, #0x0                   	// #0
  400630:	93407fc0 	sxtw	x0, w30
  400634:	aa0003e6 	mov	x6, x0
  400638:	d2800007 	mov	x7, #0x0                   	// #0
  40063c:	9b067d01 	mul	x1, x8, x6
  400640:	9bc67d00 	umulh	x0, x8, x6
  400644:	9b060120 	madd	x0, x9, x6, x0
  400648:	9b070100 	madd	x0, x8, x7, x0
  40064c:	aa0103ee 	mov	x14, x1
  400650:	aa0003ef 	mov	x15, x0
  400654:	d37bfdc0 	lsr	x0, x14, #59
  400658:	d37be9eb 	lsl	x11, x15, #5
  40065c:	aa0b000b 	orr	x11, x0, x11
  400660:	d37be9ca 	lsl	x10, x14, #5
  400664:	93407f80 	sxtw	x0, w28
  400668:	aa0003f0 	mov	x16, x0
  40066c:	d2800011 	mov	x17, #0x0                   	// #0
  400670:	93407fc0 	sxtw	x0, w30
  400674:	aa0003ec 	mov	x12, x0
  400678:	d280000d 	mov	x13, #0x0                   	// #0
  40067c:	9b0c7e01 	mul	x1, x16, x12
  400680:	9bcc7e00 	umulh	x0, x16, x12
  400684:	9b0c0220 	madd	x0, x17, x12, x0
  400688:	9b0d0200 	madd	x0, x16, x13, x0
  40068c:	aa0103f4 	mov	x20, x1
  400690:	aa0003f5 	mov	x21, x0
  400694:	d37bfe80 	lsr	x0, x20, #59
  400698:	d37beab3 	lsl	x19, x21, #5
  40069c:	aa130013 	orr	x19, x0, x19
  4006a0:	d37bea92 	lsl	x18, x20, #5
  4006a4:	93407f81 	sxtw	x1, w28
  4006a8:	93407fc0 	sxtw	x0, w30
  4006ac:	9b007c20 	mul	x0, x1, x0
  4006b0:	d37ef400 	lsl	x0, x0, #2
  4006b4:	91000c00 	add	x0, x0, #0x3
  4006b8:	91003c00 	add	x0, x0, #0xf
  4006bc:	d344fc00 	lsr	x0, x0, #4
  4006c0:	d37cec00 	lsl	x0, x0, #4
  4006c4:	cb2063ff 	sub	sp, sp, x0
  4006c8:	910003e0 	mov	x0, sp
  4006cc:	91000c00 	add	x0, x0, #0x3
  4006d0:	d342fc00 	lsr	x0, x0, #2
  4006d4:	d37ef400 	lsl	x0, x0, #2
  4006d8:	f90157a0 	str	x0, [x29,#680]
  4006dc:	b942c7a2 	ldr	w2, [x29,#708]
  4006e0:	b942c7a3 	ldr	w3, [x29,#708]
  4006e4:	93407c40 	sxtw	x0, w2
  4006e8:	d1000400 	sub	x0, x0, #0x1
  4006ec:	f90153a0 	str	x0, [x29,#672]
  4006f0:	93407c40 	sxtw	x0, w2
  4006f4:	f900d3a0 	str	x0, [x29,#416]
  4006f8:	f900d7bf 	str	xzr, [x29,#424]
  4006fc:	910683a0 	add	x0, x29, #0x1a0
  400700:	a9401404 	ldp	x4, x5, [x0]
  400704:	aa0403e1 	mov	x1, x4
  400708:	d37bfc20 	lsr	x0, x1, #59
  40070c:	aa0503e1 	mov	x1, x5
  400710:	d37be837 	lsl	x23, x1, #5
  400714:	aa170017 	orr	x23, x0, x23
  400718:	aa0403e0 	mov	x0, x4
  40071c:	d37be816 	lsl	x22, x0, #5
  400720:	93407c40 	sxtw	x0, w2
  400724:	d37ef416 	lsl	x22, x0, #2
  400728:	93407c60 	sxtw	x0, w3
  40072c:	d1000400 	sub	x0, x0, #0x1
  400730:	f9014fa0 	str	x0, [x29,#664]
  400734:	93407c40 	sxtw	x0, w2
  400738:	aa0003fa 	mov	x26, x0
  40073c:	d280001b 	mov	x27, #0x0                   	// #0
  400740:	93407c60 	sxtw	x0, w3
  400744:	aa0003f8 	mov	x24, x0
  400748:	d2800019 	mov	x25, #0x0                   	// #0
  40074c:	9b187f41 	mul	x1, x26, x24
  400750:	9bd87f40 	umulh	x0, x26, x24
  400754:	9b180360 	madd	x0, x27, x24, x0
  400758:	9b190340 	madd	x0, x26, x25, x0
  40075c:	f900cba1 	str	x1, [x29,#400]
  400760:	f900cfa0 	str	x0, [x29,#408]
  400764:	910643a0 	add	x0, x29, #0x190
  400768:	a9401404 	ldp	x4, x5, [x0]
  40076c:	aa0403e1 	mov	x1, x4
  400770:	d37bfc20 	lsr	x0, x1, #59
  400774:	9e6700a0 	fmov	d0, x5
  400778:	5f455400 	shl	d0, d0, #5
  40077c:	fd0117a0 	str	d0, [x29,#552]
  400780:	f94117a1 	ldr	x1, [x29,#552]
  400784:	aa010000 	orr	x0, x0, x1
  400788:	f90117a0 	str	x0, [x29,#552]
  40078c:	9e670080 	fmov	d0, x4
  400790:	5f455400 	shl	d0, d0, #5
  400794:	fd0113a0 	str	d0, [x29,#544]
  400798:	93407c40 	sxtw	x0, w2
  40079c:	f900c3a0 	str	x0, [x29,#384]
  4007a0:	f900c7bf 	str	xzr, [x29,#392]
  4007a4:	93407c60 	sxtw	x0, w3
  4007a8:	f900bba0 	str	x0, [x29,#368]
  4007ac:	f900bfbf 	str	xzr, [x29,#376]
  4007b0:	910603a0 	add	x0, x29, #0x180
  4007b4:	a9401c06 	ldp	x6, x7, [x0]
  4007b8:	aa0603e0 	mov	x0, x6
  4007bc:	9105c3a1 	add	x1, x29, #0x170
  4007c0:	a9401424 	ldp	x4, x5, [x1]
  4007c4:	aa0403e1 	mov	x1, x4
  4007c8:	9b017c01 	mul	x1, x0, x1
  4007cc:	aa0603e0 	mov	x0, x6
  4007d0:	aa0403e8 	mov	x8, x4
  4007d4:	9bc87c00 	umulh	x0, x0, x8
  4007d8:	aa0703e8 	mov	x8, x7
  4007dc:	aa0403e9 	mov	x9, x4
  4007e0:	9b090100 	madd	x0, x8, x9, x0
  4007e4:	aa0503e4 	mov	x4, x5
  4007e8:	9b0400c0 	madd	x0, x6, x4, x0
  4007ec:	f900b3a1 	str	x1, [x29,#352]
  4007f0:	f900b7a0 	str	x0, [x29,#360]
  4007f4:	910583a0 	add	x0, x29, #0x160
  4007f8:	a9401404 	ldp	x4, x5, [x0]
  4007fc:	aa0403e1 	mov	x1, x4
  400800:	d37bfc20 	lsr	x0, x1, #59
  400804:	9e6700a0 	fmov	d0, x5
  400808:	5f455400 	shl	d0, d0, #5
  40080c:	fd010fa0 	str	d0, [x29,#536]
  400810:	f9410fa1 	ldr	x1, [x29,#536]
  400814:	aa010000 	orr	x0, x0, x1
  400818:	f9010fa0 	str	x0, [x29,#536]
  40081c:	9e670080 	fmov	d0, x4
  400820:	5f455400 	shl	d0, d0, #5
  400824:	fd010ba0 	str	d0, [x29,#528]
  400828:	93407c41 	sxtw	x1, w2
  40082c:	93407c60 	sxtw	x0, w3
  400830:	9b007c20 	mul	x0, x1, x0
  400834:	d37ef400 	lsl	x0, x0, #2
  400838:	91000c00 	add	x0, x0, #0x3
  40083c:	91003c00 	add	x0, x0, #0xf
  400840:	d344fc00 	lsr	x0, x0, #4
  400844:	d37cec00 	lsl	x0, x0, #4
  400848:	cb2063ff 	sub	sp, sp, x0
  40084c:	910003e0 	mov	x0, sp
  400850:	91000c00 	add	x0, x0, #0x3
  400854:	d342fc00 	lsr	x0, x0, #2
  400858:	d37ef400 	lsl	x0, x0, #2
  40085c:	f9014ba0 	str	x0, [x29,#656]
  400860:	b942c7a2 	ldr	w2, [x29,#708]
  400864:	b942c7a3 	ldr	w3, [x29,#708]
  400868:	93407c40 	sxtw	x0, w2
  40086c:	d1000400 	sub	x0, x0, #0x1
  400870:	f90147a0 	str	x0, [x29,#648]
  400874:	93407c40 	sxtw	x0, w2
  400878:	f900aba0 	str	x0, [x29,#336]
  40087c:	f900afbf 	str	xzr, [x29,#344]
  400880:	910543a0 	add	x0, x29, #0x150
  400884:	a9401404 	ldp	x4, x5, [x0]
  400888:	aa0403e1 	mov	x1, x4
  40088c:	d37bfc20 	lsr	x0, x1, #59
  400890:	9e6700a0 	fmov	d0, x5
  400894:	5f455400 	shl	d0, d0, #5
  400898:	fd0107a0 	str	d0, [x29,#520]
  40089c:	f94107a1 	ldr	x1, [x29,#520]
  4008a0:	aa010000 	orr	x0, x0, x1
  4008a4:	f90107a0 	str	x0, [x29,#520]
  4008a8:	9e670080 	fmov	d0, x4
  4008ac:	5f455400 	shl	d0, d0, #5
  4008b0:	fd0103a0 	str	d0, [x29,#512]
  4008b4:	93407c40 	sxtw	x0, w2
  4008b8:	d37ef415 	lsl	x21, x0, #2
  4008bc:	93407c60 	sxtw	x0, w3
  4008c0:	d1000400 	sub	x0, x0, #0x1
  4008c4:	f90143a0 	str	x0, [x29,#640]
  4008c8:	93407c40 	sxtw	x0, w2
  4008cc:	f900a3a0 	str	x0, [x29,#320]
  4008d0:	f900a7bf 	str	xzr, [x29,#328]
  4008d4:	93407c60 	sxtw	x0, w3
  4008d8:	f9009ba0 	str	x0, [x29,#304]
  4008dc:	f9009fbf 	str	xzr, [x29,#312]
  4008e0:	910503a0 	add	x0, x29, #0x140
  4008e4:	a9401c06 	ldp	x6, x7, [x0]
  4008e8:	aa0603e0 	mov	x0, x6
  4008ec:	9104c3a1 	add	x1, x29, #0x130
  4008f0:	a9401424 	ldp	x4, x5, [x1]
  4008f4:	aa0403e1 	mov	x1, x4
  4008f8:	9b017c01 	mul	x1, x0, x1
  4008fc:	aa0603e0 	mov	x0, x6
  400900:	aa0403e8 	mov	x8, x4
  400904:	9bc87c00 	umulh	x0, x0, x8
  400908:	aa0703e8 	mov	x8, x7
  40090c:	aa0403e9 	mov	x9, x4
  400910:	9b090100 	madd	x0, x8, x9, x0
  400914:	aa0503e4 	mov	x4, x5
  400918:	9b0400c0 	madd	x0, x6, x4, x0
  40091c:	f90093a1 	str	x1, [x29,#288]
  400920:	f90097a0 	str	x0, [x29,#296]
  400924:	910483a0 	add	x0, x29, #0x120
  400928:	a9401404 	ldp	x4, x5, [x0]
  40092c:	aa0403e1 	mov	x1, x4
  400930:	d37bfc20 	lsr	x0, x1, #59
  400934:	9e6700a0 	fmov	d0, x5
  400938:	5f455400 	shl	d0, d0, #5
  40093c:	fd00ffa0 	str	d0, [x29,#504]
  400940:	f940ffa1 	ldr	x1, [x29,#504]
  400944:	aa010000 	orr	x0, x0, x1
  400948:	f900ffa0 	str	x0, [x29,#504]
  40094c:	9e670080 	fmov	d0, x4
  400950:	5f455400 	shl	d0, d0, #5
  400954:	fd00fba0 	str	d0, [x29,#496]
  400958:	93407c40 	sxtw	x0, w2
  40095c:	f9008ba0 	str	x0, [x29,#272]
  400960:	f9008fbf 	str	xzr, [x29,#280]
  400964:	93407c60 	sxtw	x0, w3
  400968:	f90083a0 	str	x0, [x29,#256]
  40096c:	f90087bf 	str	xzr, [x29,#264]
  400970:	910443a0 	add	x0, x29, #0x110
  400974:	a9401c06 	ldp	x6, x7, [x0]
  400978:	aa0603e0 	mov	x0, x6
  40097c:	910403a1 	add	x1, x29, #0x100
  400980:	a9401424 	ldp	x4, x5, [x1]
  400984:	aa0403e1 	mov	x1, x4
  400988:	9b017c01 	mul	x1, x0, x1
  40098c:	aa0603e0 	mov	x0, x6
  400990:	aa0403e8 	mov	x8, x4
  400994:	9bc87c00 	umulh	x0, x0, x8
  400998:	aa0703e8 	mov	x8, x7
  40099c:	aa0403e9 	mov	x9, x4
  4009a0:	9b090100 	madd	x0, x8, x9, x0
  4009a4:	aa0503e4 	mov	x4, x5
  4009a8:	9b0400c0 	madd	x0, x6, x4, x0
  4009ac:	f9007ba1 	str	x1, [x29,#240]
  4009b0:	f9007fa0 	str	x0, [x29,#248]
  4009b4:	a94f17a4 	ldp	x4, x5, [x29,#240]
  4009b8:	aa0403e1 	mov	x1, x4
  4009bc:	d37bfc20 	lsr	x0, x1, #59
  4009c0:	9e6700a0 	fmov	d0, x5
  4009c4:	5f455400 	shl	d0, d0, #5
  4009c8:	fd00f7a0 	str	d0, [x29,#488]
  4009cc:	f940f7a1 	ldr	x1, [x29,#488]
  4009d0:	aa010000 	orr	x0, x0, x1
  4009d4:	f900f7a0 	str	x0, [x29,#488]
  4009d8:	9e670080 	fmov	d0, x4
  4009dc:	5f455400 	shl	d0, d0, #5
  4009e0:	fd00f3a0 	str	d0, [x29,#480]
  4009e4:	93407c41 	sxtw	x1, w2
  4009e8:	93407c60 	sxtw	x0, w3
  4009ec:	9b007c20 	mul	x0, x1, x0
  4009f0:	d37ef400 	lsl	x0, x0, #2
  4009f4:	91000c00 	add	x0, x0, #0x3
  4009f8:	91003c00 	add	x0, x0, #0xf
  4009fc:	d344fc00 	lsr	x0, x0, #4
  400a00:	d37cec00 	lsl	x0, x0, #4
  400a04:	cb2063ff 	sub	sp, sp, x0
  400a08:	910003e0 	mov	x0, sp
  400a0c:	91000c00 	add	x0, x0, #0x3
  400a10:	d342fc00 	lsr	x0, x0, #2
  400a14:	d37ef400 	lsl	x0, x0, #2
  400a18:	f9013fa0 	str	x0, [x29,#632]
  400a1c:	f9013bbf 	str	xzr, [x29,#624]
  400a20:	f90137bf 	str	xzr, [x29,#616]
  400a24:	f9016fbf 	str	xzr, [x29,#728]
  400a28:	14000026 	b	400ac0 <main+0x520>
  400a2c:	f9016bbf 	str	xzr, [x29,#720]
  400a30:	1400001d 	b	400aa4 <main+0x504>
  400a34:	d342fec2 	lsr	x2, x22, #2
  400a38:	f9416fa0 	ldr	x0, [x29,#728]
  400a3c:	91000400 	add	x0, x0, #0x1
  400a40:	2a0003e1 	mov	w1, w0
  400a44:	f9416ba0 	ldr	x0, [x29,#720]
  400a48:	1b007c20 	mul	w0, w1, w0
  400a4c:	2a0003e3 	mov	w3, w0
  400a50:	f9414ba0 	ldr	x0, [x29,#656]
  400a54:	f9416fa1 	ldr	x1, [x29,#728]
  400a58:	9b017c42 	mul	x2, x2, x1
  400a5c:	f9416ba1 	ldr	x1, [x29,#720]
  400a60:	8b010041 	add	x1, x2, x1
  400a64:	b8217803 	str	w3, [x0,x1,lsl #2]
  400a68:	d342fea2 	lsr	x2, x21, #2
  400a6c:	f9416fa0 	ldr	x0, [x29,#728]
  400a70:	2a0003e1 	mov	w1, w0
  400a74:	f9416ba0 	ldr	x0, [x29,#720]
  400a78:	1b007c20 	mul	w0, w1, w0
  400a7c:	2a0003e3 	mov	w3, w0
  400a80:	f9413fa0 	ldr	x0, [x29,#632]
  400a84:	f9416fa1 	ldr	x1, [x29,#728]
  400a88:	9b017c42 	mul	x2, x2, x1
  400a8c:	f9416ba1 	ldr	x1, [x29,#720]
  400a90:	8b010041 	add	x1, x2, x1
  400a94:	b8217803 	str	w3, [x0,x1,lsl #2]
  400a98:	f9416ba0 	ldr	x0, [x29,#720]
  400a9c:	91000400 	add	x0, x0, #0x1
  400aa0:	f9016ba0 	str	x0, [x29,#720]
  400aa4:	b982c7a1 	ldrsw	x1, [x29,#708]
  400aa8:	f9416ba0 	ldr	x0, [x29,#720]
  400aac:	eb00003f 	cmp	x1, x0
  400ab0:	54fffc28 	b.hi	400a34 <main+0x494>
  400ab4:	f9416fa0 	ldr	x0, [x29,#728]
  400ab8:	91000400 	add	x0, x0, #0x1
  400abc:	f9016fa0 	str	x0, [x29,#728]
  400ac0:	b982c7a1 	ldrsw	x1, [x29,#708]
  400ac4:	f9416fa0 	ldr	x0, [x29,#728]
  400ac8:	eb00003f 	cmp	x1, x0
  400acc:	54fffb08 	b.hi	400a2c <main+0x48c>
  400ad0:	97fffe50 	bl	400410 <clock@plt>
  400ad4:	f90133a0 	str	x0, [x29,#608]
  400ad8:	d503201f 	nop
  400adc:	d503201f 	nop
  400ae0:	d503201f 	nop
  400ae4:	f9016fbf 	str	xzr, [x29,#728]
  400ae8:	14000036 	b	400bc0 <main+0x620>
  400aec:	f9016bbf 	str	xzr, [x29,#720]
  400af0:	1400002d 	b	400ba4 <main+0x604>
  400af4:	f90167bf 	str	xzr, [x29,#712]
  400af8:	14000024 	b	400b88 <main+0x5e8>
  400afc:	9e660100 	fmov	x0, d8
  400b00:	d342fc03 	lsr	x3, x0, #2
  400b04:	9e660100 	fmov	x0, d8
  400b08:	d342fc02 	lsr	x2, x0, #2
  400b0c:	f94157a0 	ldr	x0, [x29,#680]
  400b10:	f9416fa1 	ldr	x1, [x29,#728]
  400b14:	9b017c42 	mul	x2, x2, x1
  400b18:	f9416ba1 	ldr	x1, [x29,#720]
  400b1c:	8b010041 	add	x1, x2, x1
  400b20:	b8617801 	ldr	w1, [x0,x1,lsl #2]
  400b24:	d342fec4 	lsr	x4, x22, #2
  400b28:	f9414ba0 	ldr	x0, [x29,#656]
  400b2c:	f9416fa2 	ldr	x2, [x29,#728]
  400b30:	9b027c84 	mul	x4, x4, x2
  400b34:	f94167a2 	ldr	x2, [x29,#712]
  400b38:	8b020082 	add	x2, x4, x2
  400b3c:	b8627802 	ldr	w2, [x0,x2,lsl #2]
  400b40:	d342fea5 	lsr	x5, x21, #2
  400b44:	f9413fa0 	ldr	x0, [x29,#632]
  400b48:	f94167a4 	ldr	x4, [x29,#712]
  400b4c:	9b047ca5 	mul	x5, x5, x4
  400b50:	f9416ba4 	ldr	x4, [x29,#720]
  400b54:	8b0400a4 	add	x4, x5, x4
  400b58:	b8647800 	ldr	w0, [x0,x4,lsl #2]
  400b5c:	1b007c40 	mul	w0, w2, w0
  400b60:	0b000022 	add	w2, w1, w0
  400b64:	f94157a0 	ldr	x0, [x29,#680]
  400b68:	f9416fa1 	ldr	x1, [x29,#728]
  400b6c:	9b017c63 	mul	x3, x3, x1
  400b70:	f9416ba1 	ldr	x1, [x29,#720]
  400b74:	8b010061 	add	x1, x3, x1
  400b78:	b8217802 	str	w2, [x0,x1,lsl #2]
  400b7c:	f94167a0 	ldr	x0, [x29,#712]
  400b80:	91000400 	add	x0, x0, #0x1
  400b84:	f90167a0 	str	x0, [x29,#712]
  400b88:	b982c7a1 	ldrsw	x1, [x29,#708]
  400b8c:	f94167a0 	ldr	x0, [x29,#712]
  400b90:	eb00003f 	cmp	x1, x0
  400b94:	54fffb48 	b.hi	400afc <main+0x55c>
  400b98:	f9416ba0 	ldr	x0, [x29,#720]
  400b9c:	91000400 	add	x0, x0, #0x1
  400ba0:	f9016ba0 	str	x0, [x29,#720]
  400ba4:	b982c7a1 	ldrsw	x1, [x29,#708]
  400ba8:	f9416ba0 	ldr	x0, [x29,#720]
  400bac:	eb00003f 	cmp	x1, x0
  400bb0:	54fffa28 	b.hi	400af4 <main+0x554>
  400bb4:	f9416fa0 	ldr	x0, [x29,#728]
  400bb8:	91000400 	add	x0, x0, #0x1
  400bbc:	f9016fa0 	str	x0, [x29,#728]
  400bc0:	b982c7a1 	ldrsw	x1, [x29,#708]
  400bc4:	f9416fa0 	ldr	x0, [x29,#728]
  400bc8:	eb00003f 	cmp	x1, x0
  400bcc:	54fff908 	b.hi	400aec <main+0x54c>
  400bd0:	d503201f 	nop
  400bd4:	d503201f 	nop
  400bd8:	d503201f 	nop
  400bdc:	97fffe0d 	bl	400410 <clock@plt>
  400be0:	f9012fa0 	str	x0, [x29,#600]
  400be4:	f9412fa1 	ldr	x1, [x29,#600]
  400be8:	f94133a0 	ldr	x0, [x29,#608]
  400bec:	cb000020 	sub	x0, x1, x0
  400bf0:	f9013ba0 	str	x0, [x29,#624]
  400bf4:	f9413ba0 	ldr	x0, [x29,#624]
  400bf8:	9e620001 	scvtf	d1, x0
  400bfc:	5c0024a0 	ldr	d0, 401090 <main+0xaf0>
  400c00:	1e601820 	fdiv	d0, d1, d0
  400c04:	b0000000 	adrp	x0, 401000 <main+0xa60>
  400c08:	9104e000 	add	x0, x0, #0x138
  400c0c:	97fffe11 	bl	400450 <printf@plt>
  400c10:	b942c7a1 	ldr	w1, [x29,#708]
  400c14:	b942c7a2 	ldr	w2, [x29,#708]
  400c18:	910003e0 	mov	x0, sp
  400c1c:	aa0003f4 	mov	x20, x0
  400c20:	93407c20 	sxtw	x0, w1
  400c24:	d1000400 	sub	x0, x0, #0x1
  400c28:	f9012ba0 	str	x0, [x29,#592]
  400c2c:	93407c20 	sxtw	x0, w1
  400c30:	f90073a0 	str	x0, [x29,#224]
  400c34:	f90077bf 	str	xzr, [x29,#232]
  400c38:	a94e13a3 	ldp	x3, x4, [x29,#224]
  400c3c:	aa0303e5 	mov	x5, x3
  400c40:	d37bfca0 	lsr	x0, x5, #59
  400c44:	9e670080 	fmov	d0, x4
  400c48:	5f455400 	shl	d0, d0, #5
  400c4c:	fd00efa0 	str	d0, [x29,#472]
  400c50:	f940efa5 	ldr	x5, [x29,#472]
  400c54:	aa050000 	orr	x0, x0, x5
  400c58:	f900efa0 	str	x0, [x29,#472]
  400c5c:	9e670060 	fmov	d0, x3
  400c60:	5f455400 	shl	d0, d0, #5
  400c64:	fd00eba0 	str	d0, [x29,#464]
  400c68:	93407c20 	sxtw	x0, w1
  400c6c:	d37ef413 	lsl	x19, x0, #2
  400c70:	93407c40 	sxtw	x0, w2
  400c74:	d1000400 	sub	x0, x0, #0x1
  400c78:	f90127a0 	str	x0, [x29,#584]
  400c7c:	93407c20 	sxtw	x0, w1
  400c80:	f9006ba0 	str	x0, [x29,#208]
  400c84:	f9006fbf 	str	xzr, [x29,#216]
  400c88:	93407c40 	sxtw	x0, w2
  400c8c:	f90063a0 	str	x0, [x29,#192]
  400c90:	f90067bf 	str	xzr, [x29,#200]
  400c94:	a94d1fa6 	ldp	x6, x7, [x29,#208]
  400c98:	aa0603e0 	mov	x0, x6
  400c9c:	a94c17a4 	ldp	x4, x5, [x29,#192]
  400ca0:	aa0403e3 	mov	x3, x4
  400ca4:	9b037c03 	mul	x3, x0, x3
  400ca8:	aa0603e0 	mov	x0, x6
  400cac:	aa0403e8 	mov	x8, x4
  400cb0:	9bc87c00 	umulh	x0, x0, x8
  400cb4:	aa0703e8 	mov	x8, x7
  400cb8:	aa0403e9 	mov	x9, x4
  400cbc:	9b090100 	madd	x0, x8, x9, x0
  400cc0:	aa0503e4 	mov	x4, x5
  400cc4:	9b0400c0 	madd	x0, x6, x4, x0
  400cc8:	f9005ba3 	str	x3, [x29,#176]
  400ccc:	f9005fa0 	str	x0, [x29,#184]
  400cd0:	a94b13a3 	ldp	x3, x4, [x29,#176]
  400cd4:	aa0303e5 	mov	x5, x3
  400cd8:	d37bfca0 	lsr	x0, x5, #59
  400cdc:	9e670080 	fmov	d0, x4
  400ce0:	5f455400 	shl	d0, d0, #5
  400ce4:	fd00e7a0 	str	d0, [x29,#456]
  400ce8:	f940e7a5 	ldr	x5, [x29,#456]
  400cec:	aa050000 	orr	x0, x0, x5
  400cf0:	f900e7a0 	str	x0, [x29,#456]
  400cf4:	9e670060 	fmov	d0, x3
  400cf8:	5f455400 	shl	d0, d0, #5
  400cfc:	fd00e3a0 	str	d0, [x29,#448]
  400d00:	93407c20 	sxtw	x0, w1
  400d04:	f90053a0 	str	x0, [x29,#160]
  400d08:	f90057bf 	str	xzr, [x29,#168]
  400d0c:	93407c40 	sxtw	x0, w2
  400d10:	f9004ba0 	str	x0, [x29,#144]
  400d14:	f9004fbf 	str	xzr, [x29,#152]
  400d18:	a94a1fa6 	ldp	x6, x7, [x29,#160]
  400d1c:	aa0603e0 	mov	x0, x6
  400d20:	a94917a4 	ldp	x4, x5, [x29,#144]
  400d24:	aa0403e3 	mov	x3, x4
  400d28:	9b037c03 	mul	x3, x0, x3
  400d2c:	aa0603e0 	mov	x0, x6
  400d30:	aa0403e8 	mov	x8, x4
  400d34:	9bc87c00 	umulh	x0, x0, x8
  400d38:	aa0703e8 	mov	x8, x7
  400d3c:	aa0403e9 	mov	x9, x4
  400d40:	9b090100 	madd	x0, x8, x9, x0
  400d44:	aa0503e4 	mov	x4, x5
  400d48:	9b0400c0 	madd	x0, x6, x4, x0
  400d4c:	f90043a3 	str	x3, [x29,#128]
  400d50:	f90047a0 	str	x0, [x29,#136]
  400d54:	a94813a3 	ldp	x3, x4, [x29,#128]
  400d58:	aa0303e5 	mov	x5, x3
  400d5c:	d37bfca0 	lsr	x0, x5, #59
  400d60:	9e670080 	fmov	d0, x4
  400d64:	5f455400 	shl	d0, d0, #5
  400d68:	fd00dfa0 	str	d0, [x29,#440]
  400d6c:	f940dfa5 	ldr	x5, [x29,#440]
  400d70:	aa050000 	orr	x0, x0, x5
  400d74:	f900dfa0 	str	x0, [x29,#440]
  400d78:	9e670060 	fmov	d0, x3
  400d7c:	5f455400 	shl	d0, d0, #5
  400d80:	fd00dba0 	str	d0, [x29,#432]
  400d84:	93407c21 	sxtw	x1, w1
  400d88:	93407c40 	sxtw	x0, w2
  400d8c:	9b007c20 	mul	x0, x1, x0
  400d90:	d37ef400 	lsl	x0, x0, #2
  400d94:	91000c00 	add	x0, x0, #0x3
  400d98:	91003c00 	add	x0, x0, #0xf
  400d9c:	d344fc00 	lsr	x0, x0, #4
  400da0:	d37cec00 	lsl	x0, x0, #4
  400da4:	cb2063ff 	sub	sp, sp, x0
  400da8:	910003e0 	mov	x0, sp
  400dac:	91000c00 	add	x0, x0, #0x3
  400db0:	d342fc00 	lsr	x0, x0, #2
  400db4:	d37ef400 	lsl	x0, x0, #2
  400db8:	f90123a0 	str	x0, [x29,#576]
  400dbc:	f9016fbf 	str	xzr, [x29,#728]
  400dc0:	14000026 	b	400e58 <main+0x8b8>
  400dc4:	f9016bbf 	str	xzr, [x29,#720]
  400dc8:	1400001d 	b	400e3c <main+0x89c>
  400dcc:	d342fec2 	lsr	x2, x22, #2
  400dd0:	f9416fa0 	ldr	x0, [x29,#728]
  400dd4:	91000400 	add	x0, x0, #0x1
  400dd8:	2a0003e1 	mov	w1, w0
  400ddc:	f9416ba0 	ldr	x0, [x29,#720]
  400de0:	1b007c20 	mul	w0, w1, w0
  400de4:	2a0003e3 	mov	w3, w0
  400de8:	f9414ba0 	ldr	x0, [x29,#656]
  400dec:	f9416fa1 	ldr	x1, [x29,#728]
  400df0:	9b017c42 	mul	x2, x2, x1
  400df4:	f9416ba1 	ldr	x1, [x29,#720]
  400df8:	8b010041 	add	x1, x2, x1
  400dfc:	b8217803 	str	w3, [x0,x1,lsl #2]
  400e00:	d342fea2 	lsr	x2, x21, #2
  400e04:	f9416fa0 	ldr	x0, [x29,#728]
  400e08:	2a0003e1 	mov	w1, w0
  400e0c:	f9416ba0 	ldr	x0, [x29,#720]
  400e10:	1b007c20 	mul	w0, w1, w0
  400e14:	2a0003e3 	mov	w3, w0
  400e18:	f9413fa0 	ldr	x0, [x29,#632]
  400e1c:	f9416fa1 	ldr	x1, [x29,#728]
  400e20:	9b017c42 	mul	x2, x2, x1
  400e24:	f9416ba1 	ldr	x1, [x29,#720]
  400e28:	8b010041 	add	x1, x2, x1
  400e2c:	b8217803 	str	w3, [x0,x1,lsl #2]
  400e30:	f9416ba0 	ldr	x0, [x29,#720]
  400e34:	91000400 	add	x0, x0, #0x1
  400e38:	f9016ba0 	str	x0, [x29,#720]
  400e3c:	b982c7a1 	ldrsw	x1, [x29,#708]
  400e40:	f9416ba0 	ldr	x0, [x29,#720]
  400e44:	eb00003f 	cmp	x1, x0
  400e48:	54fffc28 	b.hi	400dcc <main+0x82c>
  400e4c:	f9416fa0 	ldr	x0, [x29,#728]
  400e50:	91000400 	add	x0, x0, #0x1
  400e54:	f9016fa0 	str	x0, [x29,#728]
  400e58:	b982c7a1 	ldrsw	x1, [x29,#708]
  400e5c:	f9416fa0 	ldr	x0, [x29,#728]
  400e60:	eb00003f 	cmp	x1, x0
  400e64:	54fffb08 	b.hi	400dc4 <main+0x824>
  400e68:	97fffd6a 	bl	400410 <clock@plt>
  400e6c:	f90133a0 	str	x0, [x29,#608]
  400e70:	f9016fbf 	str	xzr, [x29,#728]
  400e74:	1400001b 	b	400ee0 <main+0x940>
  400e78:	f9016bbf 	str	xzr, [x29,#720]
  400e7c:	14000012 	b	400ec4 <main+0x924>
  400e80:	d342fe63 	lsr	x3, x19, #2
  400e84:	d342fea2 	lsr	x2, x21, #2
  400e88:	f9413fa0 	ldr	x0, [x29,#632]
  400e8c:	f9416ba1 	ldr	x1, [x29,#720]
  400e90:	9b017c42 	mul	x2, x2, x1
  400e94:	f9416fa1 	ldr	x1, [x29,#728]
  400e98:	8b010041 	add	x1, x2, x1
  400e9c:	b8617802 	ldr	w2, [x0,x1,lsl #2]
  400ea0:	f94123a0 	ldr	x0, [x29,#576]
  400ea4:	f9416fa1 	ldr	x1, [x29,#728]
  400ea8:	9b017c63 	mul	x3, x3, x1
  400eac:	f9416ba1 	ldr	x1, [x29,#720]
  400eb0:	8b010061 	add	x1, x3, x1
  400eb4:	b8217802 	str	w2, [x0,x1,lsl #2]
  400eb8:	f9416ba0 	ldr	x0, [x29,#720]
  400ebc:	91000400 	add	x0, x0, #0x1
  400ec0:	f9016ba0 	str	x0, [x29,#720]
  400ec4:	b982c7a1 	ldrsw	x1, [x29,#708]
  400ec8:	f9416ba0 	ldr	x0, [x29,#720]
  400ecc:	eb00003f 	cmp	x1, x0
  400ed0:	54fffd88 	b.hi	400e80 <main+0x8e0>
  400ed4:	f9416fa0 	ldr	x0, [x29,#728]
  400ed8:	91000400 	add	x0, x0, #0x1
  400edc:	f9016fa0 	str	x0, [x29,#728]
  400ee0:	b982c7a1 	ldrsw	x1, [x29,#708]
  400ee4:	f9416fa0 	ldr	x0, [x29,#728]
  400ee8:	eb00003f 	cmp	x1, x0
  400eec:	54fffc68 	b.hi	400e78 <main+0x8d8>
  400ef0:	d503201f 	nop
  400ef4:	d503201f 	nop
  400ef8:	d503201f 	nop
  400efc:	f9016fbf 	str	xzr, [x29,#728]
  400f00:	14000036 	b	400fd8 <main+0xa38>
  400f04:	f9016bbf 	str	xzr, [x29,#720]
  400f08:	1400002d 	b	400fbc <main+0xa1c>
  400f0c:	f90167bf 	str	xzr, [x29,#712]
  400f10:	14000024 	b	400fa0 <main+0xa00>
  400f14:	9e660100 	fmov	x0, d8
  400f18:	d342fc03 	lsr	x3, x0, #2
  400f1c:	9e660100 	fmov	x0, d8
  400f20:	d342fc02 	lsr	x2, x0, #2
  400f24:	f94157a0 	ldr	x0, [x29,#680]
  400f28:	f9416fa1 	ldr	x1, [x29,#728]
  400f2c:	9b017c42 	mul	x2, x2, x1
  400f30:	f9416ba1 	ldr	x1, [x29,#720]
  400f34:	8b010041 	add	x1, x2, x1
  400f38:	b8617801 	ldr	w1, [x0,x1,lsl #2]
  400f3c:	d342fec4 	lsr	x4, x22, #2
  400f40:	f9414ba0 	ldr	x0, [x29,#656]
  400f44:	f9416fa2 	ldr	x2, [x29,#728]
  400f48:	9b027c84 	mul	x4, x4, x2
  400f4c:	f94167a2 	ldr	x2, [x29,#712]
  400f50:	8b020082 	add	x2, x4, x2
  400f54:	b8627802 	ldr	w2, [x0,x2,lsl #2]
  400f58:	d342fe65 	lsr	x5, x19, #2
  400f5c:	f94123a0 	ldr	x0, [x29,#576]
  400f60:	f9416ba4 	ldr	x4, [x29,#720]
  400f64:	9b047ca5 	mul	x5, x5, x4
  400f68:	f94167a4 	ldr	x4, [x29,#712]
  400f6c:	8b0400a4 	add	x4, x5, x4
  400f70:	b8647800 	ldr	w0, [x0,x4,lsl #2]
  400f74:	1b007c40 	mul	w0, w2, w0
  400f78:	0b000022 	add	w2, w1, w0
  400f7c:	f94157a0 	ldr	x0, [x29,#680]
  400f80:	f9416fa1 	ldr	x1, [x29,#728]
  400f84:	9b017c63 	mul	x3, x3, x1
  400f88:	f9416ba1 	ldr	x1, [x29,#720]
  400f8c:	8b010061 	add	x1, x3, x1
  400f90:	b8217802 	str	w2, [x0,x1,lsl #2]
  400f94:	f94167a0 	ldr	x0, [x29,#712]
  400f98:	91000400 	add	x0, x0, #0x1
  400f9c:	f90167a0 	str	x0, [x29,#712]
  400fa0:	b982c7a1 	ldrsw	x1, [x29,#708]
  400fa4:	f94167a0 	ldr	x0, [x29,#712]
  400fa8:	eb00003f 	cmp	x1, x0
  400fac:	54fffb48 	b.hi	400f14 <main+0x974>
  400fb0:	f9416ba0 	ldr	x0, [x29,#720]
  400fb4:	91000400 	add	x0, x0, #0x1
  400fb8:	f9016ba0 	str	x0, [x29,#720]
  400fbc:	b982c7a1 	ldrsw	x1, [x29,#708]
  400fc0:	f9416ba0 	ldr	x0, [x29,#720]
  400fc4:	eb00003f 	cmp	x1, x0
  400fc8:	54fffa28 	b.hi	400f0c <main+0x96c>
  400fcc:	f9416fa0 	ldr	x0, [x29,#728]
  400fd0:	91000400 	add	x0, x0, #0x1
  400fd4:	f9016fa0 	str	x0, [x29,#728]
  400fd8:	b982c7a1 	ldrsw	x1, [x29,#708]
  400fdc:	f9416fa0 	ldr	x0, [x29,#728]
  400fe0:	eb00003f 	cmp	x1, x0
  400fe4:	54fff908 	b.hi	400f04 <main+0x964>
  400fe8:	d503201f 	nop
  400fec:	d503201f 	nop
  400ff0:	d503201f 	nop
  400ff4:	97fffd07 	bl	400410 <clock@plt>
  400ff8:	f9012fa0 	str	x0, [x29,#600]
  400ffc:	f9412fa1 	ldr	x1, [x29,#600]
  401000:	f94133a0 	ldr	x0, [x29,#608]
  401004:	cb000020 	sub	x0, x1, x0
  401008:	f90137a0 	str	x0, [x29,#616]
  40100c:	f94137a0 	ldr	x0, [x29,#616]
  401010:	9e620001 	scvtf	d1, x0
  401014:	5c0003e0 	ldr	d0, 401090 <main+0xaf0>
  401018:	1e601820 	fdiv	d0, d1, d0
  40101c:	90000000 	adrp	x0, 401000 <main+0xa60>
  401020:	91052000 	add	x0, x0, #0x148
  401024:	97fffd0b 	bl	400450 <printf@plt>
  401028:	f9413ba1 	ldr	x1, [x29,#624]
  40102c:	f94137a0 	ldr	x0, [x29,#616]
  401030:	cb000020 	sub	x0, x1, x0
  401034:	9e620001 	scvtf	d1, x0
  401038:	f9413ba0 	ldr	x0, [x29,#624]
  40103c:	9e620000 	scvtf	d0, x0
  401040:	1e601821 	fdiv	d1, d1, d0
  401044:	5c0002a0 	ldr	d0, 401098 <main+0xaf8>
  401048:	1e600820 	fmul	d0, d1, d0
  40104c:	90000000 	adrp	x0, 401000 <main+0xa60>
  401050:	91056000 	add	x0, x0, #0x158
  401054:	97fffcff 	bl	400450 <printf@plt>
  401058:	52800000 	mov	w0, #0x0                   	// #0
  40105c:	9100029f 	mov	sp, x20
  401060:	f9403fa1 	ldr	x1, [x29,#120]
  401064:	9100003f 	mov	sp, x1
  401068:	910003bf 	mov	sp, x29
  40106c:	a94153f3 	ldp	x19, x20, [sp,#16]
  401070:	a9425bf5 	ldp	x21, x22, [sp,#32]
  401074:	a94363f7 	ldp	x23, x24, [sp,#48]
  401078:	a9446bf9 	ldp	x25, x26, [sp,#64]
  40107c:	a94573fb 	ldp	x27, x28, [sp,#80]
  401080:	fd4033e8 	ldr	d8, [sp,#96]
  401084:	a8c77bfd 	ldp	x29, x30, [sp],#112
  401088:	9109c3ff 	add	sp, sp, #0x270
  40108c:	d65f03c0 	ret
  401090:	00000000 	.word	0x00000000
  401094:	412e8480 	.word	0x412e8480
  401098:	00000000 	.word	0x00000000
  40109c:	40590000 	.word	0x40590000

00000000004010a0 <__libc_csu_init>:
  4010a0:	a9bc7bfd 	stp	x29, x30, [sp,#-64]!
  4010a4:	910003fd 	mov	x29, sp
  4010a8:	a9025bf5 	stp	x21, x22, [sp,#32]
  4010ac:	a90153f3 	stp	x19, x20, [sp,#16]
  4010b0:	90000095 	adrp	x21, 411000 <__FRAME_END__+0xfe88>
  4010b4:	90000094 	adrp	x20, 411000 <__FRAME_END__+0xfe88>
  4010b8:	910602b5 	add	x21, x21, #0x180
  4010bc:	91062294 	add	x20, x20, #0x188
  4010c0:	cb150294 	sub	x20, x20, x21
  4010c4:	aa0203f6 	mov	x22, x2
  4010c8:	9343fe94 	asr	x20, x20, #3
  4010cc:	a90363f7 	stp	x23, x24, [sp,#48]
  4010d0:	2a0003f8 	mov	w24, w0
  4010d4:	aa0103f7 	mov	x23, x1
  4010d8:	97fffcbe 	bl	4003d0 <_init>
  4010dc:	b4000154 	cbz	x20, 401104 <__libc_csu_init+0x64>
  4010e0:	d2800013 	mov	x19, #0x0                   	// #0
  4010e4:	f8737aa3 	ldr	x3, [x21,x19,lsl #3]
  4010e8:	aa1603e2 	mov	x2, x22
  4010ec:	aa1703e1 	mov	x1, x23
  4010f0:	2a1803e0 	mov	w0, w24
  4010f4:	91000673 	add	x19, x19, #0x1
  4010f8:	d63f0060 	blr	x3
  4010fc:	eb14027f 	cmp	x19, x20
  401100:	54ffff21 	b.ne	4010e4 <__libc_csu_init+0x44>
  401104:	a94153f3 	ldp	x19, x20, [sp,#16]
  401108:	a9425bf5 	ldp	x21, x22, [sp,#32]
  40110c:	a94363f7 	ldp	x23, x24, [sp,#48]
  401110:	a8c47bfd 	ldp	x29, x30, [sp],#64
  401114:	d65f03c0 	ret

0000000000401118 <__libc_csu_fini>:
  401118:	d65f03c0 	ret
  40111c:	d503201f 	nop

Disassembly of section .fini:

0000000000401120 <_fini>:
  401120:	a9bf7bfd 	stp	x29, x30, [sp,#-16]!
  401124:	910003fd 	mov	x29, sp
  401128:	a8c17bfd 	ldp	x29, x30, [sp],#16
  40112c:	d65f03c0 	ret
