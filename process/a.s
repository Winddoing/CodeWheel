
a.out:     file format elf64-x86-64


Disassembly of section .interp:

0000000000400238 <.interp>:
  400238:	2f                   	(bad)  
  400239:	6c                   	insb   (%dx),%es:(%rdi)
  40023a:	69 62 36 34 2f 6c 64 	imul   $0x646c2f34,0x36(%rdx),%esp
  400241:	2d 6c 69 6e 75       	sub    $0x756e696c,%eax
  400246:	78 2d                	js     400275 <_init-0x28b>
  400248:	78 38                	js     400282 <_init-0x27e>
  40024a:	36                   	ss
  40024b:	2d 36 34 2e 73       	sub    $0x732e3436,%eax
  400250:	6f                   	outsl  %ds:(%rsi),(%dx)
  400251:	2e 32 00             	xor    %cs:(%rax),%al

Disassembly of section .note.ABI-tag:

0000000000400254 <.note.ABI-tag>:
  400254:	04 00                	add    $0x0,%al
  400256:	00 00                	add    %al,(%rax)
  400258:	10 00                	adc    %al,(%rax)
  40025a:	00 00                	add    %al,(%rax)
  40025c:	01 00                	add    %eax,(%rax)
  40025e:	00 00                	add    %al,(%rax)
  400260:	47                   	rex.RXB
  400261:	4e 55                	rex.WRX push %rbp
  400263:	00 00                	add    %al,(%rax)
  400265:	00 00                	add    %al,(%rax)
  400267:	00 02                	add    %al,(%rdx)
  400269:	00 00                	add    %al,(%rax)
  40026b:	00 06                	add    %al,(%rsi)
  40026d:	00 00                	add    %al,(%rax)
  40026f:	00 18                	add    %bl,(%rax)
  400271:	00 00                	add    %al,(%rax)
	...

Disassembly of section .note.gnu.build-id:

0000000000400274 <.note.gnu.build-id>:
  400274:	04 00                	add    $0x0,%al
  400276:	00 00                	add    %al,(%rax)
  400278:	14 00                	adc    $0x0,%al
  40027a:	00 00                	add    %al,(%rax)
  40027c:	03 00                	add    (%rax),%eax
  40027e:	00 00                	add    %al,(%rax)
  400280:	47                   	rex.RXB
  400281:	4e 55                	rex.WRX push %rbp
  400283:	00 ef                	add    %ch,%bh
  400285:	0a 4d 82             	or     -0x7e(%rbp),%cl
  400288:	95                   	xchg   %eax,%ebp
  400289:	0c 52                	or     $0x52,%al
  40028b:	3f                   	(bad)  
  40028c:	96                   	xchg   %eax,%esi
  40028d:	4f                   	rex.WRXB
  40028e:	41 7d 6a             	rex.B jge 4002fb <_init-0x205>
  400291:	0a 3b                	or     (%rbx),%bh
  400293:	32 31                	xor    (%rcx),%dh
  400295:	5d                   	pop    %rbp
  400296:	d0                   	.byte 0xd0
  400297:	67                   	addr32

Disassembly of section .gnu.hash:

0000000000400298 <.gnu.hash>:
  400298:	02 00                	add    (%rax),%al
  40029a:	00 00                	add    %al,(%rax)
  40029c:	08 00                	or     %al,(%rax)
  40029e:	00 00                	add    %al,(%rax)
  4002a0:	01 00                	add    %eax,(%rax)
  4002a2:	00 00                	add    %al,(%rax)
  4002a4:	06                   	(bad)  
  4002a5:	00 00                	add    %al,(%rax)
  4002a7:	00 00                	add    %al,(%rax)
  4002a9:	01 00                	add    %eax,(%rax)
  4002ab:	00 00                	add    %al,(%rax)
  4002ad:	00 00                	add    %al,(%rax)
  4002af:	02 00                	add    (%rax),%al
  4002b1:	00 00                	add    %al,(%rax)
  4002b3:	00 08                	add    %cl,(%rax)
  4002b5:	00 00                	add    %al,(%rax)
  4002b7:	00 39                	add    %bh,(%rcx)
  4002b9:	f2                   	repnz
  4002ba:	8b                   	.byte 0x8b
  4002bb:	1c                   	.byte 0x1c

Disassembly of section .dynsym:

00000000004002c0 <.dynsym>:
	...
  4002d8:	15 00 00 00 12       	adc    $0x12000000,%eax
	...
  4002ed:	00 00                	add    %al,(%rax)
  4002ef:	00 2d 00 00 00 12    	add    %ch,0x12000000(%rip)        # 124002f5 <_end+0x11dff285>
	...
  400305:	00 00                	add    %al,(%rax)
  400307:	00 3f                	add    %bh,(%rdi)
  400309:	00 00                	add    %al,(%rax)
  40030b:	00 20                	add    %ah,(%rax)
	...
  40031d:	00 00                	add    %al,(%rax)
  40031f:	00 0b                	add    %cl,(%rbx)
  400321:	00 00                	add    %al,(%rax)
  400323:	00 12                	add    %dl,(%rdx)
	...
  400335:	00 00                	add    %al,(%rax)
  400337:	00 26                	add    %ah,(%rsi)
  400339:	00 00                	add    %al,(%rax)
  40033b:	00 12                	add    %dl,(%rdx)
	...
  40034d:	00 00                	add    %al,(%rax)
  40034f:	00 10                	add    %dl,(%rax)
  400351:	00 00                	add    %al,(%rax)
  400353:	00 12                	add    %dl,(%rdx)
	...
  400365:	00 00                	add    %al,(%rax)
  400367:	00 1a                	add    %bl,(%rdx)
  400369:	00 00                	add    %al,(%rax)
  40036b:	00 12                	add    %dl,(%rdx)
	...
  40037d:	00 00                	add    %al,(%rax)
  40037f:	00 1f                	add    %bl,(%rdi)
  400381:	00 00                	add    %al,(%rax)
  400383:	00 11                	add    %dl,(%rcx)
  400385:	00 19                	add    %bl,(%rcx)
  400387:	00 60 10             	add    %ah,0x10(%rax)
  40038a:	60                   	(bad)  
  40038b:	00 00                	add    %al,(%rax)
  40038d:	00 00                	add    %al,(%rax)
  40038f:	00 08                	add    %cl,(%rax)
  400391:	00 00                	add    %al,(%rax)
  400393:	00 00                	add    %al,(%rax)
  400395:	00 00                	add    %al,(%rax)
	...

Disassembly of section .dynstr:

0000000000400398 <.dynstr>:
  400398:	00 6c 69 62          	add    %ch,0x62(%rcx,%rbp,2)
  40039c:	63 2e                	movslq (%rsi),%ebp
  40039e:	73 6f                	jae    40040f <_init-0xf1>
  4003a0:	2e 36 00 65 78       	cs add %ah,%cs:%ss:0x78(%rbp)
  4003a5:	69 74 00 77 61 69 74 	imul   $0x746961,0x77(%rax,%rax,1),%esi
  4003ac:	00 
  4003ad:	70 75                	jo     400424 <_init-0xdc>
  4003af:	74 73                	je     400424 <_init-0xdc>
  4003b1:	00 66 6f             	add    %ah,0x6f(%rsi)
  4003b4:	72 6b                	jb     400421 <_init-0xdf>
  4003b6:	00 73 74             	add    %dh,0x74(%rbx)
  4003b9:	64                   	fs
  4003ba:	65                   	gs
  4003bb:	72 72                	jb     40042f <_init-0xd1>
  4003bd:	00 66 77             	add    %ah,0x77(%rsi)
  4003c0:	72 69                	jb     40042b <_init-0xd5>
  4003c2:	74 65                	je     400429 <_init-0xd7>
  4003c4:	00 5f 5f             	add    %bl,0x5f(%rdi)
  4003c7:	6c                   	insb   (%dx),%es:(%rdi)
  4003c8:	69 62 63 5f 73 74 61 	imul   $0x6174735f,0x63(%rdx),%esp
  4003cf:	72 74                	jb     400445 <_init-0xbb>
  4003d1:	5f                   	pop    %rdi
  4003d2:	6d                   	insl   (%dx),%es:(%rdi)
  4003d3:	61                   	(bad)  
  4003d4:	69 6e 00 5f 5f 67 6d 	imul   $0x6d675f5f,0x0(%rsi),%ebp
  4003db:	6f                   	outsl  %ds:(%rsi),(%dx)
  4003dc:	6e                   	outsb  %ds:(%rsi),(%dx)
  4003dd:	5f                   	pop    %rdi
  4003de:	73 74                	jae    400454 <_init-0xac>
  4003e0:	61                   	(bad)  
  4003e1:	72 74                	jb     400457 <_init-0xa9>
  4003e3:	5f                   	pop    %rdi
  4003e4:	5f                   	pop    %rdi
  4003e5:	00 47 4c             	add    %al,0x4c(%rdi)
  4003e8:	49                   	rex.WB
  4003e9:	42                   	rex.X
  4003ea:	43 5f                	rex.XB pop %r15
  4003ec:	32 2e                	xor    (%rsi),%ch
  4003ee:	32 2e                	xor    (%rsi),%ch
  4003f0:	35                   	.byte 0x35
	...

Disassembly of section .gnu.version:

00000000004003f2 <.gnu.version>:
  4003f2:	00 00                	add    %al,(%rax)
  4003f4:	02 00                	add    (%rax),%al
  4003f6:	02 00                	add    (%rax),%al
  4003f8:	00 00                	add    %al,(%rax)
  4003fa:	02 00                	add    (%rax),%al
  4003fc:	02 00                	add    (%rax),%al
  4003fe:	02 00                	add    (%rax),%al
  400400:	02 00                	add    (%rax),%al
  400402:	02 00                	add    (%rax),%al

Disassembly of section .gnu.version_r:

0000000000400408 <.gnu.version_r>:
  400408:	01 00                	add    %eax,(%rax)
  40040a:	01 00                	add    %eax,(%rax)
  40040c:	01 00                	add    %eax,(%rax)
  40040e:	00 00                	add    %al,(%rax)
  400410:	10 00                	adc    %al,(%rax)
  400412:	00 00                	add    %al,(%rax)
  400414:	00 00                	add    %al,(%rax)
  400416:	00 00                	add    %al,(%rax)
  400418:	75 1a                	jne    400434 <_init-0xcc>
  40041a:	69 09 00 00 02 00    	imul   $0x20000,(%rcx),%ecx
  400420:	4e 00 00             	rex.WRX add %r8b,(%rax)
  400423:	00 00                	add    %al,(%rax)
  400425:	00 00                	add    %al,(%rax)
	...

Disassembly of section .rela.dyn:

0000000000400428 <.rela.dyn>:
  400428:	f8                   	clc    
  400429:	0f 60 00             	punpcklbw (%rax),%mm0
  40042c:	00 00                	add    %al,(%rax)
  40042e:	00 00                	add    %al,(%rax)
  400430:	06                   	(bad)  
  400431:	00 00                	add    %al,(%rax)
  400433:	00 03                	add    %al,(%rbx)
	...
  40043d:	00 00                	add    %al,(%rax)
  40043f:	00 60 10             	add    %ah,0x10(%rax)
  400442:	60                   	(bad)  
  400443:	00 00                	add    %al,(%rax)
  400445:	00 00                	add    %al,(%rax)
  400447:	00 05 00 00 00 08    	add    %al,0x8000000(%rip)        # 840044d <_end+0x7dff3dd>
	...

Disassembly of section .rela.plt:

0000000000400458 <.rela.plt>:
  400458:	18 10                	sbb    %dl,(%rax)
  40045a:	60                   	(bad)  
  40045b:	00 00                	add    %al,(%rax)
  40045d:	00 00                	add    %al,(%rax)
  40045f:	00 07                	add    %al,(%rdi)
  400461:	00 00                	add    %al,(%rax)
  400463:	00 01                	add    %al,(%rcx)
	...
  40046d:	00 00                	add    %al,(%rax)
  40046f:	00 20                	add    %ah,(%rax)
  400471:	10 60 00             	adc    %ah,0x0(%rax)
  400474:	00 00                	add    %al,(%rax)
  400476:	00 00                	add    %al,(%rax)
  400478:	07                   	(bad)  
  400479:	00 00                	add    %al,(%rax)
  40047b:	00 02                	add    %al,(%rdx)
	...
  400485:	00 00                	add    %al,(%rax)
  400487:	00 28                	add    %ch,(%rax)
  400489:	10 60 00             	adc    %ah,0x0(%rax)
  40048c:	00 00                	add    %al,(%rax)
  40048e:	00 00                	add    %al,(%rax)
  400490:	07                   	(bad)  
  400491:	00 00                	add    %al,(%rax)
  400493:	00 03                	add    %al,(%rbx)
	...
  40049d:	00 00                	add    %al,(%rax)
  40049f:	00 30                	add    %dh,(%rax)
  4004a1:	10 60 00             	adc    %ah,0x0(%rax)
  4004a4:	00 00                	add    %al,(%rax)
  4004a6:	00 00                	add    %al,(%rax)
  4004a8:	07                   	(bad)  
  4004a9:	00 00                	add    %al,(%rax)
  4004ab:	00 04 00             	add    %al,(%rax,%rax,1)
	...
  4004b6:	00 00                	add    %al,(%rax)
  4004b8:	38 10                	cmp    %dl,(%rax)
  4004ba:	60                   	(bad)  
  4004bb:	00 00                	add    %al,(%rax)
  4004bd:	00 00                	add    %al,(%rax)
  4004bf:	00 07                	add    %al,(%rdi)
  4004c1:	00 00                	add    %al,(%rax)
  4004c3:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # 4004c9 <_init-0x37>
  4004c9:	00 00                	add    %al,(%rax)
  4004cb:	00 00                	add    %al,(%rax)
  4004cd:	00 00                	add    %al,(%rax)
  4004cf:	00 40 10             	add    %al,0x10(%rax)
  4004d2:	60                   	(bad)  
  4004d3:	00 00                	add    %al,(%rax)
  4004d5:	00 00                	add    %al,(%rax)
  4004d7:	00 07                	add    %al,(%rdi)
  4004d9:	00 00                	add    %al,(%rax)
  4004db:	00 06                	add    %al,(%rsi)
	...
  4004e5:	00 00                	add    %al,(%rax)
  4004e7:	00 48 10             	add    %cl,0x10(%rax)
  4004ea:	60                   	(bad)  
  4004eb:	00 00                	add    %al,(%rax)
  4004ed:	00 00                	add    %al,(%rax)
  4004ef:	00 07                	add    %al,(%rdi)
  4004f1:	00 00                	add    %al,(%rax)
  4004f3:	00 07                	add    %al,(%rdi)
	...

Disassembly of section .init:

0000000000400500 <_init>:
  400500:	48 83 ec 08          	sub    $0x8,%rsp
  400504:	48 8b 05 ed 0a 20 00 	mov    0x200aed(%rip),%rax        # 600ff8 <_DYNAMIC+0x1d0>
  40050b:	48 85 c0             	test   %rax,%rax
  40050e:	74 05                	je     400515 <_init+0x15>
  400510:	e8 3b 00 00 00       	callq  400550 <__gmon_start__@plt>
  400515:	48 83 c4 08          	add    $0x8,%rsp
  400519:	c3                   	retq   

Disassembly of section .plt:

0000000000400520 <puts@plt-0x10>:
  400520:	ff 35 e2 0a 20 00    	pushq  0x200ae2(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400526:	ff 25 e4 0a 20 00    	jmpq   *0x200ae4(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40052c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400530 <puts@plt>:
  400530:	ff 25 e2 0a 20 00    	jmpq   *0x200ae2(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  400536:	68 00 00 00 00       	pushq  $0x0
  40053b:	e9 e0 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400540 <__libc_start_main@plt>:
  400540:	ff 25 da 0a 20 00    	jmpq   *0x200ada(%rip)        # 601020 <_GLOBAL_OFFSET_TABLE_+0x20>
  400546:	68 01 00 00 00       	pushq  $0x1
  40054b:	e9 d0 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400550 <__gmon_start__@plt>:
  400550:	ff 25 d2 0a 20 00    	jmpq   *0x200ad2(%rip)        # 601028 <_GLOBAL_OFFSET_TABLE_+0x28>
  400556:	68 02 00 00 00       	pushq  $0x2
  40055b:	e9 c0 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400560 <exit@plt>:
  400560:	ff 25 ca 0a 20 00    	jmpq   *0x200aca(%rip)        # 601030 <_GLOBAL_OFFSET_TABLE_+0x30>
  400566:	68 03 00 00 00       	pushq  $0x3
  40056b:	e9 b0 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400570 <fwrite@plt>:
  400570:	ff 25 c2 0a 20 00    	jmpq   *0x200ac2(%rip)        # 601038 <_GLOBAL_OFFSET_TABLE_+0x38>
  400576:	68 04 00 00 00       	pushq  $0x4
  40057b:	e9 a0 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400580 <wait@plt>:
  400580:	ff 25 ba 0a 20 00    	jmpq   *0x200aba(%rip)        # 601040 <_GLOBAL_OFFSET_TABLE_+0x40>
  400586:	68 05 00 00 00       	pushq  $0x5
  40058b:	e9 90 ff ff ff       	jmpq   400520 <_init+0x20>

0000000000400590 <fork@plt>:
  400590:	ff 25 b2 0a 20 00    	jmpq   *0x200ab2(%rip)        # 601048 <_GLOBAL_OFFSET_TABLE_+0x48>
  400596:	68 06 00 00 00       	pushq  $0x6
  40059b:	e9 80 ff ff ff       	jmpq   400520 <_init+0x20>

Disassembly of section .text:

00000000004005a0 <_start>:
  4005a0:	31 ed                	xor    %ebp,%ebp
  4005a2:	49 89 d1             	mov    %rdx,%r9
  4005a5:	5e                   	pop    %rsi
  4005a6:	48 89 e2             	mov    %rsp,%rdx
  4005a9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4005ad:	50                   	push   %rax
  4005ae:	54                   	push   %rsp
  4005af:	49 c7 c0 80 07 40 00 	mov    $0x400780,%r8
  4005b6:	48 c7 c1 10 07 40 00 	mov    $0x400710,%rcx
  4005bd:	48 c7 c7 8d 06 40 00 	mov    $0x40068d,%rdi
  4005c4:	e8 77 ff ff ff       	callq  400540 <__libc_start_main@plt>
  4005c9:	f4                   	hlt    
  4005ca:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004005d0 <deregister_tm_clones>:
  4005d0:	b8 67 10 60 00       	mov    $0x601067,%eax
  4005d5:	55                   	push   %rbp
  4005d6:	48 2d 60 10 60 00    	sub    $0x601060,%rax
  4005dc:	48 83 f8 0e          	cmp    $0xe,%rax
  4005e0:	48 89 e5             	mov    %rsp,%rbp
  4005e3:	77 02                	ja     4005e7 <deregister_tm_clones+0x17>
  4005e5:	5d                   	pop    %rbp
  4005e6:	c3                   	retq   
  4005e7:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ec:	48 85 c0             	test   %rax,%rax
  4005ef:	74 f4                	je     4005e5 <deregister_tm_clones+0x15>
  4005f1:	5d                   	pop    %rbp
  4005f2:	bf 60 10 60 00       	mov    $0x601060,%edi
  4005f7:	ff e0                	jmpq   *%rax
  4005f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400600 <register_tm_clones>:
  400600:	b8 60 10 60 00       	mov    $0x601060,%eax
  400605:	55                   	push   %rbp
  400606:	48 2d 60 10 60 00    	sub    $0x601060,%rax
  40060c:	48 c1 f8 03          	sar    $0x3,%rax
  400610:	48 89 e5             	mov    %rsp,%rbp
  400613:	48 89 c2             	mov    %rax,%rdx
  400616:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40061a:	48 01 d0             	add    %rdx,%rax
  40061d:	48 d1 f8             	sar    %rax
  400620:	75 02                	jne    400624 <register_tm_clones+0x24>
  400622:	5d                   	pop    %rbp
  400623:	c3                   	retq   
  400624:	ba 00 00 00 00       	mov    $0x0,%edx
  400629:	48 85 d2             	test   %rdx,%rdx
  40062c:	74 f4                	je     400622 <register_tm_clones+0x22>
  40062e:	5d                   	pop    %rbp
  40062f:	48 89 c6             	mov    %rax,%rsi
  400632:	bf 60 10 60 00       	mov    $0x601060,%edi
  400637:	ff e2                	jmpq   *%rdx
  400639:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400640 <__do_global_dtors_aux>:
  400640:	80 3d 21 0a 20 00 00 	cmpb   $0x0,0x200a21(%rip)        # 601068 <completed.6973>
  400647:	75 11                	jne    40065a <__do_global_dtors_aux+0x1a>
  400649:	55                   	push   %rbp
  40064a:	48 89 e5             	mov    %rsp,%rbp
  40064d:	e8 7e ff ff ff       	callq  4005d0 <deregister_tm_clones>
  400652:	5d                   	pop    %rbp
  400653:	c6 05 0e 0a 20 00 01 	movb   $0x1,0x200a0e(%rip)        # 601068 <completed.6973>
  40065a:	f3 c3                	repz retq 
  40065c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400660 <frame_dummy>:
  400660:	48 83 3d b8 07 20 00 	cmpq   $0x0,0x2007b8(%rip)        # 600e20 <__JCR_END__>
  400667:	00 
  400668:	74 1e                	je     400688 <frame_dummy+0x28>
  40066a:	b8 00 00 00 00       	mov    $0x0,%eax
  40066f:	48 85 c0             	test   %rax,%rax
  400672:	74 14                	je     400688 <frame_dummy+0x28>
  400674:	55                   	push   %rbp
  400675:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  40067a:	48 89 e5             	mov    %rsp,%rbp
  40067d:	ff d0                	callq  *%rax
  40067f:	5d                   	pop    %rbp
  400680:	e9 7b ff ff ff       	jmpq   400600 <register_tm_clones>
  400685:	0f 1f 00             	nopl   (%rax)
  400688:	e9 73 ff ff ff       	jmpq   400600 <register_tm_clones>

000000000040068d <main>:
  40068d:	55                   	push   %rbp
  40068e:	48 89 e5             	mov    %rsp,%rbp
  400691:	48 83 ec 20          	sub    $0x20,%rsp
  400695:	89 7d ec             	mov    %edi,-0x14(%rbp)
  400698:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  40069c:	e8 ef fe ff ff       	callq  400590 <fork@plt>
  4006a1:	89 45 fc             	mov    %eax,-0x4(%rbp)
  4006a4:	83 7d fc 00          	cmpl   $0x0,-0x4(%rbp)
  4006a8:	79 28                	jns    4006d2 <main+0x45>
  4006aa:	48 8b 05 af 09 20 00 	mov    0x2009af(%rip),%rax        # 601060 <__TMC_END__>
  4006b1:	48 89 c1             	mov    %rax,%rcx
  4006b4:	ba 0c 00 00 00       	mov    $0xc,%edx
  4006b9:	be 01 00 00 00       	mov    $0x1,%esi
  4006be:	bf 94 07 40 00       	mov    $0x400794,%edi
  4006c3:	e8 a8 fe ff ff       	callq  400570 <fwrite@plt>
  4006c8:	bf ff ff ff ff       	mov    $0xffffffff,%edi
  4006cd:	e8 8e fe ff ff       	callq  400560 <exit@plt>
  4006d2:	83 7d fc 00          	cmpl   $0x0,-0x4(%rbp)
  4006d6:	75 0c                	jne    4006e4 <main+0x57>
  4006d8:	bf a1 07 40 00       	mov    $0x4007a1,%edi
  4006dd:	e8 4e fe ff ff       	callq  400530 <puts@plt>
  4006e2:	eb 23                	jmp    400707 <main+0x7a>
  4006e4:	bf b8 07 40 00       	mov    $0x4007b8,%edi
  4006e9:	e8 42 fe ff ff       	callq  400530 <puts@plt>
  4006ee:	bf 00 00 00 00       	mov    $0x0,%edi
  4006f3:	b8 00 00 00 00       	mov    $0x0,%eax
  4006f8:	e8 83 fe ff ff       	callq  400580 <wait@plt>
  4006fd:	bf d0 07 40 00       	mov    $0x4007d0,%edi
  400702:	e8 29 fe ff ff       	callq  400530 <puts@plt>
  400707:	b8 00 00 00 00       	mov    $0x0,%eax
  40070c:	c9                   	leaveq 
  40070d:	c3                   	retq   
  40070e:	66 90                	xchg   %ax,%ax

0000000000400710 <__libc_csu_init>:
  400710:	41 57                	push   %r15
  400712:	41 89 ff             	mov    %edi,%r15d
  400715:	41 56                	push   %r14
  400717:	49 89 f6             	mov    %rsi,%r14
  40071a:	41 55                	push   %r13
  40071c:	49 89 d5             	mov    %rdx,%r13
  40071f:	41 54                	push   %r12
  400721:	4c 8d 25 e8 06 20 00 	lea    0x2006e8(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400728:	55                   	push   %rbp
  400729:	48 8d 2d e8 06 20 00 	lea    0x2006e8(%rip),%rbp        # 600e18 <__init_array_end>
  400730:	53                   	push   %rbx
  400731:	4c 29 e5             	sub    %r12,%rbp
  400734:	31 db                	xor    %ebx,%ebx
  400736:	48 c1 fd 03          	sar    $0x3,%rbp
  40073a:	48 83 ec 08          	sub    $0x8,%rsp
  40073e:	e8 bd fd ff ff       	callq  400500 <_init>
  400743:	48 85 ed             	test   %rbp,%rbp
  400746:	74 1e                	je     400766 <__libc_csu_init+0x56>
  400748:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40074f:	00 
  400750:	4c 89 ea             	mov    %r13,%rdx
  400753:	4c 89 f6             	mov    %r14,%rsi
  400756:	44 89 ff             	mov    %r15d,%edi
  400759:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40075d:	48 83 c3 01          	add    $0x1,%rbx
  400761:	48 39 eb             	cmp    %rbp,%rbx
  400764:	75 ea                	jne    400750 <__libc_csu_init+0x40>
  400766:	48 83 c4 08          	add    $0x8,%rsp
  40076a:	5b                   	pop    %rbx
  40076b:	5d                   	pop    %rbp
  40076c:	41 5c                	pop    %r12
  40076e:	41 5d                	pop    %r13
  400770:	41 5e                	pop    %r14
  400772:	41 5f                	pop    %r15
  400774:	c3                   	retq   
  400775:	66 66 2e 0f 1f 84 00 	data32 nopw %cs:0x0(%rax,%rax,1)
  40077c:	00 00 00 00 

0000000000400780 <__libc_csu_fini>:
  400780:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400784 <_fini>:
  400784:	48 83 ec 08          	sub    $0x8,%rsp
  400788:	48 83 c4 08          	add    $0x8,%rsp
  40078c:	c3                   	retq   

Disassembly of section .rodata:

0000000000400790 <_IO_stdin_used>:
  400790:	01 00                	add    %eax,(%rax)
  400792:	02 00                	add    (%rax),%al
  400794:	46 6f                	rex.RX outsl %ds:(%rsi),(%dx)
  400796:	72 6b                	jb     400803 <_IO_stdin_used+0x73>
  400798:	20 46 61             	and    %al,0x61(%rsi)
  40079b:	69 6c 65 64 21 00 54 	imul   $0x68540021,0x64(%rbp,%riz,2),%ebp
  4007a2:	68 
  4007a3:	69 73 20 69 73 20 43 	imul   $0x43207369,0x20(%rbx),%esi
  4007aa:	68 69 6c 64 20       	pushq  $0x20646c69
  4007af:	50                   	push   %rax
  4007b0:	72 6f                	jb     400821 <_IO_stdin_used+0x91>
  4007b2:	63 65 73             	movslq 0x73(%rbp),%esp
  4007b5:	73 21                	jae    4007d8 <_IO_stdin_used+0x48>
  4007b7:	00 54 68 69          	add    %dl,0x69(%rax,%rbp,2)
  4007bb:	73 20                	jae    4007dd <_IO_stdin_used+0x4d>
  4007bd:	69 73 20 50 61 72 65 	imul   $0x65726150,0x20(%rbx),%esi
  4007c4:	6e                   	outsb  %ds:(%rsi),(%dx)
  4007c5:	74 20                	je     4007e7 <_IO_stdin_used+0x57>
  4007c7:	50                   	push   %rax
  4007c8:	72 6f                	jb     400839 <_IO_stdin_used+0xa9>
  4007ca:	63 65 73             	movslq 0x73(%rbp),%esp
  4007cd:	73 21                	jae    4007f0 <_IO_stdin_used+0x60>
  4007cf:	00 43 68             	add    %al,0x68(%rbx)
  4007d2:	69 6c 64 20 43 6f 6d 	imul   $0x706d6f43,0x20(%rsp,%riz,2),%ebp
  4007d9:	70 
  4007da:	6c                   	insb   (%dx),%es:(%rdi)
  4007db:	65                   	gs
  4007dc:	74 65                	je     400843 <_IO_stdin_used+0xb3>
  4007de:	21 00                	and    %eax,(%rax)

Disassembly of section .eh_frame_hdr:

00000000004007e0 <.eh_frame_hdr>:
  4007e0:	01 1b                	add    %ebx,(%rbx)
  4007e2:	03 3b                	add    (%rbx),%edi
  4007e4:	34 00                	xor    $0x0,%al
  4007e6:	00 00                	add    %al,(%rax)
  4007e8:	05 00 00 00 40       	add    $0x40000000,%eax
  4007ed:	fd                   	std    
  4007ee:	ff                   	(bad)  
  4007ef:	ff 80 00 00 00 c0    	incl   -0x40000000(%rax)
  4007f5:	fd                   	std    
  4007f6:	ff                   	(bad)  
  4007f7:	ff 50 00             	callq  *0x0(%rax)
  4007fa:	00 00                	add    %al,(%rax)
  4007fc:	ad                   	lods   %ds:(%rsi),%eax
  4007fd:	fe                   	(bad)  
  4007fe:	ff                   	(bad)  
  4007ff:	ff a8 00 00 00 30    	ljmpq  *0x30000000(%rax)
  400805:	ff                   	(bad)  
  400806:	ff                   	(bad)  
  400807:	ff c8                	dec    %eax
  400809:	00 00                	add    %al,(%rax)
  40080b:	00 a0 ff ff ff 10    	add    %ah,0x10ffffff(%rax)
  400811:	01 00                	add    %eax,(%rax)
	...

Disassembly of section .eh_frame:

0000000000400818 <__FRAME_END__-0xf0>:
  400818:	14 00                	adc    $0x0,%al
  40081a:	00 00                	add    %al,(%rax)
  40081c:	00 00                	add    %al,(%rax)
  40081e:	00 00                	add    %al,(%rax)
  400820:	01 7a 52             	add    %edi,0x52(%rdx)
  400823:	00 01                	add    %al,(%rcx)
  400825:	78 10                	js     400837 <_IO_stdin_used+0xa7>
  400827:	01 1b                	add    %ebx,(%rbx)
  400829:	0c 07                	or     $0x7,%al
  40082b:	08 90 01 07 10 14    	or     %dl,0x14100701(%rax)
  400831:	00 00                	add    %al,(%rax)
  400833:	00 1c 00             	add    %bl,(%rax,%rax,1)
  400836:	00 00                	add    %al,(%rax)
  400838:	68 fd ff ff 2a       	pushq  $0x2afffffd
	...
  400845:	00 00                	add    %al,(%rax)
  400847:	00 14 00             	add    %dl,(%rax,%rax,1)
  40084a:	00 00                	add    %al,(%rax)
  40084c:	00 00                	add    %al,(%rax)
  40084e:	00 00                	add    %al,(%rax)
  400850:	01 7a 52             	add    %edi,0x52(%rdx)
  400853:	00 01                	add    %al,(%rcx)
  400855:	78 10                	js     400867 <_IO_stdin_used+0xd7>
  400857:	01 1b                	add    %ebx,(%rbx)
  400859:	0c 07                	or     $0x7,%al
  40085b:	08 90 01 00 00 24    	or     %dl,0x24000001(%rax)
  400861:	00 00                	add    %al,(%rax)
  400863:	00 1c 00             	add    %bl,(%rax,%rax,1)
  400866:	00 00                	add    %al,(%rax)
  400868:	b8 fc ff ff 80       	mov    $0x80fffffc,%eax
  40086d:	00 00                	add    %al,(%rax)
  40086f:	00 00                	add    %al,(%rax)
  400871:	0e                   	(bad)  
  400872:	10 46 0e             	adc    %al,0xe(%rsi)
  400875:	18 4a 0f             	sbb    %cl,0xf(%rdx)
  400878:	0b 77 08             	or     0x8(%rdi),%esi
  40087b:	80 00 3f             	addb   $0x3f,(%rax)
  40087e:	1a 3b                	sbb    (%rbx),%bh
  400880:	2a 33                	sub    (%rbx),%dh
  400882:	24 22                	and    $0x22,%al
  400884:	00 00                	add    %al,(%rax)
  400886:	00 00                	add    %al,(%rax)
  400888:	1c 00                	sbb    $0x0,%al
  40088a:	00 00                	add    %al,(%rax)
  40088c:	44 00 00             	add    %r8b,(%rax)
  40088f:	00 fd                	add    %bh,%ch
  400891:	fd                   	std    
  400892:	ff                   	(bad)  
  400893:	ff 81 00 00 00 00    	incl   0x0(%rcx)
  400899:	41 0e                	rex.B (bad) 
  40089b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
  4008a1:	02 7c 0c 07          	add    0x7(%rsp,%rcx,1),%bh
  4008a5:	08 00                	or     %al,(%rax)
  4008a7:	00 44 00 00          	add    %al,0x0(%rax,%rax,1)
  4008ab:	00 64 00 00          	add    %ah,0x0(%rax,%rax,1)
  4008af:	00 60 fe             	add    %ah,-0x2(%rax)
  4008b2:	ff                   	(bad)  
  4008b3:	ff 65 00             	jmpq   *0x0(%rbp)
  4008b6:	00 00                	add    %al,(%rax)
  4008b8:	00 42 0e             	add    %al,0xe(%rdx)
  4008bb:	10 8f 02 45 0e 18    	adc    %cl,0x180e4502(%rdi)
  4008c1:	8e 03                	mov    (%rbx),%es
  4008c3:	45 0e                	rex.RB (bad) 
  4008c5:	20 8d 04 45 0e 28    	and    %cl,0x280e4504(%rbp)
  4008cb:	8c 05 48 0e 30 86    	mov    %es,-0x79cff1b8(%rip)        # ffffffff86701719 <_end+0xffffffff861006a9>
  4008d1:	06                   	(bad)  
  4008d2:	48 0e                	rex.W (bad) 
  4008d4:	38 83 07 4d 0e 40    	cmp    %al,0x400e4d07(%rbx)
  4008da:	6c                   	insb   (%dx),%es:(%rdi)
  4008db:	0e                   	(bad)  
  4008dc:	38 41 0e             	cmp    %al,0xe(%rcx)
  4008df:	30 41 0e             	xor    %al,0xe(%rcx)
  4008e2:	28 42 0e             	sub    %al,0xe(%rdx)
  4008e5:	20 42 0e             	and    %al,0xe(%rdx)
  4008e8:	18 42 0e             	sbb    %al,0xe(%rdx)
  4008eb:	10 42 0e             	adc    %al,0xe(%rdx)
  4008ee:	08 00                	or     %al,(%rax)
  4008f0:	14 00                	adc    $0x0,%al
  4008f2:	00 00                	add    %al,(%rax)
  4008f4:	ac                   	lods   %ds:(%rsi),%al
  4008f5:	00 00                	add    %al,(%rax)
  4008f7:	00 88 fe ff ff 02    	add    %cl,0x2fffffe(%rax)
	...

0000000000400908 <__FRAME_END__>:
  400908:	00 00                	add    %al,(%rax)
	...

Disassembly of section .init_array:

0000000000600e10 <__frame_dummy_init_array_entry>:
  600e10:	60                   	(bad)  
  600e11:	06                   	(bad)  
  600e12:	40 00 00             	add    %al,(%rax)
  600e15:	00 00                	add    %al,(%rax)
	...

Disassembly of section .fini_array:

0000000000600e18 <__do_global_dtors_aux_fini_array_entry>:
  600e18:	40 06                	rex (bad) 
  600e1a:	40 00 00             	add    %al,(%rax)
  600e1d:	00 00                	add    %al,(%rax)
	...

Disassembly of section .jcr:

0000000000600e20 <__JCR_END__>:
	...

Disassembly of section .dynamic:

0000000000600e28 <_DYNAMIC>:
  600e28:	01 00                	add    %eax,(%rax)
  600e2a:	00 00                	add    %al,(%rax)
  600e2c:	00 00                	add    %al,(%rax)
  600e2e:	00 00                	add    %al,(%rax)
  600e30:	01 00                	add    %eax,(%rax)
  600e32:	00 00                	add    %al,(%rax)
  600e34:	00 00                	add    %al,(%rax)
  600e36:	00 00                	add    %al,(%rax)
  600e38:	0c 00                	or     $0x0,%al
  600e3a:	00 00                	add    %al,(%rax)
  600e3c:	00 00                	add    %al,(%rax)
  600e3e:	00 00                	add    %al,(%rax)
  600e40:	00 05 40 00 00 00    	add    %al,0x40(%rip)        # 600e86 <_DYNAMIC+0x5e>
  600e46:	00 00                	add    %al,(%rax)
  600e48:	0d 00 00 00 00       	or     $0x0,%eax
  600e4d:	00 00                	add    %al,(%rax)
  600e4f:	00 84 07 40 00 00 00 	add    %al,0x40(%rdi,%rax,1)
  600e56:	00 00                	add    %al,(%rax)
  600e58:	19 00                	sbb    %eax,(%rax)
  600e5a:	00 00                	add    %al,(%rax)
  600e5c:	00 00                	add    %al,(%rax)
  600e5e:	00 00                	add    %al,(%rax)
  600e60:	10 0e                	adc    %cl,(%rsi)
  600e62:	60                   	(bad)  
  600e63:	00 00                	add    %al,(%rax)
  600e65:	00 00                	add    %al,(%rax)
  600e67:	00 1b                	add    %bl,(%rbx)
  600e69:	00 00                	add    %al,(%rax)
  600e6b:	00 00                	add    %al,(%rax)
  600e6d:	00 00                	add    %al,(%rax)
  600e6f:	00 08                	add    %cl,(%rax)
  600e71:	00 00                	add    %al,(%rax)
  600e73:	00 00                	add    %al,(%rax)
  600e75:	00 00                	add    %al,(%rax)
  600e77:	00 1a                	add    %bl,(%rdx)
  600e79:	00 00                	add    %al,(%rax)
  600e7b:	00 00                	add    %al,(%rax)
  600e7d:	00 00                	add    %al,(%rax)
  600e7f:	00 18                	add    %bl,(%rax)
  600e81:	0e                   	(bad)  
  600e82:	60                   	(bad)  
  600e83:	00 00                	add    %al,(%rax)
  600e85:	00 00                	add    %al,(%rax)
  600e87:	00 1c 00             	add    %bl,(%rax,%rax,1)
  600e8a:	00 00                	add    %al,(%rax)
  600e8c:	00 00                	add    %al,(%rax)
  600e8e:	00 00                	add    %al,(%rax)
  600e90:	08 00                	or     %al,(%rax)
  600e92:	00 00                	add    %al,(%rax)
  600e94:	00 00                	add    %al,(%rax)
  600e96:	00 00                	add    %al,(%rax)
  600e98:	f5                   	cmc    
  600e99:	fe                   	(bad)  
  600e9a:	ff 6f 00             	ljmpq  *0x0(%rdi)
  600e9d:	00 00                	add    %al,(%rax)
  600e9f:	00 98 02 40 00 00    	add    %bl,0x4002(%rax)
  600ea5:	00 00                	add    %al,(%rax)
  600ea7:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # 600ead <_DYNAMIC+0x85>
  600ead:	00 00                	add    %al,(%rax)
  600eaf:	00 98 03 40 00 00    	add    %bl,0x4003(%rax)
  600eb5:	00 00                	add    %al,(%rax)
  600eb7:	00 06                	add    %al,(%rsi)
  600eb9:	00 00                	add    %al,(%rax)
  600ebb:	00 00                	add    %al,(%rax)
  600ebd:	00 00                	add    %al,(%rax)
  600ebf:	00 c0                	add    %al,%al
  600ec1:	02 40 00             	add    0x0(%rax),%al
  600ec4:	00 00                	add    %al,(%rax)
  600ec6:	00 00                	add    %al,(%rax)
  600ec8:	0a 00                	or     (%rax),%al
  600eca:	00 00                	add    %al,(%rax)
  600ecc:	00 00                	add    %al,(%rax)
  600ece:	00 00                	add    %al,(%rax)
  600ed0:	5a                   	pop    %rdx
  600ed1:	00 00                	add    %al,(%rax)
  600ed3:	00 00                	add    %al,(%rax)
  600ed5:	00 00                	add    %al,(%rax)
  600ed7:	00 0b                	add    %cl,(%rbx)
  600ed9:	00 00                	add    %al,(%rax)
  600edb:	00 00                	add    %al,(%rax)
  600edd:	00 00                	add    %al,(%rax)
  600edf:	00 18                	add    %bl,(%rax)
  600ee1:	00 00                	add    %al,(%rax)
  600ee3:	00 00                	add    %al,(%rax)
  600ee5:	00 00                	add    %al,(%rax)
  600ee7:	00 15 00 00 00 00    	add    %dl,0x0(%rip)        # 600eed <_DYNAMIC+0xc5>
	...
  600ef5:	00 00                	add    %al,(%rax)
  600ef7:	00 03                	add    %al,(%rbx)
	...
  600f01:	10 60 00             	adc    %ah,0x0(%rax)
  600f04:	00 00                	add    %al,(%rax)
  600f06:	00 00                	add    %al,(%rax)
  600f08:	02 00                	add    (%rax),%al
  600f0a:	00 00                	add    %al,(%rax)
  600f0c:	00 00                	add    %al,(%rax)
  600f0e:	00 00                	add    %al,(%rax)
  600f10:	a8 00                	test   $0x0,%al
  600f12:	00 00                	add    %al,(%rax)
  600f14:	00 00                	add    %al,(%rax)
  600f16:	00 00                	add    %al,(%rax)
  600f18:	14 00                	adc    $0x0,%al
  600f1a:	00 00                	add    %al,(%rax)
  600f1c:	00 00                	add    %al,(%rax)
  600f1e:	00 00                	add    %al,(%rax)
  600f20:	07                   	(bad)  
  600f21:	00 00                	add    %al,(%rax)
  600f23:	00 00                	add    %al,(%rax)
  600f25:	00 00                	add    %al,(%rax)
  600f27:	00 17                	add    %dl,(%rdi)
  600f29:	00 00                	add    %al,(%rax)
  600f2b:	00 00                	add    %al,(%rax)
  600f2d:	00 00                	add    %al,(%rax)
  600f2f:	00 58 04             	add    %bl,0x4(%rax)
  600f32:	40 00 00             	add    %al,(%rax)
  600f35:	00 00                	add    %al,(%rax)
  600f37:	00 07                	add    %al,(%rdi)
  600f39:	00 00                	add    %al,(%rax)
  600f3b:	00 00                	add    %al,(%rax)
  600f3d:	00 00                	add    %al,(%rax)
  600f3f:	00 28                	add    %ch,(%rax)
  600f41:	04 40                	add    $0x40,%al
  600f43:	00 00                	add    %al,(%rax)
  600f45:	00 00                	add    %al,(%rax)
  600f47:	00 08                	add    %cl,(%rax)
  600f49:	00 00                	add    %al,(%rax)
  600f4b:	00 00                	add    %al,(%rax)
  600f4d:	00 00                	add    %al,(%rax)
  600f4f:	00 30                	add    %dh,(%rax)
  600f51:	00 00                	add    %al,(%rax)
  600f53:	00 00                	add    %al,(%rax)
  600f55:	00 00                	add    %al,(%rax)
  600f57:	00 09                	add    %cl,(%rcx)
  600f59:	00 00                	add    %al,(%rax)
  600f5b:	00 00                	add    %al,(%rax)
  600f5d:	00 00                	add    %al,(%rax)
  600f5f:	00 18                	add    %bl,(%rax)
  600f61:	00 00                	add    %al,(%rax)
  600f63:	00 00                	add    %al,(%rax)
  600f65:	00 00                	add    %al,(%rax)
  600f67:	00 fe                	add    %bh,%dh
  600f69:	ff                   	(bad)  
  600f6a:	ff 6f 00             	ljmpq  *0x0(%rdi)
  600f6d:	00 00                	add    %al,(%rax)
  600f6f:	00 08                	add    %cl,(%rax)
  600f71:	04 40                	add    $0x40,%al
  600f73:	00 00                	add    %al,(%rax)
  600f75:	00 00                	add    %al,(%rax)
  600f77:	00 ff                	add    %bh,%bh
  600f79:	ff                   	(bad)  
  600f7a:	ff 6f 00             	ljmpq  *0x0(%rdi)
  600f7d:	00 00                	add    %al,(%rax)
  600f7f:	00 01                	add    %al,(%rcx)
  600f81:	00 00                	add    %al,(%rax)
  600f83:	00 00                	add    %al,(%rax)
  600f85:	00 00                	add    %al,(%rax)
  600f87:	00 f0                	add    %dh,%al
  600f89:	ff                   	(bad)  
  600f8a:	ff 6f 00             	ljmpq  *0x0(%rdi)
  600f8d:	00 00                	add    %al,(%rax)
  600f8f:	00 f2                	add    %dh,%dl
  600f91:	03 40 00             	add    0x0(%rax),%eax
	...

Disassembly of section .got:

0000000000600ff8 <.got>:
	...

Disassembly of section .got.plt:

0000000000601000 <_GLOBAL_OFFSET_TABLE_>:
  601000:	28 0e                	sub    %cl,(%rsi)
  601002:	60                   	(bad)  
	...
  601017:	00 36                	add    %dh,(%rsi)
  601019:	05 40 00 00 00       	add    $0x40,%eax
  60101e:	00 00                	add    %al,(%rax)
  601020:	46 05 40 00 00 00    	rex.RX add $0x40,%eax
  601026:	00 00                	add    %al,(%rax)
  601028:	56                   	push   %rsi
  601029:	05 40 00 00 00       	add    $0x40,%eax
  60102e:	00 00                	add    %al,(%rax)
  601030:	66 05 40 00          	add    $0x40,%ax
  601034:	00 00                	add    %al,(%rax)
  601036:	00 00                	add    %al,(%rax)
  601038:	76 05                	jbe    60103f <_GLOBAL_OFFSET_TABLE_+0x3f>
  60103a:	40 00 00             	add    %al,(%rax)
  60103d:	00 00                	add    %al,(%rax)
  60103f:	00 86 05 40 00 00    	add    %al,0x4005(%rsi)
  601045:	00 00                	add    %al,(%rax)
  601047:	00 96 05 40 00 00    	add    %dl,0x4005(%rsi)
  60104d:	00 00                	add    %al,(%rax)
	...

Disassembly of section .data:

0000000000601050 <__data_start>:
	...

0000000000601058 <__dso_handle>:
	...

Disassembly of section .bss:

0000000000601060 <__bss_start>:
	...

0000000000601068 <completed.6973>:
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	47                   	rex.RXB
   1:	43                   	rex.XB
   2:	43 3a 20             	rex.XB cmp (%r8),%spl
   5:	28 55 62             	sub    %dl,0x62(%rbp)
   8:	75 6e                	jne    78 <_init-0x400488>
   a:	74 75                	je     81 <_init-0x40047f>
   c:	20 34 2e             	and    %dh,(%rsi,%rbp,1)
   f:	38 2e                	cmp    %ch,(%rsi)
  11:	34 2d                	xor    $0x2d,%al
  13:	32 75 62             	xor    0x62(%rbp),%dh
  16:	75 6e                	jne    86 <_init-0x40047a>
  18:	74 75                	je     8f <_init-0x400471>
  1a:	31 7e 31             	xor    %edi,0x31(%rsi)
  1d:	34 2e                	xor    $0x2e,%al
  1f:	30 34 2e             	xor    %dh,(%rsi,%rbp,1)
  22:	33 29                	xor    (%rcx),%ebp
  24:	20 34 2e             	and    %dh,(%rsi,%rbp,1)
  27:	38 2e                	cmp    %ch,(%rsi)
  29:	34 00                	xor    $0x0,%al
