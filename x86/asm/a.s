
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000000570 <_init>:
 570:	48 83 ec 08          	sub    $0x8,%rsp
 574:	48 8b 05 6d 0a 20 00 	mov    0x200a6d(%rip),%rax        # 200fe8 <__gmon_start__>
 57b:	48 85 c0             	test   %rax,%rax
 57e:	74 02                	je     582 <_init+0x12>
 580:	ff d0                	callq  *%rax
 582:	48 83 c4 08          	add    $0x8,%rsp
 586:	c3                   	retq   

Disassembly of section .plt:

0000000000000590 <.plt>:
 590:	ff 35 22 0a 20 00    	pushq  0x200a22(%rip)        # 200fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
 596:	ff 25 24 0a 20 00    	jmpq   *0x200a24(%rip)        # 200fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
 59c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005a0 <__stack_chk_fail@plt>:
 5a0:	ff 25 22 0a 20 00    	jmpq   *0x200a22(%rip)        # 200fc8 <__stack_chk_fail@GLIBC_2.4>
 5a6:	68 00 00 00 00       	pushq  $0x0
 5ab:	e9 e0 ff ff ff       	jmpq   590 <.plt>

00000000000005b0 <__printf_chk@plt>:
 5b0:	ff 25 1a 0a 20 00    	jmpq   *0x200a1a(%rip)        # 200fd0 <__printf_chk@GLIBC_2.3.4>
 5b6:	68 01 00 00 00       	pushq  $0x1
 5bb:	e9 d0 ff ff ff       	jmpq   590 <.plt>

Disassembly of section .plt.got:

00000000000005c0 <__cxa_finalize@plt>:
 5c0:	ff 25 32 0a 20 00    	jmpq   *0x200a32(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 5c6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

00000000000005d0 <main>:
 5d0:	48 83 ec 18          	sub    $0x18,%rsp
 5d4:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 5db:	00 00 
 5dd:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
 5e2:	31 c0                	xor    %eax,%eax
 5e4:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%rsp)
 5eb:	00 
 5ec:	90                   	nop
 5ed:	90                   	nop
 5ee:	c7 44 24 04 07 00 00 	movl   $0x7,0x4(%rsp)
 5f5:	00 
 5f6:	90                   	nop
 5f7:	90                   	nop
 5f8:	8b 54 24 04          	mov    0x4(%rsp),%edx
 5fc:	48 8d 35 11 02 00 00 	lea    0x211(%rip),%rsi        # 814 <_IO_stdin_used+0x4>
 603:	bf 01 00 00 00       	mov    $0x1,%edi
 608:	e8 a3 ff ff ff       	callq  5b0 <__printf_chk@plt>
 60d:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
 612:	b8 03 00 00 00       	mov    $0x3,%eax
 617:	ba 04 00 00 00       	mov    $0x4,%edx
 61c:	01 c2                	add    %eax,%edx
 61e:	89 11                	mov    %edx,(%rcx)
 620:	8b 54 24 04          	mov    0x4(%rsp),%edx
 624:	48 8d 35 f5 01 00 00 	lea    0x1f5(%rip),%rsi        # 820 <_IO_stdin_used+0x10>
 62b:	31 c0                	xor    %eax,%eax
 62d:	bf 01 00 00 00       	mov    $0x1,%edi
 632:	e8 79 ff ff ff       	callq  5b0 <__printf_chk@plt>
 637:	48 8b 4c 24 08       	mov    0x8(%rsp),%rcx
 63c:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 643:	00 00 
 645:	75 07                	jne    64e <main+0x7e>
 647:	31 c0                	xor    %eax,%eax
 649:	48 83 c4 18          	add    $0x18,%rsp
 64d:	c3                   	retq   
 64e:	e8 4d ff ff ff       	callq  5a0 <__stack_chk_fail@plt>
 653:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 65a:	00 00 00 
 65d:	0f 1f 00             	nopl   (%rax)

0000000000000660 <_start>:
 660:	31 ed                	xor    %ebp,%ebp
 662:	49 89 d1             	mov    %rdx,%r9
 665:	5e                   	pop    %rsi
 666:	48 89 e2             	mov    %rsp,%rdx
 669:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 66d:	50                   	push   %rax
 66e:	54                   	push   %rsp
 66f:	4c 8d 05 8a 01 00 00 	lea    0x18a(%rip),%r8        # 800 <__libc_csu_fini>
 676:	48 8d 0d 13 01 00 00 	lea    0x113(%rip),%rcx        # 790 <__libc_csu_init>
 67d:	48 8d 3d 4c ff ff ff 	lea    -0xb4(%rip),%rdi        # 5d0 <main>
 684:	ff 15 56 09 20 00    	callq  *0x200956(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 68a:	f4                   	hlt    
 68b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000690 <deregister_tm_clones>:
 690:	48 8d 3d 79 09 20 00 	lea    0x200979(%rip),%rdi        # 201010 <__TMC_END__>
 697:	55                   	push   %rbp
 698:	48 8d 05 71 09 20 00 	lea    0x200971(%rip),%rax        # 201010 <__TMC_END__>
 69f:	48 39 f8             	cmp    %rdi,%rax
 6a2:	48 89 e5             	mov    %rsp,%rbp
 6a5:	74 19                	je     6c0 <deregister_tm_clones+0x30>
 6a7:	48 8b 05 2a 09 20 00 	mov    0x20092a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 6ae:	48 85 c0             	test   %rax,%rax
 6b1:	74 0d                	je     6c0 <deregister_tm_clones+0x30>
 6b3:	5d                   	pop    %rbp
 6b4:	ff e0                	jmpq   *%rax
 6b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6bd:	00 00 00 
 6c0:	5d                   	pop    %rbp
 6c1:	c3                   	retq   
 6c2:	0f 1f 40 00          	nopl   0x0(%rax)
 6c6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6cd:	00 00 00 

00000000000006d0 <register_tm_clones>:
 6d0:	48 8d 3d 39 09 20 00 	lea    0x200939(%rip),%rdi        # 201010 <__TMC_END__>
 6d7:	48 8d 35 32 09 20 00 	lea    0x200932(%rip),%rsi        # 201010 <__TMC_END__>
 6de:	55                   	push   %rbp
 6df:	48 29 fe             	sub    %rdi,%rsi
 6e2:	48 89 e5             	mov    %rsp,%rbp
 6e5:	48 c1 fe 03          	sar    $0x3,%rsi
 6e9:	48 89 f0             	mov    %rsi,%rax
 6ec:	48 c1 e8 3f          	shr    $0x3f,%rax
 6f0:	48 01 c6             	add    %rax,%rsi
 6f3:	48 d1 fe             	sar    %rsi
 6f6:	74 18                	je     710 <register_tm_clones+0x40>
 6f8:	48 8b 05 f1 08 20 00 	mov    0x2008f1(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 6ff:	48 85 c0             	test   %rax,%rax
 702:	74 0c                	je     710 <register_tm_clones+0x40>
 704:	5d                   	pop    %rbp
 705:	ff e0                	jmpq   *%rax
 707:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 70e:	00 00 
 710:	5d                   	pop    %rbp
 711:	c3                   	retq   
 712:	0f 1f 40 00          	nopl   0x0(%rax)
 716:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 71d:	00 00 00 

0000000000000720 <__do_global_dtors_aux>:
 720:	80 3d e9 08 20 00 00 	cmpb   $0x0,0x2008e9(%rip)        # 201010 <__TMC_END__>
 727:	75 2f                	jne    758 <__do_global_dtors_aux+0x38>
 729:	48 83 3d c7 08 20 00 	cmpq   $0x0,0x2008c7(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 730:	00 
 731:	55                   	push   %rbp
 732:	48 89 e5             	mov    %rsp,%rbp
 735:	74 0c                	je     743 <__do_global_dtors_aux+0x23>
 737:	48 8b 3d ca 08 20 00 	mov    0x2008ca(%rip),%rdi        # 201008 <__dso_handle>
 73e:	e8 7d fe ff ff       	callq  5c0 <__cxa_finalize@plt>
 743:	e8 48 ff ff ff       	callq  690 <deregister_tm_clones>
 748:	c6 05 c1 08 20 00 01 	movb   $0x1,0x2008c1(%rip)        # 201010 <__TMC_END__>
 74f:	5d                   	pop    %rbp
 750:	c3                   	retq   
 751:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 758:	f3 c3                	repz retq 
 75a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000760 <frame_dummy>:
 760:	55                   	push   %rbp
 761:	48 89 e5             	mov    %rsp,%rbp
 764:	5d                   	pop    %rbp
 765:	e9 66 ff ff ff       	jmpq   6d0 <register_tm_clones>
 76a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000770 <add_asm>:
 770:	01 fe                	add    %edi,%esi
 772:	89 32                	mov    %esi,(%rdx)
 774:	c3                   	retq   
 775:	90                   	nop
 776:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 77d:	00 00 00 

0000000000000780 <add_c>:
 780:	90                   	nop
 781:	90                   	nop
 782:	01 f7                	add    %esi,%edi
 784:	89 3a                	mov    %edi,(%rdx)
 786:	90                   	nop
 787:	90                   	nop
 788:	c3                   	retq   
 789:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000000790 <__libc_csu_init>:
 790:	41 57                	push   %r15
 792:	41 56                	push   %r14
 794:	49 89 d7             	mov    %rdx,%r15
 797:	41 55                	push   %r13
 799:	41 54                	push   %r12
 79b:	4c 8d 25 0e 06 20 00 	lea    0x20060e(%rip),%r12        # 200db0 <__frame_dummy_init_array_entry>
 7a2:	55                   	push   %rbp
 7a3:	48 8d 2d 0e 06 20 00 	lea    0x20060e(%rip),%rbp        # 200db8 <__init_array_end>
 7aa:	53                   	push   %rbx
 7ab:	41 89 fd             	mov    %edi,%r13d
 7ae:	49 89 f6             	mov    %rsi,%r14
 7b1:	4c 29 e5             	sub    %r12,%rbp
 7b4:	48 83 ec 08          	sub    $0x8,%rsp
 7b8:	48 c1 fd 03          	sar    $0x3,%rbp
 7bc:	e8 af fd ff ff       	callq  570 <_init>
 7c1:	48 85 ed             	test   %rbp,%rbp
 7c4:	74 20                	je     7e6 <__libc_csu_init+0x56>
 7c6:	31 db                	xor    %ebx,%ebx
 7c8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 7cf:	00 
 7d0:	4c 89 fa             	mov    %r15,%rdx
 7d3:	4c 89 f6             	mov    %r14,%rsi
 7d6:	44 89 ef             	mov    %r13d,%edi
 7d9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 7dd:	48 83 c3 01          	add    $0x1,%rbx
 7e1:	48 39 dd             	cmp    %rbx,%rbp
 7e4:	75 ea                	jne    7d0 <__libc_csu_init+0x40>
 7e6:	48 83 c4 08          	add    $0x8,%rsp
 7ea:	5b                   	pop    %rbx
 7eb:	5d                   	pop    %rbp
 7ec:	41 5c                	pop    %r12
 7ee:	41 5d                	pop    %r13
 7f0:	41 5e                	pop    %r14
 7f2:	41 5f                	pop    %r15
 7f4:	c3                   	retq   
 7f5:	90                   	nop
 7f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7fd:	00 00 00 

0000000000000800 <__libc_csu_fini>:
 800:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000804 <_fini>:
 804:	48 83 ec 08          	sub    $0x8,%rsp
 808:	48 83 c4 08          	add    $0x8,%rsp
 80c:	c3                   	retq   
