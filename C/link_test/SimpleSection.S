
a.out:     file format elf64-x86-64


Disassembly of section .interp:

0000000000000238 <.interp>:
 238:	2f                   	(bad)  
 239:	6c                   	insb   (%dx),%es:(%rdi)
 23a:	69 62 36 34 2f 6c 64 	imul   $0x646c2f34,0x36(%rdx),%esp
 241:	2d 6c 69 6e 75       	sub    $0x756e696c,%eax
 246:	78 2d                	js     275 <_init-0x27b>
 248:	78 38                	js     282 <_init-0x26e>
 24a:	36 2d 36 34 2e 73    	ss sub $0x732e3436,%eax
 250:	6f                   	outsl  %ds:(%rsi),(%dx)
 251:	2e 32 00             	xor    %cs:(%rax),%al

Disassembly of section .note.ABI-tag:

0000000000000254 <.note.ABI-tag>:
 254:	04 00                	add    $0x0,%al
 256:	00 00                	add    %al,(%rax)
 258:	10 00                	adc    %al,(%rax)
 25a:	00 00                	add    %al,(%rax)
 25c:	01 00                	add    %eax,(%rax)
 25e:	00 00                	add    %al,(%rax)
 260:	47                   	rex.RXB
 261:	4e 55                	rex.WRX push %rbp
 263:	00 00                	add    %al,(%rax)
 265:	00 00                	add    %al,(%rax)
 267:	00 03                	add    %al,(%rbx)
 269:	00 00                	add    %al,(%rax)
 26b:	00 02                	add    %al,(%rdx)
 26d:	00 00                	add    %al,(%rax)
 26f:	00 00                	add    %al,(%rax)
 271:	00 00                	add    %al,(%rax)
	...

Disassembly of section .note.gnu.build-id:

0000000000000274 <.note.gnu.build-id>:
 274:	04 00                	add    $0x0,%al
 276:	00 00                	add    %al,(%rax)
 278:	14 00                	adc    $0x0,%al
 27a:	00 00                	add    %al,(%rax)
 27c:	03 00                	add    (%rax),%eax
 27e:	00 00                	add    %al,(%rax)
 280:	47                   	rex.RXB
 281:	4e 55                	rex.WRX push %rbp
 283:	00 45 3e             	add    %al,0x3e(%rbp)
 286:	60                   	(bad)  
 287:	47 80 8f e1 28 15 a1 	rex.RXB orb $0xc5,-0x5eead71f(%r15)
 28e:	c5 
 28f:	09 f0                	or     %esi,%eax
 291:	79 fe                	jns    291 <_init-0x25f>
 293:	5a                   	pop    %rdx
 294:	0c e6                	or     $0xe6,%al
 296:	b8                   	.byte 0xb8
 297:	29                   	.byte 0x29

Disassembly of section .gnu.hash:

0000000000000298 <.gnu.hash>:
 298:	01 00                	add    %eax,(%rax)
 29a:	00 00                	add    %al,(%rax)
 29c:	01 00                	add    %eax,(%rax)
 29e:	00 00                	add    %al,(%rax)
 2a0:	01 00                	add    %eax,(%rax)
	...

Disassembly of section .dynsym:

00000000000002b8 <.dynsym>:
	...
 2d0:	3f                   	(bad)  
 2d1:	00 00                	add    %al,(%rax)
 2d3:	00 20                	add    %ah,(%rax)
	...
 2e5:	00 00                	add    %al,(%rax)
 2e7:	00 0b                	add    %cl,(%rbx)
 2e9:	00 00                	add    %al,(%rax)
 2eb:	00 12                	add    %dl,(%rdx)
	...
 2fd:	00 00                	add    %al,(%rax)
 2ff:	00 21                	add    %ah,(%rcx)
 301:	00 00                	add    %al,(%rax)
 303:	00 12                	add    %dl,(%rdx)
	...
 315:	00 00                	add    %al,(%rax)
 317:	00 5b 00             	add    %bl,0x0(%rbx)
 31a:	00 00                	add    %al,(%rax)
 31c:	20 00                	and    %al,(%rax)
	...
 32e:	00 00                	add    %al,(%rax)
 330:	6a 00                	pushq  $0x0
 332:	00 00                	add    %al,(%rax)
 334:	20 00                	and    %al,(%rax)
	...
 346:	00 00                	add    %al,(%rax)
 348:	12 00                	adc    (%rax),%al
 34a:	00 00                	add    %al,(%rax)
 34c:	22 00                	and    (%rax),%al
	...

Disassembly of section .dynstr:

0000000000000360 <.dynstr>:
 360:	00 6c 69 62          	add    %ch,0x62(%rcx,%rbp,2)
 364:	63 2e                	movslq (%rsi),%ebp
 366:	73 6f                	jae    3d7 <_init-0x119>
 368:	2e 36 00 70 72       	cs add %dh,%ss:0x72(%rax)
 36d:	69 6e 74 66 00 5f 5f 	imul   $0x5f5f0066,0x74(%rsi),%ebp
 374:	63 78 61             	movslq 0x61(%rax),%edi
 377:	5f                   	pop    %rdi
 378:	66 69 6e 61 6c 69    	imul   $0x696c,0x61(%rsi),%bp
 37e:	7a 65                	jp     3e5 <_init-0x10b>
 380:	00 5f 5f             	add    %bl,0x5f(%rdi)
 383:	6c                   	insb   (%dx),%es:(%rdi)
 384:	69 62 63 5f 73 74 61 	imul   $0x6174735f,0x63(%rdx),%esp
 38b:	72 74                	jb     401 <_init-0xef>
 38d:	5f                   	pop    %rdi
 38e:	6d                   	insl   (%dx),%es:(%rdi)
 38f:	61                   	(bad)  
 390:	69 6e 00 47 4c 49 42 	imul   $0x42494c47,0x0(%rsi),%ebp
 397:	43 5f                	rex.XB pop %r15
 399:	32 2e                	xor    (%rsi),%ch
 39b:	32 2e                	xor    (%rsi),%ch
 39d:	35 00 5f 49 54       	xor    $0x54495f00,%eax
 3a2:	4d 5f                	rex.WRB pop %r15
 3a4:	64 65 72 65          	fs gs jb 40d <_init-0xe3>
 3a8:	67 69 73 74 65 72 54 	imul   $0x4d547265,0x74(%ebx),%esi
 3af:	4d 
 3b0:	43 6c                	rex.XB insb (%dx),%es:(%rdi)
 3b2:	6f                   	outsl  %ds:(%rsi),(%dx)
 3b3:	6e                   	outsb  %ds:(%rsi),(%dx)
 3b4:	65 54                	gs push %rsp
 3b6:	61                   	(bad)  
 3b7:	62                   	(bad)  
 3b8:	6c                   	insb   (%dx),%es:(%rdi)
 3b9:	65 00 5f 5f          	add    %bl,%gs:0x5f(%rdi)
 3bd:	67 6d                	insl   (%dx),%es:(%edi)
 3bf:	6f                   	outsl  %ds:(%rsi),(%dx)
 3c0:	6e                   	outsb  %ds:(%rsi),(%dx)
 3c1:	5f                   	pop    %rdi
 3c2:	73 74                	jae    438 <_init-0xb8>
 3c4:	61                   	(bad)  
 3c5:	72 74                	jb     43b <_init-0xb5>
 3c7:	5f                   	pop    %rdi
 3c8:	5f                   	pop    %rdi
 3c9:	00 5f 49             	add    %bl,0x49(%rdi)
 3cc:	54                   	push   %rsp
 3cd:	4d 5f                	rex.WRB pop %r15
 3cf:	72 65                	jb     436 <_init-0xba>
 3d1:	67 69 73 74 65 72 54 	imul   $0x4d547265,0x74(%ebx),%esi
 3d8:	4d 
 3d9:	43 6c                	rex.XB insb (%dx),%es:(%rdi)
 3db:	6f                   	outsl  %ds:(%rsi),(%dx)
 3dc:	6e                   	outsb  %ds:(%rsi),(%dx)
 3dd:	65 54                	gs push %rsp
 3df:	61                   	(bad)  
 3e0:	62                   	.byte 0x62
 3e1:	6c                   	insb   (%dx),%es:(%rdi)
 3e2:	65                   	gs
	...

Disassembly of section .gnu.version:

00000000000003e4 <.gnu.version>:
 3e4:	00 00                	add    %al,(%rax)
 3e6:	00 00                	add    %al,(%rax)
 3e8:	02 00                	add    (%rax),%al
 3ea:	02 00                	add    (%rax),%al
 3ec:	00 00                	add    %al,(%rax)
 3ee:	00 00                	add    %al,(%rax)
 3f0:	02 00                	add    (%rax),%al

Disassembly of section .gnu.version_r:

00000000000003f8 <.gnu.version_r>:
 3f8:	01 00                	add    %eax,(%rax)
 3fa:	01 00                	add    %eax,(%rax)
 3fc:	01 00                	add    %eax,(%rax)
 3fe:	00 00                	add    %al,(%rax)
 400:	10 00                	adc    %al,(%rax)
 402:	00 00                	add    %al,(%rax)
 404:	00 00                	add    %al,(%rax)
 406:	00 00                	add    %al,(%rax)
 408:	75 1a                	jne    424 <_init-0xcc>
 40a:	69 09 00 00 02 00    	imul   $0x20000,(%rcx),%ecx
 410:	33 00                	xor    (%rax),%eax
 412:	00 00                	add    %al,(%rax)
 414:	00 00                	add    %al,(%rax)
	...

Disassembly of section .rela.dyn:

0000000000000418 <.rela.dyn>:
 418:	b8 0d 20 00 00       	mov    $0x200d,%eax
 41d:	00 00                	add    %al,(%rax)
 41f:	00 08                	add    %cl,(%rax)
 421:	00 00                	add    %al,(%rax)
 423:	00 00                	add    %al,(%rax)
 425:	00 00                	add    %al,(%rax)
 427:	00 40 06             	add    %al,0x6(%rax)
 42a:	00 00                	add    %al,(%rax)
 42c:	00 00                	add    %al,(%rax)
 42e:	00 00                	add    %al,(%rax)
 430:	c0 0d 20 00 00 00 00 	rorb   $0x0,0x20(%rip)        # 457 <_init-0x99>
 437:	00 08                	add    %cl,(%rax)
	...
 441:	06                   	(bad)  
 442:	00 00                	add    %al,(%rax)
 444:	00 00                	add    %al,(%rax)
 446:	00 00                	add    %al,(%rax)
 448:	08 10                	or     %dl,(%rax)
 44a:	20 00                	and    %al,(%rax)
 44c:	00 00                	add    %al,(%rax)
 44e:	00 00                	add    %al,(%rax)
 450:	08 00                	or     %al,(%rax)
 452:	00 00                	add    %al,(%rax)
 454:	00 00                	add    %al,(%rax)
 456:	00 00                	add    %al,(%rax)
 458:	08 10                	or     %dl,(%rax)
 45a:	20 00                	and    %al,(%rax)
 45c:	00 00                	add    %al,(%rax)
 45e:	00 00                	add    %al,(%rax)
 460:	d8 0f                	fmuls  (%rdi)
 462:	20 00                	and    %al,(%rax)
 464:	00 00                	add    %al,(%rax)
 466:	00 00                	add    %al,(%rax)
 468:	06                   	(bad)  
 469:	00 00                	add    %al,(%rax)
 46b:	00 01                	add    %al,(%rcx)
	...
 475:	00 00                	add    %al,(%rax)
 477:	00 e0                	add    %ah,%al
 479:	0f 20 00             	mov    %cr0,%rax
 47c:	00 00                	add    %al,(%rax)
 47e:	00 00                	add    %al,(%rax)
 480:	06                   	(bad)  
 481:	00 00                	add    %al,(%rax)
 483:	00 03                	add    %al,(%rbx)
	...
 48d:	00 00                	add    %al,(%rax)
 48f:	00 e8                	add    %ch,%al
 491:	0f 20 00             	mov    %cr0,%rax
 494:	00 00                	add    %al,(%rax)
 496:	00 00                	add    %al,(%rax)
 498:	06                   	(bad)  
 499:	00 00                	add    %al,(%rax)
 49b:	00 04 00             	add    %al,(%rax,%rax,1)
	...
 4a6:	00 00                	add    %al,(%rax)
 4a8:	f0 0f 20 00          	lock mov %cr0,%rax
 4ac:	00 00                	add    %al,(%rax)
 4ae:	00 00                	add    %al,(%rax)
 4b0:	06                   	(bad)  
 4b1:	00 00                	add    %al,(%rax)
 4b3:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # 4b9 <_init-0x37>
 4b9:	00 00                	add    %al,(%rax)
 4bb:	00 00                	add    %al,(%rax)
 4bd:	00 00                	add    %al,(%rax)
 4bf:	00 f8                	add    %bh,%al
 4c1:	0f 20 00             	mov    %cr0,%rax
 4c4:	00 00                	add    %al,(%rax)
 4c6:	00 00                	add    %al,(%rax)
 4c8:	06                   	(bad)  
 4c9:	00 00                	add    %al,(%rax)
 4cb:	00 06                	add    %al,(%rsi)
	...

Disassembly of section .rela.plt:

00000000000004d8 <.rela.plt>:
 4d8:	d0 0f                	rorb   (%rdi)
 4da:	20 00                	and    %al,(%rax)
 4dc:	00 00                	add    %al,(%rax)
 4de:	00 00                	add    %al,(%rax)
 4e0:	07                   	(bad)  
 4e1:	00 00                	add    %al,(%rax)
 4e3:	00 02                	add    %al,(%rdx)
	...

Disassembly of section .init:

00000000000004f0 <_init>:
 4f0:	48 83 ec 08          	sub    $0x8,%rsp
 4f4:	48 8b 05 ed 0a 20 00 	mov    0x200aed(%rip),%rax        # 200fe8 <__gmon_start__>
 4fb:	48 85 c0             	test   %rax,%rax
 4fe:	74 02                	je     502 <_init+0x12>
 500:	ff d0                	callq  *%rax
 502:	48 83 c4 08          	add    $0x8,%rsp
 506:	c3                   	retq   

Disassembly of section .plt:

0000000000000510 <.plt>:
 510:	ff 35 aa 0a 20 00    	pushq  0x200aaa(%rip)        # 200fc0 <_GLOBAL_OFFSET_TABLE_+0x8>
 516:	ff 25 ac 0a 20 00    	jmpq   *0x200aac(%rip)        # 200fc8 <_GLOBAL_OFFSET_TABLE_+0x10>
 51c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000520 <printf@plt>:
 520:	ff 25 aa 0a 20 00    	jmpq   *0x200aaa(%rip)        # 200fd0 <printf@GLIBC_2.2.5>
 526:	68 00 00 00 00       	pushq  $0x0
 52b:	e9 e0 ff ff ff       	jmpq   510 <.plt>

Disassembly of section .plt.got:

0000000000000530 <__cxa_finalize@plt>:
 530:	ff 25 c2 0a 20 00    	jmpq   *0x200ac2(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 536:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000540 <_start>:
 540:	31 ed                	xor    %ebp,%ebp
 542:	49 89 d1             	mov    %rdx,%r9
 545:	5e                   	pop    %rsi
 546:	48 89 e2             	mov    %rsp,%rdx
 549:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 54d:	50                   	push   %rax
 54e:	54                   	push   %rsp
 54f:	4c 8d 05 ca 01 00 00 	lea    0x1ca(%rip),%r8        # 720 <__libc_csu_fini>
 556:	48 8d 0d 53 01 00 00 	lea    0x153(%rip),%rcx        # 6b0 <__libc_csu_init>
 55d:	48 8d 3d 0a 01 00 00 	lea    0x10a(%rip),%rdi        # 66e <main>
 564:	ff 15 76 0a 20 00    	callq  *0x200a76(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 56a:	f4                   	hlt    
 56b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000570 <deregister_tm_clones>:
 570:	48 8d 3d a1 0a 20 00 	lea    0x200aa1(%rip),%rdi        # 201018 <__TMC_END__>
 577:	55                   	push   %rbp
 578:	48 8d 05 99 0a 20 00 	lea    0x200a99(%rip),%rax        # 201018 <__TMC_END__>
 57f:	48 39 f8             	cmp    %rdi,%rax
 582:	48 89 e5             	mov    %rsp,%rbp
 585:	74 19                	je     5a0 <deregister_tm_clones+0x30>
 587:	48 8b 05 4a 0a 20 00 	mov    0x200a4a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 58e:	48 85 c0             	test   %rax,%rax
 591:	74 0d                	je     5a0 <deregister_tm_clones+0x30>
 593:	5d                   	pop    %rbp
 594:	ff e0                	jmpq   *%rax
 596:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 59d:	00 00 00 
 5a0:	5d                   	pop    %rbp
 5a1:	c3                   	retq   
 5a2:	0f 1f 40 00          	nopl   0x0(%rax)
 5a6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 5ad:	00 00 00 

00000000000005b0 <register_tm_clones>:
 5b0:	48 8d 3d 61 0a 20 00 	lea    0x200a61(%rip),%rdi        # 201018 <__TMC_END__>
 5b7:	48 8d 35 5a 0a 20 00 	lea    0x200a5a(%rip),%rsi        # 201018 <__TMC_END__>
 5be:	55                   	push   %rbp
 5bf:	48 29 fe             	sub    %rdi,%rsi
 5c2:	48 89 e5             	mov    %rsp,%rbp
 5c5:	48 c1 fe 03          	sar    $0x3,%rsi
 5c9:	48 89 f0             	mov    %rsi,%rax
 5cc:	48 c1 e8 3f          	shr    $0x3f,%rax
 5d0:	48 01 c6             	add    %rax,%rsi
 5d3:	48 d1 fe             	sar    %rsi
 5d6:	74 18                	je     5f0 <register_tm_clones+0x40>
 5d8:	48 8b 05 11 0a 20 00 	mov    0x200a11(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 5df:	48 85 c0             	test   %rax,%rax
 5e2:	74 0c                	je     5f0 <register_tm_clones+0x40>
 5e4:	5d                   	pop    %rbp
 5e5:	ff e0                	jmpq   *%rax
 5e7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 5ee:	00 00 
 5f0:	5d                   	pop    %rbp
 5f1:	c3                   	retq   
 5f2:	0f 1f 40 00          	nopl   0x0(%rax)
 5f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 5fd:	00 00 00 

0000000000000600 <__do_global_dtors_aux>:
 600:	80 3d 11 0a 20 00 00 	cmpb   $0x0,0x200a11(%rip)        # 201018 <__TMC_END__>
 607:	75 2f                	jne    638 <__do_global_dtors_aux+0x38>
 609:	48 83 3d e7 09 20 00 	cmpq   $0x0,0x2009e7(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 610:	00 
 611:	55                   	push   %rbp
 612:	48 89 e5             	mov    %rsp,%rbp
 615:	74 0c                	je     623 <__do_global_dtors_aux+0x23>
 617:	48 8b 3d ea 09 20 00 	mov    0x2009ea(%rip),%rdi        # 201008 <__dso_handle>
 61e:	e8 0d ff ff ff       	callq  530 <__cxa_finalize@plt>
 623:	e8 48 ff ff ff       	callq  570 <deregister_tm_clones>
 628:	c6 05 e9 09 20 00 01 	movb   $0x1,0x2009e9(%rip)        # 201018 <__TMC_END__>
 62f:	5d                   	pop    %rbp
 630:	c3                   	retq   
 631:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 638:	f3 c3                	repz retq 
 63a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000640 <frame_dummy>:
 640:	55                   	push   %rbp
 641:	48 89 e5             	mov    %rsp,%rbp
 644:	5d                   	pop    %rbp
 645:	e9 66 ff ff ff       	jmpq   5b0 <register_tm_clones>

000000000000064a <func1>:
 64a:	55                   	push   %rbp
 64b:	48 89 e5             	mov    %rsp,%rbp
 64e:	48 83 ec 10          	sub    $0x10,%rsp
 652:	89 7d fc             	mov    %edi,-0x4(%rbp)
 655:	8b 45 fc             	mov    -0x4(%rbp),%eax
 658:	89 c6                	mov    %eax,%esi
 65a:	48 8d 3d d3 00 00 00 	lea    0xd3(%rip),%rdi        # 734 <_IO_stdin_used+0x4>
 661:	b8 00 00 00 00       	mov    $0x0,%eax
 666:	e8 b5 fe ff ff       	callq  520 <printf@plt>
 66b:	90                   	nop
 66c:	c9                   	leaveq 
 66d:	c3                   	retq   

000000000000066e <main>:
 66e:	55                   	push   %rbp
 66f:	48 89 e5             	mov    %rsp,%rbp
 672:	48 83 ec 20          	sub    $0x20,%rsp
 676:	89 7d ec             	mov    %edi,-0x14(%rbp)
 679:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
 67d:	c7 45 f8 01 00 00 00 	movl   $0x1,-0x8(%rbp)
 684:	8b 15 8a 09 20 00    	mov    0x20098a(%rip),%edx        # 201014 <static_var.1803>
 68a:	8b 05 8c 09 20 00    	mov    0x20098c(%rip),%eax        # 20101c <static_var2.1804>
 690:	01 c2                	add    %eax,%edx
 692:	8b 45 f8             	mov    -0x8(%rbp),%eax
 695:	01 c2                	add    %eax,%edx
 697:	8b 45 fc             	mov    -0x4(%rbp),%eax
 69a:	01 d0                	add    %edx,%eax
 69c:	89 c7                	mov    %eax,%edi
 69e:	e8 a7 ff ff ff       	callq  64a <func1>
 6a3:	8b 45 f8             	mov    -0x8(%rbp),%eax
 6a6:	c9                   	leaveq 
 6a7:	c3                   	retq   
 6a8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 6af:	00 

00000000000006b0 <__libc_csu_init>:
 6b0:	41 57                	push   %r15
 6b2:	41 56                	push   %r14
 6b4:	49 89 d7             	mov    %rdx,%r15
 6b7:	41 55                	push   %r13
 6b9:	41 54                	push   %r12
 6bb:	4c 8d 25 f6 06 20 00 	lea    0x2006f6(%rip),%r12        # 200db8 <__frame_dummy_init_array_entry>
 6c2:	55                   	push   %rbp
 6c3:	48 8d 2d f6 06 20 00 	lea    0x2006f6(%rip),%rbp        # 200dc0 <__init_array_end>
 6ca:	53                   	push   %rbx
 6cb:	41 89 fd             	mov    %edi,%r13d
 6ce:	49 89 f6             	mov    %rsi,%r14
 6d1:	4c 29 e5             	sub    %r12,%rbp
 6d4:	48 83 ec 08          	sub    $0x8,%rsp
 6d8:	48 c1 fd 03          	sar    $0x3,%rbp
 6dc:	e8 0f fe ff ff       	callq  4f0 <_init>
 6e1:	48 85 ed             	test   %rbp,%rbp
 6e4:	74 20                	je     706 <__libc_csu_init+0x56>
 6e6:	31 db                	xor    %ebx,%ebx
 6e8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 6ef:	00 
 6f0:	4c 89 fa             	mov    %r15,%rdx
 6f3:	4c 89 f6             	mov    %r14,%rsi
 6f6:	44 89 ef             	mov    %r13d,%edi
 6f9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 6fd:	48 83 c3 01          	add    $0x1,%rbx
 701:	48 39 dd             	cmp    %rbx,%rbp
 704:	75 ea                	jne    6f0 <__libc_csu_init+0x40>
 706:	48 83 c4 08          	add    $0x8,%rsp
 70a:	5b                   	pop    %rbx
 70b:	5d                   	pop    %rbp
 70c:	41 5c                	pop    %r12
 70e:	41 5d                	pop    %r13
 710:	41 5e                	pop    %r14
 712:	41 5f                	pop    %r15
 714:	c3                   	retq   
 715:	90                   	nop
 716:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 71d:	00 00 00 

0000000000000720 <__libc_csu_fini>:
 720:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000724 <_fini>:
 724:	48 83 ec 08          	sub    $0x8,%rsp
 728:	48 83 c4 08          	add    $0x8,%rsp
 72c:	c3                   	retq   

Disassembly of section .rodata:

0000000000000730 <_IO_stdin_used>:
 730:	01 00                	add    %eax,(%rax)
 732:	02 00                	add    (%rax),%al
 734:	25                   	.byte 0x25
 735:	64 0a 00             	or     %fs:(%rax),%al

Disassembly of section .eh_frame_hdr:

0000000000000738 <__GNU_EH_FRAME_HDR>:
 738:	01 1b                	add    %ebx,(%rbx)
 73a:	03 3b                	add    (%rbx),%edi
 73c:	44 00 00             	add    %r8b,(%rax)
 73f:	00 07                	add    %al,(%rdi)
 741:	00 00                	add    %al,(%rax)
 743:	00 d8                	add    %bl,%al
 745:	fd                   	std    
 746:	ff                   	(bad)  
 747:	ff 90 00 00 00 f8    	callq  *-0x8000000(%rax)
 74d:	fd                   	std    
 74e:	ff                   	(bad)  
 74f:	ff                   	(bad)  
 750:	b8 00 00 00 08       	mov    $0x8000000,%eax
 755:	fe                   	(bad)  
 756:	ff                   	(bad)  
 757:	ff 60 00             	jmpq   *0x0(%rax)
 75a:	00 00                	add    %al,(%rax)
 75c:	12 ff                	adc    %bh,%bh
 75e:	ff                   	(bad)  
 75f:	ff d0                	callq  *%rax
 761:	00 00                	add    %al,(%rax)
 763:	00 36                	add    %dh,(%rsi)
 765:	ff                   	(bad)  
 766:	ff                   	(bad)  
 767:	ff f0                	push   %rax
 769:	00 00                	add    %al,(%rax)
 76b:	00 78 ff             	add    %bh,-0x1(%rax)
 76e:	ff                   	(bad)  
 76f:	ff 10                	callq  *(%rax)
 771:	01 00                	add    %eax,(%rax)
 773:	00 e8                	add    %ch,%al
 775:	ff                   	(bad)  
 776:	ff                   	(bad)  
 777:	ff 58 01             	lcall  *0x1(%rax)
	...

Disassembly of section .eh_frame:

0000000000000780 <__FRAME_END__-0x124>:
 780:	14 00                	adc    $0x0,%al
 782:	00 00                	add    %al,(%rax)
 784:	00 00                	add    %al,(%rax)
 786:	00 00                	add    %al,(%rax)
 788:	01 7a 52             	add    %edi,0x52(%rdx)
 78b:	00 01                	add    %al,(%rcx)
 78d:	78 10                	js     79f <__GNU_EH_FRAME_HDR+0x67>
 78f:	01 1b                	add    %ebx,(%rbx)
 791:	0c 07                	or     $0x7,%al
 793:	08 90 01 07 10 14    	or     %dl,0x14100701(%rax)
 799:	00 00                	add    %al,(%rax)
 79b:	00 1c 00             	add    %bl,(%rax,%rax,1)
 79e:	00 00                	add    %al,(%rax)
 7a0:	a0 fd ff ff 2b 00 00 	movabs 0x2bfffffd,%al
 7a7:	00 00 
 7a9:	00 00                	add    %al,(%rax)
 7ab:	00 00                	add    %al,(%rax)
 7ad:	00 00                	add    %al,(%rax)
 7af:	00 14 00             	add    %dl,(%rax,%rax,1)
 7b2:	00 00                	add    %al,(%rax)
 7b4:	00 00                	add    %al,(%rax)
 7b6:	00 00                	add    %al,(%rax)
 7b8:	01 7a 52             	add    %edi,0x52(%rdx)
 7bb:	00 01                	add    %al,(%rcx)
 7bd:	78 10                	js     7cf <__GNU_EH_FRAME_HDR+0x97>
 7bf:	01 1b                	add    %ebx,(%rbx)
 7c1:	0c 07                	or     $0x7,%al
 7c3:	08 90 01 00 00 24    	or     %dl,0x24000001(%rax)
 7c9:	00 00                	add    %al,(%rax)
 7cb:	00 1c 00             	add    %bl,(%rax,%rax,1)
 7ce:	00 00                	add    %al,(%rax)
 7d0:	40 fd                	rex std 
 7d2:	ff                   	(bad)  
 7d3:	ff 20                	jmpq   *(%rax)
 7d5:	00 00                	add    %al,(%rax)
 7d7:	00 00                	add    %al,(%rax)
 7d9:	0e                   	(bad)  
 7da:	10 46 0e             	adc    %al,0xe(%rsi)
 7dd:	18 4a 0f             	sbb    %cl,0xf(%rdx)
 7e0:	0b 77 08             	or     0x8(%rdi),%esi
 7e3:	80 00 3f             	addb   $0x3f,(%rax)
 7e6:	1a 3b                	sbb    (%rbx),%bh
 7e8:	2a 33                	sub    (%rbx),%dh
 7ea:	24 22                	and    $0x22,%al
 7ec:	00 00                	add    %al,(%rax)
 7ee:	00 00                	add    %al,(%rax)
 7f0:	14 00                	adc    $0x0,%al
 7f2:	00 00                	add    %al,(%rax)
 7f4:	44 00 00             	add    %r8b,(%rax)
 7f7:	00 38                	add    %bh,(%rax)
 7f9:	fd                   	std    
 7fa:	ff                   	(bad)  
 7fb:	ff 08                	decl   (%rax)
	...
 805:	00 00                	add    %al,(%rax)
 807:	00 1c 00             	add    %bl,(%rax,%rax,1)
 80a:	00 00                	add    %al,(%rax)
 80c:	5c                   	pop    %rsp
 80d:	00 00                	add    %al,(%rax)
 80f:	00 3a                	add    %bh,(%rdx)
 811:	fe                   	(bad)  
 812:	ff                   	(bad)  
 813:	ff 24 00             	jmpq   *(%rax,%rax,1)
 816:	00 00                	add    %al,(%rax)
 818:	00 41 0e             	add    %al,0xe(%rcx)
 81b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
 821:	5f                   	pop    %rdi
 822:	0c 07                	or     $0x7,%al
 824:	08 00                	or     %al,(%rax)
 826:	00 00                	add    %al,(%rax)
 828:	1c 00                	sbb    $0x0,%al
 82a:	00 00                	add    %al,(%rax)
 82c:	7c 00                	jl     82e <__GNU_EH_FRAME_HDR+0xf6>
 82e:	00 00                	add    %al,(%rax)
 830:	3e fe                	ds (bad) 
 832:	ff                   	(bad)  
 833:	ff                   	(bad)  
 834:	3a 00                	cmp    (%rax),%al
 836:	00 00                	add    %al,(%rax)
 838:	00 41 0e             	add    %al,0xe(%rcx)
 83b:	10 86 02 43 0d 06    	adc    %al,0x60d4302(%rsi)
 841:	75 0c                	jne    84f <__GNU_EH_FRAME_HDR+0x117>
 843:	07                   	(bad)  
 844:	08 00                	or     %al,(%rax)
 846:	00 00                	add    %al,(%rax)
 848:	44 00 00             	add    %r8b,(%rax)
 84b:	00 9c 00 00 00 60 fe 	add    %bl,-0x1a00000(%rax,%rax,1)
 852:	ff                   	(bad)  
 853:	ff 65 00             	jmpq   *0x0(%rbp)
 856:	00 00                	add    %al,(%rax)
 858:	00 42 0e             	add    %al,0xe(%rdx)
 85b:	10 8f 02 42 0e 18    	adc    %cl,0x180e4202(%rdi)
 861:	8e 03                	mov    (%rbx),%es
 863:	45 0e                	rex.RB (bad) 
 865:	20 8d 04 42 0e 28    	and    %cl,0x280e4204(%rbp)
 86b:	8c 05 48 0e 30 86    	mov    %es,-0x79cff1b8(%rip)        # ffffffff863016b9 <_end+0xffffffff86100691>
 871:	06                   	(bad)  
 872:	48 0e                	rex.W (bad) 
 874:	38 83 07 4d 0e 40    	cmp    %al,0x400e4d07(%rbx)
 87a:	72 0e                	jb     88a <__GNU_EH_FRAME_HDR+0x152>
 87c:	38 41 0e             	cmp    %al,0xe(%rcx)
 87f:	30 41 0e             	xor    %al,0xe(%rcx)
 882:	28 42 0e             	sub    %al,0xe(%rdx)
 885:	20 42 0e             	and    %al,0xe(%rdx)
 888:	18 42 0e             	sbb    %al,0xe(%rdx)
 88b:	10 42 0e             	adc    %al,0xe(%rdx)
 88e:	08 00                	or     %al,(%rax)
 890:	10 00                	adc    %al,(%rax)
 892:	00 00                	add    %al,(%rax)
 894:	e4 00                	in     $0x0,%al
 896:	00 00                	add    %al,(%rax)
 898:	88 fe                	mov    %bh,%dh
 89a:	ff                   	(bad)  
 89b:	ff 02                	incl   (%rdx)
 89d:	00 00                	add    %al,(%rax)
 89f:	00 00                	add    %al,(%rax)
 8a1:	00 00                	add    %al,(%rax)
	...

00000000000008a4 <__FRAME_END__>:
 8a4:	00 00                	add    %al,(%rax)
	...

Disassembly of section .init_array:

0000000000200db8 <__frame_dummy_init_array_entry>:
  200db8:	40 06                	rex (bad) 
  200dba:	00 00                	add    %al,(%rax)
  200dbc:	00 00                	add    %al,(%rax)
	...

Disassembly of section .fini_array:

0000000000200dc0 <__do_global_dtors_aux_fini_array_entry>:
  200dc0:	00 06                	add    %al,(%rsi)
  200dc2:	00 00                	add    %al,(%rax)
  200dc4:	00 00                	add    %al,(%rax)
	...

Disassembly of section .dynamic:

0000000000200dc8 <_DYNAMIC>:
  200dc8:	01 00                	add    %eax,(%rax)
  200dca:	00 00                	add    %al,(%rax)
  200dcc:	00 00                	add    %al,(%rax)
  200dce:	00 00                	add    %al,(%rax)
  200dd0:	01 00                	add    %eax,(%rax)
  200dd2:	00 00                	add    %al,(%rax)
  200dd4:	00 00                	add    %al,(%rax)
  200dd6:	00 00                	add    %al,(%rax)
  200dd8:	0c 00                	or     $0x0,%al
  200dda:	00 00                	add    %al,(%rax)
  200ddc:	00 00                	add    %al,(%rax)
  200dde:	00 00                	add    %al,(%rax)
  200de0:	f0 04 00             	lock add $0x0,%al
  200de3:	00 00                	add    %al,(%rax)
  200de5:	00 00                	add    %al,(%rax)
  200de7:	00 0d 00 00 00 00    	add    %cl,0x0(%rip)        # 200ded <_DYNAMIC+0x25>
  200ded:	00 00                	add    %al,(%rax)
  200def:	00 24 07             	add    %ah,(%rdi,%rax,1)
  200df2:	00 00                	add    %al,(%rax)
  200df4:	00 00                	add    %al,(%rax)
  200df6:	00 00                	add    %al,(%rax)
  200df8:	19 00                	sbb    %eax,(%rax)
  200dfa:	00 00                	add    %al,(%rax)
  200dfc:	00 00                	add    %al,(%rax)
  200dfe:	00 00                	add    %al,(%rax)
  200e00:	b8 0d 20 00 00       	mov    $0x200d,%eax
  200e05:	00 00                	add    %al,(%rax)
  200e07:	00 1b                	add    %bl,(%rbx)
  200e09:	00 00                	add    %al,(%rax)
  200e0b:	00 00                	add    %al,(%rax)
  200e0d:	00 00                	add    %al,(%rax)
  200e0f:	00 08                	add    %cl,(%rax)
  200e11:	00 00                	add    %al,(%rax)
  200e13:	00 00                	add    %al,(%rax)
  200e15:	00 00                	add    %al,(%rax)
  200e17:	00 1a                	add    %bl,(%rdx)
  200e19:	00 00                	add    %al,(%rax)
  200e1b:	00 00                	add    %al,(%rax)
  200e1d:	00 00                	add    %al,(%rax)
  200e1f:	00 c0                	add    %al,%al
  200e21:	0d 20 00 00 00       	or     $0x20,%eax
  200e26:	00 00                	add    %al,(%rax)
  200e28:	1c 00                	sbb    $0x0,%al
  200e2a:	00 00                	add    %al,(%rax)
  200e2c:	00 00                	add    %al,(%rax)
  200e2e:	00 00                	add    %al,(%rax)
  200e30:	08 00                	or     %al,(%rax)
  200e32:	00 00                	add    %al,(%rax)
  200e34:	00 00                	add    %al,(%rax)
  200e36:	00 00                	add    %al,(%rax)
  200e38:	f5                   	cmc    
  200e39:	fe                   	(bad)  
  200e3a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200e3d:	00 00                	add    %al,(%rax)
  200e3f:	00 98 02 00 00 00    	add    %bl,0x2(%rax)
  200e45:	00 00                	add    %al,(%rax)
  200e47:	00 05 00 00 00 00    	add    %al,0x0(%rip)        # 200e4d <_DYNAMIC+0x85>
  200e4d:	00 00                	add    %al,(%rax)
  200e4f:	00 60 03             	add    %ah,0x3(%rax)
  200e52:	00 00                	add    %al,(%rax)
  200e54:	00 00                	add    %al,(%rax)
  200e56:	00 00                	add    %al,(%rax)
  200e58:	06                   	(bad)  
  200e59:	00 00                	add    %al,(%rax)
  200e5b:	00 00                	add    %al,(%rax)
  200e5d:	00 00                	add    %al,(%rax)
  200e5f:	00 b8 02 00 00 00    	add    %bh,0x2(%rax)
  200e65:	00 00                	add    %al,(%rax)
  200e67:	00 0a                	add    %cl,(%rdx)
  200e69:	00 00                	add    %al,(%rax)
  200e6b:	00 00                	add    %al,(%rax)
  200e6d:	00 00                	add    %al,(%rax)
  200e6f:	00 84 00 00 00 00 00 	add    %al,0x0(%rax,%rax,1)
  200e76:	00 00                	add    %al,(%rax)
  200e78:	0b 00                	or     (%rax),%eax
  200e7a:	00 00                	add    %al,(%rax)
  200e7c:	00 00                	add    %al,(%rax)
  200e7e:	00 00                	add    %al,(%rax)
  200e80:	18 00                	sbb    %al,(%rax)
  200e82:	00 00                	add    %al,(%rax)
  200e84:	00 00                	add    %al,(%rax)
  200e86:	00 00                	add    %al,(%rax)
  200e88:	15 00 00 00 00       	adc    $0x0,%eax
	...
  200e95:	00 00                	add    %al,(%rax)
  200e97:	00 03                	add    %al,(%rbx)
  200e99:	00 00                	add    %al,(%rax)
  200e9b:	00 00                	add    %al,(%rax)
  200e9d:	00 00                	add    %al,(%rax)
  200e9f:	00 b8 0f 20 00 00    	add    %bh,0x200f(%rax)
  200ea5:	00 00                	add    %al,(%rax)
  200ea7:	00 02                	add    %al,(%rdx)
  200ea9:	00 00                	add    %al,(%rax)
  200eab:	00 00                	add    %al,(%rax)
  200ead:	00 00                	add    %al,(%rax)
  200eaf:	00 18                	add    %bl,(%rax)
  200eb1:	00 00                	add    %al,(%rax)
  200eb3:	00 00                	add    %al,(%rax)
  200eb5:	00 00                	add    %al,(%rax)
  200eb7:	00 14 00             	add    %dl,(%rax,%rax,1)
  200eba:	00 00                	add    %al,(%rax)
  200ebc:	00 00                	add    %al,(%rax)
  200ebe:	00 00                	add    %al,(%rax)
  200ec0:	07                   	(bad)  
  200ec1:	00 00                	add    %al,(%rax)
  200ec3:	00 00                	add    %al,(%rax)
  200ec5:	00 00                	add    %al,(%rax)
  200ec7:	00 17                	add    %dl,(%rdi)
  200ec9:	00 00                	add    %al,(%rax)
  200ecb:	00 00                	add    %al,(%rax)
  200ecd:	00 00                	add    %al,(%rax)
  200ecf:	00 d8                	add    %bl,%al
  200ed1:	04 00                	add    $0x0,%al
  200ed3:	00 00                	add    %al,(%rax)
  200ed5:	00 00                	add    %al,(%rax)
  200ed7:	00 07                	add    %al,(%rdi)
  200ed9:	00 00                	add    %al,(%rax)
  200edb:	00 00                	add    %al,(%rax)
  200edd:	00 00                	add    %al,(%rax)
  200edf:	00 18                	add    %bl,(%rax)
  200ee1:	04 00                	add    $0x0,%al
  200ee3:	00 00                	add    %al,(%rax)
  200ee5:	00 00                	add    %al,(%rax)
  200ee7:	00 08                	add    %cl,(%rax)
  200ee9:	00 00                	add    %al,(%rax)
  200eeb:	00 00                	add    %al,(%rax)
  200eed:	00 00                	add    %al,(%rax)
  200eef:	00 c0                	add    %al,%al
  200ef1:	00 00                	add    %al,(%rax)
  200ef3:	00 00                	add    %al,(%rax)
  200ef5:	00 00                	add    %al,(%rax)
  200ef7:	00 09                	add    %cl,(%rcx)
  200ef9:	00 00                	add    %al,(%rax)
  200efb:	00 00                	add    %al,(%rax)
  200efd:	00 00                	add    %al,(%rax)
  200eff:	00 18                	add    %bl,(%rax)
  200f01:	00 00                	add    %al,(%rax)
  200f03:	00 00                	add    %al,(%rax)
  200f05:	00 00                	add    %al,(%rax)
  200f07:	00 1e                	add    %bl,(%rsi)
  200f09:	00 00                	add    %al,(%rax)
  200f0b:	00 00                	add    %al,(%rax)
  200f0d:	00 00                	add    %al,(%rax)
  200f0f:	00 08                	add    %cl,(%rax)
  200f11:	00 00                	add    %al,(%rax)
  200f13:	00 00                	add    %al,(%rax)
  200f15:	00 00                	add    %al,(%rax)
  200f17:	00 fb                	add    %bh,%bl
  200f19:	ff                   	(bad)  
  200f1a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200f1d:	00 00                	add    %al,(%rax)
  200f1f:	00 01                	add    %al,(%rcx)
  200f21:	00 00                	add    %al,(%rax)
  200f23:	08 00                	or     %al,(%rax)
  200f25:	00 00                	add    %al,(%rax)
  200f27:	00 fe                	add    %bh,%dh
  200f29:	ff                   	(bad)  
  200f2a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200f2d:	00 00                	add    %al,(%rax)
  200f2f:	00 f8                	add    %bh,%al
  200f31:	03 00                	add    (%rax),%eax
  200f33:	00 00                	add    %al,(%rax)
  200f35:	00 00                	add    %al,(%rax)
  200f37:	00 ff                	add    %bh,%bh
  200f39:	ff                   	(bad)  
  200f3a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200f3d:	00 00                	add    %al,(%rax)
  200f3f:	00 01                	add    %al,(%rcx)
  200f41:	00 00                	add    %al,(%rax)
  200f43:	00 00                	add    %al,(%rax)
  200f45:	00 00                	add    %al,(%rax)
  200f47:	00 f0                	add    %dh,%al
  200f49:	ff                   	(bad)  
  200f4a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200f4d:	00 00                	add    %al,(%rax)
  200f4f:	00 e4                	add    %ah,%ah
  200f51:	03 00                	add    (%rax),%eax
  200f53:	00 00                	add    %al,(%rax)
  200f55:	00 00                	add    %al,(%rax)
  200f57:	00 f9                	add    %bh,%cl
  200f59:	ff                   	(bad)  
  200f5a:	ff 6f 00             	ljmp   *0x0(%rdi)
  200f5d:	00 00                	add    %al,(%rax)
  200f5f:	00 03                	add    %al,(%rbx)
	...

Disassembly of section .got:

0000000000200fb8 <_GLOBAL_OFFSET_TABLE_>:
  200fb8:	c8 0d 20 00          	enterq $0x200d,$0x0
	...
  200fd0:	26 05 00 00 00 00    	es add $0x0,%eax
	...

Disassembly of section .data:

0000000000201000 <__data_start>:
	...

0000000000201008 <__dso_handle>:
  201008:	08 10                	or     %dl,(%rax)
  20100a:	20 00                	and    %al,(%rax)
  20100c:	00 00                	add    %al,(%rax)
	...

0000000000201010 <global_init_var>:
  201010:	54                   	push   %rsp
  201011:	00 00                	add    %al,(%rax)
	...

0000000000201014 <static_var.1803>:
  201014:	55                   	push   %rbp
  201015:	00 00                	add    %al,(%rax)
	...

Disassembly of section .bss:

0000000000201018 <__bss_start>:
  201018:	00 00                	add    %al,(%rax)
	...

000000000020101c <static_var2.1804>:
  20101c:	00 00                	add    %al,(%rax)
	...

0000000000201020 <global_uninit_var>:
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	47                   	rex.RXB
   1:	43                   	rex.XB
   2:	43 3a 20             	rex.XB cmp (%r8),%spl
   5:	28 55 62             	sub    %dl,0x62(%rbp)
   8:	75 6e                	jne    78 <_init-0x478>
   a:	74 75                	je     81 <_init-0x46f>
   c:	20 37                	and    %dh,(%rdi)
   e:	2e 33 2e             	xor    %cs:(%rsi),%ebp
  11:	30 2d 32 37 75 62    	xor    %ch,0x62753732(%rip)        # 62753749 <_end+0x62552721>
  17:	75 6e                	jne    87 <_init-0x469>
  19:	74 75                	je     90 <_init-0x460>
  1b:	31 7e 31             	xor    %edi,0x31(%rsi)
  1e:	38 2e                	cmp    %ch,(%rsi)
  20:	30 34 29             	xor    %dh,(%rcx,%rbp,1)
  23:	20 37                	and    %dh,(%rdi)
  25:	2e 33 2e             	xor    %cs:(%rsi),%ebp
  28:	30 00                	xor    %al,(%rax)
