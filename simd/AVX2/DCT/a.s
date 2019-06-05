
a.out:     file format elf64-x86-64


Disassembly of section .init:

00000000000005e8 <_init>:
 5e8:	48 83 ec 08          	sub    $0x8,%rsp
 5ec:	48 8b 05 f5 59 20 00 	mov    0x2059f5(%rip),%rax        # 205fe8 <__gmon_start__>
 5f3:	48 85 c0             	test   %rax,%rax
 5f6:	74 02                	je     5fa <_init+0x12>
 5f8:	ff d0                	callq  *%rax
 5fa:	48 83 c4 08          	add    $0x8,%rsp
 5fe:	c3                   	retq   

Disassembly of section .plt:

0000000000000600 <.plt>:
 600:	ff 35 a2 59 20 00    	pushq  0x2059a2(%rip)        # 205fa8 <_GLOBAL_OFFSET_TABLE_+0x8>
 606:	ff 25 a4 59 20 00    	jmpq   *0x2059a4(%rip)        # 205fb0 <_GLOBAL_OFFSET_TABLE_+0x10>
 60c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000610 <putchar@plt>:
 610:	ff 25 a2 59 20 00    	jmpq   *0x2059a2(%rip)        # 205fb8 <putchar@GLIBC_2.2.5>
 616:	68 00 00 00 00       	pushq  $0x0
 61b:	e9 e0 ff ff ff       	jmpq   600 <.plt>

0000000000000620 <__stack_chk_fail@plt>:
 620:	ff 25 9a 59 20 00    	jmpq   *0x20599a(%rip)        # 205fc0 <__stack_chk_fail@GLIBC_2.4>
 626:	68 01 00 00 00       	pushq  $0x1
 62b:	e9 d0 ff ff ff       	jmpq   600 <.plt>

0000000000000630 <gettimeofday@plt>:
 630:	ff 25 92 59 20 00    	jmpq   *0x205992(%rip)        # 205fc8 <gettimeofday@GLIBC_2.2.5>
 636:	68 02 00 00 00       	pushq  $0x2
 63b:	e9 c0 ff ff ff       	jmpq   600 <.plt>

0000000000000640 <__printf_chk@plt>:
 640:	ff 25 8a 59 20 00    	jmpq   *0x20598a(%rip)        # 205fd0 <__printf_chk@GLIBC_2.3.4>
 646:	68 03 00 00 00       	pushq  $0x3
 64b:	e9 b0 ff ff ff       	jmpq   600 <.plt>

Disassembly of section .plt.got:

0000000000000650 <__cxa_finalize@plt>:
 650:	ff 25 a2 59 20 00    	jmpq   *0x2059a2(%rip)        # 205ff8 <__cxa_finalize@GLIBC_2.2.5>
 656:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000660 <main>:
     660:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
     665:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
     669:	41 ff 72 f8          	pushq  -0x8(%r10)
     66d:	55                   	push   %rbp
     66e:	48 89 e5             	mov    %rsp,%rbp
     671:	41 56                	push   %r14
     673:	41 55                	push   %r13
     675:	41 54                	push   %r12
     677:	41 52                	push   %r10
     679:	53                   	push   %rbx
     67a:	48 81 ec 48 01 00 00 	sub    $0x148,%rsp
     681:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
     688:	00 00 
     68a:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
     68e:	31 c0                	xor    %eax,%eax
     690:	e8 7b 3f 00 00       	callq  4610 <test0>
     695:	31 c0                	xor    %eax,%eax
     697:	e8 04 41 00 00       	callq  47a0 <test0_simd>
     69c:	c5 fd 6f 1d 1c 48 00 	vmovdqa 0x481c(%rip),%ymm3        # 4ec0 <__func__.3639+0x55>
     6a3:	00 
     6a4:	48 8d 15 a8 47 00 00 	lea    0x47a8(%rip),%rdx        # 4e53 <__func__.3683>
     6ab:	48 8d 35 16 47 00 00 	lea    0x4716(%rip),%rsi        # 4dc8 <_IO_stdin_used+0x408>
     6b2:	c5 fd 6f 15 26 48 00 	vmovdqa 0x4826(%rip),%ymm2        # 4ee0 <__func__.3639+0x75>
     6b9:	00 
     6ba:	b9 9c 00 00 00       	mov    $0x9c,%ecx
     6bf:	bf 01 00 00 00       	mov    $0x1,%edi
     6c4:	c5 fd 6f 0d 34 48 00 	vmovdqa 0x4834(%rip),%ymm1        # 4f00 <__func__.3639+0x95>
     6cb:	00 
     6cc:	31 c0                	xor    %eax,%eax
     6ce:	c5 fd 6f 05 4a 48 00 	vmovdqa 0x484a(%rip),%ymm0        # 4f20 <__func__.3639+0xb5>
     6d5:	00 
     6d6:	c5 fd 7f 9d 30 ff ff 	vmovdqa %ymm3,-0xd0(%rbp)
     6dd:	ff 
     6de:	c5 fd 7f 9d 90 fe ff 	vmovdqa %ymm3,-0x170(%rbp)
     6e5:	ff 
     6e6:	c5 fd 7f 95 50 ff ff 	vmovdqa %ymm2,-0xb0(%rbp)
     6ed:	ff 
     6ee:	c5 fd 7f 95 b0 fe ff 	vmovdqa %ymm2,-0x150(%rbp)
     6f5:	ff 
     6f6:	c5 fd 7f 8d 70 ff ff 	vmovdqa %ymm1,-0x90(%rbp)
     6fd:	ff 
     6fe:	c5 fd 7f 8d d0 fe ff 	vmovdqa %ymm1,-0x130(%rbp)
     705:	ff 
     706:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
     70b:	c5 fd 7f 85 f0 fe ff 	vmovdqa %ymm0,-0x110(%rbp)
     712:	ff 
     713:	c5 f8 77             	vzeroupper 
     716:	48 8d 9d 30 ff ff ff 	lea    -0xd0(%rbp),%rbx
     71d:	4c 8d a5 20 ff ff ff 	lea    -0xe0(%rbp),%r12
     724:	e8 17 ff ff ff       	callq  640 <__printf_chk@plt>
     729:	48 89 df             	mov    %rbx,%rdi
     72c:	e8 6f 3e 00 00       	callq  45a0 <dump_int16>
     731:	31 f6                	xor    %esi,%esi
     733:	4c 89 e7             	mov    %r12,%rdi
     736:	e8 f5 fe ff ff       	callq  630 <gettimeofday@plt>
     73b:	4c 69 ad 20 ff ff ff 	imul   $0xf4240,-0xe0(%rbp),%r13
     742:	40 42 0f 00 
     746:	48 89 df             	mov    %rbx,%rdi
     749:	4c 03 ad 28 ff ff ff 	add    -0xd8(%rbp),%r13
     750:	e8 ab 13 00 00       	callq  1b00 <jpeg_fdct_ifast>
     755:	31 f6                	xor    %esi,%esi
     757:	4c 89 e7             	mov    %r12,%rdi
     75a:	e8 d1 fe ff ff       	callq  630 <gettimeofday@plt>
     75f:	4c 69 b5 20 ff ff ff 	imul   $0xf4240,-0xe0(%rbp),%r14
     766:	40 42 0f 00 
     76a:	48 89 df             	mov    %rbx,%rdi
     76d:	4c 03 b5 28 ff ff ff 	add    -0xd8(%rbp),%r14
     774:	e8 27 3e 00 00       	callq  45a0 <dump_int16>
     779:	48 8d 15 d3 46 00 00 	lea    0x46d3(%rip),%rdx        # 4e53 <__func__.3683>
     780:	48 8d 35 91 46 00 00 	lea    0x4691(%rip),%rsi        # 4e18 <_IO_stdin_used+0x458>
     787:	b9 a2 00 00 00       	mov    $0xa2,%ecx
     78c:	bf 01 00 00 00       	mov    $0x1,%edi
     791:	31 c0                	xor    %eax,%eax
     793:	4d 89 f0             	mov    %r14,%r8
     796:	4d 29 e8             	sub    %r13,%r8
     799:	e8 a2 fe ff ff       	callq  640 <__printf_chk@plt>
     79e:	c5 fd 6f 9d 90 fe ff 	vmovdqa -0x170(%rbp),%ymm3
     7a5:	ff 
     7a6:	48 8d 15 9b 46 00 00 	lea    0x469b(%rip),%rdx        # 4e48 <__func__.3693>
     7ad:	48 8d 35 14 46 00 00 	lea    0x4614(%rip),%rsi        # 4dc8 <_IO_stdin_used+0x408>
     7b4:	c5 fd 6f 95 b0 fe ff 	vmovdqa -0x150(%rbp),%ymm2
     7bb:	ff 
     7bc:	b9 af 00 00 00       	mov    $0xaf,%ecx
     7c1:	bf 01 00 00 00       	mov    $0x1,%edi
     7c6:	c5 fd 6f 8d d0 fe ff 	vmovdqa -0x130(%rbp),%ymm1
     7cd:	ff 
     7ce:	31 c0                	xor    %eax,%eax
     7d0:	c5 fd 6f 85 f0 fe ff 	vmovdqa -0x110(%rbp),%ymm0
     7d7:	ff 
     7d8:	c5 fd 7f 9d 30 ff ff 	vmovdqa %ymm3,-0xd0(%rbp)
     7df:	ff 
     7e0:	c5 fd 7f 95 50 ff ff 	vmovdqa %ymm2,-0xb0(%rbp)
     7e7:	ff 
     7e8:	c5 fd 7f 8d 70 ff ff 	vmovdqa %ymm1,-0x90(%rbp)
     7ef:	ff 
     7f0:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
     7f5:	c5 f8 77             	vzeroupper 
     7f8:	e8 43 fe ff ff       	callq  640 <__printf_chk@plt>
     7fd:	48 89 df             	mov    %rbx,%rdi
     800:	e8 9b 3d 00 00       	callq  45a0 <dump_int16>
     805:	31 f6                	xor    %esi,%esi
     807:	4c 89 e7             	mov    %r12,%rdi
     80a:	e8 21 fe ff ff       	callq  630 <gettimeofday@plt>
     80f:	4c 69 ad 20 ff ff ff 	imul   $0xf4240,-0xe0(%rbp),%r13
     816:	40 42 0f 00 
     81a:	48 89 df             	mov    %rbx,%rdi
     81d:	31 c0                	xor    %eax,%eax
     81f:	4c 03 ad 28 ff ff ff 	add    -0xd8(%rbp),%r13
     826:	e8 95 01 00 00       	callq  9c0 <_jsimd_fdct_ifast_sse2>
     82b:	31 f6                	xor    %esi,%esi
     82d:	4c 89 e7             	mov    %r12,%rdi
     830:	e8 fb fd ff ff       	callq  630 <gettimeofday@plt>
     835:	4c 69 a5 20 ff ff ff 	imul   $0xf4240,-0xe0(%rbp),%r12
     83c:	40 42 0f 00 
     840:	48 89 df             	mov    %rbx,%rdi
     843:	4c 03 a5 28 ff ff ff 	add    -0xd8(%rbp),%r12
     84a:	e8 51 3d 00 00       	callq  45a0 <dump_int16>
     84f:	48 8d 15 f2 45 00 00 	lea    0x45f2(%rip),%rdx        # 4e48 <__func__.3693>
     856:	48 8d 35 bb 45 00 00 	lea    0x45bb(%rip),%rsi        # 4e18 <_IO_stdin_used+0x458>
     85d:	31 c0                	xor    %eax,%eax
     85f:	b9 b6 00 00 00       	mov    $0xb6,%ecx
     864:	bf 01 00 00 00       	mov    $0x1,%edi
     869:	4d 89 e0             	mov    %r12,%r8
     86c:	4d 29 e8             	sub    %r13,%r8
     86f:	e8 cc fd ff ff       	callq  640 <__printf_chk@plt>
     874:	48 8b 55 c8          	mov    -0x38(%rbp),%rdx
     878:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
     87f:	00 00 
     881:	75 18                	jne    89b <main+0x23b>
     883:	48 81 c4 48 01 00 00 	add    $0x148,%rsp
     88a:	31 c0                	xor    %eax,%eax
     88c:	5b                   	pop    %rbx
     88d:	41 5a                	pop    %r10
     88f:	41 5c                	pop    %r12
     891:	41 5d                	pop    %r13
     893:	41 5e                	pop    %r14
     895:	5d                   	pop    %rbp
     896:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
     89a:	c3                   	retq   
     89b:	e8 80 fd ff ff       	callq  620 <__stack_chk_fail@plt>

00000000000008a0 <_start>:
     8a0:	31 ed                	xor    %ebp,%ebp
     8a2:	49 89 d1             	mov    %rdx,%r9
     8a5:	5e                   	pop    %rsi
     8a6:	48 89 e2             	mov    %rsp,%rdx
     8a9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
     8ad:	50                   	push   %rax
     8ae:	54                   	push   %rsp
     8af:	4c 8d 05 ea 40 00 00 	lea    0x40ea(%rip),%r8        # 49a0 <__libc_csu_fini>
     8b6:	48 8d 0d 73 40 00 00 	lea    0x4073(%rip),%rcx        # 4930 <__libc_csu_init>
     8bd:	48 8d 3d 9c fd ff ff 	lea    -0x264(%rip),%rdi        # 660 <main>
     8c4:	ff 15 16 57 20 00    	callq  *0x205716(%rip)        # 205fe0 <__libc_start_main@GLIBC_2.2.5>
     8ca:	f4                   	hlt    
     8cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000008d0 <deregister_tm_clones>:
     8d0:	48 8d 3d e9 59 20 00 	lea    0x2059e9(%rip),%rdi        # 2062c0 <__TMC_END__>
     8d7:	55                   	push   %rbp
     8d8:	48 8d 05 e1 59 20 00 	lea    0x2059e1(%rip),%rax        # 2062c0 <__TMC_END__>
     8df:	48 39 f8             	cmp    %rdi,%rax
     8e2:	48 89 e5             	mov    %rsp,%rbp
     8e5:	74 19                	je     900 <deregister_tm_clones+0x30>
     8e7:	48 8b 05 ea 56 20 00 	mov    0x2056ea(%rip),%rax        # 205fd8 <_ITM_deregisterTMCloneTable>
     8ee:	48 85 c0             	test   %rax,%rax
     8f1:	74 0d                	je     900 <deregister_tm_clones+0x30>
     8f3:	5d                   	pop    %rbp
     8f4:	ff e0                	jmpq   *%rax
     8f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     8fd:	00 00 00 
     900:	5d                   	pop    %rbp
     901:	c3                   	retq   
     902:	0f 1f 40 00          	nopl   0x0(%rax)
     906:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     90d:	00 00 00 

0000000000000910 <register_tm_clones>:
     910:	48 8d 3d a9 59 20 00 	lea    0x2059a9(%rip),%rdi        # 2062c0 <__TMC_END__>
     917:	48 8d 35 a2 59 20 00 	lea    0x2059a2(%rip),%rsi        # 2062c0 <__TMC_END__>
     91e:	55                   	push   %rbp
     91f:	48 29 fe             	sub    %rdi,%rsi
     922:	48 89 e5             	mov    %rsp,%rbp
     925:	48 c1 fe 03          	sar    $0x3,%rsi
     929:	48 89 f0             	mov    %rsi,%rax
     92c:	48 c1 e8 3f          	shr    $0x3f,%rax
     930:	48 01 c6             	add    %rax,%rsi
     933:	48 d1 fe             	sar    %rsi
     936:	74 18                	je     950 <register_tm_clones+0x40>
     938:	48 8b 05 b1 56 20 00 	mov    0x2056b1(%rip),%rax        # 205ff0 <_ITM_registerTMCloneTable>
     93f:	48 85 c0             	test   %rax,%rax
     942:	74 0c                	je     950 <register_tm_clones+0x40>
     944:	5d                   	pop    %rbp
     945:	ff e0                	jmpq   *%rax
     947:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
     94e:	00 00 
     950:	5d                   	pop    %rbp
     951:	c3                   	retq   
     952:	0f 1f 40 00          	nopl   0x0(%rax)
     956:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     95d:	00 00 00 

0000000000000960 <__do_global_dtors_aux>:
     960:	80 3d 59 59 20 00 00 	cmpb   $0x0,0x205959(%rip)        # 2062c0 <__TMC_END__>
     967:	75 2f                	jne    998 <F_0_298+0xa>
     969:	48 83 3d 87 56 20 00 	cmpq   $0x0,0x205687(%rip)        # 205ff8 <__cxa_finalize@GLIBC_2.2.5>
     970:	00 
     971:	55                   	push   %rbp
     972:	48 89 e5             	mov    %rsp,%rbp
     975:	74 0c                	je     983 <__do_global_dtors_aux+0x23>
     977:	48 8b 3d 8a 56 20 00 	mov    0x20568a(%rip),%rdi        # 206008 <__dso_handle>
     97e:	e8 cd fc ff ff       	callq  650 <__cxa_finalize@plt>
     983:	e8 48 ff ff ff       	callq  8d0 <deregister_tm_clones>
     988:	c6 05 31 59 20 00 01 	movb   $0x1,0x205931(%rip)        # 2062c0 <__TMC_END__>
     98f:	5d                   	pop    %rbp
     990:	c3                   	retq   
     991:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
     998:	f3 c3                	repz retq 
     99a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000009a0 <frame_dummy>:
     9a0:	55                   	push   %rbp
     9a1:	48 89 e5             	mov    %rsp,%rbp
     9a4:	5d                   	pop    %rbp
     9a5:	e9 66 ff ff ff       	jmpq   910 <register_tm_clones>
     9aa:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     9b1:	00 00 00 
     9b4:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     9bb:	00 00 00 
     9be:	66 90                	xchg   %ax,%ax

00000000000009c0 <_jsimd_fdct_ifast_sse2>:
     9c0:	55                   	push   %rbp
     9c1:	48 89 e0             	mov    %rsp,%rax
     9c4:	48 83 ec 04          	sub    $0x4,%rsp
     9c8:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
     9cc:	48 89 04 24          	mov    %rax,(%rsp)
     9d0:	48 89 e5             	mov    %rsp,%rbp
     9d3:	48 8d 65 e0          	lea    -0x20(%rbp),%rsp
     9d7:	41 52                	push   %r10
     9d9:	49 89 fa             	mov    %rdi,%r10
     9dc:	4c 89 d2             	mov    %r10,%rdx
     9df:	66 0f 6f 02          	movdqa (%rdx),%xmm0
     9e3:	66 0f 6f 4a 10       	movdqa 0x10(%rdx),%xmm1
     9e8:	66 0f 6f 52 20       	movdqa 0x20(%rdx),%xmm2
     9ed:	66 0f 6f 5a 30       	movdqa 0x30(%rdx),%xmm3
     9f2:	66 0f 6f e0          	movdqa %xmm0,%xmm4
     9f6:	66 0f 61 c1          	punpcklwd %xmm1,%xmm0
     9fa:	66 0f 69 e1          	punpckhwd %xmm1,%xmm4
     9fe:	66 0f 6f ea          	movdqa %xmm2,%xmm5
     a02:	66 0f 61 d3          	punpcklwd %xmm3,%xmm2
     a06:	66 0f 69 eb          	punpckhwd %xmm3,%xmm5
     a0a:	66 0f 6f 72 40       	movdqa 0x40(%rdx),%xmm6
     a0f:	66 0f 6f 7a 50       	movdqa 0x50(%rdx),%xmm7
     a14:	66 0f 6f 4a 60       	movdqa 0x60(%rdx),%xmm1
     a19:	66 0f 6f 5a 70       	movdqa 0x70(%rdx),%xmm3
     a1e:	66 0f 7f 55 e0       	movdqa %xmm2,-0x20(%rbp)
     a23:	66 0f 7f 6d f0       	movdqa %xmm5,-0x10(%rbp)
     a28:	66 0f 6f d6          	movdqa %xmm6,%xmm2
     a2c:	66 0f 61 f7          	punpcklwd %xmm7,%xmm6
     a30:	66 0f 69 d7          	punpckhwd %xmm7,%xmm2
     a34:	66 0f 6f e9          	movdqa %xmm1,%xmm5
     a38:	66 0f 61 cb          	punpcklwd %xmm3,%xmm1
     a3c:	66 0f 69 eb          	punpckhwd %xmm3,%xmm5
     a40:	66 0f 6f fe          	movdqa %xmm6,%xmm7
     a44:	66 0f 62 f1          	punpckldq %xmm1,%xmm6
     a48:	66 0f 6a f9          	punpckhdq %xmm1,%xmm7
     a4c:	66 0f 6f da          	movdqa %xmm2,%xmm3
     a50:	66 0f 62 d5          	punpckldq %xmm5,%xmm2
     a54:	66 0f 6a dd          	punpckhdq %xmm5,%xmm3
     a58:	66 0f 6f 4d e0       	movdqa -0x20(%rbp),%xmm1
     a5d:	66 0f 6f 6d f0       	movdqa -0x10(%rbp),%xmm5
     a62:	66 0f 7f 7d e0       	movdqa %xmm7,-0x20(%rbp)
     a67:	66 0f 7f 55 f0       	movdqa %xmm2,-0x10(%rbp)
     a6c:	66 0f 6f f8          	movdqa %xmm0,%xmm7
     a70:	66 0f 62 c1          	punpckldq %xmm1,%xmm0
     a74:	66 0f 6a f9          	punpckhdq %xmm1,%xmm7
     a78:	66 0f 6f d4          	movdqa %xmm4,%xmm2
     a7c:	66 0f 62 e5          	punpckldq %xmm5,%xmm4
     a80:	66 0f 6a d5          	punpckhdq %xmm5,%xmm2
     a84:	66 0f 6f c8          	movdqa %xmm0,%xmm1
     a88:	66 0f 6c c6          	punpcklqdq %xmm6,%xmm0
     a8c:	66 0f 6d ce          	punpckhqdq %xmm6,%xmm1
     a90:	66 0f 6f ea          	movdqa %xmm2,%xmm5
     a94:	66 0f 6c d3          	punpcklqdq %xmm3,%xmm2
     a98:	66 0f 6d eb          	punpckhqdq %xmm3,%xmm5
     a9c:	66 0f 6f f1          	movdqa %xmm1,%xmm6
     aa0:	66 0f 6f d8          	movdqa %xmm0,%xmm3
     aa4:	66 0f f9 ca          	psubw  %xmm2,%xmm1
     aa8:	66 0f f9 c5          	psubw  %xmm5,%xmm0
     aac:	66 0f fd f2          	paddw  %xmm2,%xmm6
     ab0:	66 0f fd dd          	paddw  %xmm5,%xmm3
     ab4:	66 0f 6f 55 e0       	movdqa -0x20(%rbp),%xmm2
     ab9:	66 0f 6f 6d f0       	movdqa -0x10(%rbp),%xmm5
     abe:	66 0f 7f 4d e0       	movdqa %xmm1,-0x20(%rbp)
     ac3:	66 0f 7f 45 f0       	movdqa %xmm0,-0x10(%rbp)
     ac8:	66 0f 6f cf          	movdqa %xmm7,%xmm1
     acc:	66 0f 6c fa          	punpcklqdq %xmm2,%xmm7
     ad0:	66 0f 6d ca          	punpckhqdq %xmm2,%xmm1
     ad4:	66 0f 6f c4          	movdqa %xmm4,%xmm0
     ad8:	66 0f 6c e5          	punpcklqdq %xmm5,%xmm4
     adc:	66 0f 6d c5          	punpckhqdq %xmm5,%xmm0
     ae0:	66 0f 6f d1          	movdqa %xmm1,%xmm2
     ae4:	66 0f 6f ef          	movdqa %xmm7,%xmm5
     ae8:	66 0f fd cc          	paddw  %xmm4,%xmm1
     aec:	66 0f fd f8          	paddw  %xmm0,%xmm7
     af0:	66 0f f9 d4          	psubw  %xmm4,%xmm2
     af4:	66 0f f9 e8          	psubw  %xmm0,%xmm5
     af8:	66 0f 6f e3          	movdqa %xmm3,%xmm4
     afc:	66 0f 6f c6          	movdqa %xmm6,%xmm0
     b00:	66 0f f9 d9          	psubw  %xmm1,%xmm3
     b04:	66 0f f9 f7          	psubw  %xmm7,%xmm6
     b08:	66 0f fd e1          	paddw  %xmm1,%xmm4
     b0c:	66 0f fd c7          	paddw  %xmm7,%xmm0
     b10:	66 0f fd f3          	paddw  %xmm3,%xmm6
     b14:	66 0f 71 f6 02       	psllw  $0x2,%xmm6
     b19:	66 0f e5 35 ff 54 20 	pmulhw 0x2054ff(%rip),%xmm6        # 206020 <_jconst_fdct_ifast_sse2>
     b20:	00 
     b21:	66 0f 6f cc          	movdqa %xmm4,%xmm1
     b25:	66 0f 6f fb          	movdqa %xmm3,%xmm7
     b29:	66 0f f9 e0          	psubw  %xmm0,%xmm4
     b2d:	66 0f f9 de          	psubw  %xmm6,%xmm3
     b31:	66 0f fd c8          	paddw  %xmm0,%xmm1
     b35:	66 0f fd fe          	paddw  %xmm6,%xmm7
     b39:	66 0f 6f 45 e0       	movdqa -0x20(%rbp),%xmm0
     b3e:	66 0f 6f 75 f0       	movdqa -0x10(%rbp),%xmm6
     b43:	66 0f 7f 65 e0       	movdqa %xmm4,-0x20(%rbp)
     b48:	66 0f 7f 5d f0       	movdqa %xmm3,-0x10(%rbp)
     b4d:	66 0f fd d5          	paddw  %xmm5,%xmm2
     b51:	66 0f fd e8          	paddw  %xmm0,%xmm5
     b55:	66 0f fd c6          	paddw  %xmm6,%xmm0
     b59:	66 0f 71 f2 02       	psllw  $0x2,%xmm2
     b5e:	66 0f 71 f0 02       	psllw  $0x2,%xmm0
     b63:	66 0f 71 f5 02       	psllw  $0x2,%xmm5
     b68:	66 0f e5 2d b0 54 20 	pmulhw 0x2054b0(%rip),%xmm5        # 206020 <_jconst_fdct_ifast_sse2>
     b6f:	00 
     b70:	66 0f 6f e2          	movdqa %xmm2,%xmm4
     b74:	66 0f f9 d0          	psubw  %xmm0,%xmm2
     b78:	66 0f e5 15 b0 54 20 	pmulhw 0x2054b0(%rip),%xmm2        # 206030 <PW_F0382>
     b7f:	00 
     b80:	66 0f e5 25 b8 54 20 	pmulhw 0x2054b8(%rip),%xmm4        # 206040 <PW_F0541>
     b87:	00 
     b88:	66 0f e5 05 c0 54 20 	pmulhw 0x2054c0(%rip),%xmm0        # 206050 <PW_F1306>
     b8f:	00 
     b90:	66 0f fd e2          	paddw  %xmm2,%xmm4
     b94:	66 0f fd c2          	paddw  %xmm2,%xmm0
     b98:	66 0f 6f de          	movdqa %xmm6,%xmm3
     b9c:	66 0f f9 f5          	psubw  %xmm5,%xmm6
     ba0:	66 0f fd dd          	paddw  %xmm5,%xmm3
     ba4:	66 0f 6f d6          	movdqa %xmm6,%xmm2
     ba8:	66 0f 6f eb          	movdqa %xmm3,%xmm5
     bac:	66 0f f9 f4          	psubw  %xmm4,%xmm6
     bb0:	66 0f f9 d8          	psubw  %xmm0,%xmm3
     bb4:	66 0f fd d4          	paddw  %xmm4,%xmm2
     bb8:	66 0f fd e8          	paddw  %xmm0,%xmm5
     bbc:	66 0f 6f e1          	movdqa %xmm1,%xmm4
     bc0:	66 0f 61 cd          	punpcklwd %xmm5,%xmm1
     bc4:	66 0f 69 e5          	punpckhwd %xmm5,%xmm4
     bc8:	66 0f 6f c7          	movdqa %xmm7,%xmm0
     bcc:	66 0f 61 fe          	punpcklwd %xmm6,%xmm7
     bd0:	66 0f 69 c6          	punpckhwd %xmm6,%xmm0
     bd4:	66 0f 6f 6d e0       	movdqa -0x20(%rbp),%xmm5
     bd9:	66 0f 6f 75 f0       	movdqa -0x10(%rbp),%xmm6
     bde:	66 0f 7f 7d e0       	movdqa %xmm7,-0x20(%rbp)
     be3:	66 0f 7f 45 f0       	movdqa %xmm0,-0x10(%rbp)
     be8:	66 0f 6f fd          	movdqa %xmm5,%xmm7
     bec:	66 0f 61 ea          	punpcklwd %xmm2,%xmm5
     bf0:	66 0f 69 fa          	punpckhwd %xmm2,%xmm7
     bf4:	66 0f 6f c6          	movdqa %xmm6,%xmm0
     bf8:	66 0f 61 f3          	punpcklwd %xmm3,%xmm6
     bfc:	66 0f 69 c3          	punpckhwd %xmm3,%xmm0
     c00:	66 0f 6f d5          	movdqa %xmm5,%xmm2
     c04:	66 0f 62 ee          	punpckldq %xmm6,%xmm5
     c08:	66 0f 6a d6          	punpckhdq %xmm6,%xmm2
     c0c:	66 0f 6f df          	movdqa %xmm7,%xmm3
     c10:	66 0f 62 f8          	punpckldq %xmm0,%xmm7
     c14:	66 0f 6a d8          	punpckhdq %xmm0,%xmm3
     c18:	66 0f 6f 75 e0       	movdqa -0x20(%rbp),%xmm6
     c1d:	66 0f 6f 45 f0       	movdqa -0x10(%rbp),%xmm0
     c22:	66 0f 7f 55 e0       	movdqa %xmm2,-0x20(%rbp)
     c27:	66 0f 7f 7d f0       	movdqa %xmm7,-0x10(%rbp)
     c2c:	66 0f 6f d1          	movdqa %xmm1,%xmm2
     c30:	66 0f 62 ce          	punpckldq %xmm6,%xmm1
     c34:	66 0f 6a d6          	punpckhdq %xmm6,%xmm2
     c38:	66 0f 6f fc          	movdqa %xmm4,%xmm7
     c3c:	66 0f 62 e0          	punpckldq %xmm0,%xmm4
     c40:	66 0f 6a f8          	punpckhdq %xmm0,%xmm7
     c44:	66 0f 6f f1          	movdqa %xmm1,%xmm6
     c48:	66 0f 6c cd          	punpcklqdq %xmm5,%xmm1
     c4c:	66 0f 6d f5          	punpckhqdq %xmm5,%xmm6
     c50:	66 0f 6f c7          	movdqa %xmm7,%xmm0
     c54:	66 0f 6c fb          	punpcklqdq %xmm3,%xmm7
     c58:	66 0f 6d c3          	punpckhqdq %xmm3,%xmm0
     c5c:	66 0f 6f ee          	movdqa %xmm6,%xmm5
     c60:	66 0f 6f d9          	movdqa %xmm1,%xmm3
     c64:	66 0f f9 f7          	psubw  %xmm7,%xmm6
     c68:	66 0f f9 c8          	psubw  %xmm0,%xmm1
     c6c:	66 0f fd ef          	paddw  %xmm7,%xmm5
     c70:	66 0f fd d8          	paddw  %xmm0,%xmm3
     c74:	66 0f 6f 7d e0       	movdqa -0x20(%rbp),%xmm7
     c79:	66 0f 6f 45 f0       	movdqa -0x10(%rbp),%xmm0
     c7e:	66 0f 7f 75 e0       	movdqa %xmm6,-0x20(%rbp)
     c83:	66 0f 7f 4d f0       	movdqa %xmm1,-0x10(%rbp)
     c88:	66 0f 6f f2          	movdqa %xmm2,%xmm6
     c8c:	66 0f 6c d7          	punpcklqdq %xmm7,%xmm2
     c90:	66 0f 6d f7          	punpckhqdq %xmm7,%xmm6
     c94:	66 0f 6f cc          	movdqa %xmm4,%xmm1
     c98:	66 0f 6c e0          	punpcklqdq %xmm0,%xmm4
     c9c:	66 0f 6d c8          	punpckhqdq %xmm0,%xmm1
     ca0:	66 0f 6f fe          	movdqa %xmm6,%xmm7
     ca4:	66 0f 6f c2          	movdqa %xmm2,%xmm0
     ca8:	66 0f fd f4          	paddw  %xmm4,%xmm6
     cac:	66 0f fd d1          	paddw  %xmm1,%xmm2
     cb0:	66 0f f9 fc          	psubw  %xmm4,%xmm7
     cb4:	66 0f f9 c1          	psubw  %xmm1,%xmm0
     cb8:	66 0f 6f e3          	movdqa %xmm3,%xmm4
     cbc:	66 0f 6f cd          	movdqa %xmm5,%xmm1
     cc0:	66 0f f9 de          	psubw  %xmm6,%xmm3
     cc4:	66 0f f9 ea          	psubw  %xmm2,%xmm5
     cc8:	66 0f fd e6          	paddw  %xmm6,%xmm4
     ccc:	66 0f fd ca          	paddw  %xmm2,%xmm1
     cd0:	66 0f fd eb          	paddw  %xmm3,%xmm5
     cd4:	66 0f 71 f5 02       	psllw  $0x2,%xmm5
     cd9:	66 0f e5 2d 3f 53 20 	pmulhw 0x20533f(%rip),%xmm5        # 206020 <_jconst_fdct_ifast_sse2>
     ce0:	00 
     ce1:	66 0f 6f f4          	movdqa %xmm4,%xmm6
     ce5:	66 0f 6f d3          	movdqa %xmm3,%xmm2
     ce9:	66 0f f9 e1          	psubw  %xmm1,%xmm4
     ced:	66 0f f9 dd          	psubw  %xmm5,%xmm3
     cf1:	66 0f fd f1          	paddw  %xmm1,%xmm6
     cf5:	66 0f fd d5          	paddw  %xmm5,%xmm2
     cf9:	66 0f 7f 62 40       	movdqa %xmm4,0x40(%rdx)
     cfe:	66 0f 7f 5a 60       	movdqa %xmm3,0x60(%rdx)
     d03:	66 0f 7f 32          	movdqa %xmm6,(%rdx)
     d07:	66 0f 7f 52 20       	movdqa %xmm2,0x20(%rdx)
     d0c:	66 0f 6f 4d e0       	movdqa -0x20(%rbp),%xmm1
     d11:	66 0f 6f 6d f0       	movdqa -0x10(%rbp),%xmm5
     d16:	66 0f fd f8          	paddw  %xmm0,%xmm7
     d1a:	66 0f fd c1          	paddw  %xmm1,%xmm0
     d1e:	66 0f fd cd          	paddw  %xmm5,%xmm1
     d22:	66 0f 71 f7 02       	psllw  $0x2,%xmm7
     d27:	66 0f 71 f1 02       	psllw  $0x2,%xmm1
     d2c:	66 0f 71 f0 02       	psllw  $0x2,%xmm0
     d31:	66 0f e5 05 e7 52 20 	pmulhw 0x2052e7(%rip),%xmm0        # 206020 <_jconst_fdct_ifast_sse2>
     d38:	00 
     d39:	66 0f 6f e7          	movdqa %xmm7,%xmm4
     d3d:	66 0f f9 f9          	psubw  %xmm1,%xmm7
     d41:	66 0f e5 3d e7 52 20 	pmulhw 0x2052e7(%rip),%xmm7        # 206030 <PW_F0382>
     d48:	00 
     d49:	66 0f e5 25 ef 52 20 	pmulhw 0x2052ef(%rip),%xmm4        # 206040 <PW_F0541>
     d50:	00 
     d51:	66 0f e5 0d f7 52 20 	pmulhw 0x2052f7(%rip),%xmm1        # 206050 <PW_F1306>
     d58:	00 
     d59:	66 0f fd e7          	paddw  %xmm7,%xmm4
     d5d:	66 0f fd cf          	paddw  %xmm7,%xmm1
     d61:	66 0f 6f dd          	movdqa %xmm5,%xmm3
     d65:	66 0f f9 e8          	psubw  %xmm0,%xmm5
     d69:	66 0f fd d8          	paddw  %xmm0,%xmm3
     d6d:	66 0f 6f f5          	movdqa %xmm5,%xmm6
     d71:	66 0f 6f d3          	movdqa %xmm3,%xmm2
     d75:	66 0f f9 ec          	psubw  %xmm4,%xmm5
     d79:	66 0f f9 d9          	psubw  %xmm1,%xmm3
     d7d:	66 0f fd f4          	paddw  %xmm4,%xmm6
     d81:	66 0f fd d1          	paddw  %xmm1,%xmm2
     d85:	66 0f 7f 6a 30       	movdqa %xmm5,0x30(%rdx)
     d8a:	66 0f 7f 5a 70       	movdqa %xmm3,0x70(%rdx)
     d8f:	66 0f 7f 72 50       	movdqa %xmm6,0x50(%rdx)
     d94:	66 0f 7f 52 10       	movdqa %xmm2,0x10(%rdx)
     d99:	41 5a                	pop    %r10
     d9b:	48 89 ec             	mov    %rbp,%rsp
     d9e:	5c                   	pop    %rsp
     d9f:	5d                   	pop    %rbp
     da0:	c3                   	retq   
     da1:	90                   	nop
     da2:	90                   	nop
     da3:	90                   	nop
     da4:	90                   	nop
     da5:	90                   	nop
     da6:	90                   	nop
     da7:	90                   	nop
     da8:	90                   	nop
     da9:	90                   	nop
     daa:	90                   	nop
     dab:	90                   	nop
     dac:	90                   	nop
     dad:	90                   	nop
     dae:	90                   	nop
     daf:	90                   	nop
     db0:	90                   	nop
     db1:	90                   	nop
     db2:	90                   	nop
     db3:	90                   	nop
     db4:	90                   	nop
     db5:	90                   	nop
     db6:	90                   	nop
     db7:	90                   	nop
     db8:	90                   	nop
     db9:	90                   	nop
     dba:	90                   	nop
     dbb:	90                   	nop
     dbc:	90                   	nop
     dbd:	90                   	nop
     dbe:	90                   	nop
     dbf:	90                   	nop

0000000000000dc0 <_jsimd_fdct_islow_avx2>:
     dc0:	55                   	push   %rbp
     dc1:	48 89 e0             	mov    %rsp,%rax
     dc4:	48 89 e5             	mov    %rsp,%rbp
     dc7:	41 52                	push   %r10
     dc9:	49 89 fa             	mov    %rdi,%r10
     dcc:	c4 c1 7e 6f 22       	vmovdqu (%r10),%ymm4
     dd1:	c4 c1 7e 6f 6a 20    	vmovdqu 0x20(%r10),%ymm5
     dd7:	c4 c1 7e 6f 72 40    	vmovdqu 0x40(%r10),%ymm6
     ddd:	c4 c1 7e 6f 7a 60    	vmovdqu 0x60(%r10),%ymm7
     de3:	c4 e3 5d 46 c6 20    	vperm2i128 $0x20,%ymm6,%ymm4,%ymm0
     de9:	c4 e3 5d 46 ce 31    	vperm2i128 $0x31,%ymm6,%ymm4,%ymm1
     def:	c4 e3 55 46 d7 20    	vperm2i128 $0x20,%ymm7,%ymm5,%ymm2
     df5:	c4 e3 55 46 df 31    	vperm2i128 $0x31,%ymm7,%ymm5,%ymm3
     dfb:	c5 fd 61 e1          	vpunpcklwd %ymm1,%ymm0,%ymm4
     dff:	c5 fd 69 e9          	vpunpckhwd %ymm1,%ymm0,%ymm5
     e03:	c5 ed 61 f3          	vpunpcklwd %ymm3,%ymm2,%ymm6
     e07:	c5 ed 69 fb          	vpunpckhwd %ymm3,%ymm2,%ymm7
     e0b:	c5 dd 62 c6          	vpunpckldq %ymm6,%ymm4,%ymm0
     e0f:	c5 dd 6a ce          	vpunpckhdq %ymm6,%ymm4,%ymm1
     e13:	c5 d5 62 d7          	vpunpckldq %ymm7,%ymm5,%ymm2
     e17:	c5 d5 6a df          	vpunpckhdq %ymm7,%ymm5,%ymm3
     e1b:	c4 e3 fd 00 c0 8d    	vpermq $0x8d,%ymm0,%ymm0
     e21:	c4 e3 fd 00 c9 8d    	vpermq $0x8d,%ymm1,%ymm1
     e27:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
     e2d:	c4 e3 fd 00 db d8    	vpermq $0xd8,%ymm3,%ymm3
     e33:	c5 fd f9 e3          	vpsubw %ymm3,%ymm0,%ymm4
     e37:	c5 fd fd eb          	vpaddw %ymm3,%ymm0,%ymm5
     e3b:	c5 f5 fd f2          	vpaddw %ymm2,%ymm1,%ymm6
     e3f:	c5 f5 f9 fa          	vpsubw %ymm2,%ymm1,%ymm7
     e43:	c4 e3 55 46 ed 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm5
     e49:	c5 d5 fd c6          	vpaddw %ymm6,%ymm5,%ymm0
     e4d:	c5 d5 f9 ee          	vpsubw %ymm6,%ymm5,%ymm5
     e51:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
     e57:	c4 e2 7d 09 05 e0 52 	vpsignw 0x2052e0(%rip),%ymm0,%ymm0        # 206140 <PW_1_NEG1>
     e5e:	20 00 
     e60:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
     e64:	c5 fd 71 f6 02       	vpsllw $0x2,%ymm6,%ymm0
     e69:	c4 e3 55 46 f5 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm6
     e6f:	c5 d5 61 ce          	vpunpcklwd %ymm6,%ymm5,%ymm1
     e73:	c5 d5 69 ee          	vpunpckhwd %ymm6,%ymm5,%ymm5
     e77:	c5 f5 f5 0d e1 51 20 	vpmaddwd 0x2051e1(%rip),%ymm1,%ymm1        # 206060 <_jconst_fdct_islow_avx2>
     e7e:	00 
     e7f:	c5 d5 f5 2d d9 51 20 	vpmaddwd 0x2051d9(%rip),%ymm5,%ymm5        # 206060 <_jconst_fdct_islow_avx2>
     e86:	00 
     e87:	c5 f5 fe 0d 51 52 20 	vpaddd 0x205251(%rip),%ymm1,%ymm1        # 2060e0 <PD_DESCALE_P1>
     e8e:	00 
     e8f:	c5 d5 fe 2d 49 52 20 	vpaddd 0x205249(%rip),%ymm5,%ymm5        # 2060e0 <PD_DESCALE_P1>
     e96:	00 
     e97:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
     e9c:	c5 d5 72 e5 0b       	vpsrad $0xb,%ymm5,%ymm5
     ea1:	c5 f5 6b d5          	vpackssdw %ymm5,%ymm1,%ymm2
     ea5:	c5 c5 fd f4          	vpaddw %ymm4,%ymm7,%ymm6
     ea9:	c4 e3 4d 46 ce 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm1
     eaf:	c5 cd 61 e9          	vpunpcklwd %ymm1,%ymm6,%ymm5
     eb3:	c5 cd 69 f1          	vpunpckhwd %ymm1,%ymm6,%ymm6
     eb7:	c5 d5 f5 2d c1 51 20 	vpmaddwd 0x2051c1(%rip),%ymm5,%ymm5        # 206080 <PW_MF078_F117_F078_F117>
     ebe:	00 
     ebf:	c5 cd f5 35 b9 51 20 	vpmaddwd 0x2051b9(%rip),%ymm6,%ymm6        # 206080 <PW_MF078_F117_F078_F117>
     ec6:	00 
     ec7:	c4 e3 5d 46 dc 01    	vperm2i128 $0x1,%ymm4,%ymm4,%ymm3
     ecd:	c5 c5 61 cb          	vpunpcklwd %ymm3,%ymm7,%ymm1
     ed1:	c5 c5 69 db          	vpunpckhwd %ymm3,%ymm7,%ymm3
     ed5:	c5 f5 f5 0d c3 51 20 	vpmaddwd 0x2051c3(%rip),%ymm1,%ymm1        # 2060a0 <PW_MF060_MF089_MF050_MF256>
     edc:	00 
     edd:	c5 e5 f5 1d bb 51 20 	vpmaddwd 0x2051bb(%rip),%ymm3,%ymm3        # 2060a0 <PW_MF060_MF089_MF050_MF256>
     ee4:	00 
     ee5:	c5 f5 fe cd          	vpaddd %ymm5,%ymm1,%ymm1
     ee9:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
     eed:	c5 f5 fe 0d eb 51 20 	vpaddd 0x2051eb(%rip),%ymm1,%ymm1        # 2060e0 <PD_DESCALE_P1>
     ef4:	00 
     ef5:	c5 e5 fe 1d e3 51 20 	vpaddd 0x2051e3(%rip),%ymm3,%ymm3        # 2060e0 <PD_DESCALE_P1>
     efc:	00 
     efd:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
     f02:	c5 e5 72 e3 0b       	vpsrad $0xb,%ymm3,%ymm3
     f07:	c5 f5 6b db          	vpackssdw %ymm3,%ymm1,%ymm3
     f0b:	c4 e3 45 46 cf 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm1
     f11:	c5 dd 61 f9          	vpunpcklwd %ymm1,%ymm4,%ymm7
     f15:	c5 dd 69 e1          	vpunpckhwd %ymm1,%ymm4,%ymm4
     f19:	c5 c5 f5 3d 9f 51 20 	vpmaddwd 0x20519f(%rip),%ymm7,%ymm7        # 2060c0 <PW_F050_MF256_F060_MF089>
     f20:	00 
     f21:	c5 dd f5 25 97 51 20 	vpmaddwd 0x205197(%rip),%ymm4,%ymm4        # 2060c0 <PW_F050_MF256_F060_MF089>
     f28:	00 
     f29:	c5 c5 fe fd          	vpaddd %ymm5,%ymm7,%ymm7
     f2d:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
     f31:	c5 c5 fe 3d a7 51 20 	vpaddd 0x2051a7(%rip),%ymm7,%ymm7        # 2060e0 <PD_DESCALE_P1>
     f38:	00 
     f39:	c5 dd fe 25 9f 51 20 	vpaddd 0x20519f(%rip),%ymm4,%ymm4        # 2060e0 <PD_DESCALE_P1>
     f40:	00 
     f41:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
     f46:	c5 dd 72 e4 0b       	vpsrad $0xb,%ymm4,%ymm4
     f4b:	c5 c5 6b cc          	vpackssdw %ymm4,%ymm7,%ymm1
     f4f:	c4 e3 75 46 e3 20    	vperm2i128 $0x20,%ymm3,%ymm1,%ymm4
     f55:	c4 e3 75 46 cb 31    	vperm2i128 $0x31,%ymm3,%ymm1,%ymm1
     f5b:	c5 fd 61 d9          	vpunpcklwd %ymm1,%ymm0,%ymm3
     f5f:	c5 fd 69 e9          	vpunpckhwd %ymm1,%ymm0,%ymm5
     f63:	c5 ed 61 f4          	vpunpcklwd %ymm4,%ymm2,%ymm6
     f67:	c5 ed 69 fc          	vpunpckhwd %ymm4,%ymm2,%ymm7
     f6b:	c5 e5 62 c6          	vpunpckldq %ymm6,%ymm3,%ymm0
     f6f:	c5 e5 6a ce          	vpunpckhdq %ymm6,%ymm3,%ymm1
     f73:	c5 d5 62 d7          	vpunpckldq %ymm7,%ymm5,%ymm2
     f77:	c5 d5 6a e7          	vpunpckhdq %ymm7,%ymm5,%ymm4
     f7b:	c4 e3 fd 00 c0 8d    	vpermq $0x8d,%ymm0,%ymm0
     f81:	c4 e3 fd 00 c9 8d    	vpermq $0x8d,%ymm1,%ymm1
     f87:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
     f8d:	c4 e3 fd 00 e4 d8    	vpermq $0xd8,%ymm4,%ymm4
     f93:	c5 fd f9 dc          	vpsubw %ymm4,%ymm0,%ymm3
     f97:	c5 fd fd ec          	vpaddw %ymm4,%ymm0,%ymm5
     f9b:	c5 f5 fd f2          	vpaddw %ymm2,%ymm1,%ymm6
     f9f:	c5 f5 f9 fa          	vpsubw %ymm2,%ymm1,%ymm7
     fa3:	c4 e3 55 46 ed 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm5
     fa9:	c5 d5 fd c6          	vpaddw %ymm6,%ymm5,%ymm0
     fad:	c5 d5 f9 ee          	vpsubw %ymm6,%ymm5,%ymm5
     fb1:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
     fb7:	c4 e2 7d 09 05 80 51 	vpsignw 0x205180(%rip),%ymm0,%ymm0        # 206140 <PW_1_NEG1>
     fbe:	20 00 
     fc0:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
     fc4:	c5 cd fd 35 54 51 20 	vpaddw 0x205154(%rip),%ymm6,%ymm6        # 206120 <PW_DESCALE_P2X>
     fcb:	00 
     fcc:	c5 fd 71 e6 02       	vpsraw $0x2,%ymm6,%ymm0
     fd1:	c4 e3 55 46 f5 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm6
     fd7:	c5 d5 61 ce          	vpunpcklwd %ymm6,%ymm5,%ymm1
     fdb:	c5 d5 69 ee          	vpunpckhwd %ymm6,%ymm5,%ymm5
     fdf:	c5 f5 f5 0d 79 50 20 	vpmaddwd 0x205079(%rip),%ymm1,%ymm1        # 206060 <_jconst_fdct_islow_avx2>
     fe6:	00 
     fe7:	c5 d5 f5 2d 71 50 20 	vpmaddwd 0x205071(%rip),%ymm5,%ymm5        # 206060 <_jconst_fdct_islow_avx2>
     fee:	00 
     fef:	c5 f5 fe 0d 09 51 20 	vpaddd 0x205109(%rip),%ymm1,%ymm1        # 206100 <PD_DESCALE_P2>
     ff6:	00 
     ff7:	c5 d5 fe 2d 01 51 20 	vpaddd 0x205101(%rip),%ymm5,%ymm5        # 206100 <PD_DESCALE_P2>
     ffe:	00 
     fff:	c5 f5 72 e1 0f       	vpsrad $0xf,%ymm1,%ymm1
    1004:	c5 d5 72 e5 0f       	vpsrad $0xf,%ymm5,%ymm5
    1009:	c5 f5 6b d5          	vpackssdw %ymm5,%ymm1,%ymm2
    100d:	c5 c5 fd f3          	vpaddw %ymm3,%ymm7,%ymm6
    1011:	c4 e3 4d 46 ce 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm1
    1017:	c5 cd 61 e9          	vpunpcklwd %ymm1,%ymm6,%ymm5
    101b:	c5 cd 69 f1          	vpunpckhwd %ymm1,%ymm6,%ymm6
    101f:	c5 d5 f5 2d 59 50 20 	vpmaddwd 0x205059(%rip),%ymm5,%ymm5        # 206080 <PW_MF078_F117_F078_F117>
    1026:	00 
    1027:	c5 cd f5 35 51 50 20 	vpmaddwd 0x205051(%rip),%ymm6,%ymm6        # 206080 <PW_MF078_F117_F078_F117>
    102e:	00 
    102f:	c4 e3 65 46 e3 01    	vperm2i128 $0x1,%ymm3,%ymm3,%ymm4
    1035:	c5 c5 61 cc          	vpunpcklwd %ymm4,%ymm7,%ymm1
    1039:	c5 c5 69 e4          	vpunpckhwd %ymm4,%ymm7,%ymm4
    103d:	c5 f5 f5 0d 5b 50 20 	vpmaddwd 0x20505b(%rip),%ymm1,%ymm1        # 2060a0 <PW_MF060_MF089_MF050_MF256>
    1044:	00 
    1045:	c5 dd f5 25 53 50 20 	vpmaddwd 0x205053(%rip),%ymm4,%ymm4        # 2060a0 <PW_MF060_MF089_MF050_MF256>
    104c:	00 
    104d:	c5 f5 fe cd          	vpaddd %ymm5,%ymm1,%ymm1
    1051:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
    1055:	c5 f5 fe 0d a3 50 20 	vpaddd 0x2050a3(%rip),%ymm1,%ymm1        # 206100 <PD_DESCALE_P2>
    105c:	00 
    105d:	c5 dd fe 25 9b 50 20 	vpaddd 0x20509b(%rip),%ymm4,%ymm4        # 206100 <PD_DESCALE_P2>
    1064:	00 
    1065:	c5 f5 72 e1 0f       	vpsrad $0xf,%ymm1,%ymm1
    106a:	c5 dd 72 e4 0f       	vpsrad $0xf,%ymm4,%ymm4
    106f:	c5 f5 6b e4          	vpackssdw %ymm4,%ymm1,%ymm4
    1073:	c4 e3 45 46 cf 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm1
    1079:	c5 e5 61 f9          	vpunpcklwd %ymm1,%ymm3,%ymm7
    107d:	c5 e5 69 d9          	vpunpckhwd %ymm1,%ymm3,%ymm3
    1081:	c5 c5 f5 3d 37 50 20 	vpmaddwd 0x205037(%rip),%ymm7,%ymm7        # 2060c0 <PW_F050_MF256_F060_MF089>
    1088:	00 
    1089:	c5 e5 f5 1d 2f 50 20 	vpmaddwd 0x20502f(%rip),%ymm3,%ymm3        # 2060c0 <PW_F050_MF256_F060_MF089>
    1090:	00 
    1091:	c5 c5 fe fd          	vpaddd %ymm5,%ymm7,%ymm7
    1095:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
    1099:	c5 c5 fe 3d 5f 50 20 	vpaddd 0x20505f(%rip),%ymm7,%ymm7        # 206100 <PD_DESCALE_P2>
    10a0:	00 
    10a1:	c5 e5 fe 1d 57 50 20 	vpaddd 0x205057(%rip),%ymm3,%ymm3        # 206100 <PD_DESCALE_P2>
    10a8:	00 
    10a9:	c5 c5 72 e7 0f       	vpsrad $0xf,%ymm7,%ymm7
    10ae:	c5 e5 72 e3 0f       	vpsrad $0xf,%ymm3,%ymm3
    10b3:	c5 c5 6b cb          	vpackssdw %ymm3,%ymm7,%ymm1
    10b7:	c4 e3 7d 46 d9 30    	vperm2i128 $0x30,%ymm1,%ymm0,%ymm3
    10bd:	c4 e3 6d 46 e9 20    	vperm2i128 $0x20,%ymm1,%ymm2,%ymm5
    10c3:	c4 e3 7d 46 f4 31    	vperm2i128 $0x31,%ymm4,%ymm0,%ymm6
    10c9:	c4 e3 6d 46 fc 21    	vperm2i128 $0x21,%ymm4,%ymm2,%ymm7
    10cf:	c4 c1 7e 7f 1a       	vmovdqu %ymm3,(%r10)
    10d4:	c4 c1 7e 7f 6a 20    	vmovdqu %ymm5,0x20(%r10)
    10da:	c4 c1 7e 7f 72 40    	vmovdqu %ymm6,0x40(%r10)
    10e0:	c4 c1 7e 7f 7a 60    	vmovdqu %ymm7,0x60(%r10)
    10e6:	c5 f8 77             	vzeroupper 
    10e9:	41 5a                	pop    %r10
    10eb:	5d                   	pop    %rbp
    10ec:	c3                   	retq   
    10ed:	90                   	nop
    10ee:	90                   	nop
    10ef:	90                   	nop
    10f0:	90                   	nop
    10f1:	90                   	nop
    10f2:	90                   	nop
    10f3:	90                   	nop
    10f4:	90                   	nop
    10f5:	90                   	nop
    10f6:	90                   	nop
    10f7:	90                   	nop
    10f8:	90                   	nop
    10f9:	90                   	nop
    10fa:	90                   	nop
    10fb:	90                   	nop
    10fc:	90                   	nop
    10fd:	90                   	nop
    10fe:	90                   	nop
    10ff:	90                   	nop

0000000000001100 <_jsimd_idct_ifast_sse2>:
    1100:	55                   	push   %rbp
    1101:	48 89 e0             	mov    %rsp,%rax
    1104:	48 83 ec 04          	sub    $0x4,%rsp
    1108:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    110c:	48 89 04 24          	mov    %rax,(%rsp)
    1110:	48 89 e5             	mov    %rsp,%rbp
    1113:	48 8d 65 e0          	lea    -0x20(%rbp),%rsp
    1117:	41 52                	push   %r10
    1119:	49 89 fa             	mov    %rdi,%r10
    111c:	41 53                	push   %r11
    111e:	49 89 f3             	mov    %rsi,%r11
    1121:	41 54                	push   %r12
    1123:	49 89 d4             	mov    %rdx,%r12
    1126:	41 55                	push   %r13
    1128:	49 89 cd             	mov    %rcx,%r13
    112b:	4c 89 d2             	mov    %r10,%rdx
    112e:	4c 89 de             	mov    %r11,%rsi
    1131:	8b 46 10             	mov    0x10(%rsi),%eax
    1134:	0b 46 20             	or     0x20(%rsi),%eax
    1137:	0f 85 83 00 00 00    	jne    11c0 <_jsimd_idct_ifast_sse2.columnDCT>
    113d:	66 0f 6f 46 10       	movdqa 0x10(%rsi),%xmm0
    1142:	66 0f 6f 4e 20       	movdqa 0x20(%rsi),%xmm1
    1147:	66 0f eb 46 30       	por    0x30(%rsi),%xmm0
    114c:	66 0f eb 4e 40       	por    0x40(%rsi),%xmm1
    1151:	66 0f eb 46 50       	por    0x50(%rsi),%xmm0
    1156:	66 0f eb 4e 60       	por    0x60(%rsi),%xmm1
    115b:	66 0f eb 46 70       	por    0x70(%rsi),%xmm0
    1160:	66 0f eb c8          	por    %xmm0,%xmm1
    1164:	66 0f 63 c9          	packsswb %xmm1,%xmm1
    1168:	66 0f 63 c9          	packsswb %xmm1,%xmm1
    116c:	66 0f 7e c8          	movd   %xmm1,%eax
    1170:	48 85 c0             	test   %rax,%rax
    1173:	75 4b                	jne    11c0 <_jsimd_idct_ifast_sse2.columnDCT>
    1175:	66 0f 6f 06          	movdqa (%rsi),%xmm0
    1179:	66 0f d5 02          	pmullw (%rdx),%xmm0
    117d:	66 0f 6f f8          	movdqa %xmm0,%xmm7
    1181:	66 0f 61 c0          	punpcklwd %xmm0,%xmm0
    1185:	66 0f 69 ff          	punpckhwd %xmm7,%xmm7
    1189:	66 0f 70 f0 00       	pshufd $0x0,%xmm0,%xmm6
    118e:	66 0f 70 d0 55       	pshufd $0x55,%xmm0,%xmm2
    1193:	66 0f 70 e8 aa       	pshufd $0xaa,%xmm0,%xmm5
    1198:	66 0f 70 c0 ff       	pshufd $0xff,%xmm0,%xmm0
    119d:	66 0f 70 cf 00       	pshufd $0x0,%xmm7,%xmm1
    11a2:	66 0f 70 e7 55       	pshufd $0x55,%xmm7,%xmm4
    11a7:	66 0f 70 df aa       	pshufd $0xaa,%xmm7,%xmm3
    11ac:	66 0f 70 ff ff       	pshufd $0xff,%xmm7,%xmm7
    11b1:	66 0f 7f 55 e0       	movdqa %xmm2,-0x20(%rbp)
    11b6:	66 0f 7f 45 f0       	movdqa %xmm0,-0x10(%rbp)
    11bb:	e9 0c 02 00 00       	jmpq   13cc <_jsimd_idct_ifast_sse2.column_end>

00000000000011c0 <_jsimd_idct_ifast_sse2.columnDCT>:
    11c0:	66 0f 6f 06          	movdqa (%rsi),%xmm0
    11c4:	66 0f 6f 4e 20       	movdqa 0x20(%rsi),%xmm1
    11c9:	66 0f d5 02          	pmullw (%rdx),%xmm0
    11cd:	66 0f d5 4a 20       	pmullw 0x20(%rdx),%xmm1
    11d2:	66 0f 6f 56 40       	movdqa 0x40(%rsi),%xmm2
    11d7:	66 0f 6f 5e 60       	movdqa 0x60(%rsi),%xmm3
    11dc:	66 0f d5 52 40       	pmullw 0x40(%rdx),%xmm2
    11e1:	66 0f d5 5a 60       	pmullw 0x60(%rdx),%xmm3
    11e6:	66 0f 6f e0          	movdqa %xmm0,%xmm4
    11ea:	66 0f 6f e9          	movdqa %xmm1,%xmm5
    11ee:	66 0f f9 c2          	psubw  %xmm2,%xmm0
    11f2:	66 0f f9 cb          	psubw  %xmm3,%xmm1
    11f6:	66 0f fd e2          	paddw  %xmm2,%xmm4
    11fa:	66 0f fd eb          	paddw  %xmm3,%xmm5
    11fe:	66 0f 71 f1 02       	psllw  $0x2,%xmm1
    1203:	66 0f e5 0d 55 4f 20 	pmulhw 0x204f55(%rip),%xmm1        # 206160 <_jconst_idct_ifast_sse2>
    120a:	00 
    120b:	66 0f f9 cd          	psubw  %xmm5,%xmm1
    120f:	66 0f 6f f4          	movdqa %xmm4,%xmm6
    1213:	66 0f 6f f8          	movdqa %xmm0,%xmm7
    1217:	66 0f f9 e5          	psubw  %xmm5,%xmm4
    121b:	66 0f f9 c1          	psubw  %xmm1,%xmm0
    121f:	66 0f fd f5          	paddw  %xmm5,%xmm6
    1223:	66 0f fd f9          	paddw  %xmm1,%xmm7
    1227:	66 0f 7f 65 f0       	movdqa %xmm4,-0x10(%rbp)
    122c:	66 0f 7f 45 e0       	movdqa %xmm0,-0x20(%rbp)
    1231:	66 0f 6f 56 10       	movdqa 0x10(%rsi),%xmm2
    1236:	66 0f 6f 5e 30       	movdqa 0x30(%rsi),%xmm3
    123b:	66 0f d5 52 10       	pmullw 0x10(%rdx),%xmm2
    1240:	66 0f d5 5a 30       	pmullw 0x30(%rdx),%xmm3
    1245:	66 0f 6f 6e 50       	movdqa 0x50(%rsi),%xmm5
    124a:	66 0f 6f 4e 70       	movdqa 0x70(%rsi),%xmm1
    124f:	66 0f d5 6a 50       	pmullw 0x50(%rdx),%xmm5
    1254:	66 0f d5 4a 70       	pmullw 0x70(%rdx),%xmm1
    1259:	66 0f 6f e2          	movdqa %xmm2,%xmm4
    125d:	66 0f 6f c5          	movdqa %xmm5,%xmm0
    1261:	66 0f f9 d1          	psubw  %xmm1,%xmm2
    1265:	66 0f f9 eb          	psubw  %xmm3,%xmm5
    1269:	66 0f fd e1          	paddw  %xmm1,%xmm4
    126d:	66 0f fd c3          	paddw  %xmm3,%xmm0
    1271:	66 0f 6f cd          	movdqa %xmm5,%xmm1
    1275:	66 0f 71 f2 02       	psllw  $0x2,%xmm2
    127a:	66 0f 71 f5 02       	psllw  $0x2,%xmm5
    127f:	66 0f 6f dc          	movdqa %xmm4,%xmm3
    1283:	66 0f f9 e0          	psubw  %xmm0,%xmm4
    1287:	66 0f fd d8          	paddw  %xmm0,%xmm3
    128b:	66 0f 71 f4 02       	psllw  $0x2,%xmm4
    1290:	66 0f e5 25 c8 4e 20 	pmulhw 0x204ec8(%rip),%xmm4        # 206160 <_jconst_idct_ifast_sse2>
    1297:	00 
    1298:	66 0f 6f c5          	movdqa %xmm5,%xmm0
    129c:	66 0f fd ea          	paddw  %xmm2,%xmm5
    12a0:	66 0f e5 2d c8 4e 20 	pmulhw 0x204ec8(%rip),%xmm5        # 206170 <PW_F1847>
    12a7:	00 
    12a8:	66 0f e5 05 d0 4e 20 	pmulhw 0x204ed0(%rip),%xmm0        # 206180 <PW_MF1613>
    12af:	00 
    12b0:	66 0f e5 15 d8 4e 20 	pmulhw 0x204ed8(%rip),%xmm2        # 206190 <PW_F1082>
    12b7:	00 
    12b8:	66 0f f9 c1          	psubw  %xmm1,%xmm0
    12bc:	66 0f f9 d5          	psubw  %xmm5,%xmm2
    12c0:	66 0f fd c5          	paddw  %xmm5,%xmm0
    12c4:	66 0f f9 c3          	psubw  %xmm3,%xmm0
    12c8:	66 0f 6f ce          	movdqa %xmm6,%xmm1
    12cc:	66 0f 6f ef          	movdqa %xmm7,%xmm5
    12d0:	66 0f fd f3          	paddw  %xmm3,%xmm6
    12d4:	66 0f fd f8          	paddw  %xmm0,%xmm7
    12d8:	66 0f f9 cb          	psubw  %xmm3,%xmm1
    12dc:	66 0f f9 e8          	psubw  %xmm0,%xmm5
    12e0:	66 0f f9 e0          	psubw  %xmm0,%xmm4
    12e4:	66 0f 6f de          	movdqa %xmm6,%xmm3
    12e8:	66 0f 61 f7          	punpcklwd %xmm7,%xmm6
    12ec:	66 0f 69 df          	punpckhwd %xmm7,%xmm3
    12f0:	66 0f 6f c5          	movdqa %xmm5,%xmm0
    12f4:	66 0f 61 e9          	punpcklwd %xmm1,%xmm5
    12f8:	66 0f 69 c1          	punpckhwd %xmm1,%xmm0
    12fc:	66 0f 6f 7d e0       	movdqa -0x20(%rbp),%xmm7
    1301:	66 0f 6f 4d f0       	movdqa -0x10(%rbp),%xmm1
    1306:	66 0f 7f 6d e0       	movdqa %xmm5,-0x20(%rbp)
    130b:	66 0f 7f 45 f0       	movdqa %xmm0,-0x10(%rbp)
    1310:	66 0f fd d4          	paddw  %xmm4,%xmm2
    1314:	66 0f 6f ef          	movdqa %xmm7,%xmm5
    1318:	66 0f 6f c1          	movdqa %xmm1,%xmm0
    131c:	66 0f fd fc          	paddw  %xmm4,%xmm7
    1320:	66 0f fd ca          	paddw  %xmm2,%xmm1
    1324:	66 0f f9 ec          	psubw  %xmm4,%xmm5
    1328:	66 0f f9 c2          	psubw  %xmm2,%xmm0
    132c:	66 0f 6f e7          	movdqa %xmm7,%xmm4
    1330:	66 0f 61 f8          	punpcklwd %xmm0,%xmm7
    1334:	66 0f 69 e0          	punpckhwd %xmm0,%xmm4
    1338:	66 0f 6f d1          	movdqa %xmm1,%xmm2
    133c:	66 0f 61 cd          	punpcklwd %xmm5,%xmm1
    1340:	66 0f 69 d5          	punpckhwd %xmm5,%xmm2
    1344:	66 0f 6f c3          	movdqa %xmm3,%xmm0
    1348:	66 0f 62 dc          	punpckldq %xmm4,%xmm3
    134c:	66 0f 6a c4          	punpckhdq %xmm4,%xmm0
    1350:	66 0f 6f ee          	movdqa %xmm6,%xmm5
    1354:	66 0f 62 f7          	punpckldq %xmm7,%xmm6
    1358:	66 0f 6a ef          	punpckhdq %xmm7,%xmm5
    135c:	66 0f 6f 65 e0       	movdqa -0x20(%rbp),%xmm4
    1361:	66 0f 6f 7d f0       	movdqa -0x10(%rbp),%xmm7
    1366:	66 0f 7f 5d e0       	movdqa %xmm3,-0x20(%rbp)
    136b:	66 0f 7f 45 f0       	movdqa %xmm0,-0x10(%rbp)
    1370:	66 0f 6f d9          	movdqa %xmm1,%xmm3
    1374:	66 0f 62 cc          	punpckldq %xmm4,%xmm1
    1378:	66 0f 6a dc          	punpckhdq %xmm4,%xmm3
    137c:	66 0f 6f c2          	movdqa %xmm2,%xmm0
    1380:	66 0f 62 d7          	punpckldq %xmm7,%xmm2
    1384:	66 0f 6a c7          	punpckhdq %xmm7,%xmm0
    1388:	66 0f 6f e6          	movdqa %xmm6,%xmm4
    138c:	66 0f 6c f1          	punpcklqdq %xmm1,%xmm6
    1390:	66 0f 6d e1          	punpckhqdq %xmm1,%xmm4
    1394:	66 0f 6f fd          	movdqa %xmm5,%xmm7
    1398:	66 0f 6c eb          	punpcklqdq %xmm3,%xmm5
    139c:	66 0f 6d fb          	punpckhqdq %xmm3,%xmm7
    13a0:	66 0f 6f 4d e0       	movdqa -0x20(%rbp),%xmm1
    13a5:	66 0f 6f 5d f0       	movdqa -0x10(%rbp),%xmm3
    13aa:	66 0f 7f 65 e0       	movdqa %xmm4,-0x20(%rbp)
    13af:	66 0f 7f 7d f0       	movdqa %xmm7,-0x10(%rbp)
    13b4:	66 0f 6f e1          	movdqa %xmm1,%xmm4
    13b8:	66 0f 6c ca          	punpcklqdq %xmm2,%xmm1
    13bc:	66 0f 6d e2          	punpckhqdq %xmm2,%xmm4
    13c0:	66 0f 6f fb          	movdqa %xmm3,%xmm7
    13c4:	66 0f 6c d8          	punpcklqdq %xmm0,%xmm3
    13c8:	66 0f 6d f8          	punpckhqdq %xmm0,%xmm7

00000000000013cc <_jsimd_idct_ifast_sse2.column_end>:
    13cc:	0f 18 86 80 00 00 00 	prefetchnta 0x80(%rsi)
    13d3:	0f 18 86 a0 00 00 00 	prefetchnta 0xa0(%rsi)
    13da:	0f 18 86 c0 00 00 00 	prefetchnta 0xc0(%rsi)
    13e1:	0f 18 86 e0 00 00 00 	prefetchnta 0xe0(%rsi)
    13e8:	48 8b 45 00          	mov    0x0(%rbp),%rax
    13ec:	4c 89 e7             	mov    %r12,%rdi
    13ef:	44 89 e8             	mov    %r13d,%eax
    13f2:	66 0f 6f d6          	movdqa %xmm6,%xmm2
    13f6:	66 0f 6f c5          	movdqa %xmm5,%xmm0
    13fa:	66 0f f9 f1          	psubw  %xmm1,%xmm6
    13fe:	66 0f f9 eb          	psubw  %xmm3,%xmm5
    1402:	66 0f fd d1          	paddw  %xmm1,%xmm2
    1406:	66 0f fd c3          	paddw  %xmm3,%xmm0
    140a:	66 0f 71 f5 02       	psllw  $0x2,%xmm5
    140f:	66 0f e5 2d 49 4d 20 	pmulhw 0x204d49(%rip),%xmm5        # 206160 <_jconst_idct_ifast_sse2>
    1416:	00 
    1417:	66 0f f9 e8          	psubw  %xmm0,%xmm5
    141b:	66 0f 6f ca          	movdqa %xmm2,%xmm1
    141f:	66 0f 6f de          	movdqa %xmm6,%xmm3
    1423:	66 0f f9 d0          	psubw  %xmm0,%xmm2
    1427:	66 0f f9 f5          	psubw  %xmm5,%xmm6
    142b:	66 0f fd c8          	paddw  %xmm0,%xmm1
    142f:	66 0f fd dd          	paddw  %xmm5,%xmm3
    1433:	66 0f 6f 45 e0       	movdqa -0x20(%rbp),%xmm0
    1438:	66 0f 6f 6d f0       	movdqa -0x10(%rbp),%xmm5
    143d:	66 0f 7f 55 e0       	movdqa %xmm2,-0x20(%rbp)
    1442:	66 0f 7f 75 f0       	movdqa %xmm6,-0x10(%rbp)
    1447:	66 0f 6f d0          	movdqa %xmm0,%xmm2
    144b:	66 0f 6f f4          	movdqa %xmm4,%xmm6
    144f:	66 0f f9 c7          	psubw  %xmm7,%xmm0
    1453:	66 0f f9 e5          	psubw  %xmm5,%xmm4
    1457:	66 0f fd d7          	paddw  %xmm7,%xmm2
    145b:	66 0f fd f5          	paddw  %xmm5,%xmm6
    145f:	66 0f 6f fc          	movdqa %xmm4,%xmm7
    1463:	66 0f 71 f0 02       	psllw  $0x2,%xmm0
    1468:	66 0f 71 f4 02       	psllw  $0x2,%xmm4
    146d:	66 0f 6f ea          	movdqa %xmm2,%xmm5
    1471:	66 0f f9 d6          	psubw  %xmm6,%xmm2
    1475:	66 0f fd ee          	paddw  %xmm6,%xmm5
    1479:	66 0f 71 f2 02       	psllw  $0x2,%xmm2
    147e:	66 0f e5 15 da 4c 20 	pmulhw 0x204cda(%rip),%xmm2        # 206160 <_jconst_idct_ifast_sse2>
    1485:	00 
    1486:	66 0f 6f f4          	movdqa %xmm4,%xmm6
    148a:	66 0f fd e0          	paddw  %xmm0,%xmm4
    148e:	66 0f e5 25 da 4c 20 	pmulhw 0x204cda(%rip),%xmm4        # 206170 <PW_F1847>
    1495:	00 
    1496:	66 0f e5 35 e2 4c 20 	pmulhw 0x204ce2(%rip),%xmm6        # 206180 <PW_MF1613>
    149d:	00 
    149e:	66 0f e5 05 ea 4c 20 	pmulhw 0x204cea(%rip),%xmm0        # 206190 <PW_F1082>
    14a5:	00 
    14a6:	66 0f f9 f7          	psubw  %xmm7,%xmm6
    14aa:	66 0f f9 c4          	psubw  %xmm4,%xmm0
    14ae:	66 0f fd f4          	paddw  %xmm4,%xmm6
    14b2:	66 0f f9 f5          	psubw  %xmm5,%xmm6
    14b6:	66 0f 6f f9          	movdqa %xmm1,%xmm7
    14ba:	66 0f 6f e3          	movdqa %xmm3,%xmm4
    14be:	66 0f fd cd          	paddw  %xmm5,%xmm1
    14c2:	66 0f fd de          	paddw  %xmm6,%xmm3
    14c6:	66 0f 71 e1 05       	psraw  $0x5,%xmm1
    14cb:	66 0f 71 e3 05       	psraw  $0x5,%xmm3
    14d0:	66 0f f9 fd          	psubw  %xmm5,%xmm7
    14d4:	66 0f f9 e6          	psubw  %xmm6,%xmm4
    14d8:	66 0f 71 e7 05       	psraw  $0x5,%xmm7
    14dd:	66 0f 71 e4 05       	psraw  $0x5,%xmm4
    14e2:	66 0f f9 d6          	psubw  %xmm6,%xmm2
    14e6:	66 0f 63 cc          	packsswb %xmm4,%xmm1
    14ea:	66 0f 63 df          	packsswb %xmm7,%xmm3
    14ee:	66 0f 6f 6d f0       	movdqa -0x10(%rbp),%xmm5
    14f3:	66 0f 6f 75 e0       	movdqa -0x20(%rbp),%xmm6
    14f8:	66 0f fd c2          	paddw  %xmm2,%xmm0
    14fc:	66 0f 6f e5          	movdqa %xmm5,%xmm4
    1500:	66 0f 6f fe          	movdqa %xmm6,%xmm7
    1504:	66 0f fd ea          	paddw  %xmm2,%xmm5
    1508:	66 0f fd f0          	paddw  %xmm0,%xmm6
    150c:	66 0f 71 e5 05       	psraw  $0x5,%xmm5
    1511:	66 0f 71 e6 05       	psraw  $0x5,%xmm6
    1516:	66 0f f9 e2          	psubw  %xmm2,%xmm4
    151a:	66 0f f9 f8          	psubw  %xmm0,%xmm7
    151e:	66 0f 71 e4 05       	psraw  $0x5,%xmm4
    1523:	66 0f 71 e7 05       	psraw  $0x5,%xmm7
    1528:	66 0f 6f 15 70 4c 20 	movdqa 0x204c70(%rip),%xmm2        # 2061a0 <PB_CENTERJSAMP>
    152f:	00 
    1530:	66 0f 63 ee          	packsswb %xmm6,%xmm5
    1534:	66 0f 63 fc          	packsswb %xmm4,%xmm7
    1538:	66 0f fc ca          	paddb  %xmm2,%xmm1
    153c:	66 0f fc da          	paddb  %xmm2,%xmm3
    1540:	66 0f fc ea          	paddb  %xmm2,%xmm5
    1544:	66 0f fc fa          	paddb  %xmm2,%xmm7
    1548:	66 0f 6f c1          	movdqa %xmm1,%xmm0
    154c:	66 0f 60 cb          	punpcklbw %xmm3,%xmm1
    1550:	66 0f 68 c3          	punpckhbw %xmm3,%xmm0
    1554:	66 0f 6f f5          	movdqa %xmm5,%xmm6
    1558:	66 0f 60 ef          	punpcklbw %xmm7,%xmm5
    155c:	66 0f 68 f7          	punpckhbw %xmm7,%xmm6
    1560:	66 0f 6f e1          	movdqa %xmm1,%xmm4
    1564:	66 0f 61 cd          	punpcklwd %xmm5,%xmm1
    1568:	66 0f 69 e5          	punpckhwd %xmm5,%xmm4
    156c:	66 0f 6f d6          	movdqa %xmm6,%xmm2
    1570:	66 0f 61 f0          	punpcklwd %xmm0,%xmm6
    1574:	66 0f 69 d0          	punpckhwd %xmm0,%xmm2
    1578:	66 0f 6f d9          	movdqa %xmm1,%xmm3
    157c:	66 0f 62 ce          	punpckldq %xmm6,%xmm1
    1580:	66 0f 6a de          	punpckhdq %xmm6,%xmm3
    1584:	66 0f 6f fc          	movdqa %xmm4,%xmm7
    1588:	66 0f 62 e2          	punpckldq %xmm2,%xmm4
    158c:	66 0f 6a fa          	punpckhdq %xmm2,%xmm7
    1590:	66 0f 70 e9 4e       	pshufd $0x4e,%xmm1,%xmm5
    1595:	66 0f 70 c3 4e       	pshufd $0x4e,%xmm3,%xmm0
    159a:	66 0f 70 f4 4e       	pshufd $0x4e,%xmm4,%xmm6
    159f:	66 0f 70 d7 4e       	pshufd $0x4e,%xmm7,%xmm2
    15a4:	48 8b 17             	mov    (%rdi),%rdx
    15a7:	48 8b 77 10          	mov    0x10(%rdi),%rsi
    15ab:	66 0f d6 0c 02       	movq   %xmm1,(%rdx,%rax,1)
    15b0:	66 0f d6 1c 06       	movq   %xmm3,(%rsi,%rax,1)
    15b5:	48 8b 57 20          	mov    0x20(%rdi),%rdx
    15b9:	48 8b 77 30          	mov    0x30(%rdi),%rsi
    15bd:	66 0f d6 24 02       	movq   %xmm4,(%rdx,%rax,1)
    15c2:	66 0f d6 3c 06       	movq   %xmm7,(%rsi,%rax,1)
    15c7:	48 8b 57 08          	mov    0x8(%rdi),%rdx
    15cb:	48 8b 77 18          	mov    0x18(%rdi),%rsi
    15cf:	66 0f d6 2c 02       	movq   %xmm5,(%rdx,%rax,1)
    15d4:	66 0f d6 04 06       	movq   %xmm0,(%rsi,%rax,1)
    15d9:	48 8b 57 28          	mov    0x28(%rdi),%rdx
    15dd:	48 8b 77 38          	mov    0x38(%rdi),%rsi
    15e1:	66 0f d6 34 02       	movq   %xmm6,(%rdx,%rax,1)
    15e6:	66 0f d6 14 06       	movq   %xmm2,(%rsi,%rax,1)
    15eb:	41 5d                	pop    %r13
    15ed:	41 5c                	pop    %r12
    15ef:	41 5b                	pop    %r11
    15f1:	41 5a                	pop    %r10
    15f3:	48 89 ec             	mov    %rbp,%rsp
    15f6:	5c                   	pop    %rsp
    15f7:	5d                   	pop    %rbp
    15f8:	c3                   	retq   
    15f9:	c3                   	retq   
    15fa:	90                   	nop
    15fb:	90                   	nop
    15fc:	90                   	nop
    15fd:	90                   	nop
    15fe:	90                   	nop
    15ff:	90                   	nop

0000000000001600 <_jsimd_idct_islow_avx2>:
    1600:	55                   	push   %rbp
    1601:	48 89 e0             	mov    %rsp,%rax
    1604:	48 89 e5             	mov    %rsp,%rbp
    1607:	41 52                	push   %r10
    1609:	49 89 fa             	mov    %rdi,%r10
    160c:	41 53                	push   %r11
    160e:	49 89 f3             	mov    %rsi,%r11
    1611:	41 54                	push   %r12
    1613:	49 89 d4             	mov    %rdx,%r12
    1616:	41 55                	push   %r13
    1618:	49 89 cd             	mov    %rcx,%r13
    161b:	41 8b 43 10          	mov    0x10(%r11),%eax
    161f:	41 0b 43 20          	or     0x20(%r11),%eax
    1623:	0f 85 75 00 00 00    	jne    169e <_jsimd_idct_islow_avx2.columnDCT>
    1629:	66 41 0f 6f 43 10    	movdqa 0x10(%r11),%xmm0
    162f:	66 41 0f 6f 4b 20    	movdqa 0x20(%r11),%xmm1
    1635:	c4 c1 79 eb 43 30    	vpor   0x30(%r11),%xmm0,%xmm0
    163b:	c4 c1 71 eb 4b 40    	vpor   0x40(%r11),%xmm1,%xmm1
    1641:	c4 c1 79 eb 43 50    	vpor   0x50(%r11),%xmm0,%xmm0
    1647:	c4 c1 71 eb 4b 60    	vpor   0x60(%r11),%xmm1,%xmm1
    164d:	c4 c1 79 eb 43 70    	vpor   0x70(%r11),%xmm0,%xmm0
    1653:	c5 f1 eb c8          	vpor   %xmm0,%xmm1,%xmm1
    1657:	c5 f1 63 c9          	vpacksswb %xmm1,%xmm1,%xmm1
    165b:	c5 f1 63 c9          	vpacksswb %xmm1,%xmm1,%xmm1
    165f:	66 0f 7e c8          	movd   %xmm1,%eax
    1663:	48 85 c0             	test   %rax,%rax
    1666:	75 36                	jne    169e <_jsimd_idct_islow_avx2.columnDCT>
    1668:	66 41 0f 6f 2b       	movdqa (%r11),%xmm5
    166d:	c4 c1 51 d5 2a       	vpmullw (%r10),%xmm5,%xmm5
    1672:	c5 d1 71 f5 02       	vpsllw $0x2,%xmm5,%xmm5
    1677:	c5 d1 61 e5          	vpunpcklwd %xmm5,%xmm5,%xmm4
    167b:	c5 d1 69 ed          	vpunpckhwd %xmm5,%xmm5,%xmm5
    167f:	c4 e3 5d 38 e5 01    	vinserti128 $0x1,%xmm5,%ymm4,%ymm4
    1685:	c5 fd 70 c4 00       	vpshufd $0x0,%ymm4,%ymm0
    168a:	c5 fd 70 cc 55       	vpshufd $0x55,%ymm4,%ymm1
    168f:	c5 fd 70 d4 aa       	vpshufd $0xaa,%ymm4,%ymm2
    1694:	c5 fd 70 dc ff       	vpshufd $0xff,%ymm4,%ymm3
    1699:	e9 e9 01 00 00       	jmpq   1887 <_jsimd_idct_islow_avx2.column_end>

000000000000169e <_jsimd_idct_islow_avx2.columnDCT>:
    169e:	c4 c1 7e 6f 23       	vmovdqu (%r11),%ymm4
    16a3:	c4 c1 7e 6f 6b 20    	vmovdqu 0x20(%r11),%ymm5
    16a9:	c4 c1 7e 6f 73 40    	vmovdqu 0x40(%r11),%ymm6
    16af:	c4 c1 7e 6f 7b 60    	vmovdqu 0x60(%r11),%ymm7
    16b5:	c4 c1 5d d5 22       	vpmullw (%r10),%ymm4,%ymm4
    16ba:	c4 c1 55 d5 6a 20    	vpmullw 0x20(%r10),%ymm5,%ymm5
    16c0:	c4 c1 4d d5 72 40    	vpmullw 0x40(%r10),%ymm6,%ymm6
    16c6:	c4 c1 45 d5 7a 60    	vpmullw 0x60(%r10),%ymm7,%ymm7
    16cc:	c4 e3 5d 46 c6 20    	vperm2i128 $0x20,%ymm6,%ymm4,%ymm0
    16d2:	c4 e3 55 46 cc 31    	vperm2i128 $0x31,%ymm4,%ymm5,%ymm1
    16d8:	c4 e3 55 46 d7 20    	vperm2i128 $0x20,%ymm7,%ymm5,%ymm2
    16de:	c4 e3 45 46 de 31    	vperm2i128 $0x31,%ymm6,%ymm7,%ymm3
    16e4:	c4 e3 6d 46 ea 01    	vperm2i128 $0x1,%ymm2,%ymm2,%ymm5
    16ea:	c5 ed 61 e5          	vpunpcklwd %ymm5,%ymm2,%ymm4
    16ee:	c5 ed 69 ed          	vpunpckhwd %ymm5,%ymm2,%ymm5
    16f2:	c5 dd f5 25 c6 4a 20 	vpmaddwd 0x204ac6(%rip),%ymm4,%ymm4        # 2061c0 <_jconst_idct_islow_avx2>
    16f9:	00 
    16fa:	c5 d5 f5 2d be 4a 20 	vpmaddwd 0x204abe(%rip),%ymm5,%ymm5        # 2061c0 <_jconst_idct_islow_avx2>
    1701:	00 
    1702:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    1708:	c4 e2 7d 09 05 8f 4b 	vpsignw 0x204b8f(%rip),%ymm0,%ymm0        # 2062a0 <PW_1_NEG1>
    170f:	20 00 
    1711:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    1715:	c5 fd ef c0          	vpxor  %ymm0,%ymm0,%ymm0
    1719:	c5 fd 61 fe          	vpunpcklwd %ymm6,%ymm0,%ymm7
    171d:	c5 fd 69 c6          	vpunpckhwd %ymm6,%ymm0,%ymm0
    1721:	c5 c5 72 e7 03       	vpsrad $0x3,%ymm7,%ymm7
    1726:	c5 fd 72 e0 03       	vpsrad $0x3,%ymm0,%ymm0
    172b:	c5 45 fa d4          	vpsubd %ymm4,%ymm7,%ymm10
    172f:	c5 45 fe c4          	vpaddd %ymm4,%ymm7,%ymm8
    1733:	c5 7d fa dd          	vpsubd %ymm5,%ymm0,%ymm11
    1737:	c5 7d fe cd          	vpaddd %ymm5,%ymm0,%ymm9
    173b:	c5 e5 fd c1          	vpaddw %ymm1,%ymm3,%ymm0
    173f:	c4 e3 7d 46 f8 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm7
    1745:	c5 fd 61 f7          	vpunpcklwd %ymm7,%ymm0,%ymm6
    1749:	c5 fd 69 ff          	vpunpckhwd %ymm7,%ymm0,%ymm7
    174d:	c5 cd f5 35 8b 4a 20 	vpmaddwd 0x204a8b(%rip),%ymm6,%ymm6        # 2061e0 <PW_MF078_F117_F078_F117>
    1754:	00 
    1755:	c5 c5 f5 3d 83 4a 20 	vpmaddwd 0x204a83(%rip),%ymm7,%ymm7        # 2061e0 <PW_MF078_F117_F078_F117>
    175c:	00 
    175d:	c4 e3 75 46 c9 01    	vperm2i128 $0x1,%ymm1,%ymm1,%ymm1
    1763:	c5 e5 61 d1          	vpunpcklwd %ymm1,%ymm3,%ymm2
    1767:	c5 e5 69 d9          	vpunpckhwd %ymm1,%ymm3,%ymm3
    176b:	c5 ed f5 25 8d 4a 20 	vpmaddwd 0x204a8d(%rip),%ymm2,%ymm4        # 206200 <PW_MF060_MF089_MF050_MF256>
    1772:	00 
    1773:	c5 e5 f5 2d 85 4a 20 	vpmaddwd 0x204a85(%rip),%ymm3,%ymm5        # 206200 <PW_MF060_MF089_MF050_MF256>
    177a:	00 
    177b:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
    177f:	c5 d5 fe ef          	vpaddd %ymm7,%ymm5,%ymm5
    1783:	c5 ed f5 15 95 4a 20 	vpmaddwd 0x204a95(%rip),%ymm2,%ymm2        # 206220 <PW_MF089_F060_MF256_F050>
    178a:	00 
    178b:	c5 e5 f5 1d 8d 4a 20 	vpmaddwd 0x204a8d(%rip),%ymm3,%ymm3        # 206220 <PW_MF089_F060_MF256_F050>
    1792:	00 
    1793:	c4 e3 4d 46 f6 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm6
    1799:	c4 e3 45 46 ff 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm7
    179f:	c5 ed fe f6          	vpaddd %ymm6,%ymm2,%ymm6
    17a3:	c5 e5 fe ff          	vpaddd %ymm7,%ymm3,%ymm7
    17a7:	c5 bd fe c6          	vpaddd %ymm6,%ymm8,%ymm0
    17ab:	c5 b5 fe cf          	vpaddd %ymm7,%ymm9,%ymm1
    17af:	c5 fd fe 05 89 4a 20 	vpaddd 0x204a89(%rip),%ymm0,%ymm0        # 206240 <PD_DESCALE_P1>
    17b6:	00 
    17b7:	c5 f5 fe 0d 81 4a 20 	vpaddd 0x204a81(%rip),%ymm1,%ymm1        # 206240 <PD_DESCALE_P1>
    17be:	00 
    17bf:	c5 fd 72 e0 0b       	vpsrad $0xb,%ymm0,%ymm0
    17c4:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
    17c9:	c5 fd 6b c1          	vpackssdw %ymm1,%ymm0,%ymm0
    17cd:	c5 bd fa d6          	vpsubd %ymm6,%ymm8,%ymm2
    17d1:	c5 b5 fa df          	vpsubd %ymm7,%ymm9,%ymm3
    17d5:	c5 ed fe 15 63 4a 20 	vpaddd 0x204a63(%rip),%ymm2,%ymm2        # 206240 <PD_DESCALE_P1>
    17dc:	00 
    17dd:	c5 e5 fe 1d 5b 4a 20 	vpaddd 0x204a5b(%rip),%ymm3,%ymm3        # 206240 <PD_DESCALE_P1>
    17e4:	00 
    17e5:	c5 ed 72 e2 0b       	vpsrad $0xb,%ymm2,%ymm2
    17ea:	c5 e5 72 e3 0b       	vpsrad $0xb,%ymm3,%ymm3
    17ef:	c5 ed 6b db          	vpackssdw %ymm3,%ymm2,%ymm3
    17f3:	c5 ad fe f4          	vpaddd %ymm4,%ymm10,%ymm6
    17f7:	c5 a5 fe fd          	vpaddd %ymm5,%ymm11,%ymm7
    17fb:	c5 cd fe 35 3d 4a 20 	vpaddd 0x204a3d(%rip),%ymm6,%ymm6        # 206240 <PD_DESCALE_P1>
    1802:	00 
    1803:	c5 c5 fe 3d 35 4a 20 	vpaddd 0x204a35(%rip),%ymm7,%ymm7        # 206240 <PD_DESCALE_P1>
    180a:	00 
    180b:	c5 cd 72 e6 0b       	vpsrad $0xb,%ymm6,%ymm6
    1810:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
    1815:	c5 cd 6b cf          	vpackssdw %ymm7,%ymm6,%ymm1
    1819:	c5 ad fa f4          	vpsubd %ymm4,%ymm10,%ymm6
    181d:	c5 a5 fa fd          	vpsubd %ymm5,%ymm11,%ymm7
    1821:	c5 cd fe 35 17 4a 20 	vpaddd 0x204a17(%rip),%ymm6,%ymm6        # 206240 <PD_DESCALE_P1>
    1828:	00 
    1829:	c5 c5 fe 3d 0f 4a 20 	vpaddd 0x204a0f(%rip),%ymm7,%ymm7        # 206240 <PD_DESCALE_P1>
    1830:	00 
    1831:	c5 cd 72 e6 0b       	vpsrad $0xb,%ymm6,%ymm6
    1836:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
    183b:	c5 cd 6b d7          	vpackssdw %ymm7,%ymm6,%ymm2
    183f:	c4 e3 fd 00 e0 d8    	vpermq $0xd8,%ymm0,%ymm4
    1845:	c4 e3 fd 00 e9 72    	vpermq $0x72,%ymm1,%ymm5
    184b:	c4 e3 fd 00 f2 d8    	vpermq $0xd8,%ymm2,%ymm6
    1851:	c4 e3 fd 00 fb 72    	vpermq $0x72,%ymm3,%ymm7
    1857:	c5 dd 61 c5          	vpunpcklwd %ymm5,%ymm4,%ymm0
    185b:	c5 dd 69 cd          	vpunpckhwd %ymm5,%ymm4,%ymm1
    185f:	c5 cd 61 d7          	vpunpcklwd %ymm7,%ymm6,%ymm2
    1863:	c5 cd 69 df          	vpunpckhwd %ymm7,%ymm6,%ymm3
    1867:	c5 fd 61 e1          	vpunpcklwd %ymm1,%ymm0,%ymm4
    186b:	c5 ed 61 eb          	vpunpcklwd %ymm3,%ymm2,%ymm5
    186f:	c5 fd 69 f1          	vpunpckhwd %ymm1,%ymm0,%ymm6
    1873:	c5 ed 69 fb          	vpunpckhwd %ymm3,%ymm2,%ymm7
    1877:	c5 dd 6c c5          	vpunpcklqdq %ymm5,%ymm4,%ymm0
    187b:	c5 dd 6d cd          	vpunpckhqdq %ymm5,%ymm4,%ymm1
    187f:	c5 cd 6c d7          	vpunpcklqdq %ymm7,%ymm6,%ymm2
    1883:	c5 cd 6d df          	vpunpckhqdq %ymm7,%ymm6,%ymm3

0000000000001887 <_jsimd_idct_islow_avx2.column_end>:
    1887:	41 0f 18 83 80 00 00 	prefetchnta 0x80(%r11)
    188e:	00 
    188f:	41 0f 18 83 a0 00 00 	prefetchnta 0xa0(%r11)
    1896:	00 
    1897:	41 0f 18 83 c0 00 00 	prefetchnta 0xc0(%r11)
    189e:	00 
    189f:	41 0f 18 83 e0 00 00 	prefetchnta 0xe0(%r11)
    18a6:	00 
    18a7:	c4 e3 65 46 e1 31    	vperm2i128 $0x31,%ymm1,%ymm3,%ymm4
    18ad:	c4 e3 65 46 c9 20    	vperm2i128 $0x20,%ymm1,%ymm3,%ymm1
    18b3:	c4 e3 6d 46 ea 01    	vperm2i128 $0x1,%ymm2,%ymm2,%ymm5
    18b9:	c5 ed 61 dd          	vpunpcklwd %ymm5,%ymm2,%ymm3
    18bd:	c5 ed 69 ed          	vpunpckhwd %ymm5,%ymm2,%ymm5
    18c1:	c5 e5 f5 1d f7 48 20 	vpmaddwd 0x2048f7(%rip),%ymm3,%ymm3        # 2061c0 <_jconst_idct_islow_avx2>
    18c8:	00 
    18c9:	c5 d5 f5 2d ef 48 20 	vpmaddwd 0x2048ef(%rip),%ymm5,%ymm5        # 2061c0 <_jconst_idct_islow_avx2>
    18d0:	00 
    18d1:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    18d7:	c4 e2 7d 09 05 c0 49 	vpsignw 0x2049c0(%rip),%ymm0,%ymm0        # 2062a0 <PW_1_NEG1>
    18de:	20 00 
    18e0:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    18e4:	c5 fd ef c0          	vpxor  %ymm0,%ymm0,%ymm0
    18e8:	c5 fd 61 fe          	vpunpcklwd %ymm6,%ymm0,%ymm7
    18ec:	c5 fd 69 c6          	vpunpckhwd %ymm6,%ymm0,%ymm0
    18f0:	c5 c5 72 e7 03       	vpsrad $0x3,%ymm7,%ymm7
    18f5:	c5 fd 72 e0 03       	vpsrad $0x3,%ymm0,%ymm0
    18fa:	c5 45 fa d3          	vpsubd %ymm3,%ymm7,%ymm10
    18fe:	c5 45 fe c3          	vpaddd %ymm3,%ymm7,%ymm8
    1902:	c5 7d fa dd          	vpsubd %ymm5,%ymm0,%ymm11
    1906:	c5 7d fe cd          	vpaddd %ymm5,%ymm0,%ymm9
    190a:	c5 dd fd c1          	vpaddw %ymm1,%ymm4,%ymm0
    190e:	c4 e3 7d 46 f8 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm7
    1914:	c5 fd 61 f7          	vpunpcklwd %ymm7,%ymm0,%ymm6
    1918:	c5 fd 69 ff          	vpunpckhwd %ymm7,%ymm0,%ymm7
    191c:	c5 cd f5 35 bc 48 20 	vpmaddwd 0x2048bc(%rip),%ymm6,%ymm6        # 2061e0 <PW_MF078_F117_F078_F117>
    1923:	00 
    1924:	c5 c5 f5 3d b4 48 20 	vpmaddwd 0x2048b4(%rip),%ymm7,%ymm7        # 2061e0 <PW_MF078_F117_F078_F117>
    192b:	00 
    192c:	c4 e3 75 46 c9 01    	vperm2i128 $0x1,%ymm1,%ymm1,%ymm1
    1932:	c5 dd 61 d1          	vpunpcklwd %ymm1,%ymm4,%ymm2
    1936:	c5 dd 69 e1          	vpunpckhwd %ymm1,%ymm4,%ymm4
    193a:	c5 ed f5 1d be 48 20 	vpmaddwd 0x2048be(%rip),%ymm2,%ymm3        # 206200 <PW_MF060_MF089_MF050_MF256>
    1941:	00 
    1942:	c5 dd f5 2d b6 48 20 	vpmaddwd 0x2048b6(%rip),%ymm4,%ymm5        # 206200 <PW_MF060_MF089_MF050_MF256>
    1949:	00 
    194a:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
    194e:	c5 d5 fe ef          	vpaddd %ymm7,%ymm5,%ymm5
    1952:	c5 ed f5 15 c6 48 20 	vpmaddwd 0x2048c6(%rip),%ymm2,%ymm2        # 206220 <PW_MF089_F060_MF256_F050>
    1959:	00 
    195a:	c5 dd f5 25 be 48 20 	vpmaddwd 0x2048be(%rip),%ymm4,%ymm4        # 206220 <PW_MF089_F060_MF256_F050>
    1961:	00 
    1962:	c4 e3 4d 46 f6 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm6
    1968:	c4 e3 45 46 ff 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm7
    196e:	c5 ed fe f6          	vpaddd %ymm6,%ymm2,%ymm6
    1972:	c5 dd fe ff          	vpaddd %ymm7,%ymm4,%ymm7
    1976:	c5 bd fe c6          	vpaddd %ymm6,%ymm8,%ymm0
    197a:	c5 b5 fe cf          	vpaddd %ymm7,%ymm9,%ymm1
    197e:	c5 fd fe 05 da 48 20 	vpaddd 0x2048da(%rip),%ymm0,%ymm0        # 206260 <PD_DESCALE_P2>
    1985:	00 
    1986:	c5 f5 fe 0d d2 48 20 	vpaddd 0x2048d2(%rip),%ymm1,%ymm1        # 206260 <PD_DESCALE_P2>
    198d:	00 
    198e:	c5 fd 72 e0 12       	vpsrad $0x12,%ymm0,%ymm0
    1993:	c5 f5 72 e1 12       	vpsrad $0x12,%ymm1,%ymm1
    1998:	c5 fd 6b c1          	vpackssdw %ymm1,%ymm0,%ymm0
    199c:	c5 bd fa d6          	vpsubd %ymm6,%ymm8,%ymm2
    19a0:	c5 b5 fa e7          	vpsubd %ymm7,%ymm9,%ymm4
    19a4:	c5 ed fe 15 b4 48 20 	vpaddd 0x2048b4(%rip),%ymm2,%ymm2        # 206260 <PD_DESCALE_P2>
    19ab:	00 
    19ac:	c5 dd fe 25 ac 48 20 	vpaddd 0x2048ac(%rip),%ymm4,%ymm4        # 206260 <PD_DESCALE_P2>
    19b3:	00 
    19b4:	c5 ed 72 e2 12       	vpsrad $0x12,%ymm2,%ymm2
    19b9:	c5 dd 72 e4 12       	vpsrad $0x12,%ymm4,%ymm4
    19be:	c5 ed 6b e4          	vpackssdw %ymm4,%ymm2,%ymm4
    19c2:	c5 ad fe f3          	vpaddd %ymm3,%ymm10,%ymm6
    19c6:	c5 a5 fe fd          	vpaddd %ymm5,%ymm11,%ymm7
    19ca:	c5 cd fe 35 8e 48 20 	vpaddd 0x20488e(%rip),%ymm6,%ymm6        # 206260 <PD_DESCALE_P2>
    19d1:	00 
    19d2:	c5 c5 fe 3d 86 48 20 	vpaddd 0x204886(%rip),%ymm7,%ymm7        # 206260 <PD_DESCALE_P2>
    19d9:	00 
    19da:	c5 cd 72 e6 12       	vpsrad $0x12,%ymm6,%ymm6
    19df:	c5 c5 72 e7 12       	vpsrad $0x12,%ymm7,%ymm7
    19e4:	c5 cd 6b cf          	vpackssdw %ymm7,%ymm6,%ymm1
    19e8:	c5 ad fa f3          	vpsubd %ymm3,%ymm10,%ymm6
    19ec:	c5 a5 fa fd          	vpsubd %ymm5,%ymm11,%ymm7
    19f0:	c5 cd fe 35 68 48 20 	vpaddd 0x204868(%rip),%ymm6,%ymm6        # 206260 <PD_DESCALE_P2>
    19f7:	00 
    19f8:	c5 c5 fe 3d 60 48 20 	vpaddd 0x204860(%rip),%ymm7,%ymm7        # 206260 <PD_DESCALE_P2>
    19ff:	00 
    1a00:	c5 cd 72 e6 12       	vpsrad $0x12,%ymm6,%ymm6
    1a05:	c5 c5 72 e7 12       	vpsrad $0x12,%ymm7,%ymm7
    1a0a:	c5 cd 6b d7          	vpackssdw %ymm7,%ymm6,%ymm2
    1a0e:	c4 e3 fd 00 d8 d8    	vpermq $0xd8,%ymm0,%ymm3
    1a14:	c4 e3 fd 00 e9 72    	vpermq $0x72,%ymm1,%ymm5
    1a1a:	c4 e3 fd 00 f2 d8    	vpermq $0xd8,%ymm2,%ymm6
    1a20:	c4 e3 fd 00 fc 72    	vpermq $0x72,%ymm4,%ymm7
    1a26:	c5 e5 61 c5          	vpunpcklwd %ymm5,%ymm3,%ymm0
    1a2a:	c5 e5 69 cd          	vpunpckhwd %ymm5,%ymm3,%ymm1
    1a2e:	c5 cd 61 d7          	vpunpcklwd %ymm7,%ymm6,%ymm2
    1a32:	c5 cd 69 e7          	vpunpckhwd %ymm7,%ymm6,%ymm4
    1a36:	c5 fd 61 d9          	vpunpcklwd %ymm1,%ymm0,%ymm3
    1a3a:	c5 ed 61 ec          	vpunpcklwd %ymm4,%ymm2,%ymm5
    1a3e:	c5 fd 69 f1          	vpunpckhwd %ymm1,%ymm0,%ymm6
    1a42:	c5 ed 69 fc          	vpunpckhwd %ymm4,%ymm2,%ymm7
    1a46:	c5 e5 6c c5          	vpunpcklqdq %ymm5,%ymm3,%ymm0
    1a4a:	c5 e5 6d cd          	vpunpckhqdq %ymm5,%ymm3,%ymm1
    1a4e:	c5 cd 6c d7          	vpunpcklqdq %ymm7,%ymm6,%ymm2
    1a52:	c5 cd 6d e7          	vpunpckhqdq %ymm7,%ymm6,%ymm4
    1a56:	c5 fd 63 c1          	vpacksswb %ymm1,%ymm0,%ymm0
    1a5a:	c5 ed 63 cc          	vpacksswb %ymm4,%ymm2,%ymm1
    1a5e:	c5 fd fc 05 1a 48 20 	vpaddb 0x20481a(%rip),%ymm0,%ymm0        # 206280 <PB_CENTERJSAMP>
    1a65:	00 
    1a66:	c5 f5 fc 0d 12 48 20 	vpaddb 0x204812(%rip),%ymm1,%ymm1        # 206280 <PB_CENTERJSAMP>
    1a6d:	00 
    1a6e:	c4 e3 7d 39 ce 01    	vextracti128 $0x1,%ymm1,%xmm6
    1a74:	c4 e3 7d 39 c4 01    	vextracti128 $0x1,%ymm0,%xmm4
    1a7a:	c4 e3 7d 39 ca 00    	vextracti128 $0x0,%ymm1,%xmm2
    1a80:	c4 e3 7d 39 c0 00    	vextracti128 $0x0,%ymm0,%xmm0
    1a86:	c5 f9 70 c8 4e       	vpshufd $0x4e,%xmm0,%xmm1
    1a8b:	c5 f9 70 da 4e       	vpshufd $0x4e,%xmm2,%xmm3
    1a90:	c5 f9 70 ec 4e       	vpshufd $0x4e,%xmm4,%xmm5
    1a95:	c5 f9 70 fe 4e       	vpshufd $0x4e,%xmm6,%xmm7
    1a9a:	c5 f8 77             	vzeroupper 
    1a9d:	44 89 e8             	mov    %r13d,%eax
    1aa0:	49 8b 14 24          	mov    (%r12),%rdx
    1aa4:	49 8b 74 24 08       	mov    0x8(%r12),%rsi
    1aa9:	66 0f d6 04 02       	movq   %xmm0,(%rdx,%rax,1)
    1aae:	66 0f d6 0c 06       	movq   %xmm1,(%rsi,%rax,1)
    1ab3:	49 8b 54 24 10       	mov    0x10(%r12),%rdx
    1ab8:	49 8b 74 24 18       	mov    0x18(%r12),%rsi
    1abd:	66 0f d6 14 02       	movq   %xmm2,(%rdx,%rax,1)
    1ac2:	66 0f d6 1c 06       	movq   %xmm3,(%rsi,%rax,1)
    1ac7:	49 8b 54 24 20       	mov    0x20(%r12),%rdx
    1acc:	49 8b 74 24 28       	mov    0x28(%r12),%rsi
    1ad1:	66 0f d6 24 02       	movq   %xmm4,(%rdx,%rax,1)
    1ad6:	66 0f d6 2c 06       	movq   %xmm5,(%rsi,%rax,1)
    1adb:	49 8b 54 24 30       	mov    0x30(%r12),%rdx
    1ae0:	49 8b 74 24 38       	mov    0x38(%r12),%rsi
    1ae5:	66 0f d6 34 02       	movq   %xmm6,(%rdx,%rax,1)
    1aea:	66 0f d6 3c 06       	movq   %xmm7,(%rsi,%rax,1)
    1aef:	41 5d                	pop    %r13
    1af1:	41 5c                	pop    %r12
    1af3:	41 5b                	pop    %r11
    1af5:	41 5a                	pop    %r10
    1af7:	5d                   	pop    %rbp
    1af8:	c3                   	retq   
    1af9:	90                   	nop
    1afa:	90                   	nop
    1afb:	90                   	nop
    1afc:	90                   	nop
    1afd:	90                   	nop
    1afe:	90                   	nop
    1aff:	90                   	nop

0000000000001b00 <jpeg_fdct_ifast>:
    1b00:	41 56                	push   %r14
    1b02:	41 55                	push   %r13
    1b04:	48 89 f8             	mov    %rdi,%rax
    1b07:	41 54                	push   %r12
    1b09:	55                   	push   %rbp
    1b0a:	53                   	push   %rbx
    1b0b:	48 8d 9f 80 00 00 00 	lea    0x80(%rdi),%rbx
    1b12:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1b18:	0f b7 50 0e          	movzwl 0xe(%rax),%edx
    1b1c:	44 0f b7 48 0c       	movzwl 0xc(%rax),%r9d
    1b21:	48 83 c0 10          	add    $0x10,%rax
    1b25:	0f b7 70 f0          	movzwl -0x10(%rax),%esi
    1b29:	0f b7 48 f2          	movzwl -0xe(%rax),%ecx
    1b2d:	44 0f b7 50 fa       	movzwl -0x6(%rax),%r10d
    1b32:	44 0f b7 58 f8       	movzwl -0x8(%rax),%r11d
    1b37:	44 8d 04 16          	lea    (%rsi,%rdx,1),%r8d
    1b3b:	29 d6                	sub    %edx,%esi
    1b3d:	42 8d 14 09          	lea    (%rcx,%r9,1),%edx
    1b41:	44 29 c9             	sub    %r9d,%ecx
    1b44:	44 0f b7 48 f4       	movzwl -0xc(%rax),%r9d
    1b49:	47 8d 24 11          	lea    (%r9,%r10,1),%r12d
    1b4d:	45 29 d1             	sub    %r10d,%r9d
    1b50:	44 0f b7 50 f6       	movzwl -0xa(%rax),%r10d
    1b55:	47 8d 2c 1a          	lea    (%r10,%r11,1),%r13d
    1b59:	43 8d 2c 28          	lea    (%r8,%r13,1),%ebp
    1b5d:	45 29 e8             	sub    %r13d,%r8d
    1b60:	46 8d 2c 22          	lea    (%rdx,%r12,1),%r13d
    1b64:	44 29 e2             	sub    %r12d,%edx
    1b67:	46 8d 74 2d 00       	lea    0x0(%rbp,%r13,1),%r14d
    1b6c:	44 29 ed             	sub    %r13d,%ebp
    1b6f:	0f bf d2             	movswl %dx,%edx
    1b72:	66 89 68 f8          	mov    %bp,-0x8(%rax)
    1b76:	41 0f bf e8          	movswl %r8w,%ebp
    1b7a:	01 ea                	add    %ebp,%edx
    1b7c:	66 44 89 70 f0       	mov    %r14w,-0x10(%rax)
    1b81:	48 63 d2             	movslq %edx,%rdx
    1b84:	48 69 d2 b5 00 00 00 	imul   $0xb5,%rdx,%rdx
    1b8b:	48 c1 fa 08          	sar    $0x8,%rdx
    1b8f:	41 8d 2c 10          	lea    (%r8,%rdx,1),%ebp
    1b93:	41 29 d0             	sub    %edx,%r8d
    1b96:	66 44 89 40 fc       	mov    %r8w,-0x4(%rax)
    1b9b:	45 89 c8             	mov    %r9d,%r8d
    1b9e:	45 29 d8             	sub    %r11d,%r8d
    1ba1:	44 8d 1c 0e          	lea    (%rsi,%rcx,1),%r11d
    1ba5:	44 01 c9             	add    %r9d,%ecx
    1ba8:	45 01 c2             	add    %r8d,%r10d
    1bab:	48 0f bf c9          	movswq %cx,%rcx
    1baf:	66 89 68 f4          	mov    %bp,-0xc(%rax)
    1bb3:	45 0f bf c3          	movswl %r11w,%r8d
    1bb7:	41 0f bf d2          	movswl %r10w,%edx
    1bbb:	44 29 c2             	sub    %r8d,%edx
    1bbe:	4d 0f bf c2          	movswq %r10w,%r8
    1bc2:	48 63 d2             	movslq %edx,%rdx
    1bc5:	48 6b d2 62          	imul   $0x62,%rdx,%rdx
    1bc9:	4d 69 c0 8b 00 00 00 	imul   $0x8b,%r8,%r8
    1bd0:	48 69 c9 b5 00 00 00 	imul   $0xb5,%rcx,%rcx
    1bd7:	48 c1 fa 08          	sar    $0x8,%rdx
    1bdb:	49 c1 f8 08          	sar    $0x8,%r8
    1bdf:	48 89 d5             	mov    %rdx,%rbp
    1be2:	41 01 d0             	add    %edx,%r8d
    1be5:	49 0f bf d3          	movswq %r11w,%rdx
    1be9:	48 c1 f9 08          	sar    $0x8,%rcx
    1bed:	48 69 d2 4e 01 00 00 	imul   $0x14e,%rdx,%rdx
    1bf4:	44 8d 0c 0e          	lea    (%rsi,%rcx,1),%r9d
    1bf8:	29 ce                	sub    %ecx,%esi
    1bfa:	41 8d 0c 30          	lea    (%r8,%rsi,1),%ecx
    1bfe:	44 29 c6             	sub    %r8d,%esi
    1c01:	66 89 70 f6          	mov    %si,-0xa(%rax)
    1c05:	66 89 48 fa          	mov    %cx,-0x6(%rax)
    1c09:	48 c1 fa 08          	sar    $0x8,%rdx
    1c0d:	01 ea                	add    %ebp,%edx
    1c0f:	42 8d 0c 0a          	lea    (%rdx,%r9,1),%ecx
    1c13:	41 29 d1             	sub    %edx,%r9d
    1c16:	66 44 89 48 fe       	mov    %r9w,-0x2(%rax)
    1c1b:	66 89 48 f2          	mov    %cx,-0xe(%rax)
    1c1f:	48 39 d8             	cmp    %rbx,%rax
    1c22:	0f 85 f0 fe ff ff    	jne    1b18 <jpeg_fdct_ifast+0x18>
    1c28:	4c 8d 47 10          	lea    0x10(%rdi),%r8
    1c2c:	0f 1f 40 00          	nopl   0x0(%rax)
    1c30:	0f b7 47 70          	movzwl 0x70(%rdi),%eax
    1c34:	44 0f b7 57 60       	movzwl 0x60(%rdi),%r10d
    1c39:	48 83 c7 02          	add    $0x2,%rdi
    1c3d:	0f b7 77 fe          	movzwl -0x2(%rdi),%esi
    1c41:	0f b7 4f 0e          	movzwl 0xe(%rdi),%ecx
    1c45:	0f b7 57 1e          	movzwl 0x1e(%rdi),%edx
    1c49:	44 0f b7 5f 2e       	movzwl 0x2e(%rdi),%r11d
    1c4e:	44 8d 0c 06          	lea    (%rsi,%rax,1),%r9d
    1c52:	29 c6                	sub    %eax,%esi
    1c54:	42 8d 04 11          	lea    (%rcx,%r10,1),%eax
    1c58:	44 29 d1             	sub    %r10d,%ecx
    1c5b:	44 0f b7 57 4e       	movzwl 0x4e(%rdi),%r10d
    1c60:	42 8d 2c 12          	lea    (%rdx,%r10,1),%ebp
    1c64:	44 29 d2             	sub    %r10d,%edx
    1c67:	44 0f b7 57 3e       	movzwl 0x3e(%rdi),%r10d
    1c6c:	47 8d 24 13          	lea    (%r11,%r10,1),%r12d
    1c70:	43 8d 1c 21          	lea    (%r9,%r12,1),%ebx
    1c74:	45 29 e1             	sub    %r12d,%r9d
    1c77:	44 8d 24 28          	lea    (%rax,%rbp,1),%r12d
    1c7b:	29 e8                	sub    %ebp,%eax
    1c7d:	46 8d 2c 23          	lea    (%rbx,%r12,1),%r13d
    1c81:	44 29 e3             	sub    %r12d,%ebx
    1c84:	98                   	cwtl   
    1c85:	66 89 5f 3e          	mov    %bx,0x3e(%rdi)
    1c89:	41 0f bf d9          	movswl %r9w,%ebx
    1c8d:	01 d8                	add    %ebx,%eax
    1c8f:	66 44 89 6f fe       	mov    %r13w,-0x2(%rdi)
    1c94:	48 98                	cltq   
    1c96:	48 69 c0 b5 00 00 00 	imul   $0xb5,%rax,%rax
    1c9d:	48 c1 f8 08          	sar    $0x8,%rax
    1ca1:	41 8d 1c 01          	lea    (%r9,%rax,1),%ebx
    1ca5:	41 29 c1             	sub    %eax,%r9d
    1ca8:	89 d0                	mov    %edx,%eax
    1caa:	44 29 d0             	sub    %r10d,%eax
    1cad:	66 44 89 4f 5e       	mov    %r9w,0x5e(%rdi)
    1cb2:	01 ca                	add    %ecx,%edx
    1cb4:	41 89 c2             	mov    %eax,%r10d
    1cb7:	48 0f bf d2          	movswq %dx,%rdx
    1cbb:	66 89 5f 1e          	mov    %bx,0x1e(%rdi)
    1cbf:	45 01 da             	add    %r11d,%r10d
    1cc2:	44 8d 1c 0e          	lea    (%rsi,%rcx,1),%r11d
    1cc6:	41 0f bf c2          	movswl %r10w,%eax
    1cca:	4d 0f bf d2          	movswq %r10w,%r10
    1cce:	45 0f bf cb          	movswl %r11w,%r9d
    1cd2:	44 29 c8             	sub    %r9d,%eax
    1cd5:	48 98                	cltq   
    1cd7:	48 6b c0 62          	imul   $0x62,%rax,%rax
    1cdb:	4d 69 d2 8b 00 00 00 	imul   $0x8b,%r10,%r10
    1ce2:	48 69 d2 b5 00 00 00 	imul   $0xb5,%rdx,%rdx
    1ce9:	48 c1 f8 08          	sar    $0x8,%rax
    1ced:	49 c1 fa 08          	sar    $0x8,%r10
    1cf1:	49 89 c1             	mov    %rax,%r9
    1cf4:	41 01 c2             	add    %eax,%r10d
    1cf7:	49 0f bf c3          	movswq %r11w,%rax
    1cfb:	48 c1 fa 08          	sar    $0x8,%rdx
    1cff:	48 69 c0 4e 01 00 00 	imul   $0x14e,%rax,%rax
    1d06:	8d 0c 16             	lea    (%rsi,%rdx,1),%ecx
    1d09:	29 d6                	sub    %edx,%esi
    1d0b:	41 8d 14 32          	lea    (%r10,%rsi,1),%edx
    1d0f:	44 29 d6             	sub    %r10d,%esi
    1d12:	66 89 77 2e          	mov    %si,0x2e(%rdi)
    1d16:	66 89 57 4e          	mov    %dx,0x4e(%rdi)
    1d1a:	48 c1 f8 08          	sar    $0x8,%rax
    1d1e:	44 01 c8             	add    %r9d,%eax
    1d21:	8d 14 08             	lea    (%rax,%rcx,1),%edx
    1d24:	29 c1                	sub    %eax,%ecx
    1d26:	66 89 4f 6e          	mov    %cx,0x6e(%rdi)
    1d2a:	66 89 57 0e          	mov    %dx,0xe(%rdi)
    1d2e:	4c 39 c7             	cmp    %r8,%rdi
    1d31:	0f 85 f9 fe ff ff    	jne    1c30 <jpeg_fdct_ifast+0x130>
    1d37:	5b                   	pop    %rbx
    1d38:	5d                   	pop    %rbp
    1d39:	41 5c                	pop    %r12
    1d3b:	41 5d                	pop    %r13
    1d3d:	41 5e                	pop    %r14
    1d3f:	c3                   	retq   

0000000000001d40 <jpeg_fdct_islow>:
    1d40:	41 54                	push   %r12
    1d42:	4c 8d 97 80 00 00 00 	lea    0x80(%rdi),%r10
    1d49:	55                   	push   %rbp
    1d4a:	53                   	push   %rbx
    1d4b:	48 89 f8             	mov    %rdi,%rax
    1d4e:	66 90                	xchg   %ax,%ax
    1d50:	0f bf 50 0e          	movswl 0xe(%rax),%edx
    1d54:	0f bf 08             	movswl (%rax),%ecx
    1d57:	0f bf 70 02          	movswl 0x2(%rax),%esi
    1d5b:	44 0f bf 40 04       	movswl 0x4(%rax),%r8d
    1d60:	44 0f bf 48 08       	movswl 0x8(%rax),%r9d
    1d65:	8d 1c 11             	lea    (%rcx,%rdx,1),%ebx
    1d68:	29 d1                	sub    %edx,%ecx
    1d6a:	0f bf 50 0c          	movswl 0xc(%rax),%edx
    1d6e:	48 63 c9             	movslq %ecx,%rcx
    1d71:	48 63 db             	movslq %ebx,%rbx
    1d74:	44 8d 1c 16          	lea    (%rsi,%rdx,1),%r11d
    1d78:	29 d6                	sub    %edx,%esi
    1d7a:	0f bf 50 0a          	movswl 0xa(%rax),%edx
    1d7e:	48 63 f6             	movslq %esi,%rsi
    1d81:	4d 63 db             	movslq %r11d,%r11
    1d84:	41 8d 2c 10          	lea    (%r8,%rdx,1),%ebp
    1d88:	41 29 d0             	sub    %edx,%r8d
    1d8b:	0f bf 50 06          	movswl 0x6(%rax),%edx
    1d8f:	4d 63 c0             	movslq %r8d,%r8
    1d92:	48 63 ed             	movslq %ebp,%rbp
    1d95:	46 8d 24 0a          	lea    (%rdx,%r9,1),%r12d
    1d99:	44 29 ca             	sub    %r9d,%edx
    1d9c:	48 63 d2             	movslq %edx,%rdx
    1d9f:	4d 63 e4             	movslq %r12d,%r12
    1da2:	4e 8d 0c 23          	lea    (%rbx,%r12,1),%r9
    1da6:	4c 29 e3             	sub    %r12,%rbx
    1da9:	4d 8d 24 2b          	lea    (%r11,%rbp,1),%r12
    1dad:	49 29 eb             	sub    %rbp,%r11
    1db0:	4b 8d 2c 21          	lea    (%r9,%r12,1),%rbp
    1db4:	4d 29 e1             	sub    %r12,%r9
    1db7:	4e 8d 24 01          	lea    (%rcx,%r8,1),%r12
    1dbb:	4d 01 c9             	add    %r9,%r9
    1dbe:	66 44 89 48 08       	mov    %r9w,0x8(%rax)
    1dc3:	4e 8d 0c 1b          	lea    (%rbx,%r11,1),%r9
    1dc7:	48 01 ed             	add    %rbp,%rbp
    1dca:	48 69 db 7e 18 00 00 	imul   $0x187e,%rbx,%rbx
    1dd1:	66 89 28             	mov    %bp,(%rax)
    1dd4:	4a 8d 2c 06          	lea    (%rsi,%r8,1),%rbp
    1dd8:	4d 69 c9 51 11 00 00 	imul   $0x1151,%r9,%r9
    1ddf:	4d 69 db df c4 ff ff 	imul   $0xffffffffffffc4df,%r11,%r11
    1de6:	49 8d 9c 19 00 08 00 	lea    0x800(%r9,%rbx,1),%rbx
    1ded:	00 
    1dee:	4f 8d 8c 19 00 08 00 	lea    0x800(%r9,%r11,1),%r9
    1df5:	00 
    1df6:	4c 8d 1c 11          	lea    (%rcx,%rdx,1),%r11
    1dfa:	48 c1 fb 0c          	sar    $0xc,%rbx
    1dfe:	66 89 58 04          	mov    %bx,0x4(%rax)
    1e02:	48 8d 1c 16          	lea    (%rsi,%rdx,1),%rbx
    1e06:	49 c1 f9 0c          	sar    $0xc,%r9
    1e0a:	66 44 89 48 0c       	mov    %r9w,0xc(%rax)
    1e0f:	4e 8d 0c 23          	lea    (%rbx,%r12,1),%r9
    1e13:	4d 69 db 33 e3 ff ff 	imul   $0xffffffffffffe333,%r11,%r11
    1e1a:	4d 69 c9 a1 25 00 00 	imul   $0x25a1,%r9,%r9
    1e21:	48 69 db 3b c1 ff ff 	imul   $0xffffffffffffc13b,%rbx,%rbx
    1e28:	48 69 d2 8e 09 00 00 	imul   $0x98e,%rdx,%rdx
    1e2f:	4c 01 cb             	add    %r9,%rbx
    1e32:	48 69 ed fd ad ff ff 	imul   $0xffffffffffffadfd,%rbp,%rbp
    1e39:	4d 69 e4 84 f3 ff ff 	imul   $0xfffffffffffff384,%r12,%r12
    1e40:	4c 01 da             	add    %r11,%rdx
    1e43:	48 8d 94 13 00 08 00 	lea    0x800(%rbx,%rdx,1),%rdx
    1e4a:	00 
    1e4b:	4d 69 c0 b3 41 00 00 	imul   $0x41b3,%r8,%r8
    1e52:	4d 01 e1             	add    %r12,%r9
    1e55:	48 69 f6 54 62 00 00 	imul   $0x6254,%rsi,%rsi
    1e5c:	48 c1 fa 0c          	sar    $0xc,%rdx
    1e60:	66 89 50 0e          	mov    %dx,0xe(%rax)
    1e64:	49 01 e8             	add    %rbp,%r8
    1e67:	4b 8d 94 01 00 08 00 	lea    0x800(%r9,%r8,1),%rdx
    1e6e:	00 
    1e6f:	48 01 ee             	add    %rbp,%rsi
    1e72:	48 69 c9 0b 30 00 00 	imul   $0x300b,%rcx,%rcx
    1e79:	48 c1 fa 0c          	sar    $0xc,%rdx
    1e7d:	66 89 50 0a          	mov    %dx,0xa(%rax)
    1e81:	48 8d 94 33 00 08 00 	lea    0x800(%rbx,%rsi,1),%rdx
    1e88:	00 
    1e89:	4c 01 d9             	add    %r11,%rcx
    1e8c:	48 c1 fa 0c          	sar    $0xc,%rdx
    1e90:	66 89 50 06          	mov    %dx,0x6(%rax)
    1e94:	49 8d 94 09 00 08 00 	lea    0x800(%r9,%rcx,1),%rdx
    1e9b:	00 
    1e9c:	48 c1 fa 0c          	sar    $0xc,%rdx
    1ea0:	48 83 c0 10          	add    $0x10,%rax
    1ea4:	66 89 50 f2          	mov    %dx,-0xe(%rax)
    1ea8:	4c 39 d0             	cmp    %r10,%rax
    1eab:	0f 85 9f fe ff ff    	jne    1d50 <jpeg_fdct_islow+0x10>
    1eb1:	4c 8d 4f 10          	lea    0x10(%rdi),%r9
    1eb5:	0f 1f 00             	nopl   (%rax)
    1eb8:	0f bf 57 70          	movswl 0x70(%rdi),%edx
    1ebc:	0f bf 07             	movswl (%rdi),%eax
    1ebf:	0f bf 4f 60          	movswl 0x60(%rdi),%ecx
    1ec3:	0f bf 77 50          	movswl 0x50(%rdi),%esi
    1ec7:	44 0f bf 47 40       	movswl 0x40(%rdi),%r8d
    1ecc:	44 8d 1c 10          	lea    (%rax,%rdx,1),%r11d
    1ed0:	29 d0                	sub    %edx,%eax
    1ed2:	0f bf 57 10          	movswl 0x10(%rdi),%edx
    1ed6:	48 98                	cltq   
    1ed8:	4d 63 db             	movslq %r11d,%r11
    1edb:	44 8d 14 0a          	lea    (%rdx,%rcx,1),%r10d
    1edf:	29 ca                	sub    %ecx,%edx
    1ee1:	0f bf 4f 20          	movswl 0x20(%rdi),%ecx
    1ee5:	48 63 d2             	movslq %edx,%rdx
    1ee8:	4d 63 d2             	movslq %r10d,%r10
    1eeb:	8d 1c 31             	lea    (%rcx,%rsi,1),%ebx
    1eee:	29 f1                	sub    %esi,%ecx
    1ef0:	0f bf 77 30          	movswl 0x30(%rdi),%esi
    1ef4:	48 63 c9             	movslq %ecx,%rcx
    1ef7:	48 63 db             	movslq %ebx,%rbx
    1efa:	42 8d 2c 06          	lea    (%rsi,%r8,1),%ebp
    1efe:	44 29 c6             	sub    %r8d,%esi
    1f01:	48 63 f6             	movslq %esi,%rsi
    1f04:	48 63 ed             	movslq %ebp,%rbp
    1f07:	4d 8d 04 2b          	lea    (%r11,%rbp,1),%r8
    1f0b:	49 29 eb             	sub    %rbp,%r11
    1f0e:	49 8d 2c 1a          	lea    (%r10,%rbx,1),%rbp
    1f12:	49 29 da             	sub    %rbx,%r10
    1f15:	49 8d 5c 28 01       	lea    0x1(%r8,%rbp,1),%rbx
    1f1a:	49 29 e8             	sub    %rbp,%r8
    1f1d:	48 8d 2c 08          	lea    (%rax,%rcx,1),%rbp
    1f21:	49 83 c0 01          	add    $0x1,%r8
    1f25:	49 d1 f8             	sar    %r8
    1f28:	48 d1 fb             	sar    %rbx
    1f2b:	66 44 89 47 40       	mov    %r8w,0x40(%rdi)
    1f30:	4f 8d 04 13          	lea    (%r11,%r10,1),%r8
    1f34:	66 89 1f             	mov    %bx,(%rdi)
    1f37:	4d 69 db 7e 18 00 00 	imul   $0x187e,%r11,%r11
    1f3e:	48 8d 1c 0a          	lea    (%rdx,%rcx,1),%rbx
    1f42:	4d 69 c0 51 11 00 00 	imul   $0x1151,%r8,%r8
    1f49:	4d 69 d2 df c4 ff ff 	imul   $0xffffffffffffc4df,%r10,%r10
    1f50:	4f 8d 9c 18 00 20 00 	lea    0x2000(%r8,%r11,1),%r11
    1f57:	00 
    1f58:	4f 8d 84 10 00 20 00 	lea    0x2000(%r8,%r10,1),%r8
    1f5f:	00 
    1f60:	4c 8d 14 30          	lea    (%rax,%rsi,1),%r10
    1f64:	49 c1 fb 0e          	sar    $0xe,%r11
    1f68:	66 44 89 5f 20       	mov    %r11w,0x20(%rdi)
    1f6d:	4c 8d 1c 32          	lea    (%rdx,%rsi,1),%r11
    1f71:	49 c1 f8 0e          	sar    $0xe,%r8
    1f75:	66 44 89 47 60       	mov    %r8w,0x60(%rdi)
    1f7a:	4d 8d 04 2b          	lea    (%r11,%rbp,1),%r8
    1f7e:	48 69 db fd ad ff ff 	imul   $0xffffffffffffadfd,%rbx,%rbx
    1f85:	4d 69 c0 a1 25 00 00 	imul   $0x25a1,%r8,%r8
    1f8c:	4d 69 d2 33 e3 ff ff 	imul   $0xffffffffffffe333,%r10,%r10
    1f93:	4d 69 db 3b c1 ff ff 	imul   $0xffffffffffffc13b,%r11,%r11
    1f9a:	48 69 ed 84 f3 ff ff 	imul   $0xfffffffffffff384,%rbp,%rbp
    1fa1:	48 69 f6 8e 09 00 00 	imul   $0x98e,%rsi,%rsi
    1fa8:	4d 01 c3             	add    %r8,%r11
    1fab:	49 01 e8             	add    %rbp,%r8
    1fae:	48 69 c9 b3 41 00 00 	imul   $0x41b3,%rcx,%rcx
    1fb5:	4c 01 d6             	add    %r10,%rsi
    1fb8:	48 69 d2 54 62 00 00 	imul   $0x6254,%rdx,%rdx
    1fbf:	49 8d b4 33 00 20 00 	lea    0x2000(%r11,%rsi,1),%rsi
    1fc6:	00 
    1fc7:	48 69 c0 0b 30 00 00 	imul   $0x300b,%rax,%rax
    1fce:	48 c1 fe 0e          	sar    $0xe,%rsi
    1fd2:	48 01 d9             	add    %rbx,%rcx
    1fd5:	66 89 77 70          	mov    %si,0x70(%rdi)
    1fd9:	48 01 da             	add    %rbx,%rdx
    1fdc:	49 8d 8c 08 00 20 00 	lea    0x2000(%r8,%rcx,1),%rcx
    1fe3:	00 
    1fe4:	49 8d 94 13 00 20 00 	lea    0x2000(%r11,%rdx,1),%rdx
    1feb:	00 
    1fec:	48 c1 f9 0e          	sar    $0xe,%rcx
    1ff0:	48 c1 fa 0e          	sar    $0xe,%rdx
    1ff4:	4c 01 d0             	add    %r10,%rax
    1ff7:	66 89 4f 50          	mov    %cx,0x50(%rdi)
    1ffb:	49 8d 84 00 00 20 00 	lea    0x2000(%r8,%rax,1),%rax
    2002:	00 
    2003:	66 89 57 30          	mov    %dx,0x30(%rdi)
    2007:	48 83 c7 02          	add    $0x2,%rdi
    200b:	48 c1 f8 0e          	sar    $0xe,%rax
    200f:	66 89 47 0e          	mov    %ax,0xe(%rdi)
    2013:	4c 39 cf             	cmp    %r9,%rdi
    2016:	0f 85 9c fe ff ff    	jne    1eb8 <jpeg_fdct_islow+0x178>
    201c:	5b                   	pop    %rbx
    201d:	5d                   	pop    %rbp
    201e:	41 5c                	pop    %r12
    2020:	c3                   	retq   
    2021:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    2028:	00 00 00 
    202b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000002030 <fastDCT_x86>:
    2030:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
    2035:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
    2039:	41 ff 72 f8          	pushq  -0x8(%r10)
    203d:	55                   	push   %rbp
    203e:	48 89 e5             	mov    %rsp,%rbp
    2041:	41 52                	push   %r10
    2043:	48 81 ec a8 03 00 00 	sub    $0x3a8,%rsp
    204a:	c4 e2 7d 30 3f       	vpmovzxbw (%rdi),%ymm7
    204f:	c4 e2 7d 30 5f 30    	vpmovzxbw 0x30(%rdi),%ymm3
    2055:	c5 fd 7f bd b0 fc ff 	vmovdqa %ymm7,-0x350(%rbp)
    205c:	ff 
    205d:	c4 e2 7d 30 7f 10    	vpmovzxbw 0x10(%rdi),%ymm7
    2063:	c5 fd 7f 9d 50 fc ff 	vmovdqa %ymm3,-0x3b0(%rbp)
    206a:	ff 
    206b:	c5 fd 7f bd 90 fc ff 	vmovdqa %ymm7,-0x370(%rbp)
    2072:	ff 
    2073:	c4 e2 7d 30 7f 20    	vpmovzxbw 0x20(%rdi),%ymm7
    2079:	c5 fd 6f 15 5f 29 00 	vmovdqa 0x295f(%rip),%ymm2        # 49e0 <_IO_stdin_used+0x20>
    2080:	00 
    2081:	c5 fd 7f bd 70 fc ff 	vmovdqa %ymm7,-0x390(%rbp)
    2088:	ff 
    2089:	c5 f9 6f 9d b0 fc ff 	vmovdqa -0x350(%rbp),%xmm3
    2090:	ff 
    2091:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    2098:	00 00 
    209a:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    209e:	31 c0                	xor    %eax,%eax
    20a0:	c5 f9 6f bd c0 fc ff 	vmovdqa -0x340(%rbp),%xmm7
    20a7:	ff 
    20a8:	c4 e2 7d 23 db       	vpmovsxwd %xmm3,%ymm3
    20ad:	c5 79 6f 95 90 fc ff 	vmovdqa -0x370(%rbp),%xmm10
    20b4:	ff 
    20b5:	c4 e2 7d 23 ff       	vpmovsxwd %xmm7,%ymm7
    20ba:	c5 e5 fe da          	vpaddd %ymm2,%ymm3,%ymm3
    20be:	c5 79 6f 8d a0 fc ff 	vmovdqa -0x360(%rbp),%xmm9
    20c5:	ff 
    20c6:	c4 42 7d 23 d2       	vpmovsxwd %xmm10,%ymm10
    20cb:	c5 c5 fe fa          	vpaddd %ymm2,%ymm7,%ymm7
    20cf:	c5 f9 6f b5 70 fc ff 	vmovdqa -0x390(%rbp),%xmm6
    20d6:	ff 
    20d7:	c4 42 7d 23 c9       	vpmovsxwd %xmm9,%ymm9
    20dc:	c5 2d fe d2          	vpaddd %ymm2,%ymm10,%ymm10
    20e0:	c5 79 6f 85 80 fc ff 	vmovdqa -0x380(%rbp),%xmm8
    20e7:	ff 
    20e8:	c4 e2 7d 23 f6       	vpmovsxwd %xmm6,%ymm6
    20ed:	c5 35 fe ca          	vpaddd %ymm2,%ymm9,%ymm9
    20f1:	c5 f9 6f 85 50 fc ff 	vmovdqa -0x3b0(%rbp),%xmm0
    20f8:	ff 
    20f9:	c4 42 7d 23 c0       	vpmovsxwd %xmm8,%ymm8
    20fe:	c5 cd fe f2          	vpaddd %ymm2,%ymm6,%ymm6
    2102:	c4 c1 4d fe e1       	vpaddd %ymm9,%ymm6,%ymm4
    2107:	c5 b5 fa f6          	vpsubd %ymm6,%ymm9,%ymm6
    210b:	c5 cd fe f6          	vpaddd %ymm6,%ymm6,%ymm6
    210f:	c5 f9 6f 8d 60 fc ff 	vmovdqa -0x3a0(%rbp),%xmm1
    2116:	ff 
    2117:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
    211c:	c5 3d fe c2          	vpaddd %ymm2,%ymm8,%ymm8
    2120:	c4 c1 3d fe ea       	vpaddd %ymm10,%ymm8,%ymm5
    2125:	c4 41 2d fa d0       	vpsubd %ymm8,%ymm10,%ymm10
    212a:	c4 e2 7d 23 c9       	vpmovsxwd %xmm1,%ymm1
    212f:	c5 7d 6f 2d 29 29 00 	vmovdqa 0x2929(%rip),%ymm13        # 4a60 <_IO_stdin_used+0xa0>
    2136:	00 
    2137:	c5 fd fe c2          	vpaddd %ymm2,%ymm0,%ymm0
    213b:	c5 7d fe df          	vpaddd %ymm7,%ymm0,%ymm11
    213f:	c5 c5 fa c0          	vpsubd %ymm0,%ymm7,%ymm0
    2143:	c5 fd 6f 3d b5 28 00 	vmovdqa 0x28b5(%rip),%ymm7        # 4a00 <_IO_stdin_used+0x40>
    214a:	00 
    214b:	c5 f5 fe d2          	vpaddd %ymm2,%ymm1,%ymm2
    214f:	c5 6d fe e3          	vpaddd %ymm3,%ymm2,%ymm12
    2153:	c5 e5 fa ca          	vpsubd %ymm2,%ymm3,%ymm1
    2157:	c4 41 5d fe cc       	vpaddd %ymm12,%ymm4,%ymm9
    215c:	c5 fd 6f 1d bc 28 00 	vmovdqa 0x28bc(%rip),%ymm3        # 4a20 <_IO_stdin_used+0x60>
    2163:	00 
    2164:	c5 9d fa e4          	vpsubd %ymm4,%ymm12,%ymm4
    2168:	c5 7d 6f 25 d0 28 00 	vmovdqa 0x28d0(%rip),%ymm12        # 4a40 <_IO_stdin_used+0x80>
    216f:	00 
    2170:	c4 c1 7d fe d2       	vpaddd %ymm10,%ymm0,%ymm2
    2175:	c4 41 55 fe c3       	vpaddd %ymm11,%ymm5,%ymm8
    217a:	c4 41 7d fa d2       	vpsubd %ymm10,%ymm0,%ymm10
    217f:	c5 dd fe e4          	vpaddd %ymm4,%ymm4,%ymm4
    2183:	c5 a5 fa ed          	vpsubd %ymm5,%ymm11,%ymm5
    2187:	c4 c2 5d 40 c4       	vpmulld %ymm12,%ymm4,%ymm0
    218c:	c4 e2 4d 40 f7       	vpmulld %ymm7,%ymm6,%ymm6
    2191:	c5 55 fe dd          	vpaddd %ymm5,%ymm5,%ymm11
    2195:	c5 cd fe f3          	vpaddd %ymm3,%ymm6,%ymm6
    2199:	c4 c2 25 40 ed       	vpmulld %ymm13,%ymm11,%ymm5
    219e:	c5 cd 72 e6 10       	vpsrad $0x10,%ymm6,%ymm6
    21a3:	c4 42 25 40 dc       	vpmulld %ymm12,%ymm11,%ymm11
    21a8:	c5 fd fe c5          	vpaddd %ymm5,%ymm0,%ymm0
    21ac:	c5 f5 fe c9          	vpaddd %ymm1,%ymm1,%ymm1
    21b0:	c5 fd fe c3          	vpaddd %ymm3,%ymm0,%ymm0
    21b4:	c4 e2 75 40 cf       	vpmulld %ymm7,%ymm1,%ymm1
    21b9:	c5 d5 72 e0 10       	vpsrad $0x10,%ymm0,%ymm5
    21be:	c5 f5 fe cb          	vpaddd %ymm3,%ymm1,%ymm1
    21c2:	c4 c2 5d 40 c5       	vpmulld %ymm13,%ymm4,%ymm0
    21c7:	c5 fd 7f ad 10 fe ff 	vmovdqa %ymm5,-0x1f0(%rbp)
    21ce:	ff 
    21cf:	c5 f5 72 e1 10       	vpsrad $0x10,%ymm1,%ymm1
    21d4:	c5 fd fe c3          	vpaddd %ymm3,%ymm0,%ymm0
    21d8:	c5 ed fe e1          	vpaddd %ymm1,%ymm2,%ymm4
    21dc:	c4 c1 7d fa c3       	vpsubd %ymm11,%ymm0,%ymm0
    21e1:	c5 f5 fa ca          	vpsubd %ymm2,%ymm1,%ymm1
    21e5:	c5 2d fe de          	vpaddd %ymm6,%ymm10,%ymm11
    21e9:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    21ee:	c4 41 4d fa d2       	vpsubd %ymm10,%ymm6,%ymm10
    21f3:	c5 fd 7f 85 90 fe ff 	vmovdqa %ymm0,-0x170(%rbp)
    21fa:	ff 
    21fb:	c4 c1 3d fe f1       	vpaddd %ymm9,%ymm8,%ymm6
    2200:	c5 fd 6f 15 78 28 00 	vmovdqa 0x2878(%rip),%ymm2        # 4a80 <_IO_stdin_used+0xc0>
    2207:	00 
    2208:	c4 41 35 fa c0       	vpsubd %ymm8,%ymm9,%ymm8
    220d:	c5 fd 7f b5 d0 fd ff 	vmovdqa %ymm6,-0x230(%rbp)
    2214:	ff 
    2215:	c5 7d 7f 85 50 fe ff 	vmovdqa %ymm8,-0x1b0(%rbp)
    221c:	ff 
    221d:	c4 62 5d 40 ca       	vpmulld %ymm2,%ymm4,%ymm9
    2222:	c4 e2 25 40 d2       	vpmulld %ymm2,%ymm11,%ymm2
    2227:	c5 7d 6f 25 71 28 00 	vmovdqa 0x2871(%rip),%ymm12        # 4aa0 <_IO_stdin_used+0xe0>
    222e:	00 
    222f:	c4 42 25 40 ec       	vpmulld %ymm12,%ymm11,%ymm13
    2234:	c4 c2 5d 40 e4       	vpmulld %ymm12,%ymm4,%ymm4
    2239:	c5 7d 6f 1d 7f 28 00 	vmovdqa 0x287f(%rip),%ymm11        # 4ac0 <_IO_stdin_used+0x100>
    2240:	00 
    2241:	c5 7d 6f 25 97 28 00 	vmovdqa 0x2897(%rip),%ymm12        # 4ae0 <_IO_stdin_used+0x120>
    2248:	00 
    2249:	c4 41 35 fe cd       	vpaddd %ymm13,%ymm9,%ymm9
    224e:	c5 dd fe e3          	vpaddd %ymm3,%ymm4,%ymm4
    2252:	c5 35 fe cb          	vpaddd %ymm3,%ymm9,%ymm9
    2256:	c5 dd fa e2          	vpsubd %ymm2,%ymm4,%ymm4
    225a:	c4 c1 35 72 e1 10    	vpsrad $0x10,%ymm9,%ymm9
    2260:	c4 42 2d 40 ec       	vpmulld %ymm12,%ymm10,%ymm13
    2265:	c4 c2 75 40 d3       	vpmulld %ymm11,%ymm1,%ymm2
    226a:	c4 42 2d 40 d3       	vpmulld %ymm11,%ymm10,%ymm10
    226f:	c4 c2 75 40 cc       	vpmulld %ymm12,%ymm1,%ymm1
    2274:	c5 f5 fe cb          	vpaddd %ymm3,%ymm1,%ymm1
    2278:	c4 c1 75 fa ca       	vpsubd %ymm10,%ymm1,%ymm1
    227d:	c4 43 4d 46 d1 20    	vperm2i128 $0x20,%ymm9,%ymm6,%ymm10
    2283:	c5 7d 7f 8d f0 fd ff 	vmovdqa %ymm9,-0x210(%rbp)
    228a:	ff 
    228b:	c5 f5 72 e1 10       	vpsrad $0x10,%ymm1,%ymm1
    2290:	c4 c3 4d 46 f1 31    	vperm2i128 $0x31,%ymm9,%ymm6,%ymm6
    2296:	c5 fd 7f 8d 30 fe ff 	vmovdqa %ymm1,-0x1d0(%rbp)
    229d:	ff 
    229e:	c5 dd 72 e4 10       	vpsrad $0x10,%ymm4,%ymm4
    22a3:	c4 c1 6d fe d5       	vpaddd %ymm13,%ymm2,%ymm2
    22a8:	c5 ed fe d3          	vpaddd %ymm3,%ymm2,%ymm2
    22ac:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    22b1:	c4 41 7d 70 ca d8    	vpshufd $0xd8,%ymm10,%ymm9
    22b7:	c5 fd 7f 95 70 fe ff 	vmovdqa %ymm2,-0x190(%rbp)
    22be:	ff 
    22bf:	c4 63 55 46 d1 20    	vperm2i128 $0x20,%ymm1,%ymm5,%ymm10
    22c5:	c5 fd 7f a5 b0 fe ff 	vmovdqa %ymm4,-0x150(%rbp)
    22cc:	ff 
    22cd:	c4 e3 55 46 c9 31    	vperm2i128 $0x31,%ymm1,%ymm5,%ymm1
    22d3:	c5 fd 70 f6 d8       	vpshufd $0xd8,%ymm6,%ymm6
    22d8:	c4 41 7d 70 d2 d8    	vpshufd $0xd8,%ymm10,%ymm10
    22de:	c5 fd 70 c9 d8       	vpshufd $0xd8,%ymm1,%ymm1
    22e3:	c5 35 6c de          	vpunpcklqdq %ymm6,%ymm9,%ymm11
    22e7:	c4 e3 7d 46 ec 20    	vperm2i128 $0x20,%ymm4,%ymm0,%ymm5
    22ed:	c5 b5 6d f6          	vpunpckhqdq %ymm6,%ymm9,%ymm6
    22f1:	c4 e3 7d 46 c4 31    	vperm2i128 $0x31,%ymm4,%ymm0,%ymm0
    22f7:	c5 2d 6c c9          	vpunpcklqdq %ymm1,%ymm10,%ymm9
    22fb:	c5 2d 6d d1          	vpunpckhqdq %ymm1,%ymm10,%ymm10
    22ff:	c4 e3 3d 46 ca 20    	vperm2i128 $0x20,%ymm2,%ymm8,%ymm1
    2305:	c4 63 3d 46 c2 31    	vperm2i128 $0x31,%ymm2,%ymm8,%ymm8
    230b:	c5 fd 70 c0 d8       	vpshufd $0xd8,%ymm0,%ymm0
    2310:	c5 fd 70 d1 d8       	vpshufd $0xd8,%ymm1,%ymm2
    2315:	c5 fd 70 ed d8       	vpshufd $0xd8,%ymm5,%ymm5
    231a:	c4 c1 7d 70 c8 d8    	vpshufd $0xd8,%ymm8,%ymm1
    2320:	c5 55 6c e0          	vpunpcklqdq %ymm0,%ymm5,%ymm12
    2324:	c5 6d 6c c1          	vpunpcklqdq %ymm1,%ymm2,%ymm8
    2328:	c5 d5 6d e8          	vpunpckhqdq %ymm0,%ymm5,%ymm5
    232c:	c5 ed 6d d1          	vpunpckhqdq %ymm1,%ymm2,%ymm2
    2330:	c4 c3 25 46 c1 31    	vperm2i128 $0x31,%ymm9,%ymm11,%ymm0
    2336:	c4 c3 25 46 c9 20    	vperm2i128 $0x20,%ymm9,%ymm11,%ymm1
    233c:	c4 43 3d 46 cc 20    	vperm2i128 $0x20,%ymm12,%ymm8,%ymm9
    2342:	c5 fd 70 c0 d8       	vpshufd $0xd8,%ymm0,%ymm0
    2347:	c5 fd 70 c9 d8       	vpshufd $0xd8,%ymm1,%ymm1
    234c:	c4 43 4d 46 da 20    	vperm2i128 $0x20,%ymm10,%ymm6,%ymm11
    2352:	c5 f5 6c e0          	vpunpcklqdq %ymm0,%ymm1,%ymm4
    2356:	c5 f5 6d c8          	vpunpckhqdq %ymm0,%ymm1,%ymm1
    235a:	c4 c3 3d 46 c4 31    	vperm2i128 $0x31,%ymm12,%ymm8,%ymm0
    2360:	c4 41 7d 70 c1 d8    	vpshufd $0xd8,%ymm9,%ymm8
    2366:	c5 fd 70 c0 d8       	vpshufd $0xd8,%ymm0,%ymm0
    236b:	c5 3d 6c c8          	vpunpcklqdq %ymm0,%ymm8,%ymm9
    236f:	c5 3d 6d c0          	vpunpckhqdq %ymm0,%ymm8,%ymm8
    2373:	c4 c3 4d 46 c2 31    	vperm2i128 $0x31,%ymm10,%ymm6,%ymm0
    2379:	c4 41 7d 70 d3 d8    	vpshufd $0xd8,%ymm11,%ymm10
    237f:	c4 63 6d 46 dd 20    	vperm2i128 $0x20,%ymm5,%ymm2,%ymm11
    2385:	c4 e3 6d 46 d5 31    	vperm2i128 $0x31,%ymm5,%ymm2,%ymm2
    238b:	c5 fd 70 c0 d8       	vpshufd $0xd8,%ymm0,%ymm0
    2390:	c4 41 7d 70 db d8    	vpshufd $0xd8,%ymm11,%ymm11
    2396:	c5 fd 70 d2 d8       	vpshufd $0xd8,%ymm2,%ymm2
    239b:	c5 ad 6c f0          	vpunpcklqdq %ymm0,%ymm10,%ymm6
    239f:	c4 c3 5d 46 e9 20    	vperm2i128 $0x20,%ymm9,%ymm4,%ymm5
    23a5:	c5 25 6c e2          	vpunpcklqdq %ymm2,%ymm11,%ymm12
    23a9:	c4 c3 5d 46 e1 31    	vperm2i128 $0x31,%ymm9,%ymm4,%ymm4
    23af:	c5 ad 6d c0          	vpunpckhqdq %ymm0,%ymm10,%ymm0
    23b3:	c4 43 4d 46 cc 20    	vperm2i128 $0x20,%ymm12,%ymm6,%ymm9
    23b9:	c5 7d 70 d5 d8       	vpshufd $0xd8,%ymm5,%ymm10
    23be:	c5 fd 70 ec d8       	vpshufd $0xd8,%ymm4,%ymm5
    23c3:	c4 c3 4d 46 e4 31    	vperm2i128 $0x31,%ymm12,%ymm6,%ymm4
    23c9:	c5 25 6d da          	vpunpckhqdq %ymm2,%ymm11,%ymm11
    23cd:	c4 41 7d 70 c9 d8    	vpshufd $0xd8,%ymm9,%ymm9
    23d3:	c5 ad 6c d5          	vpunpcklqdq %ymm5,%ymm10,%ymm2
    23d7:	c4 c3 75 46 f0 20    	vperm2i128 $0x20,%ymm8,%ymm1,%ymm6
    23dd:	c5 fd 70 e4 d8       	vpshufd $0xd8,%ymm4,%ymm4
    23e2:	c4 c3 75 46 c8 31    	vperm2i128 $0x31,%ymm8,%ymm1,%ymm1
    23e8:	c5 fd 7f 95 d0 fc ff 	vmovdqa %ymm2,-0x330(%rbp)
    23ef:	ff 
    23f0:	c5 fd 70 f6 d8       	vpshufd $0xd8,%ymm6,%ymm6
    23f5:	c5 b5 6c d4          	vpunpcklqdq %ymm4,%ymm9,%ymm2
    23f9:	c5 fd 70 c9 d8       	vpshufd $0xd8,%ymm1,%ymm1
    23fe:	c5 ad 6d ed          	vpunpckhqdq %ymm5,%ymm10,%ymm5
    2402:	c5 fd 7f 95 f0 fc ff 	vmovdqa %ymm2,-0x310(%rbp)
    2409:	ff 
    240a:	c5 b5 6d e4          	vpunpckhqdq %ymm4,%ymm9,%ymm4
    240e:	c5 cd 6c d1          	vpunpcklqdq %ymm1,%ymm6,%ymm2
    2412:	c5 cd 6d c9          	vpunpckhqdq %ymm1,%ymm6,%ymm1
    2416:	c5 fd 7f ad 50 fd ff 	vmovdqa %ymm5,-0x2b0(%rbp)
    241d:	ff 
    241e:	c5 fd 7f a5 70 fd ff 	vmovdqa %ymm4,-0x290(%rbp)
    2425:	ff 
    2426:	c5 fd 7f 95 10 fd ff 	vmovdqa %ymm2,-0x2f0(%rbp)
    242d:	ff 
    242e:	c4 c3 7d 46 d3 20    	vperm2i128 $0x20,%ymm11,%ymm0,%ymm2
    2434:	c4 c3 7d 46 c3 31    	vperm2i128 $0x31,%ymm11,%ymm0,%ymm0
    243a:	c5 fd 7f 8d 90 fd ff 	vmovdqa %ymm1,-0x270(%rbp)
    2441:	ff 
    2442:	c5 7f f0 95 10 fd ff 	vlddqu -0x2f0(%rbp),%ymm10
    2449:	ff 
    244a:	c5 fd 70 d2 d8       	vpshufd $0xd8,%ymm2,%ymm2
    244f:	c5 ff f0 a5 90 fd ff 	vlddqu -0x270(%rbp),%ymm4
    2456:	ff 
    2457:	c5 fd 70 c0 d8       	vpshufd $0xd8,%ymm0,%ymm0
    245c:	c5 ff f0 ad f0 fc ff 	vlddqu -0x310(%rbp),%ymm5
    2463:	ff 
    2464:	c5 7f f0 8d 70 fd ff 	vlddqu -0x290(%rbp),%ymm9
    246b:	ff 
    246c:	c5 5d fe e5          	vpaddd %ymm5,%ymm4,%ymm12
    2470:	c5 d5 fa ec          	vpsubd %ymm4,%ymm5,%ymm5
    2474:	c5 6d 6c c0          	vpunpcklqdq %ymm0,%ymm2,%ymm8
    2478:	c4 c1 35 fe f2       	vpaddd %ymm10,%ymm9,%ymm6
    247d:	c4 41 2d fa c9       	vpsubd %ymm9,%ymm10,%ymm9
    2482:	c5 ed 6d c0          	vpunpckhqdq %ymm0,%ymm2,%ymm0
    2486:	c4 41 4d fe d4       	vpaddd %ymm12,%ymm6,%ymm10
    248b:	c5 9d fa f6          	vpsubd %ymm6,%ymm12,%ymm6
    248f:	c5 7d 7f 85 30 fd ff 	vmovdqa %ymm8,-0x2d0(%rbp)
    2496:	ff 
    2497:	c5 fd 7f 85 b0 fd ff 	vmovdqa %ymm0,-0x250(%rbp)
    249e:	ff 
    249f:	c5 ff f0 8d 30 fd ff 	vlddqu -0x2d0(%rbp),%ymm1
    24a6:	ff 
    24a7:	c5 ff f0 85 d0 fc ff 	vlddqu -0x330(%rbp),%ymm0
    24ae:	ff 
    24af:	c5 7f f0 9d b0 fd ff 	vlddqu -0x250(%rbp),%ymm11
    24b6:	ff 
    24b7:	c5 7f f0 85 50 fd ff 	vlddqu -0x2b0(%rbp),%ymm8
    24be:	ff 
    24bf:	c5 25 fe e8          	vpaddd %ymm0,%ymm11,%ymm13
    24c3:	c5 bd fe d1          	vpaddd %ymm1,%ymm8,%ymm2
    24c7:	c4 41 75 fa c0       	vpsubd %ymm8,%ymm1,%ymm8
    24cc:	c4 c1 7d fa cb       	vpsubd %ymm11,%ymm0,%ymm1
    24d1:	c4 62 3d 40 c7       	vpmulld %ymm7,%ymm8,%ymm8
    24d6:	c5 fd 6f 05 42 26 00 	vmovdqa 0x2642(%rip),%ymm0        # 4b20 <_IO_stdin_used+0x160>
    24dd:	00 
    24de:	c4 e2 75 40 cf       	vpmulld %ymm7,%ymm1,%ymm1
    24e3:	c4 c1 55 fe f9       	vpaddd %ymm9,%ymm5,%ymm7
    24e8:	c4 c1 55 fa e9       	vpsubd %ymm9,%ymm5,%ymm5
    24ed:	c5 7d 6f 0d 4b 26 00 	vmovdqa 0x264b(%rip),%ymm9        # 4b40 <_IO_stdin_used+0x180>
    24f4:	00 
    24f5:	c4 c1 6d fe e5       	vpaddd %ymm13,%ymm2,%ymm4
    24fa:	c5 f5 fe cb          	vpaddd %ymm3,%ymm1,%ymm1
    24fe:	c5 dd fe 25 fa 25 00 	vpaddd 0x25fa(%rip),%ymm4,%ymm4        # 4b00 <_IO_stdin_used+0x140>
    2505:	00 
    2506:	c5 95 fa d2          	vpsubd %ymm2,%ymm13,%ymm2
    250a:	c5 2d fe dc          	vpaddd %ymm4,%ymm10,%ymm11
    250e:	c4 42 6d 40 e1       	vpmulld %ymm9,%ymm2,%ymm12
    2513:	c4 c1 25 72 e3 03    	vpsrad $0x3,%ymm11,%ymm11
    2519:	c4 41 5d fa d2       	vpsubd %ymm10,%ymm4,%ymm10
    251e:	c5 7d 7f 9d d0 fe ff 	vmovdqa %ymm11,-0x130(%rbp)
    2525:	ff 
    2526:	c4 c1 2d 72 e2 03    	vpsrad $0x3,%ymm10,%ymm10
    252c:	c4 e2 4d 40 e0       	vpmulld %ymm0,%ymm6,%ymm4
    2531:	c5 7d 7f 95 50 ff ff 	vmovdqa %ymm10,-0xb0(%rbp)
    2538:	ff 
    2539:	c4 c1 5d fe e4       	vpaddd %ymm12,%ymm4,%ymm4
    253e:	c4 e2 6d 40 c0       	vpmulld %ymm0,%ymm2,%ymm0
    2543:	c5 7d 6f 25 15 26 00 	vmovdqa 0x2615(%rip),%ymm12        # 4b60 <_IO_stdin_used+0x1a0>
    254a:	00 
    254b:	c5 f5 72 e1 10       	vpsrad $0x10,%ymm1,%ymm1
    2550:	c5 f5 fe c9          	vpaddd %ymm1,%ymm1,%ymm1
    2554:	c5 c5 fe d1          	vpaddd %ymm1,%ymm7,%ymm2
    2558:	c5 f5 fa cf          	vpsubd %ymm7,%ymm1,%ymm1
    255c:	c5 79 7f df          	vmovdqa %xmm11,%xmm7
    2560:	c4 c2 4d 40 f1       	vpmulld %ymm9,%ymm6,%ymm6
    2565:	c4 41 79 6f da       	vmovdqa %xmm10,%xmm11
    256a:	c5 7d 6f 15 0e 26 00 	vmovdqa 0x260e(%rip),%ymm10        # 4b80 <_IO_stdin_used+0x1c0>
    2571:	00 
    2572:	c5 3d fe c3          	vpaddd %ymm3,%ymm8,%ymm8
    2576:	c5 dd fe e3          	vpaddd %ymm3,%ymm4,%ymm4
    257a:	c5 fd fe c3          	vpaddd %ymm3,%ymm0,%ymm0
    257e:	c5 dd 72 e4 10       	vpsrad $0x10,%ymm4,%ymm4
    2583:	c5 fd fa c6          	vpsubd %ymm6,%ymm0,%ymm0
    2587:	c5 fd 7f a5 10 ff ff 	vmovdqa %ymm4,-0xf0(%rbp)
    258e:	ff 
    258f:	c4 c2 6d 40 f2       	vpmulld %ymm10,%ymm2,%ymm6
    2594:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    2599:	c4 c2 6d 40 d4       	vpmulld %ymm12,%ymm2,%ymm2
    259e:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
    25a3:	c4 c1 3d 72 e0 10    	vpsrad $0x10,%ymm8,%ymm8
    25a9:	c4 41 3d fe c0       	vpaddd %ymm8,%ymm8,%ymm8
    25ae:	c4 41 55 fe c8       	vpaddd %ymm8,%ymm5,%ymm9
    25b3:	c5 3d fa c5          	vpsubd %ymm5,%ymm8,%ymm8
    25b7:	c5 f9 6f ec          	vmovdqa %xmm4,%xmm5
    25bb:	c5 ed fe d3          	vpaddd %ymm3,%ymm2,%ymm2
    25bf:	c5 f9 6f e0          	vmovdqa %xmm0,%xmm4
    25c3:	c4 c2 35 40 c4       	vpmulld %ymm12,%ymm9,%ymm0
    25c8:	c4 42 35 40 ca       	vpmulld %ymm10,%ymm9,%ymm9
    25cd:	c5 fd fe c6          	vpaddd %ymm6,%ymm0,%ymm0
    25d1:	c5 fd fe c3          	vpaddd %ymm3,%ymm0,%ymm0
    25d5:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    25da:	c5 fd 7f 85 f0 fe ff 	vmovdqa %ymm0,-0x110(%rbp)
    25e1:	ff 
    25e2:	c5 f9 6f f0          	vmovdqa %xmm0,%xmm6
    25e6:	c4 c1 6d fa d1       	vpsubd %ymm9,%ymm2,%ymm2
    25eb:	c5 7d 6f 25 ad 25 00 	vmovdqa 0x25ad(%rip),%ymm12        # 4ba0 <_IO_stdin_used+0x1e0>
    25f2:	00 
    25f3:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    25f8:	c5 f9 6f c2          	vmovdqa %xmm2,%xmm0
    25fc:	c5 7d 6f 15 bc 25 00 	vmovdqa 0x25bc(%rip),%ymm10        # 4bc0 <_IO_stdin_used+0x200>
    2603:	00 
    2604:	c5 fd 7f 55 b0       	vmovdqa %ymm2,-0x50(%rbp)
    2609:	c4 42 3d 40 cc       	vpmulld %ymm12,%ymm8,%ymm9
    260e:	c4 c2 75 40 d2       	vpmulld %ymm10,%ymm1,%ymm2
    2613:	c4 42 3d 40 c2       	vpmulld %ymm10,%ymm8,%ymm8
    2618:	c4 c2 75 40 cc       	vpmulld %ymm12,%ymm1,%ymm1
    261d:	c5 f5 fe cb          	vpaddd %ymm3,%ymm1,%ymm1
    2621:	c4 c1 75 fa c8       	vpsubd %ymm8,%ymm1,%ymm1
    2626:	c5 f5 72 e1 10       	vpsrad $0x10,%ymm1,%ymm1
    262b:	c5 fd 7f 8d 30 ff ff 	vmovdqa %ymm1,-0xd0(%rbp)
    2632:	ff 
    2633:	c5 35 fe ca          	vpaddd %ymm2,%ymm9,%ymm9
    2637:	c5 35 fe cb          	vpaddd %ymm3,%ymm9,%ymm9
    263b:	c5 79 6f 85 00 ff ff 	vmovdqa -0x100(%rbp),%xmm8
    2642:	ff 
    2643:	c4 c1 35 72 e1 10    	vpsrad $0x10,%ymm9,%ymm9
    2649:	c5 79 7f ca          	vmovdqa %xmm9,%xmm2
    264d:	c5 79 6f b5 40 ff ff 	vmovdqa -0xc0(%rbp),%xmm14
    2654:	ff 
    2655:	c4 c1 48 c6 d8 88    	vshufps $0x88,%xmm8,%xmm6,%xmm3
    265b:	c5 7d 7f 8d 70 ff ff 	vmovdqa %ymm9,-0x90(%rbp)
    2662:	ff 
    2663:	c5 79 6f c9          	vmovdqa %xmm1,%xmm9
    2667:	c5 79 6f 95 20 ff ff 	vmovdqa -0xe0(%rbp),%xmm10
    266e:	ff 
    266f:	c4 41 48 c6 c0 dd    	vshufps $0xdd,%xmm8,%xmm6,%xmm8
    2675:	c5 f9 6f 8d e0 fe ff 	vmovdqa -0x120(%rbp),%xmm1
    267c:	ff 
    267d:	c4 41 50 c6 e2 88    	vshufps $0x88,%xmm10,%xmm5,%xmm12
    2683:	c4 41 50 c6 d2 dd    	vshufps $0xdd,%xmm10,%xmm5,%xmm10
    2689:	c5 40 c6 e9 88       	vshufps $0x88,%xmm1,%xmm7,%xmm13
    268e:	c4 c1 30 c6 ee dd    	vshufps $0xdd,%xmm14,%xmm9,%xmm5
    2694:	c4 c1 30 c6 f6 88    	vshufps $0x88,%xmm14,%xmm9,%xmm6
    269a:	c5 79 6f 4d c0       	vmovdqa -0x40(%rbp),%xmm9
    269f:	c5 10 c6 f3 88       	vshufps $0x88,%xmm3,%xmm13,%xmm14
    26a4:	c5 90 c6 db dd       	vshufps $0xdd,%xmm3,%xmm13,%xmm3
    26a9:	c5 28 c6 ed 88       	vshufps $0x88,%xmm5,%xmm10,%xmm13
    26ae:	c5 a8 c6 ed dd       	vshufps $0xdd,%xmm5,%xmm10,%xmm5
    26b3:	c5 79 6f 55 80       	vmovdqa -0x80(%rbp),%xmm10
    26b8:	c5 c0 c6 c9 dd       	vshufps $0xdd,%xmm1,%xmm7,%xmm1
    26bd:	c5 98 c6 fe 88       	vshufps $0x88,%xmm6,%xmm12,%xmm7
    26c2:	c5 98 c6 f6 dd       	vshufps $0xdd,%xmm6,%xmm12,%xmm6
    26c7:	c5 78 29 ad 90 fc ff 	vmovaps %xmm13,-0x370(%rbp)
    26ce:	ff 
    26cf:	c5 f8 29 ad b0 fc ff 	vmovaps %xmm5,-0x350(%rbp)
    26d6:	ff 
    26d7:	c5 f8 29 b5 70 fc ff 	vmovaps %xmm6,-0x390(%rbp)
    26de:	ff 
    26df:	c4 c1 70 c6 f0 88    	vshufps $0x88,%xmm8,%xmm1,%xmm6
    26e5:	c4 c1 70 c6 c8 dd    	vshufps $0xdd,%xmm8,%xmm1,%xmm1
    26eb:	c4 41 78 c6 c1 88    	vshufps $0x88,%xmm9,%xmm0,%xmm8
    26f1:	c5 f9 6f ad 60 ff ff 	vmovdqa -0xa0(%rbp),%xmm5
    26f8:	ff 
    26f9:	c4 c1 78 c6 c1 dd    	vshufps $0xdd,%xmm9,%xmm0,%xmm0
    26ff:	c5 79 6f 6d a0       	vmovdqa -0x60(%rbp),%xmm13
    2704:	c5 20 c6 e5 88       	vshufps $0x88,%xmm5,%xmm11,%xmm12
    2709:	c5 20 c6 dd dd       	vshufps $0xdd,%xmm5,%xmm11,%xmm11
    270e:	c4 c1 68 c6 ea 88    	vshufps $0x88,%xmm10,%xmm2,%xmm5
    2714:	c4 c1 68 c6 d2 dd    	vshufps $0xdd,%xmm10,%xmm2,%xmm2
    271a:	c4 41 58 c6 d5 88    	vshufps $0x88,%xmm13,%xmm4,%xmm10
    2720:	c5 18 c6 cd 88       	vshufps $0x88,%xmm5,%xmm12,%xmm9
    2725:	c4 c1 58 c6 e5 dd    	vshufps $0xdd,%xmm13,%xmm4,%xmm4
    272b:	c4 41 28 c6 e8 88    	vshufps $0x88,%xmm8,%xmm10,%xmm13
    2731:	c4 41 28 c6 d0 dd    	vshufps $0xdd,%xmm8,%xmm10,%xmm10
    2737:	c5 20 c6 c2 88       	vshufps $0x88,%xmm2,%xmm11,%xmm8
    273c:	c4 41 30 c6 fd 88    	vshufps $0x88,%xmm13,%xmm9,%xmm15
    2742:	c5 a0 c6 d2 dd       	vshufps $0xdd,%xmm2,%xmm11,%xmm2
    2747:	c5 08 c6 df 88       	vshufps $0x88,%xmm7,%xmm14,%xmm11
    274c:	c5 98 c6 ed dd       	vshufps $0xdd,%xmm5,%xmm12,%xmm5
    2751:	c5 58 c6 e0 88       	vshufps $0x88,%xmm0,%xmm4,%xmm12
    2756:	c5 d8 c6 e0 dd       	vshufps $0xdd,%xmm0,%xmm4,%xmm4
    275b:	c5 f9 6f 05 1d 26 00 	vmovdqa 0x261d(%rip),%xmm0        # 4d80 <_IO_stdin_used+0x3c0>
    2762:	00 
    2763:	c5 08 c6 f7 dd       	vshufps $0xdd,%xmm7,%xmm14,%xmm14
    2768:	c4 41 79 db ff       	vpand  %xmm15,%xmm0,%xmm15
    276d:	c4 41 79 db db       	vpand  %xmm11,%xmm0,%xmm11
    2772:	c4 41 30 c6 cd dd    	vshufps $0xdd,%xmm13,%xmm9,%xmm9
    2778:	c4 41 79 db f6       	vpand  %xmm14,%xmm0,%xmm14
    277d:	c4 42 21 2b df       	vpackusdw %xmm15,%xmm11,%xmm11
    2782:	c4 41 38 c6 fc 88    	vshufps $0x88,%xmm12,%xmm8,%xmm15
    2788:	c4 c1 79 db f9       	vpand  %xmm9,%xmm0,%xmm7
    278d:	c5 78 11 1e          	vmovups %xmm11,(%rsi)
    2791:	c5 48 c6 9d 90 fc ff 	vshufps $0x88,-0x370(%rbp),%xmm6,%xmm11
    2798:	ff 88 
    279a:	c4 41 79 db ff       	vpand  %xmm15,%xmm0,%xmm15
    279f:	c4 e2 09 2b ff       	vpackusdw %xmm7,%xmm14,%xmm7
    27a4:	c5 c8 c6 b5 90 fc ff 	vshufps $0xdd,-0x370(%rbp),%xmm6,%xmm6
    27ab:	ff dd 
    27ad:	c4 41 79 db db       	vpand  %xmm11,%xmm0,%xmm11
    27b2:	c5 f8 11 7e 40       	vmovups %xmm7,0x40(%rsi)
    27b7:	c4 c1 38 c6 fc dd    	vshufps $0xdd,%xmm12,%xmm8,%xmm7
    27bd:	c4 42 21 2b df       	vpackusdw %xmm15,%xmm11,%xmm11
    27c2:	c4 41 50 c6 fa 88    	vshufps $0x88,%xmm10,%xmm5,%xmm15
    27c8:	c4 c1 50 c6 ea dd    	vshufps $0xdd,%xmm10,%xmm5,%xmm5
    27ce:	c5 78 11 5e 10       	vmovups %xmm11,0x10(%rsi)
    27d3:	c5 60 c6 9d 70 fc ff 	vshufps $0x88,-0x390(%rbp),%xmm3,%xmm11
    27da:	ff 88 
    27dc:	c4 41 79 db ff       	vpand  %xmm15,%xmm0,%xmm15
    27e1:	c5 e0 c6 9d 70 fc ff 	vshufps $0xdd,-0x390(%rbp),%xmm3,%xmm3
    27e8:	ff dd 
    27ea:	c4 41 79 db db       	vpand  %xmm11,%xmm0,%xmm11
    27ef:	c5 f9 db ff          	vpand  %xmm7,%xmm0,%xmm7
    27f3:	c5 f9 db ed          	vpand  %xmm5,%xmm0,%xmm5
    27f7:	c4 42 21 2b df       	vpackusdw %xmm15,%xmm11,%xmm11
    27fc:	c5 68 c6 fc 88       	vshufps $0x88,%xmm4,%xmm2,%xmm15
    2801:	c5 e8 c6 d4 dd       	vshufps $0xdd,%xmm4,%xmm2,%xmm2
    2806:	c5 78 11 5e 20       	vmovups %xmm11,0x20(%rsi)
    280b:	c5 70 c6 9d b0 fc ff 	vshufps $0x88,-0x350(%rbp),%xmm1,%xmm11
    2812:	ff 88 
    2814:	c4 41 79 db ff       	vpand  %xmm15,%xmm0,%xmm15
    2819:	c5 f0 c6 8d b0 fc ff 	vshufps $0xdd,-0x350(%rbp),%xmm1,%xmm1
    2820:	ff dd 
    2822:	c4 41 79 db db       	vpand  %xmm11,%xmm0,%xmm11
    2827:	c5 f9 db f6          	vpand  %xmm6,%xmm0,%xmm6
    282b:	c5 f9 db db          	vpand  %xmm3,%xmm0,%xmm3
    282f:	c5 f9 db c9          	vpand  %xmm1,%xmm0,%xmm1
    2833:	c5 f9 db c2          	vpand  %xmm2,%xmm0,%xmm0
    2837:	c4 42 21 2b df       	vpackusdw %xmm15,%xmm11,%xmm11
    283c:	c4 e2 49 2b f7       	vpackusdw %xmm7,%xmm6,%xmm6
    2841:	c4 e2 61 2b dd       	vpackusdw %xmm5,%xmm3,%xmm3
    2846:	c4 e2 71 2b c0       	vpackusdw %xmm0,%xmm1,%xmm0
    284b:	c5 78 11 5e 30       	vmovups %xmm11,0x30(%rsi)
    2850:	c5 f8 11 76 50       	vmovups %xmm6,0x50(%rsi)
    2855:	c5 f8 11 5e 60       	vmovups %xmm3,0x60(%rsi)
    285a:	c5 f8 11 46 70       	vmovups %xmm0,0x70(%rsi)
    285f:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    2863:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    286a:	00 00 
    286c:	75 12                	jne    2880 <F_1_175+0x2df>
    286e:	c5 f8 77             	vzeroupper 
    2871:	48 81 c4 a8 03 00 00 	add    $0x3a8,%rsp
    2878:	41 5a                	pop    %r10
    287a:	5d                   	pop    %rbp
    287b:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    287f:	c3                   	retq   
    2880:	c5 f8 77             	vzeroupper 
    2883:	e8 98 dd ff ff       	callq  620 <__stack_chk_fail@plt>
    2888:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    288f:	00 

0000000000002890 <fastDCT>:
    2890:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
    2895:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
    2899:	85 d2                	test   %edx,%edx
    289b:	41 ff 72 f8          	pushq  -0x8(%r10)
    289f:	55                   	push   %rbp
    28a0:	48 89 e5             	mov    %rsp,%rbp
    28a3:	41 57                	push   %r15
    28a5:	41 56                	push   %r14
    28a7:	41 55                	push   %r13
    28a9:	41 54                	push   %r12
    28ab:	41 52                	push   %r10
    28ad:	53                   	push   %rbx
    28ae:	89 55 c8             	mov    %edx,-0x38(%rbp)
    28b1:	0f 84 80 0b 00 00    	je     3437 <F_1_501+0x42c>
    28b7:	89 d0                	mov    %edx,%eax
    28b9:	49 89 fe             	mov    %rdi,%r14
    28bc:	48 89 75 b8          	mov    %rsi,-0x48(%rbp)
    28c0:	83 e8 01             	sub    $0x1,%eax
    28c3:	48 89 75 a0          	mov    %rsi,-0x60(%rbp)
    28c7:	48 83 c0 01          	add    $0x1,%rax
    28cb:	48 c1 e0 06          	shl    $0x6,%rax
    28cf:	48 01 f8             	add    %rdi,%rax
    28d2:	48 89 45 a8          	mov    %rax,-0x58(%rbp)
    28d6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    28dd:	00 00 00 
    28e0:	49 8d 46 08          	lea    0x8(%r14),%rax
    28e4:	4c 8b 65 b8          	mov    -0x48(%rbp),%r12
    28e8:	4c 89 75 b0          	mov    %r14,-0x50(%rbp)
    28ec:	48 89 45 c0          	mov    %rax,-0x40(%rbp)
    28f0:	41 0f b6 06          	movzbl (%r14),%eax
    28f4:	45 0f b6 4e 38       	movzbl 0x38(%r14),%r9d
    28f9:	41 0f b6 56 18       	movzbl 0x18(%r14),%edx
    28fe:	45 0f b6 56 20       	movzbl 0x20(%r14),%r10d
    2903:	45 0f b6 46 10       	movzbl 0x10(%r14),%r8d
    2908:	41 0f b6 5e 28       	movzbl 0x28(%r14),%ebx
    290d:	41 0f b6 7e 08       	movzbl 0x8(%r14),%edi
    2912:	45 0f b6 5e 30       	movzbl 0x30(%r14),%r11d
    2917:	41 83 c1 80          	add    $0xffffff80,%r9d
    291b:	83 c0 80             	add    $0xffffff80,%eax
    291e:	42 8d 34 08          	lea    (%rax,%r9,1),%esi
    2922:	44 29 c8             	sub    %r9d,%eax
    2925:	41 83 c2 80          	add    $0xffffff80,%r10d
    2929:	83 c2 80             	add    $0xffffff80,%edx
    292c:	01 c0                	add    %eax,%eax
    292e:	83 c3 80             	add    $0xffffff80,%ebx
    2931:	46 8d 2c 12          	lea    (%rdx,%r10,1),%r13d
    2935:	98                   	cwtl   
    2936:	41 83 c0 80          	add    $0xffffff80,%r8d
    293a:	44 29 d2             	sub    %r10d,%edx
    293d:	41 83 c3 80          	add    $0xffffff80,%r11d
    2941:	45 8d 3c 18          	lea    (%r8,%rbx,1),%r15d
    2945:	69 c0 05 b5 00 00    	imul   $0xb505,%eax,%eax
    294b:	41 29 d8             	sub    %ebx,%r8d
    294e:	83 c7 80             	add    $0xffffff80,%edi
    2951:	42 8d 1c 2e          	lea    (%rsi,%r13,1),%ebx
    2955:	01 d2                	add    %edx,%edx
    2957:	42 8d 0c 1f          	lea    (%rdi,%r11,1),%ecx
    295b:	0f bf d2             	movswl %dx,%edx
    295e:	44 29 df             	sub    %r11d,%edi
    2961:	44 29 ee             	sub    %r13d,%esi
    2964:	66 89 5d ce          	mov    %bx,-0x32(%rbp)
    2968:	05 00 80 00 00       	add    $0x8000,%eax
    296d:	41 8d 1c 38          	lea    (%r8,%rdi,1),%ebx
    2971:	69 d2 05 b5 00 00    	imul   $0xb505,%edx,%edx
    2977:	c1 f8 10             	sar    $0x10,%eax
    297a:	44 29 c7             	sub    %r8d,%edi
    297d:	44 8d 0c 03          	lea    (%rbx,%rax,1),%r9d
    2981:	29 d8                	sub    %ebx,%eax
    2983:	0f b7 5d ce          	movzwl -0x32(%rbp),%ebx
    2987:	46 8d 1c 39          	lea    (%rcx,%r15,1),%r11d
    298b:	44 29 f9             	sub    %r15d,%ecx
    298e:	01 f6                	add    %esi,%esi
    2990:	81 c2 00 80 00 00    	add    $0x8000,%edx
    2996:	45 0f bf c9          	movswl %r9w,%r9d
    299a:	01 c9                	add    %ecx,%ecx
    299c:	c1 fa 10             	sar    $0x10,%edx
    299f:	0f bf c9             	movswl %cx,%ecx
    29a2:	0f bf f6             	movswl %si,%esi
    29a5:	44 8d 04 17          	lea    (%rdi,%rdx,1),%r8d
    29a9:	29 fa                	sub    %edi,%edx
    29ab:	42 8d 3c 1b          	lea    (%rbx,%r11,1),%edi
    29af:	0f bf d2             	movswl %dx,%edx
    29b2:	98                   	cwtl   
    29b3:	45 0f bf c0          	movswl %r8w,%r8d
    29b7:	66 41 89 3c 24       	mov    %di,(%r12)
    29bc:	45 69 d0 f1 31 00 00 	imul   $0x31f1,%r8d,%r10d
    29c3:	41 69 f9 15 fb 00 00 	imul   $0xfb15,%r9d,%edi
    29ca:	41 8d bc 3a 00 80 00 	lea    0x8000(%r10,%rdi,1),%edi
    29d1:	00 
    29d2:	44 69 d1 46 45 00 00 	imul   $0x4546,%ecx,%r10d
    29d9:	c1 ff 10             	sar    $0x10,%edi
    29dc:	66 41 89 7c 24 10    	mov    %di,0x10(%r12)
    29e2:	69 fe 3d a7 00 00    	imul   $0xa73d,%esi,%edi
    29e8:	41 8d bc 3a 00 80 00 	lea    0x8000(%r10,%rdi,1),%edi
    29ef:	00 
    29f0:	c1 ff 10             	sar    $0x10,%edi
    29f3:	44 29 db             	sub    %r11d,%ebx
    29f6:	49 83 c4 02          	add    $0x2,%r12
    29fa:	66 41 89 7c 24 1e    	mov    %di,0x1e(%r12)
    2a00:	66 41 89 5c 24 3e    	mov    %bx,0x3e(%r12)
    2a06:	49 83 c6 01          	add    $0x1,%r14
    2a0a:	44 69 d0 db d4 00 00 	imul   $0xd4db,%eax,%r10d
    2a11:	69 fa c6 71 ff ff    	imul   $0xffff71c6,%edx,%edi
    2a17:	69 c0 3a 8e 00 00    	imul   $0x8e3a,%eax,%eax
    2a1d:	41 8d bc 3a 00 80 00 	lea    0x8000(%r10,%rdi,1),%edi
    2a24:	00 
    2a25:	69 d2 db d4 00 00    	imul   $0xd4db,%edx,%edx
    2a2b:	c1 ff 10             	sar    $0x10,%edi
    2a2e:	66 41 89 7c 24 2e    	mov    %di,0x2e(%r12)
    2a34:	69 f6 46 45 00 00    	imul   $0x4546,%esi,%esi
    2a3a:	8d 84 02 00 80 00 00 	lea    0x8000(%rdx,%rax,1),%eax
    2a41:	69 c9 c3 58 ff ff    	imul   $0xffff58c3,%ecx,%ecx
    2a47:	c1 f8 10             	sar    $0x10,%eax
    2a4a:	66 41 89 44 24 4e    	mov    %ax,0x4e(%r12)
    2a50:	45 69 c9 f1 31 00 00 	imul   $0x31f1,%r9d,%r9d
    2a57:	8d 84 0e 00 80 00 00 	lea    0x8000(%rsi,%rcx,1),%eax
    2a5e:	45 69 c0 eb 04 ff ff 	imul   $0xffff04eb,%r8d,%r8d
    2a65:	c1 f8 10             	sar    $0x10,%eax
    2a68:	66 41 89 44 24 5e    	mov    %ax,0x5e(%r12)
    2a6e:	43 8d 84 01 00 80 00 	lea    0x8000(%r9,%r8,1),%eax
    2a75:	00 
    2a76:	c1 f8 10             	sar    $0x10,%eax
    2a79:	66 41 89 44 24 6e    	mov    %ax,0x6e(%r12)
    2a7f:	4c 39 75 c0          	cmp    %r14,-0x40(%rbp)
    2a83:	0f 85 67 fe ff ff    	jne    28f0 <fastDCT+0x60>
    2a89:	4c 8b 75 b0          	mov    -0x50(%rbp),%r14
    2a8d:	48 83 6d b8 80       	subq   $0xffffffffffffff80,-0x48(%rbp)
    2a92:	49 83 c6 40          	add    $0x40,%r14
    2a96:	4c 39 75 a8          	cmp    %r14,-0x58(%rbp)
    2a9a:	0f 85 40 fe ff ff    	jne    28e0 <fastDCT+0x50>
    2aa0:	44 8b 75 c8          	mov    -0x38(%rbp),%r14d
    2aa4:	4c 8b 6d a0          	mov    -0x60(%rbp),%r13
    2aa8:	41 c1 e6 03          	shl    $0x3,%r14d
    2aac:	45 85 f6             	test   %r14d,%r14d
    2aaf:	0f 84 82 09 00 00    	je     3437 <F_1_501+0x42c>
    2ab5:	41 8d 46 ff          	lea    -0x1(%r14),%eax
    2ab9:	83 f8 0e             	cmp    $0xe,%eax
    2abc:	0f 86 86 09 00 00    	jbe    3448 <F_1_501+0x43d>
    2ac2:	44 89 f1             	mov    %r14d,%ecx
    2ac5:	c5 7d 6f 2d 13 21 00 	vmovdqa 0x2113(%rip),%ymm13        # 4be0 <_IO_stdin_used+0x220>
    2acc:	00 
    2acd:	4c 89 e8             	mov    %r13,%rax
    2ad0:	c5 7d 6f 35 48 1f 00 	vmovdqa 0x1f48(%rip),%ymm14        # 4a20 <_IO_stdin_used+0x60>
    2ad7:	00 
    2ad8:	c1 e9 04             	shr    $0x4,%ecx
    2adb:	31 d2                	xor    %edx,%edx
    2add:	0f 1f 00             	nopl   (%rax)
    2ae0:	c5 fa 6f 00          	vmovdqu (%rax),%xmm0
    2ae4:	83 c2 01             	add    $0x1,%edx
    2ae7:	48 05 00 01 00 00    	add    $0x100,%rax
    2aed:	c5 fa 6f a0 20 ff ff 	vmovdqu -0xe0(%rax),%xmm4
    2af4:	ff 
    2af5:	c4 e3 7d 38 80 10 ff 	vinserti128 $0x1,-0xf0(%rax),%ymm0,%ymm0
    2afc:	ff ff 01 
    2aff:	c5 fa 6f 88 40 ff ff 	vmovdqu -0xc0(%rax),%xmm1
    2b06:	ff 
    2b07:	c4 e3 5d 38 a0 30 ff 	vinserti128 $0x1,-0xd0(%rax),%ymm4,%ymm4
    2b0e:	ff ff 01 
    2b11:	c5 7a 6f 90 60 ff ff 	vmovdqu -0xa0(%rax),%xmm10
    2b18:	ff 
    2b19:	c4 e3 75 38 88 50 ff 	vinserti128 $0x1,-0xb0(%rax),%ymm1,%ymm1
    2b20:	ff ff 01 
    2b23:	c5 fa 6f 50 80       	vmovdqu -0x80(%rax),%xmm2
    2b28:	c4 63 2d 38 90 70 ff 	vinserti128 $0x1,-0x90(%rax),%ymm10,%ymm10
    2b2f:	ff ff 01 
    2b32:	c5 7a 6f 48 a0       	vmovdqu -0x60(%rax),%xmm9
    2b37:	c4 e3 6d 38 50 90 01 	vinserti128 $0x1,-0x70(%rax),%ymm2,%ymm2
    2b3e:	c5 fa 6f 58 c0       	vmovdqu -0x40(%rax),%xmm3
    2b43:	c4 63 35 38 48 b0 01 	vinserti128 $0x1,-0x50(%rax),%ymm9,%ymm9
    2b4a:	c5 fa 6f 68 e0       	vmovdqu -0x20(%rax),%xmm5
    2b4f:	c5 95 db fc          	vpand  %ymm4,%ymm13,%ymm7
    2b53:	c5 dd 72 d4 10       	vpsrld $0x10,%ymm4,%ymm4
    2b58:	c5 95 db f0          	vpand  %ymm0,%ymm13,%ymm6
    2b5c:	c5 fd 72 d0 10       	vpsrld $0x10,%ymm0,%ymm0
    2b61:	c5 15 db c1          	vpand  %ymm1,%ymm13,%ymm8
    2b65:	c4 e2 7d 2b c4       	vpackusdw %ymm4,%ymm0,%ymm0
    2b6a:	c4 c1 15 db e2       	vpand  %ymm10,%ymm13,%ymm4
    2b6f:	c4 c1 2d 72 d2 10    	vpsrld $0x10,%ymm10,%ymm10
    2b75:	c4 e3 65 38 58 d0 01 	vinserti128 $0x1,-0x30(%rax),%ymm3,%ymm3
    2b7c:	c4 e3 55 38 68 f0 01 	vinserti128 $0x1,-0x10(%rax),%ymm5,%ymm5
    2b83:	c4 e2 4d 2b f7       	vpackusdw %ymm7,%ymm6,%ymm6
    2b88:	c5 c5 72 d1 10       	vpsrld $0x10,%ymm1,%ymm7
    2b8d:	c4 62 3d 2b c4       	vpackusdw %ymm4,%ymm8,%ymm8
    2b92:	c4 c1 15 db c9       	vpand  %ymm9,%ymm13,%ymm1
    2b97:	c4 c1 35 72 d1 10    	vpsrld $0x10,%ymm9,%ymm9
    2b9d:	c5 95 db e2          	vpand  %ymm2,%ymm13,%ymm4
    2ba1:	c5 ed 72 d2 10       	vpsrld $0x10,%ymm2,%ymm2
    2ba6:	c4 c2 6d 2b d1       	vpackusdw %ymm9,%ymm2,%ymm2
    2bab:	c5 15 db cb          	vpand  %ymm3,%ymm13,%ymm9
    2baf:	c5 e5 72 d3 10       	vpsrld $0x10,%ymm3,%ymm3
    2bb4:	c4 e2 5d 2b e1       	vpackusdw %ymm1,%ymm4,%ymm4
    2bb9:	c5 95 db cd          	vpand  %ymm5,%ymm13,%ymm1
    2bbd:	c5 d5 72 d5 10       	vpsrld $0x10,%ymm5,%ymm5
    2bc2:	c4 e3 fd 00 f6 d8    	vpermq $0xd8,%ymm6,%ymm6
    2bc8:	c4 43 fd 00 c0 d8    	vpermq $0xd8,%ymm8,%ymm8
    2bce:	c4 62 35 2b c9       	vpackusdw %ymm1,%ymm9,%ymm9
    2bd3:	c4 e3 fd 00 e4 d8    	vpermq $0xd8,%ymm4,%ymm4
    2bd9:	c4 c1 15 db c8       	vpand  %ymm8,%ymm13,%ymm1
    2bde:	c4 c1 3d 72 d0 10    	vpsrld $0x10,%ymm8,%ymm8
    2be4:	c4 c2 45 2b fa       	vpackusdw %ymm10,%ymm7,%ymm7
    2be9:	c4 43 fd 00 c9 d8    	vpermq $0xd8,%ymm9,%ymm9
    2bef:	c5 15 db de          	vpand  %ymm6,%ymm13,%ymm11
    2bf3:	c5 cd 72 d6 10       	vpsrld $0x10,%ymm6,%ymm6
    2bf8:	c4 e3 fd 00 c0 d8    	vpermq $0xd8,%ymm0,%ymm0
    2bfe:	c4 e3 fd 00 ff d8    	vpermq $0xd8,%ymm7,%ymm7
    2c04:	c4 e2 65 2b dd       	vpackusdw %ymm5,%ymm3,%ymm3
    2c09:	c4 62 25 2b d9       	vpackusdw %ymm1,%ymm11,%ymm11
    2c0e:	c5 15 db e4          	vpand  %ymm4,%ymm13,%ymm12
    2c12:	c4 c1 15 db c9       	vpand  %ymm9,%ymm13,%ymm1
    2c17:	c4 c1 35 72 d1 10    	vpsrld $0x10,%ymm9,%ymm9
    2c1d:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
    2c23:	c5 95 db ef          	vpand  %ymm7,%ymm13,%ymm5
    2c27:	c5 c5 72 d7 10       	vpsrld $0x10,%ymm7,%ymm7
    2c2c:	c4 e3 fd 00 db d8    	vpermq $0xd8,%ymm3,%ymm3
    2c32:	c4 62 1d 2b e1       	vpackusdw %ymm1,%ymm12,%ymm12
    2c37:	c5 f5 72 d4 10       	vpsrld $0x10,%ymm4,%ymm1
    2c3c:	c5 95 db e0          	vpand  %ymm0,%ymm13,%ymm4
    2c40:	c5 fd 72 d0 10       	vpsrld $0x10,%ymm0,%ymm0
    2c45:	c4 43 fd 00 db d8    	vpermq $0xd8,%ymm11,%ymm11
    2c4b:	c4 43 fd 00 e4 d8    	vpermq $0xd8,%ymm12,%ymm12
    2c51:	c4 e2 5d 2b e5       	vpackusdw %ymm5,%ymm4,%ymm4
    2c56:	c5 15 db fa          	vpand  %ymm2,%ymm13,%ymm15
    2c5a:	c5 95 db eb          	vpand  %ymm3,%ymm13,%ymm5
    2c5e:	c5 e5 72 d3 10       	vpsrld $0x10,%ymm3,%ymm3
    2c63:	c4 41 15 db d4       	vpand  %ymm12,%ymm13,%ymm10
    2c68:	c4 c1 1d 72 d4 10    	vpsrld $0x10,%ymm12,%ymm12
    2c6e:	c4 c2 4d 2b f0       	vpackusdw %ymm8,%ymm6,%ymm6
    2c73:	c4 e2 7d 2b c7       	vpackusdw %ymm7,%ymm0,%ymm0
    2c78:	c4 62 05 2b fd       	vpackusdw %ymm5,%ymm15,%ymm15
    2c7d:	c5 d5 72 d2 10       	vpsrld $0x10,%ymm2,%ymm5
    2c82:	c4 41 15 db c3       	vpand  %ymm11,%ymm13,%ymm8
    2c87:	c4 c1 25 72 d3 10    	vpsrld $0x10,%ymm11,%ymm11
    2c8d:	c4 e2 55 2b eb       	vpackusdw %ymm3,%ymm5,%ymm5
    2c92:	c4 e3 fd 00 c0 d8    	vpermq $0xd8,%ymm0,%ymm0
    2c98:	c4 42 3d 2b c2       	vpackusdw %ymm10,%ymm8,%ymm8
    2c9d:	c4 e3 fd 00 ed d8    	vpermq $0xd8,%ymm5,%ymm5
    2ca3:	c4 e3 fd 00 e4 d8    	vpermq $0xd8,%ymm4,%ymm4
    2ca9:	c4 43 fd 00 d0 d8    	vpermq $0xd8,%ymm8,%ymm10
    2caf:	c5 bd 72 d0 10       	vpsrld $0x10,%ymm0,%ymm8
    2cb4:	c5 c5 72 d5 10       	vpsrld $0x10,%ymm5,%ymm7
    2cb9:	c4 43 fd 00 ff d8    	vpermq $0xd8,%ymm15,%ymm15
    2cbf:	c4 62 3d 2b c7       	vpackusdw %ymm7,%ymm8,%ymm8
    2cc4:	c4 c2 75 2b c9       	vpackusdw %ymm9,%ymm1,%ymm1
    2cc9:	c5 95 db d4          	vpand  %ymm4,%ymm13,%ymm2
    2ccd:	c4 c3 fd 00 f8 d8    	vpermq $0xd8,%ymm8,%ymm7
    2cd3:	c4 41 15 db c7       	vpand  %ymm15,%ymm13,%ymm8
    2cd8:	c4 c1 05 72 d7 10    	vpsrld $0x10,%ymm15,%ymm15
    2cde:	c4 e3 fd 00 f6 d8    	vpermq $0xd8,%ymm6,%ymm6
    2ce4:	c4 e3 fd 00 c9 d8    	vpermq $0xd8,%ymm1,%ymm1
    2cea:	c4 c1 45 fd da       	vpaddw %ymm10,%ymm7,%ymm3
    2cef:	c4 c2 6d 2b d0       	vpackusdw %ymm8,%ymm2,%ymm2
    2cf4:	c5 95 db ed          	vpand  %ymm5,%ymm13,%ymm5
    2cf8:	c5 95 db c0          	vpand  %ymm0,%ymm13,%ymm0
    2cfc:	c5 b5 72 d1 10       	vpsrld $0x10,%ymm1,%ymm9
    2d01:	c4 63 fd 00 c2 d8    	vpermq $0xd8,%ymm2,%ymm8
    2d07:	c5 ed 72 d6 10       	vpsrld $0x10,%ymm6,%ymm2
    2d0c:	c4 42 25 2b e4       	vpackusdw %ymm12,%ymm11,%ymm12
    2d11:	c4 c2 6d 2b d1       	vpackusdw %ymm9,%ymm2,%ymm2
    2d16:	c4 e2 7d 2b c5       	vpackusdw %ymm5,%ymm0,%ymm0
    2d1b:	c4 43 fd 00 e4 d8    	vpermq $0xd8,%ymm12,%ymm12
    2d21:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
    2d27:	c4 e3 fd 00 e8 d8    	vpermq $0xd8,%ymm0,%ymm5
    2d2d:	c5 95 db f6          	vpand  %ymm6,%ymm13,%ymm6
    2d31:	c4 41 6d fd c8       	vpaddw %ymm8,%ymm2,%ymm9
    2d36:	c5 bd f9 d2          	vpsubw %ymm2,%ymm8,%ymm2
    2d3a:	c5 2d f9 c7          	vpsubw %ymm7,%ymm10,%ymm8
    2d3e:	c4 c1 55 f9 c4       	vpsubw %ymm12,%ymm5,%ymm0
    2d43:	c4 e2 7d 23 f8       	vpmovsxwd %xmm0,%ymm7
    2d48:	c4 e3 7d 39 c0 01    	vextracti128 $0x1,%ymm0,%xmm0
    2d4e:	c5 95 db c9          	vpand  %ymm1,%ymm13,%ymm1
    2d52:	c5 1d fd dd          	vpaddw %ymm5,%ymm12,%ymm11
    2d56:	c5 a5 fd eb          	vpaddw %ymm3,%ymm11,%ymm5
    2d5a:	c4 e2 45 40 3d 9d 1c 	vpmulld 0x1c9d(%rip),%ymm7,%ymm7        # 4a00 <_IO_stdin_used+0x40>
    2d61:	00 00 
    2d63:	c4 c1 45 fe fe       	vpaddd %ymm14,%ymm7,%ymm7
    2d68:	c5 c5 72 e7 10       	vpsrad $0x10,%ymm7,%ymm7
    2d6d:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
    2d72:	c5 95 db ff          	vpand  %ymm7,%ymm13,%ymm7
    2d76:	c4 c1 65 f9 db       	vpsubw %ymm11,%ymm3,%ymm3
    2d7b:	c4 e2 4d 2b c9       	vpackusdw %ymm1,%ymm6,%ymm1
    2d80:	c5 cd 72 d4 10       	vpsrld $0x10,%ymm4,%ymm6
    2d85:	c5 d5 fd 2d 73 1e 00 	vpaddw 0x1e73(%rip),%ymm5,%ymm5        # 4c00 <_IO_stdin_used+0x240>
    2d8c:	00 
    2d8d:	c4 e2 7d 40 05 6a 1c 	vpmulld 0x1c6a(%rip),%ymm0,%ymm0        # 4a00 <_IO_stdin_used+0x40>
    2d94:	00 00 
    2d96:	c4 c1 7d fe c6       	vpaddd %ymm14,%ymm0,%ymm0
    2d9b:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    2da0:	c5 95 db c0          	vpand  %ymm0,%ymm13,%ymm0
    2da4:	c4 c2 4d 2b f7       	vpackusdw %ymm15,%ymm6,%ymm6
    2da9:	c4 e3 fd 00 c9 d8    	vpermq $0xd8,%ymm1,%ymm1
    2daf:	c4 e2 45 2b f8       	vpackusdw %ymm0,%ymm7,%ymm7
    2db4:	c4 c2 7d 23 c0       	vpmovsxwd %xmm8,%ymm0
    2db9:	c4 43 7d 39 c0 01    	vextracti128 $0x1,%ymm8,%xmm8
    2dbf:	c4 e3 fd 00 f6 d8    	vpermq $0xd8,%ymm6,%ymm6
    2dc5:	c4 e2 7d 40 05 32 1c 	vpmulld 0x1c32(%rip),%ymm0,%ymm0        # 4a00 <_IO_stdin_used+0x40>
    2dcc:	00 00 
    2dce:	c4 c1 7d fe c6       	vpaddd %ymm14,%ymm0,%ymm0
    2dd3:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    2dd8:	c5 95 db c0          	vpand  %ymm0,%ymm13,%ymm0
    2ddc:	c4 42 7d 23 c0       	vpmovsxwd %xmm8,%ymm8
    2de1:	c4 e3 fd 00 ff d8    	vpermq $0xd8,%ymm7,%ymm7
    2de7:	c5 cd fd e1          	vpaddw %ymm1,%ymm6,%ymm4
    2deb:	c5 f5 f9 ce          	vpsubw %ymm6,%ymm1,%ymm1
    2def:	c4 c1 5d fd f1       	vpaddw %ymm9,%ymm4,%ymm6
    2df4:	c4 62 3d 40 05 03 1c 	vpmulld 0x1c03(%rip),%ymm8,%ymm8        # 4a00 <_IO_stdin_used+0x40>
    2dfb:	00 00 
    2dfd:	c4 41 3d fe c6       	vpaddd %ymm14,%ymm8,%ymm8
    2e02:	c4 c1 3d 72 e0 10    	vpsrad $0x10,%ymm8,%ymm8
    2e08:	c4 41 15 db c0       	vpand  %ymm8,%ymm13,%ymm8
    2e0d:	c5 b5 f9 e4          	vpsubw %ymm4,%ymm9,%ymm4
    2e11:	c5 6d fd d1          	vpaddw %ymm1,%ymm2,%ymm10
    2e15:	c5 65 d5 0d 03 1e 00 	vpmullw 0x1e03(%rip),%ymm3,%ymm9        # 4c20 <_IO_stdin_used+0x260>
    2e1c:	00 
    2e1d:	c5 ed f9 c9          	vpsubw %ymm1,%ymm2,%ymm1
    2e21:	c5 4d fd dd          	vpaddw %ymm5,%ymm6,%ymm11
    2e25:	c5 e5 e5 15 f3 1d 00 	vpmulhw 0x1df3(%rip),%ymm3,%ymm2        # 4c20 <_IO_stdin_used+0x260>
    2e2c:	00 
    2e2d:	c5 d5 f9 ee          	vpsubw %ymm6,%ymm5,%ymm5
    2e31:	c5 35 61 e2          	vpunpcklwd %ymm2,%ymm9,%ymm12
    2e35:	c5 dd e5 35 03 1e 00 	vpmulhw 0x1e03(%rip),%ymm4,%ymm6        # 4c40 <_IO_stdin_used+0x280>
    2e3c:	00 
    2e3d:	c5 35 69 ca          	vpunpckhwd %ymm2,%ymm9,%ymm9
    2e41:	c5 c5 71 f7 01       	vpsllw $0x1,%ymm7,%ymm7
    2e46:	c4 c2 7d 2b c0       	vpackusdw %ymm8,%ymm0,%ymm0
    2e4b:	c5 5d d5 05 ed 1d 00 	vpmullw 0x1ded(%rip),%ymm4,%ymm8        # 4c40 <_IO_stdin_used+0x280>
    2e52:	00 
    2e53:	c5 d5 71 e5 03       	vpsraw $0x3,%ymm5,%ymm5
    2e58:	c5 bd 61 d6          	vpunpcklwd %ymm6,%ymm8,%ymm2
    2e5c:	c4 c1 25 71 e3 03    	vpsraw $0x3,%ymm11,%ymm11
    2e62:	c5 bd 69 f6          	vpunpckhwd %ymm6,%ymm8,%ymm6
    2e66:	c4 43 1d 46 f9 20    	vperm2i128 $0x20,%ymm9,%ymm12,%ymm15
    2e6c:	c4 43 1d 46 c9 31    	vperm2i128 $0x31,%ymm9,%ymm12,%ymm9
    2e72:	c4 63 6d 46 c6 20    	vperm2i128 $0x20,%ymm6,%ymm2,%ymm8
    2e78:	c4 e3 6d 46 d6 31    	vperm2i128 $0x31,%ymm6,%ymm2,%ymm2
    2e7e:	c4 e3 fd 00 c0 d8    	vpermq $0xd8,%ymm0,%ymm0
    2e84:	c4 41 3d fe c7       	vpaddd %ymm15,%ymm8,%ymm8
    2e89:	c4 41 3d fe c6       	vpaddd %ymm14,%ymm8,%ymm8
    2e8e:	c4 c1 3d 72 e0 10    	vpsrad $0x10,%ymm8,%ymm8
    2e94:	c4 c1 6d fe d1       	vpaddd %ymm9,%ymm2,%ymm2
    2e99:	c4 41 15 db c0       	vpand  %ymm8,%ymm13,%ymm8
    2e9e:	c4 c1 6d fe d6       	vpaddd %ymm14,%ymm2,%ymm2
    2ea3:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    2ea8:	c5 95 db d2          	vpand  %ymm2,%ymm13,%ymm2
    2eac:	c5 fd 71 f0 01       	vpsllw $0x1,%ymm0,%ymm0
    2eb1:	c4 62 3d 2b c2       	vpackusdw %ymm2,%ymm8,%ymm8
    2eb6:	c5 dd d5 15 a2 1d 00 	vpmullw 0x1da2(%rip),%ymm4,%ymm2        # 4c60 <_IO_stdin_used+0x2a0>
    2ebd:	00 
    2ebe:	c5 dd e5 25 9a 1d 00 	vpmulhw 0x1d9a(%rip),%ymm4,%ymm4        # 4c60 <_IO_stdin_used+0x2a0>
    2ec5:	00 
    2ec6:	c5 6d 61 e4          	vpunpcklwd %ymm4,%ymm2,%ymm12
    2eca:	c5 ed 69 e4          	vpunpckhwd %ymm4,%ymm2,%ymm4
    2ece:	c5 e5 d5 15 6a 1d 00 	vpmullw 0x1d6a(%rip),%ymm3,%ymm2        # 4c40 <_IO_stdin_used+0x280>
    2ed5:	00 
    2ed6:	c5 e5 e5 1d 62 1d 00 	vpmulhw 0x1d62(%rip),%ymm3,%ymm3        # 4c40 <_IO_stdin_used+0x280>
    2edd:	00 
    2ede:	c5 ed 61 f3          	vpunpcklwd %ymm3,%ymm2,%ymm6
    2ee2:	c5 ed 69 db          	vpunpckhwd %ymm3,%ymm2,%ymm3
    2ee6:	c4 63 1d 46 cc 20    	vperm2i128 $0x20,%ymm4,%ymm12,%ymm9
    2eec:	c4 e3 1d 46 e4 31    	vperm2i128 $0x31,%ymm4,%ymm12,%ymm4
    2ef2:	c4 e3 4d 46 d3 20    	vperm2i128 $0x20,%ymm3,%ymm6,%ymm2
    2ef8:	c4 43 fd 00 c0 d8    	vpermq $0xd8,%ymm8,%ymm8
    2efe:	c4 41 6d fe c9       	vpaddd %ymm9,%ymm2,%ymm9
    2f03:	c4 e3 4d 46 d3 31    	vperm2i128 $0x31,%ymm3,%ymm6,%ymm2
    2f09:	c4 41 35 fe ce       	vpaddd %ymm14,%ymm9,%ymm9
    2f0e:	c4 c1 35 72 e1 10    	vpsrad $0x10,%ymm9,%ymm9
    2f14:	c4 41 15 db c9       	vpand  %ymm9,%ymm13,%ymm9
    2f19:	c5 ad fd f0          	vpaddw %ymm0,%ymm10,%ymm6
    2f1d:	c4 c1 7d f9 c2       	vpsubw %ymm10,%ymm0,%ymm0
    2f22:	c5 4d d5 15 56 1d 00 	vpmullw 0x1d56(%rip),%ymm6,%ymm10        # 4c80 <_IO_stdin_used+0x2c0>
    2f29:	00 
    2f2a:	c5 ed fe d4          	vpaddd %ymm4,%ymm2,%ymm2
    2f2e:	c4 c1 6d fe d6       	vpaddd %ymm14,%ymm2,%ymm2
    2f33:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    2f38:	c5 95 db d2          	vpand  %ymm2,%ymm13,%ymm2
    2f3c:	c5 f5 fd e7          	vpaddw %ymm7,%ymm1,%ymm4
    2f40:	c5 c5 f9 f9          	vpsubw %ymm1,%ymm7,%ymm7
    2f44:	c5 dd d5 1d 54 1d 00 	vpmullw 0x1d54(%rip),%ymm4,%ymm3        # 4ca0 <_IO_stdin_used+0x2e0>
    2f4b:	00 
    2f4c:	c5 cd e5 0d 2c 1d 00 	vpmulhw 0x1d2c(%rip),%ymm6,%ymm1        # 4c80 <_IO_stdin_used+0x2c0>
    2f53:	00 
    2f54:	c5 2d 61 e1          	vpunpcklwd %ymm1,%ymm10,%ymm12
    2f58:	c5 2d 69 d1          	vpunpckhwd %ymm1,%ymm10,%ymm10
    2f5c:	c5 dd e5 0d 3c 1d 00 	vpmulhw 0x1d3c(%rip),%ymm4,%ymm1        # 4ca0 <_IO_stdin_used+0x2e0>
    2f63:	00 
    2f64:	c4 62 35 2b ca       	vpackusdw %ymm2,%ymm9,%ymm9
    2f69:	c5 e5 61 d1          	vpunpcklwd %ymm1,%ymm3,%ymm2
    2f6d:	c5 e5 69 c9          	vpunpckhwd %ymm1,%ymm3,%ymm1
    2f71:	c4 43 1d 46 fa 20    	vperm2i128 $0x20,%ymm10,%ymm12,%ymm15
    2f77:	c4 43 1d 46 d2 31    	vperm2i128 $0x31,%ymm10,%ymm12,%ymm10
    2f7d:	c5 4d d5 25 1b 1d 00 	vpmullw 0x1d1b(%rip),%ymm6,%ymm12        # 4ca0 <_IO_stdin_used+0x2e0>
    2f84:	00 
    2f85:	c5 cd e5 35 13 1d 00 	vpmulhw 0x1d13(%rip),%ymm6,%ymm6        # 4ca0 <_IO_stdin_used+0x2e0>
    2f8c:	00 
    2f8d:	c4 e3 6d 46 d9 20    	vperm2i128 $0x20,%ymm1,%ymm2,%ymm3
    2f93:	c4 e3 6d 46 d1 31    	vperm2i128 $0x31,%ymm1,%ymm2,%ymm2
    2f99:	c5 9d 61 ce          	vpunpcklwd %ymm6,%ymm12,%ymm1
    2f9d:	c4 c1 65 fe df       	vpaddd %ymm15,%ymm3,%ymm3
    2fa2:	c4 c1 65 fe de       	vpaddd %ymm14,%ymm3,%ymm3
    2fa7:	c5 e5 72 e3 10       	vpsrad $0x10,%ymm3,%ymm3
    2fac:	c4 c1 6d fe d2       	vpaddd %ymm10,%ymm2,%ymm2
    2fb1:	c5 95 db db          	vpand  %ymm3,%ymm13,%ymm3
    2fb5:	c4 c1 6d fe d6       	vpaddd %ymm14,%ymm2,%ymm2
    2fba:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    2fbf:	c5 95 db d2          	vpand  %ymm2,%ymm13,%ymm2
    2fc3:	c5 5d d5 15 f5 1c 00 	vpmullw 0x1cf5(%rip),%ymm4,%ymm10        # 4cc0 <_IO_stdin_used+0x300>
    2fca:	00 
    2fcb:	c5 9d 69 f6          	vpunpckhwd %ymm6,%ymm12,%ymm6
    2fcf:	c5 dd e5 25 e9 1c 00 	vpmulhw 0x1ce9(%rip),%ymm4,%ymm4        # 4cc0 <_IO_stdin_used+0x300>
    2fd6:	00 
    2fd7:	c4 43 fd 00 c9 d8    	vpermq $0xd8,%ymm9,%ymm9
    2fdd:	c4 e2 65 2b da       	vpackusdw %ymm2,%ymm3,%ymm3
    2fe2:	c5 ad 61 d4          	vpunpcklwd %ymm4,%ymm10,%ymm2
    2fe6:	c5 2d 69 d4          	vpunpckhwd %ymm4,%ymm10,%ymm10
    2fea:	c4 63 75 46 e6 20    	vperm2i128 $0x20,%ymm6,%ymm1,%ymm12
    2ff0:	c4 e3 75 46 ce 31    	vperm2i128 $0x31,%ymm6,%ymm1,%ymm1
    2ff6:	c4 c3 6d 46 e2 20    	vperm2i128 $0x20,%ymm10,%ymm2,%ymm4
    2ffc:	c4 c3 6d 46 d2 31    	vperm2i128 $0x31,%ymm10,%ymm2,%ymm2
    3002:	c5 45 d5 15 f6 1c 00 	vpmullw 0x1cf6(%rip),%ymm7,%ymm10        # 4d00 <_IO_stdin_used+0x340>
    3009:	00 
    300a:	c4 e3 fd 00 db d8    	vpermq $0xd8,%ymm3,%ymm3
    3010:	c5 9d fe e4          	vpaddd %ymm4,%ymm12,%ymm4
    3014:	c4 c1 5d fe e6       	vpaddd %ymm14,%ymm4,%ymm4
    3019:	c5 dd 72 e4 10       	vpsrad $0x10,%ymm4,%ymm4
    301e:	c5 f5 fe ca          	vpaddd %ymm2,%ymm1,%ymm1
    3022:	c5 95 db e4          	vpand  %ymm4,%ymm13,%ymm4
    3026:	c4 c1 75 fe ce       	vpaddd %ymm14,%ymm1,%ymm1
    302b:	c5 f5 72 e1 10       	vpsrad $0x10,%ymm1,%ymm1
    3030:	c5 95 db c9          	vpand  %ymm1,%ymm13,%ymm1
    3034:	c5 fd d5 15 a4 1c 00 	vpmullw 0x1ca4(%rip),%ymm0,%ymm2        # 4ce0 <_IO_stdin_used+0x320>
    303b:	00 
    303c:	c4 e2 5d 2b e1       	vpackusdw %ymm1,%ymm4,%ymm4
    3041:	c5 fd e5 0d 97 1c 00 	vpmulhw 0x1c97(%rip),%ymm0,%ymm1        # 4ce0 <_IO_stdin_used+0x320>
    3048:	00 
    3049:	c5 6d 61 e1          	vpunpcklwd %ymm1,%ymm2,%ymm12
    304d:	c5 ed 69 d1          	vpunpckhwd %ymm1,%ymm2,%ymm2
    3051:	c5 c5 e5 0d a7 1c 00 	vpmulhw 0x1ca7(%rip),%ymm7,%ymm1        # 4d00 <_IO_stdin_used+0x340>
    3058:	00 
    3059:	c5 ad 61 f1          	vpunpcklwd %ymm1,%ymm10,%ymm6
    305d:	c5 ad 69 c9          	vpunpckhwd %ymm1,%ymm10,%ymm1
    3061:	c4 63 1d 46 fa 20    	vperm2i128 $0x20,%ymm2,%ymm12,%ymm15
    3067:	c4 e3 1d 46 d2 31    	vperm2i128 $0x31,%ymm2,%ymm12,%ymm2
    306d:	c5 7d d5 25 8b 1c 00 	vpmullw 0x1c8b(%rip),%ymm0,%ymm12        # 4d00 <_IO_stdin_used+0x340>
    3074:	00 
    3075:	c5 fd e5 05 83 1c 00 	vpmulhw 0x1c83(%rip),%ymm0,%ymm0        # 4d00 <_IO_stdin_used+0x340>
    307c:	00 
    307d:	c4 63 4d 46 d1 20    	vperm2i128 $0x20,%ymm1,%ymm6,%ymm10
    3083:	c4 e3 4d 46 f1 31    	vperm2i128 $0x31,%ymm1,%ymm6,%ymm6
    3089:	c5 c5 d5 0d 8f 1c 00 	vpmullw 0x1c8f(%rip),%ymm7,%ymm1        # 4d20 <_IO_stdin_used+0x360>
    3090:	00 
    3091:	c5 c5 e5 3d 87 1c 00 	vpmulhw 0x1c87(%rip),%ymm7,%ymm7        # 4d20 <_IO_stdin_used+0x360>
    3098:	00 
    3099:	c4 e3 fd 00 e4 d8    	vpermq $0xd8,%ymm4,%ymm4
    309f:	c4 41 2d fe d7       	vpaddd %ymm15,%ymm10,%ymm10
    30a4:	c4 41 2d fe d6       	vpaddd %ymm14,%ymm10,%ymm10
    30a9:	c4 c1 2d 72 e2 10    	vpsrad $0x10,%ymm10,%ymm10
    30af:	c5 cd fe f2          	vpaddd %ymm2,%ymm6,%ymm6
    30b3:	c4 41 15 db d2       	vpand  %ymm10,%ymm13,%ymm10
    30b8:	c4 c1 4d fe f6       	vpaddd %ymm14,%ymm6,%ymm6
    30bd:	c5 cd 72 e6 10       	vpsrad $0x10,%ymm6,%ymm6
    30c2:	c5 95 db f6          	vpand  %ymm6,%ymm13,%ymm6
    30c6:	c4 62 2d 2b d6       	vpackusdw %ymm6,%ymm10,%ymm10
    30cb:	c5 f5 61 f7          	vpunpcklwd %ymm7,%ymm1,%ymm6
    30cf:	c5 f5 69 ff          	vpunpckhwd %ymm7,%ymm1,%ymm7
    30d3:	c5 9d 61 c8          	vpunpcklwd %ymm0,%ymm12,%ymm1
    30d7:	c5 9d 69 c0          	vpunpckhwd %ymm0,%ymm12,%ymm0
    30db:	c4 e3 4d 46 d7 20    	vperm2i128 $0x20,%ymm7,%ymm6,%ymm2
    30e1:	c4 e3 4d 46 ff 31    	vperm2i128 $0x31,%ymm7,%ymm6,%ymm7
    30e7:	c4 63 75 46 e0 20    	vperm2i128 $0x20,%ymm0,%ymm1,%ymm12
    30ed:	c4 e3 75 46 c0 31    	vperm2i128 $0x31,%ymm0,%ymm1,%ymm0
    30f3:	c4 43 fd 00 d2 d8    	vpermq $0xd8,%ymm10,%ymm10
    30f9:	c5 9d fe d2          	vpaddd %ymm2,%ymm12,%ymm2
    30fd:	c4 c1 6d fe d6       	vpaddd %ymm14,%ymm2,%ymm2
    3102:	c5 ed 72 e2 10       	vpsrad $0x10,%ymm2,%ymm2
    3107:	c5 fd fe c7          	vpaddd %ymm7,%ymm0,%ymm0
    310b:	c5 95 db ca          	vpand  %ymm2,%ymm13,%ymm1
    310f:	c4 c1 7d fe c6       	vpaddd %ymm14,%ymm0,%ymm0
    3114:	c5 fd 72 e0 10       	vpsrad $0x10,%ymm0,%ymm0
    3119:	c5 95 db c0          	vpand  %ymm0,%ymm13,%ymm0
    311d:	c4 e2 75 2b c0       	vpackusdw %ymm0,%ymm1,%ymm0
    3122:	c5 a5 61 cd          	vpunpcklwd %ymm5,%ymm11,%ymm1
    3126:	c5 25 69 dd          	vpunpckhwd %ymm5,%ymm11,%ymm11
    312a:	c4 e3 fd 00 c0 d8    	vpermq $0xd8,%ymm0,%ymm0
    3130:	c4 c3 75 46 f3 20    	vperm2i128 $0x20,%ymm11,%ymm1,%ymm6
    3136:	c4 43 75 46 db 31    	vperm2i128 $0x31,%ymm11,%ymm1,%ymm11
    313c:	c4 c1 65 61 ca       	vpunpcklwd %ymm10,%ymm3,%ymm1
    3141:	c4 c1 65 69 da       	vpunpckhwd %ymm10,%ymm3,%ymm3
    3146:	c4 63 75 46 e3 20    	vperm2i128 $0x20,%ymm3,%ymm1,%ymm12
    314c:	c4 63 75 46 d3 31    	vperm2i128 $0x31,%ymm3,%ymm1,%ymm10
    3152:	c4 c1 3d 61 c9       	vpunpcklwd %ymm9,%ymm8,%ymm1
    3157:	c4 41 3d 69 c1       	vpunpckhwd %ymm9,%ymm8,%ymm8
    315c:	c4 c3 75 46 d0 20    	vperm2i128 $0x20,%ymm8,%ymm1,%ymm2
    3162:	c4 43 75 46 c0 31    	vperm2i128 $0x31,%ymm8,%ymm1,%ymm8
    3168:	c5 fd 61 cc          	vpunpcklwd %ymm4,%ymm0,%ymm1
    316c:	c5 fd 69 e4          	vpunpckhwd %ymm4,%ymm0,%ymm4
    3170:	c5 cd 61 ea          	vpunpcklwd %ymm2,%ymm6,%ymm5
    3174:	c4 41 25 69 c8       	vpunpckhwd %ymm8,%ymm11,%ymm9
    3179:	c4 63 75 46 fc 20    	vperm2i128 $0x20,%ymm4,%ymm1,%ymm15
    317f:	c5 cd 69 d2          	vpunpckhwd %ymm2,%ymm6,%ymm2
    3183:	c4 e3 75 46 e4 31    	vperm2i128 $0x31,%ymm4,%ymm1,%ymm4
    3189:	c4 c1 1d 61 c7       	vpunpcklwd %ymm15,%ymm12,%ymm0
    318e:	c4 c1 25 61 c8       	vpunpcklwd %ymm8,%ymm11,%ymm1
    3193:	c4 41 1d 69 c7       	vpunpckhwd %ymm15,%ymm12,%ymm8
    3198:	c4 e3 55 46 fa 20    	vperm2i128 $0x20,%ymm2,%ymm5,%ymm7
    319e:	c4 e3 55 46 ea 31    	vperm2i128 $0x31,%ymm2,%ymm5,%ymm5
    31a4:	c4 c3 7d 46 d0 20    	vperm2i128 $0x20,%ymm8,%ymm0,%ymm2
    31aa:	c4 c3 75 46 d9 20    	vperm2i128 $0x20,%ymm9,%ymm1,%ymm3
    31b0:	c4 43 7d 46 c0 31    	vperm2i128 $0x31,%ymm8,%ymm0,%ymm8
    31b6:	c5 c5 61 f2          	vpunpcklwd %ymm2,%ymm7,%ymm6
    31ba:	c4 43 75 46 c9 31    	vperm2i128 $0x31,%ymm9,%ymm1,%ymm9
    31c0:	c5 c5 69 fa          	vpunpckhwd %ymm2,%ymm7,%ymm7
    31c4:	c5 ad 61 cc          	vpunpcklwd %ymm4,%ymm10,%ymm1
    31c8:	c5 2d 69 d4          	vpunpckhwd %ymm4,%ymm10,%ymm10
    31cc:	c4 e3 4d 46 c7 20    	vperm2i128 $0x20,%ymm7,%ymm6,%ymm0
    31d2:	c4 c1 55 61 e0       	vpunpcklwd %ymm8,%ymm5,%ymm4
    31d7:	c4 43 75 46 da 20    	vperm2i128 $0x20,%ymm10,%ymm1,%ymm11
    31dd:	c4 c1 55 69 e8       	vpunpckhwd %ymm8,%ymm5,%ymm5
    31e2:	c5 f8 11 80 00 ff ff 	vmovups %xmm0,-0x100(%rax)
    31e9:	ff 
    31ea:	c4 e3 7d 39 80 10 ff 	vextracti128 $0x1,%ymm0,-0xf0(%rax)
    31f1:	ff ff 01 
    31f4:	c4 c1 65 61 d3       	vpunpcklwd %ymm11,%ymm3,%ymm2
    31f9:	c4 e3 5d 46 c5 20    	vperm2i128 $0x20,%ymm5,%ymm4,%ymm0
    31ff:	c4 c1 65 69 db       	vpunpckhwd %ymm11,%ymm3,%ymm3
    3204:	c4 c3 75 46 ca 31    	vperm2i128 $0x31,%ymm10,%ymm1,%ymm1
    320a:	c5 f8 11 80 40 ff ff 	vmovups %xmm0,-0xc0(%rax)
    3211:	ff 
    3212:	c4 e3 7d 39 80 50 ff 	vextracti128 $0x1,%ymm0,-0xb0(%rax)
    3219:	ff ff 01 
    321c:	c4 e3 6d 46 c3 20    	vperm2i128 $0x20,%ymm3,%ymm2,%ymm0
    3222:	c4 e3 6d 46 d3 31    	vperm2i128 $0x31,%ymm3,%ymm2,%ymm2
    3228:	c4 e3 4d 46 f7 31    	vperm2i128 $0x31,%ymm7,%ymm6,%ymm6
    322e:	c5 f8 11 40 80       	vmovups %xmm0,-0x80(%rax)
    3233:	c4 e3 7d 39 40 90 01 	vextracti128 $0x1,%ymm0,-0x70(%rax)
    323a:	c5 b5 61 c1          	vpunpcklwd %ymm1,%ymm9,%ymm0
    323e:	c5 b5 69 c9          	vpunpckhwd %ymm1,%ymm9,%ymm1
    3242:	c5 f8 11 50 a0       	vmovups %xmm2,-0x60(%rax)
    3247:	c4 e3 7d 39 50 b0 01 	vextracti128 $0x1,%ymm2,-0x50(%rax)
    324e:	c4 e3 5d 46 e5 31    	vperm2i128 $0x31,%ymm5,%ymm4,%ymm4
    3254:	c4 e3 7d 46 d1 20    	vperm2i128 $0x20,%ymm1,%ymm0,%ymm2
    325a:	c4 e3 7d 46 c1 31    	vperm2i128 $0x31,%ymm1,%ymm0,%ymm0
    3260:	c5 f8 11 b0 20 ff ff 	vmovups %xmm6,-0xe0(%rax)
    3267:	ff 
    3268:	c4 e3 7d 39 b0 30 ff 	vextracti128 $0x1,%ymm6,-0xd0(%rax)
    326f:	ff ff 01 
    3272:	c5 f8 11 a0 60 ff ff 	vmovups %xmm4,-0xa0(%rax)
    3279:	ff 
    327a:	c4 e3 7d 39 a0 70 ff 	vextracti128 $0x1,%ymm4,-0x90(%rax)
    3281:	ff ff 01 
    3284:	c5 f8 11 50 c0       	vmovups %xmm2,-0x40(%rax)
    3289:	c4 e3 7d 39 50 d0 01 	vextracti128 $0x1,%ymm2,-0x30(%rax)
    3290:	c5 f8 11 40 e0       	vmovups %xmm0,-0x20(%rax)
    3295:	c4 e3 7d 39 40 f0 01 	vextracti128 $0x1,%ymm0,-0x10(%rax)
    329c:	39 ca                	cmp    %ecx,%edx
    329e:	0f 82 3c f8 ff ff    	jb     2ae0 <fastDCT+0x250>
    32a4:	45 89 f2             	mov    %r14d,%r10d
    32a7:	41 83 e2 f0          	and    $0xfffffff0,%r10d
    32ab:	44 89 d0             	mov    %r10d,%eax
    32ae:	48 c1 e0 04          	shl    $0x4,%rax
    32b2:	49 01 c5             	add    %rax,%r13
    32b5:	45 39 d6             	cmp    %r10d,%r14d
    32b8:	0f 84 92 01 00 00    	je     3450 <F_1_501+0x445>
    32be:	c5 f8 77             	vzeroupper 
    32c1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    32c8:	41 0f b7 55 00       	movzwl 0x0(%r13),%edx
    32cd:	41 0f b7 4d 0e       	movzwl 0xe(%r13),%ecx
    32d2:	45 0f b7 65 08       	movzwl 0x8(%r13),%r12d
    32d7:	41 0f b7 45 06       	movzwl 0x6(%r13),%eax
    32dc:	45 0f b7 45 0c       	movzwl 0xc(%r13),%r8d
    32e1:	45 0f b7 7d 0a       	movzwl 0xa(%r13),%r15d
    32e6:	41 0f b7 7d 02       	movzwl 0x2(%r13),%edi
    32eb:	45 0f b7 4d 04       	movzwl 0x4(%r13),%r9d
    32f0:	01 d1                	add    %edx,%ecx
    32f2:	66 41 2b 55 0e       	sub    0xe(%r13),%dx
    32f7:	45 8d 1c 04          	lea    (%r12,%rax,1),%r11d
    32fb:	44 29 e0             	sub    %r12d,%eax
    32fe:	98                   	cwtl   
    32ff:	69 c0 05 b5 00 00    	imul   $0xb505,%eax,%eax
    3305:	41 8d 34 38          	lea    (%r8,%rdi,1),%esi
    3309:	43 8d 1c 0f          	lea    (%r15,%r9,1),%ebx
    330d:	0f bf d2             	movswl %dx,%edx
    3310:	45 29 f9             	sub    %r15d,%r9d
    3313:	44 29 c7             	sub    %r8d,%edi
    3316:	44 8d 3c 33          	lea    (%rbx,%rsi,1),%r15d
    331a:	45 8d 44 0b 04       	lea    0x4(%r11,%rcx,1),%r8d
    331f:	46 8d 24 0f          	lea    (%rdi,%r9,1),%r12d
    3323:	69 d2 05 b5 00 00    	imul   $0xb505,%edx,%edx
    3329:	05 00 80 00 00       	add    $0x8000,%eax
    332e:	44 29 cf             	sub    %r9d,%edi
    3331:	c1 f8 10             	sar    $0x10,%eax
    3334:	44 29 d9             	sub    %r11d,%ecx
    3337:	29 de                	sub    %ebx,%esi
    3339:	01 c0                	add    %eax,%eax
    333b:	0f bf f6             	movswl %si,%esi
    333e:	0f bf c9             	movswl %cx,%ecx
    3341:	81 c2 00 80 00 00    	add    $0x8000,%edx
    3347:	44 8d 1c 07          	lea    (%rdi,%rax,1),%r11d
    334b:	29 f8                	sub    %edi,%eax
    334d:	c1 fa 10             	sar    $0x10,%edx
    3350:	43 8d 3c 07          	lea    (%r15,%r8,1),%edi
    3354:	98                   	cwtl   
    3355:	01 d2                	add    %edx,%edx
    3357:	45 0f bf db          	movswl %r11w,%r11d
    335b:	45 29 f8             	sub    %r15d,%r8d
    335e:	45 8d 0c 14          	lea    (%r12,%rdx,1),%r9d
    3362:	66 c1 ff 03          	sar    $0x3,%di
    3366:	44 29 e2             	sub    %r12d,%edx
    3369:	66 41 89 7d 00       	mov    %di,0x0(%r13)
    336e:	0f bf d2             	movswl %dx,%edx
    3371:	66 41 c1 f8 03       	sar    $0x3,%r8w
    3376:	45 0f bf c9          	movswl %r9w,%r9d
    337a:	66 45 89 45 08       	mov    %r8w,0x8(%r13)
    337f:	41 69 d9 62 1f 00 00 	imul   $0x1f62,%r9d,%ebx
    3386:	41 69 fb 3e 06 00 00 	imul   $0x63e,%r11d,%edi
    338d:	8d bc 3b 00 80 00 00 	lea    0x8000(%rbx,%rdi,1),%edi
    3394:	69 d9 cf 29 00 00    	imul   $0x29cf,%ecx,%ebx
    339a:	c1 ff 10             	sar    $0x10,%edi
    339d:	66 41 89 7d 02       	mov    %di,0x2(%r13)
    33a2:	69 fe 51 11 00 00    	imul   $0x1151,%esi,%edi
    33a8:	8d bc 3b 00 80 00 00 	lea    0x8000(%rbx,%rdi,1),%edi
    33af:	69 d8 39 ee ff ff    	imul   $0xffffee39,%eax,%ebx
    33b5:	c1 ff 10             	sar    $0x10,%edi
    33b8:	66 41 89 7d 04       	mov    %di,0x4(%r13)
    33bd:	69 c0 9b 1a 00 00    	imul   $0x1a9b,%eax,%eax
    33c3:	69 fa 9b 1a 00 00    	imul   $0x1a9b,%edx,%edi
    33c9:	69 d2 c7 11 00 00    	imul   $0x11c7,%edx,%edx
    33cf:	8d bc 3b 00 80 00 00 	lea    0x8000(%rbx,%rdi,1),%edi
    33d6:	c1 ff 10             	sar    $0x10,%edi
    33d9:	8d 84 02 00 80 00 00 	lea    0x8000(%rdx,%rax,1),%eax
    33e0:	66 41 89 7d 06       	mov    %di,0x6(%r13)
    33e5:	c1 f8 10             	sar    $0x10,%eax
    33e8:	41 83 c2 01          	add    $0x1,%r10d
    33ec:	49 83 c5 10          	add    $0x10,%r13
    33f0:	69 f6 31 d6 ff ff    	imul   $0xffffd631,%esi,%esi
    33f6:	66 41 89 45 fa       	mov    %ax,-0x6(%r13)
    33fb:	69 c9 51 11 00 00    	imul   $0x1151,%ecx,%ecx
    3401:	45 69 db 9e e0 ff ff 	imul   $0xffffe09e,%r11d,%r11d
    3408:	8d 84 0e 00 80 00 00 	lea    0x8000(%rsi,%rcx,1),%eax
    340f:	45 69 c9 3e 06 00 00 	imul   $0x63e,%r9d,%r9d
    3416:	c1 f8 10             	sar    $0x10,%eax
    3419:	66 41 89 45 fc       	mov    %ax,-0x4(%r13)
    341e:	43 8d 84 0b 00 80 00 	lea    0x8000(%r11,%r9,1),%eax
    3425:	00 
    3426:	c1 f8 10             	sar    $0x10,%eax
    3429:	66 41 89 45 fe       	mov    %ax,-0x2(%r13)
    342e:	45 39 d6             	cmp    %r10d,%r14d
    3431:	0f 87 91 fe ff ff    	ja     32c8 <F_1_501+0x2bd>
    3437:	5b                   	pop    %rbx
    3438:	41 5a                	pop    %r10
    343a:	41 5c                	pop    %r12
    343c:	41 5d                	pop    %r13
    343e:	41 5e                	pop    %r14
    3440:	41 5f                	pop    %r15
    3442:	5d                   	pop    %rbp
    3443:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    3447:	c3                   	retq   
    3448:	45 31 d2             	xor    %r10d,%r10d
    344b:	e9 78 fe ff ff       	jmpq   32c8 <F_1_501+0x2bd>
    3450:	c5 f8 77             	vzeroupper 
    3453:	eb e2                	jmp    3437 <F_1_501+0x42c>
    3455:	90                   	nop
    3456:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    345d:	00 00 00 

0000000000003460 <fastIDCTInit>:
    3460:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
    3465:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
    3469:	41 ff 72 f8          	pushq  -0x8(%r10)
    346d:	55                   	push   %rbp
    346e:	48 89 e5             	mov    %rsp,%rbp
    3471:	41 52                	push   %r10
    3473:	4c 8d 45 90          	lea    -0x70(%rbp),%r8
    3477:	48 83 ec 68          	sub    $0x68,%rsp
    347b:	c5 fd 6f 05 bd 18 00 	vmovdqa 0x18bd(%rip),%ymm0        # 4d40 <_IO_stdin_used+0x380>
    3482:	00 
    3483:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    348a:	00 00 
    348c:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    3490:	31 c0                	xor    %eax,%eax
    3492:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
    3497:	c5 fd 6f 05 c1 18 00 	vmovdqa 0x18c1(%rip),%ymm0        # 4d60 <_IO_stdin_used+0x3a0>
    349e:	00 
    349f:	c5 fd 7f 45 b0       	vmovdqa %ymm0,-0x50(%rbp)
    34a4:	0f 1f 40 00          	nopl   0x0(%rax)
    34a8:	0f b7 0c 47          	movzwl (%rdi,%rax,2),%ecx
    34ac:	49 0f be 14 00       	movsbq (%r8,%rax,1),%rdx
    34b1:	48 83 c0 01          	add    $0x1,%rax
    34b5:	48 83 f8 40          	cmp    $0x40,%rax
    34b9:	66 89 0c 56          	mov    %cx,(%rsi,%rdx,2)
    34bd:	75 e9                	jne    34a8 <fastIDCTInit+0x48>
    34bf:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    34c3:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    34ca:	00 00 
    34cc:	75 0f                	jne    34dd <fastIDCTInit+0x7d>
    34ce:	c5 f8 77             	vzeroupper 
    34d1:	48 83 c4 68          	add    $0x68,%rsp
    34d5:	41 5a                	pop    %r10
    34d7:	5d                   	pop    %rbp
    34d8:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    34dc:	c3                   	retq   
    34dd:	c5 f8 77             	vzeroupper 
    34e0:	e8 3b d1 ff ff       	callq  620 <__stack_chk_fail@plt>
    34e5:	90                   	nop
    34e6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    34ed:	00 00 00 

00000000000034f0 <fastIDCT>:
    34f0:	41 57                	push   %r15
    34f2:	48 8d 47 10          	lea    0x10(%rdi),%rax
    34f6:	41 56                	push   %r14
    34f8:	41 55                	push   %r13
    34fa:	41 54                	push   %r12
    34fc:	4c 8d 9f 90 00 00 00 	lea    0x90(%rdi),%r11
    3503:	55                   	push   %rbp
    3504:	53                   	push   %rbx
    3505:	48 89 c3             	mov    %rax,%rbx
    3508:	c5 f9 6f 25 80 18 00 	vmovdqa 0x1880(%rip),%xmm4        # 4d90 <_IO_stdin_used+0x3d0>
    350f:	00 
    3510:	48 89 44 24 f0       	mov    %rax,-0x10(%rsp)
    3515:	c5 f9 6f 1d 63 18 00 	vmovdqa 0x1863(%rip),%xmm3        # 4d80 <_IO_stdin_used+0x3c0>
    351c:	00 
    351d:	0f 1f 00             	nopl   (%rax)
    3520:	44 0f bf 53 f2       	movswl -0xe(%rbx),%r10d
    3525:	44 0f bf 63 f4       	movswl -0xc(%rbx),%r12d
    352a:	44 0f bf 4b f6       	movswl -0xa(%rbx),%r9d
    352f:	0f bf 4b f8          	movswl -0x8(%rbx),%ecx
    3533:	0f bf 73 fa          	movswl -0x6(%rbx),%esi
    3537:	0f bf 53 fc          	movswl -0x4(%rbx),%edx
    353b:	0f bf 6b fe          	movswl -0x2(%rbx),%ebp
    353f:	0f bf 43 f0          	movswl -0x10(%rbx),%eax
    3543:	45 89 d0             	mov    %r10d,%r8d
    3546:	45 09 e0             	or     %r12d,%r8d
    3549:	45 09 c8             	or     %r9d,%r8d
    354c:	41 09 c8             	or     %ecx,%r8d
    354f:	41 89 c5             	mov    %eax,%r13d
    3552:	41 09 f0             	or     %esi,%r8d
    3555:	41 09 d0             	or     %edx,%r8d
    3558:	66 41 09 e8          	or     %bp,%r8w
    355c:	0f 84 de 01 00 00    	je     3740 <fastIDCT+0x250>
    3562:	46 8d 04 20          	lea    (%rax,%r12,1),%r8d
    3566:	44 29 e0             	sub    %r12d,%eax
    3569:	c1 e0 0d             	shl    $0xd,%eax
    356c:	41 c1 e0 0d          	shl    $0xd,%r8d
    3570:	45 85 c9             	test   %r9d,%r9d
    3573:	0f 84 f7 00 00 00    	je     3670 <fastIDCT+0x180>
    3579:	45 85 d2             	test   %r10d,%r10d
    357c:	0f 84 7e 06 00 00    	je     3c00 <F_1_847+0xdf>
    3582:	47 8d 24 0a          	lea    (%r10,%r9,1),%r12d
    3586:	45 69 c9 df c4 ff ff 	imul   $0xffffc4df,%r9d,%r9d
    358d:	45 69 e4 51 11 00 00 	imul   $0x1151,%r12d,%r12d
    3594:	45 69 d2 7e 18 00 00 	imul   $0x187e,%r10d,%r10d
    359b:	47 8d 2c 21          	lea    (%r9,%r12,1),%r13d
    359f:	45 01 d4             	add    %r10d,%r12d
    35a2:	45 89 c1             	mov    %r8d,%r9d
    35a5:	41 89 c2             	mov    %eax,%r10d
    35a8:	45 01 e0             	add    %r12d,%r8d
    35ab:	45 29 e1             	sub    %r12d,%r9d
    35ae:	45 29 ea             	sub    %r13d,%r10d
    35b1:	44 01 e8             	add    %r13d,%eax
    35b4:	85 ed                	test   %ebp,%ebp
    35b6:	0f 84 d4 00 00 00    	je     3690 <fastIDCT+0x1a0>
    35bc:	85 d2                	test   %edx,%edx
    35be:	0f 84 9c 05 00 00    	je     3b60 <F_1_847+0x3f>
    35c4:	85 f6                	test   %esi,%esi
    35c6:	0f 84 ac 08 00 00    	je     3e78 <F_1_847+0x357>
    35cc:	44 69 f6 54 62 00 00 	imul   $0x6254,%esi,%r14d
    35d3:	44 8d 2c 16          	lea    (%rsi,%rdx,1),%r13d
    35d7:	01 ee                	add    %ebp,%esi
    35d9:	44 69 fa b3 41 00 00 	imul   $0x41b3,%edx,%r15d
    35e0:	44 89 74 24 e4       	mov    %r14d,-0x1c(%rsp)
    35e5:	45 69 ed fd ad ff ff 	imul   $0xffffadfd,%r13d,%r13d
    35ec:	44 69 f5 8e 09 00 00 	imul   $0x98e,%ebp,%r14d
    35f3:	44 69 e6 3b c1 ff ff 	imul   $0xffffc13b,%esi,%r12d
    35fa:	85 c9                	test   %ecx,%ecx
    35fc:	0f 84 be 0e 00 00    	je     44c0 <F_2_053+0x30d>
    3602:	01 ca                	add    %ecx,%edx
    3604:	01 cd                	add    %ecx,%ebp
    3606:	01 d6                	add    %edx,%esi
    3608:	69 f6 a1 25 00 00    	imul   $0x25a1,%esi,%esi
    360e:	69 ed 33 e3 ff ff    	imul   $0xffffe333,%ebp,%ebp
    3614:	41 01 f4             	add    %esi,%r12d
    3617:	69 d2 84 f3 ff ff    	imul   $0xfffff384,%edx,%edx
    361d:	69 c9 0b 30 00 00    	imul   $0x300b,%ecx,%ecx
    3623:	01 d6                	add    %edx,%esi
    3625:	42 8d 54 25 00       	lea    0x0(%rbp,%r12,1),%edx
    362a:	45 01 ec             	add    %r13d,%r12d
    362d:	41 01 d6             	add    %edx,%r14d
    3630:	42 8d 14 2e          	lea    (%rsi,%r13,1),%edx
    3634:	01 ee                	add    %ebp,%esi
    3636:	01 f1                	add    %esi,%ecx
    3638:	89 c6                	mov    %eax,%esi
    363a:	45 89 c5             	mov    %r8d,%r13d
    363d:	41 01 d7             	add    %edx,%r15d
    3640:	8b 54 24 e4          	mov    -0x1c(%rsp),%edx
    3644:	41 29 cd             	sub    %ecx,%r13d
    3647:	41 01 c8             	add    %ecx,%r8d
    364a:	44 01 e2             	add    %r12d,%edx
    364d:	45 89 d4             	mov    %r10d,%r12d
    3650:	45 01 fa             	add    %r15d,%r10d
    3653:	29 d6                	sub    %edx,%esi
    3655:	45 29 fc             	sub    %r15d,%r12d
    3658:	01 d0                	add    %edx,%eax
    365a:	89 74 24 e8          	mov    %esi,-0x18(%rsp)
    365e:	44 89 ce             	mov    %r9d,%esi
    3661:	45 01 f1             	add    %r14d,%r9d
    3664:	44 29 f6             	sub    %r14d,%esi
    3667:	89 74 24 ec          	mov    %esi,-0x14(%rsp)
    366b:	e9 b0 09 00 00       	jmpq   4020 <F_1_961+0x15b>
    3670:	45 85 d2             	test   %r10d,%r10d
    3673:	0f 84 9f 05 00 00    	je     3c18 <F_1_847+0xf7>
    3679:	45 69 ea 51 11 00 00 	imul   $0x1151,%r10d,%r13d
    3680:	45 69 e2 cf 29 00 00 	imul   $0x29cf,%r10d,%r12d
    3687:	e9 16 ff ff ff       	jmpq   35a2 <fastIDCT+0xb2>
    368c:	0f 1f 40 00          	nopl   0x0(%rax)
    3690:	85 d2                	test   %edx,%edx
    3692:	0f 84 90 05 00 00    	je     3c28 <F_1_847+0x107>
    3698:	85 f6                	test   %esi,%esi
    369a:	0f 84 f0 08 00 00    	je     3f90 <F_1_961+0xcb>
    36a0:	44 69 e6 3b c1 ff ff 	imul   $0xffffc13b,%esi,%r12d
    36a7:	85 c9                	test   %ecx,%ecx
    36a9:	8d 2c 16             	lea    (%rsi,%rdx,1),%ebp
    36ac:	0f 84 ae 0d 00 00    	je     4460 <F_2_053+0x2ad>
    36b2:	44 8d 2c 11          	lea    (%rcx,%rdx,1),%r13d
    36b6:	69 ed fd ad ff ff    	imul   $0xffffadfd,%ebp,%ebp
    36bc:	46 8d 34 2e          	lea    (%rsi,%r13,1),%r14d
    36c0:	45 69 ed 84 f3 ff ff 	imul   $0xfffff384,%r13d,%r13d
    36c7:	45 69 f6 a1 25 00 00 	imul   $0x25a1,%r14d,%r14d
    36ce:	44 69 f9 0b 30 00 00 	imul   $0x300b,%ecx,%r15d
    36d5:	45 01 f5             	add    %r14d,%r13d
    36d8:	45 01 f4             	add    %r14d,%r12d
    36db:	69 c9 33 e3 ff ff    	imul   $0xffffe333,%ecx,%ecx
    36e1:	46 8d 74 2d 00       	lea    0x0(%rbp,%r13,1),%r14d
    36e6:	44 01 e5             	add    %r12d,%ebp
    36e9:	44 89 7c 24 e4       	mov    %r15d,-0x1c(%rsp)
    36ee:	69 d2 b3 41 00 00    	imul   $0x41b3,%edx,%edx
    36f4:	46 8d 3c 21          	lea    (%rcx,%r12,1),%r15d
    36f8:	44 01 e9             	add    %r13d,%ecx
    36fb:	69 f6 54 62 00 00    	imul   $0x6254,%esi,%esi
    3701:	03 4c 24 e4          	add    -0x1c(%rsp),%ecx
    3705:	45 89 c5             	mov    %r8d,%r13d
    3708:	45 89 d4             	mov    %r10d,%r12d
    370b:	44 01 f2             	add    %r14d,%edx
    370e:	41 89 c6             	mov    %eax,%r14d
    3711:	01 ee                	add    %ebp,%esi
    3713:	41 29 d4             	sub    %edx,%r12d
    3716:	41 01 d2             	add    %edx,%r10d
    3719:	41 29 f6             	sub    %esi,%r14d
    371c:	41 29 cd             	sub    %ecx,%r13d
    371f:	01 f0                	add    %esi,%eax
    3721:	44 89 74 24 e8       	mov    %r14d,-0x18(%rsp)
    3726:	45 89 ce             	mov    %r9d,%r14d
    3729:	41 01 c8             	add    %ecx,%r8d
    372c:	45 29 fe             	sub    %r15d,%r14d
    372f:	45 01 f9             	add    %r15d,%r9d
    3732:	44 89 74 24 ec       	mov    %r14d,-0x14(%rsp)
    3737:	e9 e4 08 00 00       	jmpq   4020 <F_1_961+0x15b>
    373c:	0f 1f 40 00          	nopl   0x0(%rax)
    3740:	85 c0                	test   %eax,%eax
    3742:	74 24                	je     3768 <fastIDCT+0x278>
    3744:	41 c1 e5 02          	shl    $0x2,%r13d
    3748:	41 0f b7 c5          	movzwl %r13w,%eax
    374c:	41 c1 e5 10          	shl    $0x10,%r13d
    3750:	41 09 c5             	or     %eax,%r13d
    3753:	44 89 6b f0          	mov    %r13d,-0x10(%rbx)
    3757:	44 89 6b f4          	mov    %r13d,-0xc(%rbx)
    375b:	44 89 6b f8          	mov    %r13d,-0x8(%rbx)
    375f:	44 89 6b fc          	mov    %r13d,-0x4(%rbx)
    3763:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3768:	48 83 c3 10          	add    $0x10,%rbx
    376c:	4c 39 db             	cmp    %r11,%rbx
    376f:	0f 85 ab fd ff ff    	jne    3520 <fastIDCT+0x30>
    3775:	48 8b 6c 24 f0       	mov    -0x10(%rsp),%rbp
    377a:	e9 94 01 00 00       	jmpq   3913 <fastIDCT+0x423>
    377f:	90                   	nop
    3780:	85 c9                	test   %ecx,%ecx
    3782:	0f 84 38 03 00 00    	je     3ac0 <fastIDCT+0x5d0>
    3788:	44 8d 14 31          	lea    (%rcx,%rsi,1),%r10d
    378c:	69 f6 df c4 ff ff    	imul   $0xffffc4df,%esi,%esi
    3792:	45 69 d2 51 11 00 00 	imul   $0x1151,%r10d,%r10d
    3799:	69 c9 7e 18 00 00    	imul   $0x187e,%ecx,%ecx
    379f:	46 8d 1c 16          	lea    (%rsi,%r10,1),%r11d
    37a3:	41 01 ca             	add    %ecx,%r10d
    37a6:	89 d1                	mov    %edx,%ecx
    37a8:	89 c6                	mov    %eax,%esi
    37aa:	44 01 d2             	add    %r10d,%edx
    37ad:	44 29 d1             	sub    %r10d,%ecx
    37b0:	44 29 de             	sub    %r11d,%esi
    37b3:	44 01 d8             	add    %r11d,%eax
    37b6:	45 85 f6             	test   %r14d,%r14d
    37b9:	0f 84 b1 01 00 00    	je     3970 <fastIDCT+0x480>
    37bf:	85 db                	test   %ebx,%ebx
    37c1:	0f 84 59 02 00 00    	je     3a20 <fastIDCT+0x530>
    37c7:	45 85 c0             	test   %r8d,%r8d
    37ca:	0f 84 10 06 00 00    	je     3de0 <F_1_847+0x2bf>
    37d0:	45 8d 1c 18          	lea    (%r8,%rbx,1),%r11d
    37d4:	45 69 f8 54 62 00 00 	imul   $0x6254,%r8d,%r15d
    37db:	45 01 f0             	add    %r14d,%r8d
    37de:	44 69 e3 b3 41 00 00 	imul   $0x41b3,%ebx,%r12d
    37e5:	45 69 db fd ad ff ff 	imul   $0xffffadfd,%r11d,%r11d
    37ec:	45 69 ee 8e 09 00 00 	imul   $0x98e,%r14d,%r13d
    37f3:	45 69 d0 3b c1 ff ff 	imul   $0xffffc13b,%r8d,%r10d
    37fa:	45 85 c9             	test   %r9d,%r9d
    37fd:	0f 84 fd 09 00 00    	je     4200 <F_2_053+0x4d>
    3803:	44 01 cb             	add    %r9d,%ebx
    3806:	45 01 ce             	add    %r9d,%r14d
    3809:	41 01 d8             	add    %ebx,%r8d
    380c:	45 69 c0 a1 25 00 00 	imul   $0x25a1,%r8d,%r8d
    3813:	45 69 f6 33 e3 ff ff 	imul   $0xffffe333,%r14d,%r14d
    381a:	45 01 c2             	add    %r8d,%r10d
    381d:	69 db 84 f3 ff ff    	imul   $0xfffff384,%ebx,%ebx
    3823:	45 69 c9 0b 30 00 00 	imul   $0x300b,%r9d,%r9d
    382a:	41 01 d8             	add    %ebx,%r8d
    382d:	43 8d 1c 16          	lea    (%r14,%r10,1),%ebx
    3831:	45 01 c6             	add    %r8d,%r14d
    3834:	41 01 dd             	add    %ebx,%r13d
    3837:	43 8d 1c 18          	lea    (%r8,%r11,1),%ebx
    383b:	45 01 ce             	add    %r9d,%r14d
    383e:	41 01 dc             	add    %ebx,%r12d
    3841:	43 8d 1c 1a          	lea    (%r10,%r11,1),%ebx
    3845:	44 01 fb             	add    %r15d,%ebx
    3848:	41 89 d3             	mov    %edx,%r11d
    384b:	41 89 c2             	mov    %eax,%r10d
    384e:	41 89 f1             	mov    %esi,%r9d
    3851:	41 89 c8             	mov    %ecx,%r8d
    3854:	45 29 f3             	sub    %r14d,%r11d
    3857:	41 29 da             	sub    %ebx,%r10d
    385a:	45 29 e1             	sub    %r12d,%r9d
    385d:	45 29 e8             	sub    %r13d,%r8d
    3860:	44 01 e9             	add    %r13d,%ecx
    3863:	44 01 e6             	add    %r12d,%esi
    3866:	01 d8                	add    %ebx,%eax
    3868:	44 01 f2             	add    %r14d,%edx
    386b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3870:	81 c2 00 00 02 00    	add    $0x20000,%edx
    3876:	41 81 c3 00 00 02 00 	add    $0x20000,%r11d
    387d:	05 00 00 02 00       	add    $0x20000,%eax
    3882:	41 81 c2 00 00 02 00 	add    $0x20000,%r10d
    3889:	81 c6 00 00 02 00    	add    $0x20000,%esi
    388f:	41 81 c1 00 00 02 00 	add    $0x20000,%r9d
    3896:	81 c1 00 00 02 00    	add    $0x20000,%ecx
    389c:	41 81 c0 00 00 02 00 	add    $0x20000,%r8d
    38a3:	c1 fa 12             	sar    $0x12,%edx
    38a6:	41 c1 fb 12          	sar    $0x12,%r11d
    38aa:	c1 f8 12             	sar    $0x12,%eax
    38ad:	41 c1 fa 12          	sar    $0x12,%r10d
    38b1:	c1 fe 12             	sar    $0x12,%esi
    38b4:	41 c1 f9 12          	sar    $0x12,%r9d
    38b8:	c1 f9 12             	sar    $0x12,%ecx
    38bb:	41 c1 f8 12          	sar    $0x12,%r8d
    38bf:	66 83 ea 80          	sub    $0xff80,%dx
    38c3:	66 41 83 eb 80       	sub    $0xff80,%r11w
    38c8:	66 83 e8 80          	sub    $0xff80,%ax
    38cc:	66 41 83 ea 80       	sub    $0xff80,%r10w
    38d1:	66 83 ee 80          	sub    $0xff80,%si
    38d5:	66 41 83 e9 80       	sub    $0xff80,%r9w
    38da:	66 83 e9 80          	sub    $0xff80,%cx
    38de:	66 41 83 e8 80       	sub    $0xff80,%r8w
    38e3:	66 89 17             	mov    %dx,(%rdi)
    38e6:	66 44 89 5f 70       	mov    %r11w,0x70(%rdi)
    38eb:	48 83 c7 02          	add    $0x2,%rdi
    38ef:	66 89 47 0e          	mov    %ax,0xe(%rdi)
    38f3:	66 44 89 57 5e       	mov    %r10w,0x5e(%rdi)
    38f8:	66 89 77 1e          	mov    %si,0x1e(%rdi)
    38fc:	66 44 89 4f 4e       	mov    %r9w,0x4e(%rdi)
    3901:	66 89 4f 2e          	mov    %cx,0x2e(%rdi)
    3905:	66 44 89 47 3e       	mov    %r8w,0x3e(%rdi)
    390a:	48 39 ef             	cmp    %rbp,%rdi
    390d:	0f 84 4d 04 00 00    	je     3d60 <F_1_847+0x23f>
    3913:	0f bf 07             	movswl (%rdi),%eax
    3916:	44 0f bf 57 40       	movswl 0x40(%rdi),%r10d
    391b:	0f bf 77 60          	movswl 0x60(%rdi),%esi
    391f:	44 0f bf 4f 10       	movswl 0x10(%rdi),%r9d
    3924:	0f bf 4f 20          	movswl 0x20(%rdi),%ecx
    3928:	44 0f bf 47 30       	movswl 0x30(%rdi),%r8d
    392d:	0f bf 5f 50          	movswl 0x50(%rdi),%ebx
    3931:	44 0f bf 77 70       	movswl 0x70(%rdi),%r14d
    3936:	42 8d 14 10          	lea    (%rax,%r10,1),%edx
    393a:	44 29 d0             	sub    %r10d,%eax
    393d:	c1 e0 0d             	shl    $0xd,%eax
    3940:	c1 e2 0d             	shl    $0xd,%edx
    3943:	85 f6                	test   %esi,%esi
    3945:	0f 85 35 fe ff ff    	jne    3780 <fastIDCT+0x290>
    394b:	85 c9                	test   %ecx,%ecx
    394d:	0f 84 85 01 00 00    	je     3ad8 <fastIDCT+0x5e8>
    3953:	44 69 d9 51 11 00 00 	imul   $0x1151,%ecx,%r11d
    395a:	44 69 d1 cf 29 00 00 	imul   $0x29cf,%ecx,%r10d
    3961:	e9 40 fe ff ff       	jmpq   37a6 <fastIDCT+0x2b6>
    3966:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    396d:	00 00 00 
    3970:	85 db                	test   %ebx,%ebx
    3972:	0f 84 70 01 00 00    	je     3ae8 <fastIDCT+0x5f8>
    3978:	45 85 c0             	test   %r8d,%r8d
    397b:	0f 84 1f 03 00 00    	je     3ca0 <F_1_847+0x17f>
    3981:	45 69 d8 3b c1 ff ff 	imul   $0xffffc13b,%r8d,%r11d
    3988:	45 85 c9             	test   %r9d,%r9d
    398b:	45 8d 14 18          	lea    (%r8,%rbx,1),%r10d
    398f:	0f 84 0b 08 00 00    	je     41a0 <F_1_961+0x2db>
    3995:	45 8d 24 19          	lea    (%r9,%rbx,1),%r12d
    3999:	45 69 f9 0b 30 00 00 	imul   $0x300b,%r9d,%r15d
    39a0:	47 8d 2c 20          	lea    (%r8,%r12,1),%r13d
    39a4:	45 69 e4 84 f3 ff ff 	imul   $0xfffff384,%r12d,%r12d
    39ab:	44 89 7c 24 e4       	mov    %r15d,-0x1c(%rsp)
    39b0:	45 69 ed a1 25 00 00 	imul   $0x25a1,%r13d,%r13d
    39b7:	45 69 c9 33 e3 ff ff 	imul   $0xffffe333,%r9d,%r9d
    39be:	45 01 eb             	add    %r13d,%r11d
    39c1:	45 01 e5             	add    %r12d,%r13d
    39c4:	45 69 d2 fd ad ff ff 	imul   $0xffffadfd,%r10d,%r10d
    39cb:	47 8d 34 19          	lea    (%r9,%r11,1),%r14d
    39cf:	69 db b3 41 00 00    	imul   $0x41b3,%ebx,%ebx
    39d5:	47 8d 3c 2a          	lea    (%r10,%r13,1),%r15d
    39d9:	45 01 cd             	add    %r9d,%r13d
    39dc:	45 69 c0 54 62 00 00 	imul   $0x6254,%r8d,%r8d
    39e3:	44 03 6c 24 e4       	add    -0x1c(%rsp),%r13d
    39e8:	45 01 da             	add    %r11d,%r10d
    39eb:	41 89 f1             	mov    %esi,%r9d
    39ee:	41 89 d3             	mov    %edx,%r11d
    39f1:	41 01 df             	add    %ebx,%r15d
    39f4:	43 8d 1c 02          	lea    (%r10,%r8,1),%ebx
    39f8:	41 89 c2             	mov    %eax,%r10d
    39fb:	41 89 c8             	mov    %ecx,%r8d
    39fe:	45 29 eb             	sub    %r13d,%r11d
    3a01:	45 29 f9             	sub    %r15d,%r9d
    3a04:	45 29 f0             	sub    %r14d,%r8d
    3a07:	41 29 da             	sub    %ebx,%r10d
    3a0a:	44 01 f1             	add    %r14d,%ecx
    3a0d:	44 01 fe             	add    %r15d,%esi
    3a10:	01 d8                	add    %ebx,%eax
    3a12:	44 01 ea             	add    %r13d,%edx
    3a15:	e9 56 fe ff ff       	jmpq   3870 <fastIDCT+0x380>
    3a1a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    3a20:	45 85 c0             	test   %r8d,%r8d
    3a23:	0f 84 47 03 00 00    	je     3d70 <F_1_847+0x24f>
    3a29:	45 69 e8 fd ad ff ff 	imul   $0xffffadfd,%r8d,%r13d
    3a30:	45 85 c9             	test   %r9d,%r9d
    3a33:	47 8d 14 30          	lea    (%r8,%r14,1),%r10d
    3a37:	0f 84 53 06 00 00    	je     4090 <F_1_961+0x1cb>
    3a3d:	47 8d 24 11          	lea    (%r9,%r10,1),%r12d
    3a41:	47 8d 1c 31          	lea    (%r9,%r14,1),%r11d
    3a45:	45 69 d2 3b c1 ff ff 	imul   $0xffffc13b,%r10d,%r10d
    3a4c:	45 69 e4 a1 25 00 00 	imul   $0x25a1,%r12d,%r12d
    3a53:	45 69 db 33 e3 ff ff 	imul   $0xffffe333,%r11d,%r11d
    3a5a:	45 01 e2             	add    %r12d,%r10d
    3a5d:	45 69 f9 0b 30 00 00 	imul   $0x300b,%r9d,%r15d
    3a64:	43 8d 1c 13          	lea    (%r11,%r10,1),%ebx
    3a68:	45 01 ea             	add    %r13d,%r10d
    3a6b:	45 69 c9 84 f3 ff ff 	imul   $0xfffff384,%r9d,%r9d
    3a72:	45 69 c0 54 62 00 00 	imul   $0x6254,%r8d,%r8d
    3a79:	45 01 e1             	add    %r12d,%r9d
    3a7c:	45 69 f6 8e 09 00 00 	imul   $0x98e,%r14d,%r14d
    3a83:	47 8d 24 29          	lea    (%r9,%r13,1),%r12d
    3a87:	45 01 d9             	add    %r11d,%r9d
    3a8a:	45 01 cf             	add    %r9d,%r15d
    3a8d:	41 89 d3             	mov    %edx,%r11d
    3a90:	41 89 f1             	mov    %esi,%r9d
    3a93:	45 29 fb             	sub    %r15d,%r11d
    3a96:	45 29 e1             	sub    %r12d,%r9d
    3a99:	44 01 e6             	add    %r12d,%esi
    3a9c:	41 01 de             	add    %ebx,%r14d
    3a9f:	43 8d 1c 02          	lea    (%r10,%r8,1),%ebx
    3aa3:	41 89 c2             	mov    %eax,%r10d
    3aa6:	41 89 c8             	mov    %ecx,%r8d
    3aa9:	44 01 fa             	add    %r15d,%edx
    3aac:	44 01 f1             	add    %r14d,%ecx
    3aaf:	41 29 da             	sub    %ebx,%r10d
    3ab2:	45 29 f0             	sub    %r14d,%r8d
    3ab5:	01 d8                	add    %ebx,%eax
    3ab7:	e9 b4 fd ff ff       	jmpq   3870 <fastIDCT+0x380>
    3abc:	0f 1f 40 00          	nopl   0x0(%rax)
    3ac0:	44 69 de 31 d6 ff ff 	imul   $0xffffd631,%esi,%r11d
    3ac7:	44 69 d6 51 11 00 00 	imul   $0x1151,%esi,%r10d
    3ace:	e9 d3 fc ff ff       	jmpq   37a6 <fastIDCT+0x2b6>
    3ad3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3ad8:	89 d1                	mov    %edx,%ecx
    3ada:	89 c6                	mov    %eax,%esi
    3adc:	e9 d5 fc ff ff       	jmpq   37b6 <fastIDCT+0x2c6>
    3ae1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    3ae8:	45 85 c0             	test   %r8d,%r8d
    3aeb:	0f 84 1f 02 00 00    	je     3d10 <F_1_847+0x1ef>
    3af1:	45 85 c9             	test   %r9d,%r9d
    3af4:	0f 84 5e 06 00 00    	je     4158 <F_1_961+0x293>
    3afa:	47 8d 14 01          	lea    (%r9,%r8,1),%r10d
    3afe:	45 69 e0 8b d1 ff ff 	imul   $0xffffd18b,%r8d,%r12d
    3b05:	45 69 da 24 19 00 00 	imul   $0x1924,%r10d,%r11d
    3b0c:	45 69 c0 79 ba ff ff 	imul   $0xffffba79,%r8d,%r8d
    3b13:	45 69 d2 a1 25 00 00 	imul   $0x25a1,%r10d,%r10d
    3b1a:	47 8d 34 18          	lea    (%r8,%r11,1),%r14d
    3b1e:	41 89 c8             	mov    %ecx,%r8d
    3b21:	41 69 d9 c2 06 00 00 	imul   $0x6c2,%r9d,%ebx
    3b28:	45 01 d4             	add    %r10d,%r12d
    3b2b:	45 69 e9 f9 21 00 00 	imul   $0x21f9,%r9d,%r13d
    3b32:	41 89 f1             	mov    %esi,%r9d
    3b35:	44 01 f6             	add    %r14d,%esi
    3b38:	45 29 f1             	sub    %r14d,%r9d
    3b3b:	44 01 d3             	add    %r10d,%ebx
    3b3e:	41 89 c2             	mov    %eax,%r10d
    3b41:	44 01 e0             	add    %r12d,%eax
    3b44:	45 29 dd             	sub    %r11d,%r13d
    3b47:	41 89 d3             	mov    %edx,%r11d
    3b4a:	45 29 e2             	sub    %r12d,%r10d
    3b4d:	41 29 db             	sub    %ebx,%r11d
    3b50:	45 29 e8             	sub    %r13d,%r8d
    3b53:	44 01 e9             	add    %r13d,%ecx
    3b56:	01 da                	add    %ebx,%edx
    3b58:	e9 13 fd ff ff       	jmpq   3870 <fastIDCT+0x380>
    3b5d:	0f 1f 00             	nopl   (%rax)
    3b60:	85 f6                	test   %esi,%esi
    3b62:	0f 84 b0 03 00 00    	je     3f18 <F_1_961+0x53>
    3b68:	44 69 ee fd ad ff ff 	imul   $0xffffadfd,%esi,%r13d
    3b6f:	85 c9                	test   %ecx,%ecx
    3b71:	8d 14 2e             	lea    (%rsi,%rbp,1),%edx
    3b74:	0f 84 86 08 00 00    	je     4400 <F_2_053+0x24d>
    3b7a:	44 8d 34 11          	lea    (%rcx,%rdx,1),%r14d
    3b7e:	44 8d 24 29          	lea    (%rcx,%rbp,1),%r12d
    3b82:	69 d2 3b c1 ff ff    	imul   $0xffffc13b,%edx,%edx
    3b88:	45 69 f6 a1 25 00 00 	imul   $0x25a1,%r14d,%r14d
    3b8f:	45 69 e4 33 e3 ff ff 	imul   $0xffffe333,%r12d,%r12d
    3b96:	44 01 f2             	add    %r14d,%edx
    3b99:	44 69 f9 0b 30 00 00 	imul   $0x300b,%ecx,%r15d
    3ba0:	69 c9 84 f3 ff ff    	imul   $0xfffff384,%ecx,%ecx
    3ba6:	69 f6 54 62 00 00    	imul   $0x6254,%esi,%esi
    3bac:	44 01 f1             	add    %r14d,%ecx
    3baf:	45 8d 34 14          	lea    (%r12,%rdx,1),%r14d
    3bb3:	44 01 ea             	add    %r13d,%edx
    3bb6:	69 ed 8e 09 00 00    	imul   $0x98e,%ebp,%ebp
    3bbc:	01 d6                	add    %edx,%esi
    3bbe:	89 c2                	mov    %eax,%edx
    3bc0:	29 f2                	sub    %esi,%edx
    3bc2:	01 f0                	add    %esi,%eax
    3bc4:	89 54 24 e8          	mov    %edx,-0x18(%rsp)
    3bc8:	44 89 ca             	mov    %r9d,%edx
    3bcb:	44 01 f5             	add    %r14d,%ebp
    3bce:	46 8d 34 29          	lea    (%rcx,%r13,1),%r14d
    3bd2:	44 01 e1             	add    %r12d,%ecx
    3bd5:	41 01 cf             	add    %ecx,%r15d
    3bd8:	45 89 c5             	mov    %r8d,%r13d
    3bdb:	45 89 d4             	mov    %r10d,%r12d
    3bde:	29 ea                	sub    %ebp,%edx
    3be0:	45 29 fd             	sub    %r15d,%r13d
    3be3:	45 29 f4             	sub    %r14d,%r12d
    3be6:	89 54 24 ec          	mov    %edx,-0x14(%rsp)
    3bea:	41 01 e9             	add    %ebp,%r9d
    3bed:	45 01 f2             	add    %r14d,%r10d
    3bf0:	45 01 f8             	add    %r15d,%r8d
    3bf3:	e9 28 04 00 00       	jmpq   4020 <F_1_961+0x15b>
    3bf8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    3bff:	00 
    3c00:	45 69 e9 31 d6 ff ff 	imul   $0xffffd631,%r9d,%r13d
    3c07:	45 69 e1 51 11 00 00 	imul   $0x1151,%r9d,%r12d
    3c0e:	e9 8f f9 ff ff       	jmpq   35a2 <fastIDCT+0xb2>
    3c13:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3c18:	45 89 c1             	mov    %r8d,%r9d
    3c1b:	41 89 c2             	mov    %eax,%r10d
    3c1e:	e9 91 f9 ff ff       	jmpq   35b4 <fastIDCT+0xc4>
    3c23:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3c28:	85 f6                	test   %esi,%esi
    3c2a:	0f 84 d0 03 00 00    	je     4000 <F_1_961+0x13b>
    3c30:	85 c9                	test   %ecx,%ecx
    3c32:	0f 84 78 07 00 00    	je     43b0 <F_2_053+0x1fd>
    3c38:	8d 14 31             	lea    (%rcx,%rsi,1),%edx
    3c3b:	41 89 c7             	mov    %eax,%r15d
    3c3e:	45 89 c5             	mov    %r8d,%r13d
    3c41:	44 69 f6 8b d1 ff ff 	imul   $0xffffd18b,%esi,%r14d
    3c48:	44 69 e2 24 19 00 00 	imul   $0x1924,%edx,%r12d
    3c4f:	69 d2 a1 25 00 00    	imul   $0x25a1,%edx,%edx
    3c55:	69 e9 c2 06 00 00    	imul   $0x6c2,%ecx,%ebp
    3c5b:	41 01 d6             	add    %edx,%r14d
    3c5e:	69 c9 f9 21 00 00    	imul   $0x21f9,%ecx,%ecx
    3c64:	45 29 f7             	sub    %r14d,%r15d
    3c67:	01 ea                	add    %ebp,%edx
    3c69:	69 f6 79 ba ff ff    	imul   $0xffffba79,%esi,%esi
    3c6f:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    3c74:	45 89 cf             	mov    %r9d,%r15d
    3c77:	41 29 d5             	sub    %edx,%r13d
    3c7a:	44 01 f0             	add    %r14d,%eax
    3c7d:	41 01 d0             	add    %edx,%r8d
    3c80:	44 29 e1             	sub    %r12d,%ecx
    3c83:	44 01 e6             	add    %r12d,%esi
    3c86:	41 29 cf             	sub    %ecx,%r15d
    3c89:	45 89 d4             	mov    %r10d,%r12d
    3c8c:	41 29 f4             	sub    %esi,%r12d
    3c8f:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    3c94:	41 01 c9             	add    %ecx,%r9d
    3c97:	41 01 f2             	add    %esi,%r10d
    3c9a:	e9 81 03 00 00       	jmpq   4020 <F_1_961+0x15b>
    3c9f:	90                   	nop
    3ca0:	45 85 c9             	test   %r9d,%r9d
    3ca3:	0f 84 d7 04 00 00    	je     4180 <F_1_961+0x2bb>
    3ca9:	45 8d 04 19          	lea    (%r9,%rbx,1),%r8d
    3cad:	41 89 d3             	mov    %edx,%r11d
    3cb0:	45 69 e1 3e 13 00 00 	imul   $0x133e,%r9d,%r12d
    3cb7:	45 69 d0 a1 25 00 00 	imul   $0x25a1,%r8d,%r10d
    3cbe:	45 69 c9 33 e3 ff ff 	imul   $0xffffe333,%r9d,%r9d
    3cc5:	45 69 c0 24 19 00 00 	imul   $0x1924,%r8d,%r8d
    3ccc:	47 8d 34 11          	lea    (%r9,%r10,1),%r14d
    3cd0:	41 89 f1             	mov    %esi,%r9d
    3cd3:	44 69 eb b0 ef ff ff 	imul   $0xffffefb0,%ebx,%r13d
    3cda:	45 01 c4             	add    %r8d,%r12d
    3cdd:	69 db fd ad ff ff    	imul   $0xffffadfd,%ebx,%ebx
    3ce3:	45 29 e3             	sub    %r12d,%r11d
    3ce6:	44 01 e2             	add    %r12d,%edx
    3ce9:	45 01 c5             	add    %r8d,%r13d
    3cec:	41 89 c8             	mov    %ecx,%r8d
    3cef:	44 01 f1             	add    %r14d,%ecx
    3cf2:	44 01 d3             	add    %r10d,%ebx
    3cf5:	41 89 c2             	mov    %eax,%r10d
    3cf8:	45 29 e9             	sub    %r13d,%r9d
    3cfb:	41 29 da             	sub    %ebx,%r10d
    3cfe:	45 29 f0             	sub    %r14d,%r8d
    3d01:	44 01 ee             	add    %r13d,%esi
    3d04:	01 d8                	add    %ebx,%eax
    3d06:	e9 65 fb ff ff       	jmpq   3870 <fastIDCT+0x380>
    3d0b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3d10:	45 85 c9             	test   %r9d,%r9d
    3d13:	0f 84 27 05 00 00    	je     4240 <F_2_053+0x8d>
    3d19:	45 69 f1 d4 08 00 00 	imul   $0x8d4,%r9d,%r14d
    3d20:	45 69 e9 24 19 00 00 	imul   $0x1924,%r9d,%r13d
    3d27:	45 69 e1 a1 25 00 00 	imul   $0x25a1,%r9d,%r12d
    3d2e:	41 69 d9 63 2c 00 00 	imul   $0x2c63,%r9d,%ebx
    3d35:	41 89 d3             	mov    %edx,%r11d
    3d38:	41 89 c2             	mov    %eax,%r10d
    3d3b:	41 89 f1             	mov    %esi,%r9d
    3d3e:	41 89 c8             	mov    %ecx,%r8d
    3d41:	41 29 db             	sub    %ebx,%r11d
    3d44:	45 29 e2             	sub    %r12d,%r10d
    3d47:	45 29 e9             	sub    %r13d,%r9d
    3d4a:	45 29 f0             	sub    %r14d,%r8d
    3d4d:	44 01 f1             	add    %r14d,%ecx
    3d50:	44 01 ee             	add    %r13d,%esi
    3d53:	44 01 e0             	add    %r12d,%eax
    3d56:	01 da                	add    %ebx,%edx
    3d58:	e9 13 fb ff ff       	jmpq   3870 <fastIDCT+0x380>
    3d5d:	0f 1f 00             	nopl   (%rax)
    3d60:	5b                   	pop    %rbx
    3d61:	5d                   	pop    %rbp
    3d62:	41 5c                	pop    %r12
    3d64:	41 5d                	pop    %r13
    3d66:	41 5e                	pop    %r14
    3d68:	41 5f                	pop    %r15
    3d6a:	c3                   	retq   
    3d6b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3d70:	45 85 c9             	test   %r9d,%r9d
    3d73:	0f 84 5f 04 00 00    	je     41d8 <F_2_053+0x25>
    3d79:	43 8d 1c 31          	lea    (%r9,%r14,1),%ebx
    3d7d:	41 89 d3             	mov    %edx,%r11d
    3d80:	45 69 d1 8e 23 00 00 	imul   $0x238e,%r9d,%r10d
    3d87:	44 69 c3 a1 25 00 00 	imul   $0x25a1,%ebx,%r8d
    3d8e:	45 69 c9 84 f3 ff ff 	imul   $0xfffff384,%r9d,%r9d
    3d95:	69 db d4 08 00 00    	imul   $0x8d4,%ebx,%ebx
    3d9b:	47 8d 2c 01          	lea    (%r9,%r8,1),%r13d
    3d9f:	41 89 f1             	mov    %esi,%r9d
    3da2:	45 69 e6 c9 ca ff ff 	imul   $0xffffcac9,%r14d,%r12d
    3da9:	45 69 f6 3b c1 ff ff 	imul   $0xffffc13b,%r14d,%r14d
    3db0:	45 29 e9             	sub    %r13d,%r9d
    3db3:	44 01 ee             	add    %r13d,%esi
    3db6:	41 01 dc             	add    %ebx,%r12d
    3db9:	44 01 d3             	add    %r10d,%ebx
    3dbc:	41 89 c2             	mov    %eax,%r10d
    3dbf:	45 01 c6             	add    %r8d,%r14d
    3dc2:	41 89 c8             	mov    %ecx,%r8d
    3dc5:	41 29 db             	sub    %ebx,%r11d
    3dc8:	45 29 f2             	sub    %r14d,%r10d
    3dcb:	45 29 e0             	sub    %r12d,%r8d
    3dce:	44 01 e1             	add    %r12d,%ecx
    3dd1:	44 01 f0             	add    %r14d,%eax
    3dd4:	01 da                	add    %ebx,%edx
    3dd6:	e9 95 fa ff ff       	jmpq   3870 <fastIDCT+0x380>
    3ddb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    3de0:	44 69 e3 fd ad ff ff 	imul   $0xffffadfd,%ebx,%r12d
    3de7:	45 69 c6 3b c1 ff ff 	imul   $0xffffc13b,%r14d,%r8d
    3dee:	45 85 c9             	test   %r9d,%r9d
    3df1:	0f 84 f9 02 00 00    	je     40f0 <F_1_961+0x22b>
    3df7:	45 8d 1c 19          	lea    (%r9,%rbx,1),%r11d
    3dfb:	47 8d 14 31          	lea    (%r9,%r14,1),%r10d
    3dff:	45 69 f9 0b 30 00 00 	imul   $0x300b,%r9d,%r15d
    3e06:	47 8d 2c 1e          	lea    (%r14,%r11,1),%r13d
    3e0a:	45 69 ed a1 25 00 00 	imul   $0x25a1,%r13d,%r13d
    3e11:	45 69 ca 33 e3 ff ff 	imul   $0xffffe333,%r10d,%r9d
    3e18:	45 01 e8             	add    %r13d,%r8d
    3e1b:	45 69 db 84 f3 ff ff 	imul   $0xfffff384,%r11d,%r11d
    3e22:	47 8d 14 01          	lea    (%r9,%r8,1),%r10d
    3e26:	45 69 f6 8e 09 00 00 	imul   $0x98e,%r14d,%r14d
    3e2d:	45 01 dd             	add    %r11d,%r13d
    3e30:	41 89 d3             	mov    %edx,%r11d
    3e33:	69 db b3 41 00 00    	imul   $0x41b3,%ebx,%ebx
    3e39:	45 01 d6             	add    %r10d,%r14d
    3e3c:	47 8d 54 25 00       	lea    0x0(%r13,%r12,1),%r10d
    3e41:	45 01 cd             	add    %r9d,%r13d
    3e44:	45 01 c4             	add    %r8d,%r12d
    3e47:	41 89 f1             	mov    %esi,%r9d
    3e4a:	45 01 ef             	add    %r13d,%r15d
    3e4d:	41 89 c8             	mov    %ecx,%r8d
    3e50:	44 01 f1             	add    %r14d,%ecx
    3e53:	44 01 d3             	add    %r10d,%ebx
    3e56:	41 89 c2             	mov    %eax,%r10d
    3e59:	45 29 fb             	sub    %r15d,%r11d
    3e5c:	45 29 e2             	sub    %r12d,%r10d
    3e5f:	41 29 d9             	sub    %ebx,%r9d
    3e62:	45 29 f0             	sub    %r14d,%r8d
    3e65:	01 de                	add    %ebx,%esi
    3e67:	44 01 e0             	add    %r12d,%eax
    3e6a:	44 01 fa             	add    %r15d,%edx
    3e6d:	e9 fe f9 ff ff       	jmpq   3870 <fastIDCT+0x380>
    3e72:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    3e78:	44 69 f2 fd ad ff ff 	imul   $0xffffadfd,%edx,%r14d
    3e7f:	69 f5 3b c1 ff ff    	imul   $0xffffc13b,%ebp,%esi
    3e85:	85 c9                	test   %ecx,%ecx
    3e87:	0f 84 bb 04 00 00    	je     4348 <F_2_053+0x195>
    3e8d:	44 8d 2c 11          	lea    (%rcx,%rdx,1),%r13d
    3e91:	44 8d 24 29          	lea    (%rcx,%rbp,1),%r12d
    3e95:	69 d2 b3 41 00 00    	imul   $0x41b3,%edx,%edx
    3e9b:	46 8d 7c 2d 00       	lea    0x0(%rbp,%r13,1),%r15d
    3ea0:	45 69 ff a1 25 00 00 	imul   $0x25a1,%r15d,%r15d
    3ea7:	45 69 e4 33 e3 ff ff 	imul   $0xffffe333,%r12d,%r12d
    3eae:	44 01 fe             	add    %r15d,%esi
    3eb1:	45 69 ed 84 f3 ff ff 	imul   $0xfffff384,%r13d,%r13d
    3eb8:	69 ed 8e 09 00 00    	imul   $0x98e,%ebp,%ebp
    3ebe:	45 01 fd             	add    %r15d,%r13d
    3ec1:	45 8d 3c 34          	lea    (%r12,%rsi,1),%r15d
    3ec5:	44 01 f6             	add    %r14d,%esi
    3ec8:	69 c9 0b 30 00 00    	imul   $0x300b,%ecx,%ecx
    3ece:	45 01 ec             	add    %r13d,%r12d
    3ed1:	44 01 fd             	add    %r15d,%ebp
    3ed4:	47 8d 7c 35 00       	lea    0x0(%r13,%r14,1),%r15d
    3ed9:	45 89 c5             	mov    %r8d,%r13d
    3edc:	44 01 fa             	add    %r15d,%edx
    3edf:	41 89 c7             	mov    %eax,%r15d
    3ee2:	44 01 e1             	add    %r12d,%ecx
    3ee5:	41 29 f7             	sub    %esi,%r15d
    3ee8:	45 89 d4             	mov    %r10d,%r12d
    3eeb:	41 29 cd             	sub    %ecx,%r13d
    3eee:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    3ef3:	45 89 cf             	mov    %r9d,%r15d
    3ef6:	41 29 d4             	sub    %edx,%r12d
    3ef9:	41 29 ef             	sub    %ebp,%r15d
    3efc:	41 01 e9             	add    %ebp,%r9d
    3eff:	41 01 d2             	add    %edx,%r10d
    3f02:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    3f07:	01 f0                	add    %esi,%eax
    3f09:	41 01 c8             	add    %ecx,%r8d
    3f0c:	e9 0f 01 00 00       	jmpq   4020 <F_1_961+0x15b>
    3f11:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    3f18:	85 c9                	test   %ecx,%ecx
    3f1a:	0f 84 d8 03 00 00    	je     42f8 <F_2_053+0x145>
    3f20:	8d 14 29             	lea    (%rcx,%rbp,1),%edx
    3f23:	41 89 c7             	mov    %eax,%r15d
    3f26:	69 f5 c9 ca ff ff    	imul   $0xffffcac9,%ebp,%esi
    3f2c:	44 69 ea a1 25 00 00 	imul   $0x25a1,%edx,%r13d
    3f33:	69 ed 3b c1 ff ff    	imul   $0xffffc13b,%ebp,%ebp
    3f39:	69 d2 d4 08 00 00    	imul   $0x8d4,%edx,%edx
    3f3f:	44 01 ed             	add    %r13d,%ebp
    3f42:	44 69 e1 8e 23 00 00 	imul   $0x238e,%ecx,%r12d
    3f49:	41 29 ef             	sub    %ebp,%r15d
    3f4c:	01 d6                	add    %edx,%esi
    3f4e:	69 c9 84 f3 ff ff    	imul   $0xfffff384,%ecx,%ecx
    3f54:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    3f59:	45 89 cf             	mov    %r9d,%r15d
    3f5c:	41 29 f7             	sub    %esi,%r15d
    3f5f:	41 01 f1             	add    %esi,%r9d
    3f62:	01 e8                	add    %ebp,%eax
    3f64:	44 01 e2             	add    %r12d,%edx
    3f67:	45 89 d4             	mov    %r10d,%r12d
    3f6a:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    3f6f:	44 01 e9             	add    %r13d,%ecx
    3f72:	45 89 c5             	mov    %r8d,%r13d
    3f75:	41 01 d0             	add    %edx,%r8d
    3f78:	41 29 d5             	sub    %edx,%r13d
    3f7b:	41 29 cc             	sub    %ecx,%r12d
    3f7e:	41 01 ca             	add    %ecx,%r10d
    3f81:	e9 9a 00 00 00       	jmpq   4020 <F_1_961+0x15b>
    3f86:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    3f8d:	00 00 00 
    3f90:	85 c9                	test   %ecx,%ecx
    3f92:	0f 84 10 03 00 00    	je     42a8 <F_2_053+0xf5>
    3f98:	8d 34 11             	lea    (%rcx,%rdx,1),%esi
    3f9b:	41 89 c7             	mov    %eax,%r15d
    3f9e:	45 89 c5             	mov    %r8d,%r13d
    3fa1:	44 69 f2 b0 ef ff ff 	imul   $0xffffefb0,%edx,%r14d
    3fa8:	44 69 e6 a1 25 00 00 	imul   $0x25a1,%esi,%r12d
    3faf:	69 d2 fd ad ff ff    	imul   $0xffffadfd,%edx,%edx
    3fb5:	69 e9 3e 13 00 00    	imul   $0x133e,%ecx,%ebp
    3fbb:	44 01 e2             	add    %r12d,%edx
    3fbe:	69 c9 33 e3 ff ff    	imul   $0xffffe333,%ecx,%ecx
    3fc4:	41 29 d7             	sub    %edx,%r15d
    3fc7:	01 d0                	add    %edx,%eax
    3fc9:	69 f6 24 19 00 00    	imul   $0x1924,%esi,%esi
    3fcf:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    3fd4:	45 89 cf             	mov    %r9d,%r15d
    3fd7:	44 01 e1             	add    %r12d,%ecx
    3fda:	45 89 d4             	mov    %r10d,%r12d
    3fdd:	41 01 f6             	add    %esi,%r14d
    3fe0:	41 29 cf             	sub    %ecx,%r15d
    3fe3:	01 ee                	add    %ebp,%esi
    3fe5:	41 29 f5             	sub    %esi,%r13d
    3fe8:	45 29 f4             	sub    %r14d,%r12d
    3feb:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    3ff0:	41 01 c9             	add    %ecx,%r9d
    3ff3:	45 01 f2             	add    %r14d,%r10d
    3ff6:	41 01 f0             	add    %esi,%r8d
    3ff9:	eb 25                	jmp    4020 <F_1_961+0x15b>
    3ffb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    4000:	85 c9                	test   %ecx,%ecx
    4002:	0f 85 50 02 00 00    	jne    4258 <F_2_053+0xa5>
    4008:	44 89 4c 24 ec       	mov    %r9d,-0x14(%rsp)
    400d:	45 89 d4             	mov    %r10d,%r12d
    4010:	89 44 24 e8          	mov    %eax,-0x18(%rsp)
    4014:	45 89 c5             	mov    %r8d,%r13d
    4017:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
    401e:	00 00 
    4020:	44 89 54 24 e4       	mov    %r10d,-0x1c(%rsp)
    4025:	c5 f9 6e 7c 24 e8    	vmovd  -0x18(%rsp),%xmm7
    402b:	c5 f9 6e 6c 24 e4    	vmovd  -0x1c(%rsp),%xmm5
    4031:	44 89 44 24 e4       	mov    %r8d,-0x1c(%rsp)
    4036:	c5 f9 6e 74 24 e4    	vmovd  -0x1c(%rsp),%xmm6
    403c:	c4 c3 51 22 c1 01    	vpinsrd $0x1,%r9d,%xmm5,%xmm0
    4042:	c5 f9 6e 54 24 ec    	vmovd  -0x14(%rsp),%xmm2
    4048:	c4 e3 49 22 c8 01    	vpinsrd $0x1,%eax,%xmm6,%xmm1
    404e:	c4 c3 69 22 d4 01    	vpinsrd $0x1,%r12d,%xmm2,%xmm2
    4054:	c5 f1 6c c8          	vpunpcklqdq %xmm0,%xmm1,%xmm1
    4058:	c4 c3 41 22 c5 01    	vpinsrd $0x1,%r13d,%xmm7,%xmm0
    405e:	c5 f1 fe cc          	vpaddd %xmm4,%xmm1,%xmm1
    4062:	c5 f1 72 e1 0b       	vpsrad $0xb,%xmm1,%xmm1
    4067:	c5 e1 db c9          	vpand  %xmm1,%xmm3,%xmm1
    406b:	c5 e9 6c c0          	vpunpcklqdq %xmm0,%xmm2,%xmm0
    406f:	c5 f9 fe c4          	vpaddd %xmm4,%xmm0,%xmm0
    4073:	c5 f9 72 e0 0b       	vpsrad $0xb,%xmm0,%xmm0
    4078:	c5 e1 db c0          	vpand  %xmm0,%xmm3,%xmm0
    407c:	c4 e2 71 2b c0       	vpackusdw %xmm0,%xmm1,%xmm0
    4081:	c5 f8 11 43 f0       	vmovups %xmm0,-0x10(%rbx)
    4086:	e9 dd f6 ff ff       	jmpq   3768 <fastIDCT+0x278>
    408b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    4090:	45 69 ca a1 25 00 00 	imul   $0x25a1,%r10d,%r9d
    4097:	41 89 d3             	mov    %edx,%r11d
    409a:	45 69 d2 dc e6 ff ff 	imul   $0xffffe6dc,%r10d,%r10d
    40a1:	45 01 cd             	add    %r9d,%r13d
    40a4:	45 69 c0 50 10 00 00 	imul   $0x1050,%r8d,%r8d
    40ab:	45 69 e6 c2 ec ff ff 	imul   $0xffffecc2,%r14d,%r12d
    40b2:	43 8d 1c 10          	lea    (%r8,%r10,1),%ebx
    40b6:	41 89 c8             	mov    %ecx,%r8d
    40b9:	45 69 f6 33 e3 ff ff 	imul   $0xffffe333,%r14d,%r14d
    40c0:	45 01 d4             	add    %r10d,%r12d
    40c3:	41 89 c2             	mov    %eax,%r10d
    40c6:	41 29 da             	sub    %ebx,%r10d
    40c9:	45 29 e0             	sub    %r12d,%r8d
    40cc:	44 01 e1             	add    %r12d,%ecx
    40cf:	01 d8                	add    %ebx,%eax
    40d1:	45 01 ce             	add    %r9d,%r14d
    40d4:	41 89 f1             	mov    %esi,%r9d
    40d7:	44 01 ee             	add    %r13d,%esi
    40da:	45 29 f3             	sub    %r14d,%r11d
    40dd:	45 29 e9             	sub    %r13d,%r9d
    40e0:	44 01 f2             	add    %r14d,%edx
    40e3:	e9 88 f7 ff ff       	jmpq   3870 <fastIDCT+0x380>
    40e8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    40ef:	00 
    40f0:	44 69 d3 b0 ef ff ff 	imul   $0xffffefb0,%ebx,%r10d
    40f7:	44 69 db 84 f3 ff ff 	imul   $0xfffff384,%ebx,%r11d
    40fe:	44 01 f3             	add    %r14d,%ebx
    4101:	44 69 cb a1 25 00 00 	imul   $0x25a1,%ebx,%r9d
    4108:	45 69 ee c2 ec ff ff 	imul   $0xffffecc2,%r14d,%r13d
    410f:	45 01 c8             	add    %r9d,%r8d
    4112:	45 01 d9             	add    %r11d,%r9d
    4115:	41 89 d3             	mov    %edx,%r11d
    4118:	45 69 f6 33 e3 ff ff 	imul   $0xffffe333,%r14d,%r14d
    411f:	43 8d 1c 0a          	lea    (%r10,%r9,1),%ebx
    4123:	45 01 c5             	add    %r8d,%r13d
    4126:	45 01 c4             	add    %r8d,%r12d
    4129:	41 89 c2             	mov    %eax,%r10d
    412c:	41 89 c8             	mov    %ecx,%r8d
    412f:	45 29 e2             	sub    %r12d,%r10d
    4132:	45 29 e8             	sub    %r13d,%r8d
    4135:	44 01 e9             	add    %r13d,%ecx
    4138:	45 01 ce             	add    %r9d,%r14d
    413b:	41 89 f1             	mov    %esi,%r9d
    413e:	44 01 e0             	add    %r12d,%eax
    4141:	45 29 f3             	sub    %r14d,%r11d
    4144:	41 29 d9             	sub    %ebx,%r9d
    4147:	01 de                	add    %ebx,%esi
    4149:	44 01 f2             	add    %r14d,%edx
    414c:	e9 1f f7 ff ff       	jmpq   3870 <fastIDCT+0x380>
    4151:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    4158:	45 69 f0 dc e6 ff ff 	imul   $0xffffe6dc,%r8d,%r14d
    415f:	45 69 e8 9d d3 ff ff 	imul   $0xffffd39d,%r8d,%r13d
    4166:	45 69 e0 2c f7 ff ff 	imul   $0xfffff72c,%r8d,%r12d
    416d:	41 69 d8 a1 25 00 00 	imul   $0x25a1,%r8d,%ebx
    4174:	e9 bc fb ff ff       	jmpq   3d35 <F_1_847+0x214>
    4179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    4180:	44 69 f3 a1 25 00 00 	imul   $0x25a1,%ebx,%r14d
    4187:	44 69 eb d4 08 00 00 	imul   $0x8d4,%ebx,%r13d
    418e:	44 69 e3 9d d3 ff ff 	imul   $0xffffd39d,%ebx,%r12d
    4195:	69 db 24 19 00 00    	imul   $0x1924,%ebx,%ebx
    419b:	e9 95 fb ff ff       	jmpq   3d35 <F_1_847+0x214>
    41a0:	45 69 ca a1 25 00 00 	imul   $0x25a1,%r10d,%r9d
    41a7:	44 69 eb 37 35 00 00 	imul   $0x3537,%ebx,%r13d
    41ae:	47 8d 34 19          	lea    (%r9,%r11,1),%r14d
    41b2:	45 69 d2 9d d3 ff ff 	imul   $0xffffd39d,%r10d,%r10d
    41b9:	45 69 c0 8e 23 00 00 	imul   $0x238e,%r8d,%r8d
    41c0:	45 01 d5             	add    %r10d,%r13d
    41c3:	69 db 84 f3 ff ff    	imul   $0xfffff384,%ebx,%ebx
    41c9:	47 8d 24 02          	lea    (%r10,%r8,1),%r12d
    41cd:	44 01 cb             	add    %r9d,%ebx
    41d0:	e9 60 fb ff ff       	jmpq   3d35 <F_1_847+0x214>
    41d5:	0f 1f 00             	nopl   (%rax)
    41d8:	45 69 ee 9d d3 ff ff 	imul   $0xffffd39d,%r14d,%r13d
    41df:	45 69 e6 a1 25 00 00 	imul   $0x25a1,%r14d,%r12d
    41e6:	41 69 de dc e6 ff ff 	imul   $0xffffe6dc,%r14d,%ebx
    41ed:	45 69 f6 d4 08 00 00 	imul   $0x8d4,%r14d,%r14d
    41f4:	e9 4f f6 ff ff       	jmpq   3848 <fastIDCT+0x358>
    41f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    4200:	41 01 d8             	add    %ebx,%r8d
    4203:	45 69 c0 a1 25 00 00 	imul   $0x25a1,%r8d,%r8d
    420a:	45 69 f6 33 e3 ff ff 	imul   $0xffffe333,%r14d,%r14d
    4211:	45 01 c2             	add    %r8d,%r10d
    4214:	69 db 84 f3 ff ff    	imul   $0xfffff384,%ebx,%ebx
    421a:	47 8d 0c 16          	lea    (%r14,%r10,1),%r9d
    421e:	45 01 da             	add    %r11d,%r10d
    4221:	45 01 cd             	add    %r9d,%r13d
    4224:	41 01 d8             	add    %ebx,%r8d
    4227:	43 8d 1c 3a          	lea    (%r10,%r15,1),%ebx
    422b:	47 8d 0c 18          	lea    (%r8,%r11,1),%r9d
    422f:	45 01 c6             	add    %r8d,%r14d
    4232:	45 01 cc             	add    %r9d,%r12d
    4235:	e9 0e f6 ff ff       	jmpq   3848 <fastIDCT+0x358>
    423a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    4240:	41 89 c8             	mov    %ecx,%r8d
    4243:	41 89 f1             	mov    %esi,%r9d
    4246:	41 89 c2             	mov    %eax,%r10d
    4249:	41 89 d3             	mov    %edx,%r11d
    424c:	e9 1f f6 ff ff       	jmpq   3870 <fastIDCT+0x380>
    4251:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    4258:	69 d1 a1 25 00 00    	imul   $0x25a1,%ecx,%edx
    425e:	41 89 c7             	mov    %eax,%r15d
    4261:	45 89 c5             	mov    %r8d,%r13d
    4264:	69 e9 d4 08 00 00    	imul   $0x8d4,%ecx,%ebp
    426a:	45 89 d4             	mov    %r10d,%r12d
    426d:	41 29 d7             	sub    %edx,%r15d
    4270:	01 d0                	add    %edx,%eax
    4272:	69 f1 24 19 00 00    	imul   $0x1924,%ecx,%esi
    4278:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    427d:	45 89 cf             	mov    %r9d,%r15d
    4280:	69 c9 63 2c 00 00    	imul   $0x2c63,%ecx,%ecx
    4286:	41 29 ef             	sub    %ebp,%r15d
    4289:	41 01 e9             	add    %ebp,%r9d
    428c:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    4291:	41 29 f4             	sub    %esi,%r12d
    4294:	41 01 f2             	add    %esi,%r10d
    4297:	41 29 cd             	sub    %ecx,%r13d
    429a:	41 01 c8             	add    %ecx,%r8d
    429d:	e9 7e fd ff ff       	jmpq   4020 <F_1_961+0x15b>
    42a2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    42a8:	69 ca 9d d3 ff ff    	imul   $0xffffd39d,%edx,%ecx
    42ae:	41 89 c7             	mov    %eax,%r15d
    42b1:	45 89 c5             	mov    %r8d,%r13d
    42b4:	69 ea a1 25 00 00    	imul   $0x25a1,%edx,%ebp
    42ba:	45 89 d4             	mov    %r10d,%r12d
    42bd:	41 29 cf             	sub    %ecx,%r15d
    42c0:	01 c8                	add    %ecx,%eax
    42c2:	69 f2 d4 08 00 00    	imul   $0x8d4,%edx,%esi
    42c8:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    42cd:	45 89 cf             	mov    %r9d,%r15d
    42d0:	69 d2 24 19 00 00    	imul   $0x1924,%edx,%edx
    42d6:	41 29 ef             	sub    %ebp,%r15d
    42d9:	41 01 e9             	add    %ebp,%r9d
    42dc:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    42e1:	41 29 f4             	sub    %esi,%r12d
    42e4:	41 01 f2             	add    %esi,%r10d
    42e7:	41 29 d5             	sub    %edx,%r13d
    42ea:	41 01 d0             	add    %edx,%r8d
    42ed:	e9 2e fd ff ff       	jmpq   4020 <F_1_961+0x15b>
    42f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    42f8:	69 d5 dc e6 ff ff    	imul   $0xffffe6dc,%ebp,%edx
    42fe:	41 89 c7             	mov    %eax,%r15d
    4301:	45 89 c5             	mov    %r8d,%r13d
    4304:	69 f5 9d d3 ff ff    	imul   $0xffffd39d,%ebp,%esi
    430a:	45 89 d4             	mov    %r10d,%r12d
    430d:	41 29 d7             	sub    %edx,%r15d
    4310:	01 d0                	add    %edx,%eax
    4312:	69 cd a1 25 00 00    	imul   $0x25a1,%ebp,%ecx
    4318:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    431d:	45 89 cf             	mov    %r9d,%r15d
    4320:	69 ed d4 08 00 00    	imul   $0x8d4,%ebp,%ebp
    4326:	41 29 f7             	sub    %esi,%r15d
    4329:	41 01 f1             	add    %esi,%r9d
    432c:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    4331:	41 29 cc             	sub    %ecx,%r12d
    4334:	41 01 ca             	add    %ecx,%r10d
    4337:	41 29 ed             	sub    %ebp,%r13d
    433a:	41 01 e8             	add    %ebp,%r8d
    433d:	e9 de fc ff ff       	jmpq   4020 <F_1_961+0x15b>
    4342:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    4348:	44 69 ea b0 ef ff ff 	imul   $0xffffefb0,%edx,%r13d
    434f:	44 69 e2 84 f3 ff ff 	imul   $0xfffff384,%edx,%r12d
    4356:	01 ea                	add    %ebp,%edx
    4358:	69 d2 a1 25 00 00    	imul   $0x25a1,%edx,%edx
    435e:	69 cd c2 ec ff ff    	imul   $0xffffecc2,%ebp,%ecx
    4364:	01 d6                	add    %edx,%esi
    4366:	41 01 d4             	add    %edx,%r12d
    4369:	01 f1                	add    %esi,%ecx
    436b:	41 01 f6             	add    %esi,%r14d
    436e:	89 c6                	mov    %eax,%esi
    4370:	69 ed 33 e3 ff ff    	imul   $0xffffe333,%ebp,%ebp
    4376:	44 29 f6             	sub    %r14d,%esi
    4379:	43 8d 54 25 00       	lea    0x0(%r13,%r12,1),%edx
    437e:	89 74 24 e8          	mov    %esi,-0x18(%rsp)
    4382:	44 89 ce             	mov    %r9d,%esi
    4385:	45 89 c5             	mov    %r8d,%r13d
    4388:	29 ce                	sub    %ecx,%esi
    438a:	41 01 c9             	add    %ecx,%r9d
    438d:	44 01 f0             	add    %r14d,%eax
    4390:	44 01 e5             	add    %r12d,%ebp
    4393:	45 89 d4             	mov    %r10d,%r12d
    4396:	89 74 24 ec          	mov    %esi,-0x14(%rsp)
    439a:	41 29 ed             	sub    %ebp,%r13d
    439d:	41 29 d4             	sub    %edx,%r12d
    43a0:	41 01 d2             	add    %edx,%r10d
    43a3:	41 01 e8             	add    %ebp,%r8d
    43a6:	e9 75 fc ff ff       	jmpq   4020 <F_1_961+0x15b>
    43ab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    43b0:	69 d6 2c f7 ff ff    	imul   $0xfffff72c,%esi,%edx
    43b6:	41 89 c7             	mov    %eax,%r15d
    43b9:	45 89 c5             	mov    %r8d,%r13d
    43bc:	69 ee dc e6 ff ff    	imul   $0xffffe6dc,%esi,%ebp
    43c2:	45 89 d4             	mov    %r10d,%r12d
    43c5:	41 29 d7             	sub    %edx,%r15d
    43c8:	01 d0                	add    %edx,%eax
    43ca:	69 ce 9d d3 ff ff    	imul   $0xffffd39d,%esi,%ecx
    43d0:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    43d5:	45 89 cf             	mov    %r9d,%r15d
    43d8:	69 f6 a1 25 00 00    	imul   $0x25a1,%esi,%esi
    43de:	41 29 ef             	sub    %ebp,%r15d
    43e1:	41 01 e9             	add    %ebp,%r9d
    43e4:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    43e9:	41 29 cc             	sub    %ecx,%r12d
    43ec:	41 01 ca             	add    %ecx,%r10d
    43ef:	41 29 f5             	sub    %esi,%r13d
    43f2:	41 01 f0             	add    %esi,%r8d
    43f5:	e9 26 fc ff ff       	jmpq   4020 <F_1_961+0x15b>
    43fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    4400:	44 69 e2 a1 25 00 00 	imul   $0x25a1,%edx,%r12d
    4407:	69 cd c2 ec ff ff    	imul   $0xffffecc2,%ebp,%ecx
    440d:	47 8d 34 2c          	lea    (%r12,%r13,1),%r14d
    4411:	45 89 c5             	mov    %r8d,%r13d
    4414:	69 f6 50 10 00 00    	imul   $0x1050,%esi,%esi
    441a:	69 d2 dc e6 ff ff    	imul   $0xffffe6dc,%edx,%edx
    4420:	69 ed 33 e3 ff ff    	imul   $0xffffe333,%ebp,%ebp
    4426:	01 d1                	add    %edx,%ecx
    4428:	01 f2                	add    %esi,%edx
    442a:	89 c6                	mov    %eax,%esi
    442c:	01 d0                	add    %edx,%eax
    442e:	29 d6                	sub    %edx,%esi
    4430:	89 74 24 e8          	mov    %esi,-0x18(%rsp)
    4434:	44 89 ce             	mov    %r9d,%esi
    4437:	44 01 e5             	add    %r12d,%ebp
    443a:	29 ce                	sub    %ecx,%esi
    443c:	45 89 d4             	mov    %r10d,%r12d
    443f:	41 29 ed             	sub    %ebp,%r13d
    4442:	45 29 f4             	sub    %r14d,%r12d
    4445:	89 74 24 ec          	mov    %esi,-0x14(%rsp)
    4449:	41 01 c9             	add    %ecx,%r9d
    444c:	45 01 f2             	add    %r14d,%r10d
    444f:	41 01 e8             	add    %ebp,%r8d
    4452:	e9 c9 fb ff ff       	jmpq   4020 <F_1_961+0x15b>
    4457:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
    445e:	00 00 
    4460:	44 69 ed a1 25 00 00 	imul   $0x25a1,%ebp,%r13d
    4467:	41 89 c7             	mov    %eax,%r15d
    446a:	69 ed 9d d3 ff ff    	imul   $0xffffd39d,%ebp,%ebp
    4470:	47 8d 74 25 00       	lea    0x0(%r13,%r12,1),%r14d
    4475:	45 89 d4             	mov    %r10d,%r12d
    4478:	69 f6 8e 23 00 00    	imul   $0x238e,%esi,%esi
    447e:	69 ca 37 35 00 00    	imul   $0x3537,%edx,%ecx
    4484:	01 ee                	add    %ebp,%esi
    4486:	69 d2 84 f3 ff ff    	imul   $0xfffff384,%edx,%edx
    448c:	41 29 f7             	sub    %esi,%r15d
    448f:	01 e9                	add    %ebp,%ecx
    4491:	44 89 7c 24 e8       	mov    %r15d,-0x18(%rsp)
    4496:	45 89 cf             	mov    %r9d,%r15d
    4499:	41 29 cc             	sub    %ecx,%r12d
    449c:	45 29 f7             	sub    %r14d,%r15d
    449f:	45 01 f1             	add    %r14d,%r9d
    44a2:	41 01 ca             	add    %ecx,%r10d
    44a5:	44 01 ea             	add    %r13d,%edx
    44a8:	45 89 c5             	mov    %r8d,%r13d
    44ab:	44 89 7c 24 ec       	mov    %r15d,-0x14(%rsp)
    44b0:	41 29 d5             	sub    %edx,%r13d
    44b3:	01 f0                	add    %esi,%eax
    44b5:	41 01 d0             	add    %edx,%r8d
    44b8:	e9 63 fb ff ff       	jmpq   4020 <F_1_961+0x15b>
    44bd:	0f 1f 00             	nopl   (%rax)
    44c0:	01 d6                	add    %edx,%esi
    44c2:	89 c1                	mov    %eax,%ecx
    44c4:	69 f6 a1 25 00 00    	imul   $0x25a1,%esi,%esi
    44ca:	69 ed 33 e3 ff ff    	imul   $0xffffe333,%ebp,%ebp
    44d0:	41 01 f4             	add    %esi,%r12d
    44d3:	69 d2 84 f3 ff ff    	imul   $0xfffff384,%edx,%edx
    44d9:	01 d6                	add    %edx,%esi
    44db:	42 8d 54 25 00       	lea    0x0(%rbp,%r12,1),%edx
    44e0:	45 01 ec             	add    %r13d,%r12d
    44e3:	01 f5                	add    %esi,%ebp
    44e5:	41 01 d6             	add    %edx,%r14d
    44e8:	42 8d 14 2e          	lea    (%rsi,%r13,1),%edx
    44ec:	45 89 c5             	mov    %r8d,%r13d
    44ef:	41 29 ed             	sub    %ebp,%r13d
    44f2:	41 01 e8             	add    %ebp,%r8d
    44f5:	41 01 d7             	add    %edx,%r15d
    44f8:	8b 54 24 e4          	mov    -0x1c(%rsp),%edx
    44fc:	44 01 e2             	add    %r12d,%edx
    44ff:	45 89 d4             	mov    %r10d,%r12d
    4502:	45 01 fa             	add    %r15d,%r10d
    4505:	29 d1                	sub    %edx,%ecx
    4507:	45 29 fc             	sub    %r15d,%r12d
    450a:	01 d0                	add    %edx,%eax
    450c:	89 4c 24 e8          	mov    %ecx,-0x18(%rsp)
    4510:	44 89 c9             	mov    %r9d,%ecx
    4513:	45 01 f1             	add    %r14d,%r9d
    4516:	44 29 f1             	sub    %r14d,%ecx
    4519:	89 4c 24 ec          	mov    %ecx,-0x14(%rsp)
    451d:	e9 fe fa ff ff       	jmpq   4020 <F_1_961+0x15b>
    4522:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    4529:	00 00 00 
    452c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000004530 <dump_uint8>:
    4530:	41 54                	push   %r12
    4532:	55                   	push   %rbp
    4533:	48 8d 35 66 08 00 00 	lea    0x866(%rip),%rsi        # 4da0 <_IO_stdin_used+0x3e0>
    453a:	53                   	push   %rbx
    453b:	0f b6 17             	movzbl (%rdi),%edx
    453e:	4c 8d 25 5b 08 00 00 	lea    0x85b(%rip),%r12        # 4da0 <_IO_stdin_used+0x3e0>
    4545:	48 89 fd             	mov    %rdi,%rbp
    4548:	31 c0                	xor    %eax,%eax
    454a:	bf 01 00 00 00       	mov    $0x1,%edi
    454f:	bb 02 00 00 00       	mov    $0x2,%ebx
    4554:	e8 e7 c0 ff ff       	callq  640 <__printf_chk@plt>
    4559:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    4560:	0f b6 54 1d ff       	movzbl -0x1(%rbp,%rbx,1),%edx
    4565:	31 c0                	xor    %eax,%eax
    4567:	4c 89 e6             	mov    %r12,%rsi
    456a:	bf 01 00 00 00       	mov    $0x1,%edi
    456f:	e8 cc c0 ff ff       	callq  640 <__printf_chk@plt>
    4574:	f6 c3 07             	test   $0x7,%bl
    4577:	75 0a                	jne    4583 <dump_uint8+0x53>
    4579:	bf 0a 00 00 00       	mov    $0xa,%edi
    457e:	e8 8d c0 ff ff       	callq  610 <putchar@plt>
    4583:	48 83 c3 01          	add    $0x1,%rbx
    4587:	48 83 fb 41          	cmp    $0x41,%rbx
    458b:	75 d3                	jne    4560 <dump_uint8+0x30>
    458d:	5b                   	pop    %rbx
    458e:	5d                   	pop    %rbp
    458f:	41 5c                	pop    %r12
    4591:	bf 0a 00 00 00       	mov    $0xa,%edi
    4596:	e9 75 c0 ff ff       	jmpq   610 <putchar@plt>
    459b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000045a0 <dump_int16>:
    45a0:	41 54                	push   %r12
    45a2:	55                   	push   %rbp
    45a3:	48 8d 35 f6 07 00 00 	lea    0x7f6(%rip),%rsi        # 4da0 <_IO_stdin_used+0x3e0>
    45aa:	53                   	push   %rbx
    45ab:	0f bf 17             	movswl (%rdi),%edx
    45ae:	4c 8d 25 eb 07 00 00 	lea    0x7eb(%rip),%r12        # 4da0 <_IO_stdin_used+0x3e0>
    45b5:	48 89 fd             	mov    %rdi,%rbp
    45b8:	31 c0                	xor    %eax,%eax
    45ba:	bf 01 00 00 00       	mov    $0x1,%edi
    45bf:	bb 02 00 00 00       	mov    $0x2,%ebx
    45c4:	e8 77 c0 ff ff       	callq  640 <__printf_chk@plt>
    45c9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    45d0:	0f bf 54 5d fe       	movswl -0x2(%rbp,%rbx,2),%edx
    45d5:	31 c0                	xor    %eax,%eax
    45d7:	4c 89 e6             	mov    %r12,%rsi
    45da:	bf 01 00 00 00       	mov    $0x1,%edi
    45df:	e8 5c c0 ff ff       	callq  640 <__printf_chk@plt>
    45e4:	f6 c3 07             	test   $0x7,%bl
    45e7:	75 0a                	jne    45f3 <dump_int16+0x53>
    45e9:	bf 0a 00 00 00       	mov    $0xa,%edi
    45ee:	e8 1d c0 ff ff       	callq  610 <putchar@plt>
    45f3:	48 83 c3 01          	add    $0x1,%rbx
    45f7:	48 83 fb 41          	cmp    $0x41,%rbx
    45fb:	75 d3                	jne    45d0 <dump_int16+0x30>
    45fd:	5b                   	pop    %rbx
    45fe:	5d                   	pop    %rbp
    45ff:	41 5c                	pop    %r12
    4601:	bf 0a 00 00 00       	mov    $0xa,%edi
    4606:	e9 05 c0 ff ff       	jmpq   610 <putchar@plt>
    460b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000004610 <test0>:
    4610:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
    4615:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
    4619:	b9 10 00 00 00       	mov    $0x10,%ecx
    461e:	48 8d 15 46 08 00 00 	lea    0x846(%rip),%rdx        # 4e6b <__func__.3639>
    4625:	48 8d 35 9c 07 00 00 	lea    0x79c(%rip),%rsi        # 4dc8 <_IO_stdin_used+0x408>
    462c:	41 ff 72 f8          	pushq  -0x8(%r10)
    4630:	55                   	push   %rbp
    4631:	48 89 e5             	mov    %rsp,%rbp
    4634:	41 56                	push   %r14
    4636:	41 55                	push   %r13
    4638:	41 54                	push   %r12
    463a:	41 52                	push   %r10
    463c:	48 8d bd 70 fe ff ff 	lea    -0x190(%rbp),%rdi
    4643:	53                   	push   %rbx
    4644:	48 81 ec 88 01 00 00 	sub    $0x188,%rsp
    464b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    4652:	00 00 
    4654:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
    4658:	31 c0                	xor    %eax,%eax
    465a:	c5 fd 6f 05 1e 08 00 	vmovdqa 0x81e(%rip),%ymm0        # 4e80 <__func__.3639+0x15>
    4661:	00 
    4662:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    4665:	48 8d bd f0 fe ff ff 	lea    -0x110(%rbp),%rdi
    466c:	c5 fd 7f 85 70 ff ff 	vmovdqa %ymm0,-0x90(%rbp)
    4673:	ff 
    4674:	b9 10 00 00 00       	mov    $0x10,%ecx
    4679:	c5 fd 6f 05 1f 08 00 	vmovdqa 0x81f(%rip),%ymm0        # 4ea0 <__func__.3639+0x35>
    4680:	00 
    4681:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
    4686:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    4689:	b9 42 00 00 00       	mov    $0x42,%ecx
    468e:	bf 01 00 00 00       	mov    $0x1,%edi
    4693:	c5 f8 77             	vzeroupper 
    4696:	4c 8d b5 70 ff ff ff 	lea    -0x90(%rbp),%r14
    469d:	4c 8d ad 60 fe ff ff 	lea    -0x1a0(%rbp),%r13
    46a4:	e8 97 bf ff ff       	callq  640 <__printf_chk@plt>
    46a9:	4c 89 f7             	mov    %r14,%rdi
    46ac:	4c 8d a5 70 fe ff ff 	lea    -0x190(%rbp),%r12
    46b3:	e8 78 fe ff ff       	callq  4530 <dump_uint8>
    46b8:	31 f6                	xor    %esi,%esi
    46ba:	4c 89 ef             	mov    %r13,%rdi
    46bd:	e8 6e bf ff ff       	callq  630 <gettimeofday@plt>
    46c2:	48 69 9d 60 fe ff ff 	imul   $0xf4240,-0x1a0(%rbp),%rbx
    46c9:	40 42 0f 00 
    46cd:	ba 01 00 00 00       	mov    $0x1,%edx
    46d2:	4c 89 e6             	mov    %r12,%rsi
    46d5:	4c 89 f7             	mov    %r14,%rdi
    46d8:	31 c0                	xor    %eax,%eax
    46da:	48 03 9d 68 fe ff ff 	add    -0x198(%rbp),%rbx
    46e1:	e8 aa e1 ff ff       	callq  2890 <fastDCT>
    46e6:	31 f6                	xor    %esi,%esi
    46e8:	4c 89 ef             	mov    %r13,%rdi
    46eb:	e8 40 bf ff ff       	callq  630 <gettimeofday@plt>
    46f0:	4c 69 ad 60 fe ff ff 	imul   $0xf4240,-0x1a0(%rbp),%r13
    46f7:	40 42 0f 00 
    46fb:	4c 89 e7             	mov    %r12,%rdi
    46fe:	4c 03 ad 68 fe ff ff 	add    -0x198(%rbp),%r13
    4705:	e8 96 fe ff ff       	callq  45a0 <dump_int16>
    470a:	48 8d 15 5a 07 00 00 	lea    0x75a(%rip),%rdx        # 4e6b <__func__.3639>
    4711:	48 8d 35 d0 06 00 00 	lea    0x6d0(%rip),%rsi        # 4de8 <_IO_stdin_used+0x428>
    4718:	b9 49 00 00 00       	mov    $0x49,%ecx
    471d:	bf 01 00 00 00       	mov    $0x1,%edi
    4722:	31 c0                	xor    %eax,%eax
    4724:	4d 89 e8             	mov    %r13,%r8
    4727:	49 29 d8             	sub    %rbx,%r8
    472a:	48 8d 9d f0 fe ff ff 	lea    -0x110(%rbp),%rbx
    4731:	e8 0a bf ff ff       	callq  640 <__printf_chk@plt>
    4736:	48 89 de             	mov    %rbx,%rsi
    4739:	4c 89 e7             	mov    %r12,%rdi
    473c:	e8 1f ed ff ff       	callq  3460 <fastIDCTInit>
    4741:	48 89 df             	mov    %rbx,%rdi
    4744:	e8 a7 ed ff ff       	callq  34f0 <fastIDCT>
    4749:	48 89 df             	mov    %rbx,%rdi
    474c:	e8 4f fe ff ff       	callq  45a0 <dump_int16>
    4751:	48 8d 15 13 07 00 00 	lea    0x713(%rip),%rdx        # 4e6b <__func__.3639>
    4758:	48 8d 35 46 06 00 00 	lea    0x646(%rip),%rsi        # 4da5 <_IO_stdin_used+0x3e5>
    475f:	31 c0                	xor    %eax,%eax
    4761:	b9 4e 00 00 00       	mov    $0x4e,%ecx
    4766:	bf 01 00 00 00       	mov    $0x1,%edi
    476b:	e8 d0 be ff ff       	callq  640 <__printf_chk@plt>
    4770:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    4774:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    477b:	00 00 
    477d:	75 16                	jne    4795 <test0+0x185>
    477f:	48 81 c4 88 01 00 00 	add    $0x188,%rsp
    4786:	5b                   	pop    %rbx
    4787:	41 5a                	pop    %r10
    4789:	41 5c                	pop    %r12
    478b:	41 5d                	pop    %r13
    478d:	41 5e                	pop    %r14
    478f:	5d                   	pop    %rbp
    4790:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    4794:	c3                   	retq   
    4795:	e8 86 be ff ff       	callq  620 <__stack_chk_fail@plt>
    479a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000047a0 <test0_simd>:
    47a0:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
    47a5:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
    47a9:	b9 10 00 00 00       	mov    $0x10,%ecx
    47ae:	48 8d 15 ab 06 00 00 	lea    0x6ab(%rip),%rdx        # 4e60 <__func__.3653>
    47b5:	48 8d 35 0c 06 00 00 	lea    0x60c(%rip),%rsi        # 4dc8 <_IO_stdin_used+0x408>
    47bc:	41 ff 72 f8          	pushq  -0x8(%r10)
    47c0:	55                   	push   %rbp
    47c1:	48 89 e5             	mov    %rsp,%rbp
    47c4:	41 56                	push   %r14
    47c6:	41 55                	push   %r13
    47c8:	41 54                	push   %r12
    47ca:	41 52                	push   %r10
    47cc:	48 8d bd 70 fe ff ff 	lea    -0x190(%rbp),%rdi
    47d3:	53                   	push   %rbx
    47d4:	48 81 ec 88 01 00 00 	sub    $0x188,%rsp
    47db:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    47e2:	00 00 
    47e4:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
    47e8:	31 c0                	xor    %eax,%eax
    47ea:	c5 fd 6f 05 8e 06 00 	vmovdqa 0x68e(%rip),%ymm0        # 4e80 <__func__.3639+0x15>
    47f1:	00 
    47f2:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    47f5:	48 8d bd f0 fe ff ff 	lea    -0x110(%rbp),%rdi
    47fc:	c5 fd 7f 85 70 ff ff 	vmovdqa %ymm0,-0x90(%rbp)
    4803:	ff 
    4804:	b9 10 00 00 00       	mov    $0x10,%ecx
    4809:	c5 fd 6f 05 8f 06 00 	vmovdqa 0x68f(%rip),%ymm0        # 4ea0 <__func__.3639+0x35>
    4810:	00 
    4811:	c5 fd 7f 45 90       	vmovdqa %ymm0,-0x70(%rbp)
    4816:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    4819:	b9 5d 00 00 00       	mov    $0x5d,%ecx
    481e:	bf 01 00 00 00       	mov    $0x1,%edi
    4823:	c5 f8 77             	vzeroupper 
    4826:	4c 8d b5 70 ff ff ff 	lea    -0x90(%rbp),%r14
    482d:	4c 8d ad 60 fe ff ff 	lea    -0x1a0(%rbp),%r13
    4834:	e8 07 be ff ff       	callq  640 <__printf_chk@plt>
    4839:	4c 89 f7             	mov    %r14,%rdi
    483c:	4c 8d a5 70 fe ff ff 	lea    -0x190(%rbp),%r12
    4843:	e8 e8 fc ff ff       	callq  4530 <dump_uint8>
    4848:	31 f6                	xor    %esi,%esi
    484a:	4c 89 ef             	mov    %r13,%rdi
    484d:	e8 de bd ff ff       	callq  630 <gettimeofday@plt>
    4852:	48 69 9d 60 fe ff ff 	imul   $0xf4240,-0x1a0(%rbp),%rbx
    4859:	40 42 0f 00 
    485d:	4c 89 e6             	mov    %r12,%rsi
    4860:	4c 89 f7             	mov    %r14,%rdi
    4863:	48 03 9d 68 fe ff ff 	add    -0x198(%rbp),%rbx
    486a:	e8 c1 d7 ff ff       	callq  2030 <fastDCT_x86>
    486f:	31 f6                	xor    %esi,%esi
    4871:	4c 89 ef             	mov    %r13,%rdi
    4874:	e8 b7 bd ff ff       	callq  630 <gettimeofday@plt>
    4879:	4c 69 ad 60 fe ff ff 	imul   $0xf4240,-0x1a0(%rbp),%r13
    4880:	40 42 0f 00 
    4884:	4c 89 e7             	mov    %r12,%rdi
    4887:	4c 03 ad 68 fe ff ff 	add    -0x198(%rbp),%r13
    488e:	e8 0d fd ff ff       	callq  45a0 <dump_int16>
    4893:	48 8d 15 c6 05 00 00 	lea    0x5c6(%rip),%rdx        # 4e60 <__func__.3653>
    489a:	48 8d 35 47 05 00 00 	lea    0x547(%rip),%rsi        # 4de8 <_IO_stdin_used+0x428>
    48a1:	b9 63 00 00 00       	mov    $0x63,%ecx
    48a6:	bf 01 00 00 00       	mov    $0x1,%edi
    48ab:	31 c0                	xor    %eax,%eax
    48ad:	4d 89 e8             	mov    %r13,%r8
    48b0:	49 29 d8             	sub    %rbx,%r8
    48b3:	48 8d 9d f0 fe ff ff 	lea    -0x110(%rbp),%rbx
    48ba:	e8 81 bd ff ff       	callq  640 <__printf_chk@plt>
    48bf:	48 89 de             	mov    %rbx,%rsi
    48c2:	4c 89 e7             	mov    %r12,%rdi
    48c5:	e8 96 eb ff ff       	callq  3460 <fastIDCTInit>
    48ca:	48 89 df             	mov    %rbx,%rdi
    48cd:	e8 1e ec ff ff       	callq  34f0 <fastIDCT>
    48d2:	48 89 df             	mov    %rbx,%rdi
    48d5:	e8 c6 fc ff ff       	callq  45a0 <dump_int16>
    48da:	48 8d 15 7f 05 00 00 	lea    0x57f(%rip),%rdx        # 4e60 <__func__.3653>
    48e1:	48 8d 35 bd 04 00 00 	lea    0x4bd(%rip),%rsi        # 4da5 <_IO_stdin_used+0x3e5>
    48e8:	31 c0                	xor    %eax,%eax
    48ea:	b9 68 00 00 00       	mov    $0x68,%ecx
    48ef:	bf 01 00 00 00       	mov    $0x1,%edi
    48f4:	e8 47 bd ff ff       	callq  640 <__printf_chk@plt>
    48f9:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    48fd:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    4904:	00 00 
    4906:	75 16                	jne    491e <test0_simd+0x17e>
    4908:	48 81 c4 88 01 00 00 	add    $0x188,%rsp
    490f:	5b                   	pop    %rbx
    4910:	41 5a                	pop    %r10
    4912:	41 5c                	pop    %r12
    4914:	41 5d                	pop    %r13
    4916:	41 5e                	pop    %r14
    4918:	5d                   	pop    %rbp
    4919:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    491d:	c3                   	retq   
    491e:	e8 fd bc ff ff       	callq  620 <__stack_chk_fail@plt>
    4923:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    492a:	00 00 00 
    492d:	0f 1f 00             	nopl   (%rax)

0000000000004930 <__libc_csu_init>:
    4930:	41 57                	push   %r15
    4932:	41 56                	push   %r14
    4934:	49 89 d7             	mov    %rdx,%r15
    4937:	41 55                	push   %r13
    4939:	41 54                	push   %r12
    493b:	4c 8d 25 5e 14 20 00 	lea    0x20145e(%rip),%r12        # 205da0 <__frame_dummy_init_array_entry>
    4942:	55                   	push   %rbp
    4943:	48 8d 2d 5e 14 20 00 	lea    0x20145e(%rip),%rbp        # 205da8 <__init_array_end>
    494a:	53                   	push   %rbx
    494b:	41 89 fd             	mov    %edi,%r13d
    494e:	49 89 f6             	mov    %rsi,%r14
    4951:	4c 29 e5             	sub    %r12,%rbp
    4954:	48 83 ec 08          	sub    $0x8,%rsp
    4958:	48 c1 fd 03          	sar    $0x3,%rbp
    495c:	e8 87 bc ff ff       	callq  5e8 <_init>
    4961:	48 85 ed             	test   %rbp,%rbp
    4964:	74 20                	je     4986 <__libc_csu_init+0x56>
    4966:	31 db                	xor    %ebx,%ebx
    4968:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    496f:	00 
    4970:	4c 89 fa             	mov    %r15,%rdx
    4973:	4c 89 f6             	mov    %r14,%rsi
    4976:	44 89 ef             	mov    %r13d,%edi
    4979:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
    497d:	48 83 c3 01          	add    $0x1,%rbx
    4981:	48 39 dd             	cmp    %rbx,%rbp
    4984:	75 ea                	jne    4970 <__libc_csu_init+0x40>
    4986:	48 83 c4 08          	add    $0x8,%rsp
    498a:	5b                   	pop    %rbx
    498b:	5d                   	pop    %rbp
    498c:	41 5c                	pop    %r12
    498e:	41 5d                	pop    %r13
    4990:	41 5e                	pop    %r14
    4992:	41 5f                	pop    %r15
    4994:	c3                   	retq   
    4995:	90                   	nop
    4996:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    499d:	00 00 00 

00000000000049a0 <__libc_csu_fini>:
    49a0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000000049a4 <_fini>:
    49a4:	48 83 ec 08          	sub    $0x8,%rsp
    49a8:	48 83 c4 08          	add    $0x8,%rsp
    49ac:	c3                   	retq   
