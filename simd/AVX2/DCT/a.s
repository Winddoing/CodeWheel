
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000000588 <_init>:
 588:	48 83 ec 08          	sub    $0x8,%rsp
 58c:	48 8b 05 55 5a 20 00 	mov    0x205a55(%rip),%rax        # 205fe8 <__gmon_start__>
 593:	48 85 c0             	test   %rax,%rax
 596:	74 02                	je     59a <_init+0x12>
 598:	ff d0                	callq  *%rax
 59a:	48 83 c4 08          	add    $0x8,%rsp
 59e:	c3                   	retq   

Disassembly of section .plt:

00000000000005a0 <.plt>:
 5a0:	ff 35 0a 5a 20 00    	pushq  0x205a0a(%rip)        # 205fb0 <_GLOBAL_OFFSET_TABLE_+0x8>
 5a6:	ff 25 0c 5a 20 00    	jmpq   *0x205a0c(%rip)        # 205fb8 <_GLOBAL_OFFSET_TABLE_+0x10>
 5ac:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000005b0 <putchar@plt>:
 5b0:	ff 25 0a 5a 20 00    	jmpq   *0x205a0a(%rip)        # 205fc0 <putchar@GLIBC_2.2.5>
 5b6:	68 00 00 00 00       	pushq  $0x0
 5bb:	e9 e0 ff ff ff       	jmpq   5a0 <.plt>

00000000000005c0 <__stack_chk_fail@plt>:
 5c0:	ff 25 02 5a 20 00    	jmpq   *0x205a02(%rip)        # 205fc8 <__stack_chk_fail@GLIBC_2.4>
 5c6:	68 01 00 00 00       	pushq  $0x1
 5cb:	e9 d0 ff ff ff       	jmpq   5a0 <.plt>

00000000000005d0 <printf@plt>:
 5d0:	ff 25 fa 59 20 00    	jmpq   *0x2059fa(%rip)        # 205fd0 <printf@GLIBC_2.2.5>
 5d6:	68 02 00 00 00       	pushq  $0x2
 5db:	e9 c0 ff ff ff       	jmpq   5a0 <.plt>

Disassembly of section .plt.got:

00000000000005e0 <__cxa_finalize@plt>:
 5e0:	ff 25 12 5a 20 00    	jmpq   *0x205a12(%rip)        # 205ff8 <__cxa_finalize@GLIBC_2.2.5>
 5e6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000600 <_start>:
     600:	31 ed                	xor    %ebp,%ebp
     602:	49 89 d1             	mov    %rdx,%r9
     605:	5e                   	pop    %rsi
     606:	48 89 e2             	mov    %rsp,%rdx
     609:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
     60d:	50                   	push   %rax
     60e:	54                   	push   %rsp
     60f:	4c 8d 05 3a 4d 00 00 	lea    0x4d3a(%rip),%r8        # 5350 <__libc_csu_fini>
     616:	48 8d 0d c3 4c 00 00 	lea    0x4cc3(%rip),%rcx        # 52e0 <__libc_csu_init>
     61d:	48 8d 3d 74 43 00 00 	lea    0x4374(%rip),%rdi        # 4998 <main>
     624:	ff 15 b6 59 20 00    	callq  *0x2059b6(%rip)        # 205fe0 <__libc_start_main@GLIBC_2.2.5>
     62a:	f4                   	hlt    
     62b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000630 <deregister_tm_clones>:
     630:	48 8d 3d e9 5b 20 00 	lea    0x205be9(%rip),%rdi        # 206220 <__TMC_END__>
     637:	55                   	push   %rbp
     638:	48 8d 05 e1 5b 20 00 	lea    0x205be1(%rip),%rax        # 206220 <__TMC_END__>
     63f:	48 39 f8             	cmp    %rdi,%rax
     642:	48 89 e5             	mov    %rsp,%rbp
     645:	74 19                	je     660 <deregister_tm_clones+0x30>
     647:	48 8b 05 8a 59 20 00 	mov    0x20598a(%rip),%rax        # 205fd8 <_ITM_deregisterTMCloneTable>
     64e:	48 85 c0             	test   %rax,%rax
     651:	74 0d                	je     660 <deregister_tm_clones+0x30>
     653:	5d                   	pop    %rbp
     654:	ff e0                	jmpq   *%rax
     656:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     65d:	00 00 00 
     660:	5d                   	pop    %rbp
     661:	c3                   	retq   
     662:	0f 1f 40 00          	nopl   0x0(%rax)
     666:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     66d:	00 00 00 

0000000000000670 <register_tm_clones>:
     670:	48 8d 3d a9 5b 20 00 	lea    0x205ba9(%rip),%rdi        # 206220 <__TMC_END__>
     677:	48 8d 35 a2 5b 20 00 	lea    0x205ba2(%rip),%rsi        # 206220 <__TMC_END__>
     67e:	55                   	push   %rbp
     67f:	48 29 fe             	sub    %rdi,%rsi
     682:	48 89 e5             	mov    %rsp,%rbp
     685:	48 c1 fe 03          	sar    $0x3,%rsi
     689:	48 89 f0             	mov    %rsi,%rax
     68c:	48 c1 e8 3f          	shr    $0x3f,%rax
     690:	48 01 c6             	add    %rax,%rsi
     693:	48 d1 fe             	sar    %rsi
     696:	74 18                	je     6b0 <register_tm_clones+0x40>
     698:	48 8b 05 51 59 20 00 	mov    0x205951(%rip),%rax        # 205ff0 <_ITM_registerTMCloneTable>
     69f:	48 85 c0             	test   %rax,%rax
     6a2:	74 0c                	je     6b0 <register_tm_clones+0x40>
     6a4:	5d                   	pop    %rbp
     6a5:	ff e0                	jmpq   *%rax
     6a7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
     6ae:	00 00 
     6b0:	5d                   	pop    %rbp
     6b1:	c3                   	retq   
     6b2:	0f 1f 40 00          	nopl   0x0(%rax)
     6b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
     6bd:	00 00 00 

00000000000006c0 <__do_global_dtors_aux>:
     6c0:	80 3d 59 5b 20 00 00 	cmpb   $0x0,0x205b59(%rip)        # 206220 <__TMC_END__>
     6c7:	75 2f                	jne    6f8 <__do_global_dtors_aux+0x38>
     6c9:	48 83 3d 27 59 20 00 	cmpq   $0x0,0x205927(%rip)        # 205ff8 <__cxa_finalize@GLIBC_2.2.5>
     6d0:	00 
     6d1:	55                   	push   %rbp
     6d2:	48 89 e5             	mov    %rsp,%rbp
     6d5:	74 0c                	je     6e3 <__do_global_dtors_aux+0x23>
     6d7:	48 8b 3d 2a 59 20 00 	mov    0x20592a(%rip),%rdi        # 206008 <__dso_handle>
     6de:	e8 fd fe ff ff       	callq  5e0 <__cxa_finalize@plt>
     6e3:	e8 48 ff ff ff       	callq  630 <deregister_tm_clones>
     6e8:	c6 05 31 5b 20 00 01 	movb   $0x1,0x205b31(%rip)        # 206220 <__TMC_END__>
     6ef:	5d                   	pop    %rbp
     6f0:	c3                   	retq   
     6f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
     6f8:	f3 c3                	repz retq 
     6fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000700 <frame_dummy>:
     700:	55                   	push   %rbp
     701:	48 89 e5             	mov    %rsp,%rbp
     704:	5d                   	pop    %rbp
     705:	e9 66 ff ff ff       	jmpq   670 <register_tm_clones>

000000000000070a <fastDCT_x86>:
     70a:	4c 8d 54 24 08       	lea    0x8(%rsp),%r10
     70f:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
     713:	41 ff 72 f8          	pushq  -0x8(%r10)
     717:	55                   	push   %rbp
     718:	48 89 e5             	mov    %rsp,%rbp
     71b:	41 52                	push   %r10
     71d:	48 81 ec a8 2e 00 00 	sub    $0x2ea8,%rsp
     724:	48 89 bd 68 d1 ff ff 	mov    %rdi,-0x2e98(%rbp)
     72b:	48 89 b5 60 d1 ff ff 	mov    %rsi,-0x2ea0(%rbp)
     732:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
     739:	00 00 
     73b:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
     73f:	31 c0                	xor    %eax,%eax
     741:	c7 85 e4 d1 ff ff 62 	movl   $0x1f62,-0x2e1c(%rbp)
     748:	1f 00 00 
     74b:	c4 e2 7d 58 85 e4 d1 	vpbroadcastd -0x2e1c(%rbp),%ymm0
     752:	ff ff 
     754:	c5 fd 7f 85 90 d3 ff 	vmovdqa %ymm0,-0x2c70(%rbp)
     75b:	ff 
     75c:	c7 85 e0 d1 ff ff 9b 	movl   $0x1a9b,-0x2e20(%rbp)
     763:	1a 00 00 
     766:	c4 e2 7d 58 85 e0 d1 	vpbroadcastd -0x2e20(%rbp),%ymm0
     76d:	ff ff 
     76f:	c5 fd 7f 85 b0 d3 ff 	vmovdqa %ymm0,-0x2c50(%rbp)
     776:	ff 
     777:	c7 85 dc d1 ff ff c7 	movl   $0x11c7,-0x2e24(%rbp)
     77e:	11 00 00 
     781:	c4 e2 7d 58 85 dc d1 	vpbroadcastd -0x2e24(%rbp),%ymm0
     788:	ff ff 
     78a:	c5 fd 7f 85 d0 d3 ff 	vmovdqa %ymm0,-0x2c30(%rbp)
     791:	ff 
     792:	c7 85 d8 d1 ff ff 3e 	movl   $0x63e,-0x2e28(%rbp)
     799:	06 00 00 
     79c:	c4 e2 7d 58 85 d8 d1 	vpbroadcastd -0x2e28(%rbp),%ymm0
     7a3:	ff ff 
     7a5:	c5 fd 7f 85 f0 d3 ff 	vmovdqa %ymm0,-0x2c10(%rbp)
     7ac:	ff 
     7ad:	c7 85 d4 d1 ff ff cf 	movl   $0x29cf,-0x2e2c(%rbp)
     7b4:	29 00 00 
     7b7:	c4 e2 7d 58 85 d4 d1 	vpbroadcastd -0x2e2c(%rbp),%ymm0
     7be:	ff ff 
     7c0:	c5 fd 7f 85 10 d4 ff 	vmovdqa %ymm0,-0x2bf0(%rbp)
     7c7:	ff 
     7c8:	c7 85 d0 d1 ff ff 51 	movl   $0x1151,-0x2e30(%rbp)
     7cf:	11 00 00 
     7d2:	c4 e2 7d 58 85 d0 d1 	vpbroadcastd -0x2e30(%rbp),%ymm0
     7d9:	ff ff 
     7db:	c5 fd 7f 85 30 d4 ff 	vmovdqa %ymm0,-0x2bd0(%rbp)
     7e2:	ff 
     7e3:	c7 85 cc d1 ff ff 15 	movl   $0xfb15,-0x2e34(%rbp)
     7ea:	fb 00 00 
     7ed:	c4 e2 7d 58 85 cc d1 	vpbroadcastd -0x2e34(%rbp),%ymm0
     7f4:	ff ff 
     7f6:	c5 fd 7f 85 50 d4 ff 	vmovdqa %ymm0,-0x2bb0(%rbp)
     7fd:	ff 
     7fe:	c7 85 c8 d1 ff ff db 	movl   $0xd4db,-0x2e38(%rbp)
     805:	d4 00 00 
     808:	c4 e2 7d 58 85 c8 d1 	vpbroadcastd -0x2e38(%rbp),%ymm0
     80f:	ff ff 
     811:	c5 fd 7f 85 70 d4 ff 	vmovdqa %ymm0,-0x2b90(%rbp)
     818:	ff 
     819:	c7 85 c4 d1 ff ff 3a 	movl   $0x8e3a,-0x2e3c(%rbp)
     820:	8e 00 00 
     823:	c4 e2 7d 58 85 c4 d1 	vpbroadcastd -0x2e3c(%rbp),%ymm0
     82a:	ff ff 
     82c:	c5 fd 7f 85 90 d4 ff 	vmovdqa %ymm0,-0x2b70(%rbp)
     833:	ff 
     834:	c7 85 c0 d1 ff ff f1 	movl   $0x31f1,-0x2e40(%rbp)
     83b:	31 00 00 
     83e:	c4 e2 7d 58 85 c0 d1 	vpbroadcastd -0x2e40(%rbp),%ymm0
     845:	ff ff 
     847:	c5 fd 7f 85 b0 d4 ff 	vmovdqa %ymm0,-0x2b50(%rbp)
     84e:	ff 
     84f:	c7 85 bc d1 ff ff 3d 	movl   $0xa73d,-0x2e44(%rbp)
     856:	a7 00 00 
     859:	c4 e2 7d 58 85 bc d1 	vpbroadcastd -0x2e44(%rbp),%ymm0
     860:	ff ff 
     862:	c5 fd 7f 85 d0 d4 ff 	vmovdqa %ymm0,-0x2b30(%rbp)
     869:	ff 
     86a:	c7 85 b8 d1 ff ff 46 	movl   $0x4546,-0x2e48(%rbp)
     871:	45 00 00 
     874:	c4 e2 7d 58 85 b8 d1 	vpbroadcastd -0x2e48(%rbp),%ymm0
     87b:	ff ff 
     87d:	c5 fd 7f 85 f0 d4 ff 	vmovdqa %ymm0,-0x2b10(%rbp)
     884:	ff 
     885:	c7 85 b4 d1 ff ff 05 	movl   $0xb505,-0x2e4c(%rbp)
     88c:	b5 00 00 
     88f:	c4 e2 7d 58 85 b4 d1 	vpbroadcastd -0x2e4c(%rbp),%ymm0
     896:	ff ff 
     898:	c5 fd 7f 85 10 d5 ff 	vmovdqa %ymm0,-0x2af0(%rbp)
     89f:	ff 
     8a0:	c7 85 b0 d1 ff ff 80 	movl   $0x80,-0x2e50(%rbp)
     8a7:	00 00 00 
     8aa:	c4 e2 7d 58 85 b0 d1 	vpbroadcastd -0x2e50(%rbp),%ymm0
     8b1:	ff ff 
     8b3:	c5 fd 7f 85 30 d5 ff 	vmovdqa %ymm0,-0x2ad0(%rbp)
     8ba:	ff 
     8bb:	c7 85 ac d1 ff ff 04 	movl   $0x4,-0x2e54(%rbp)
     8c2:	00 00 00 
     8c5:	c4 e2 7d 58 85 ac d1 	vpbroadcastd -0x2e54(%rbp),%ymm0
     8cc:	ff ff 
     8ce:	c5 fd 7f 85 50 d5 ff 	vmovdqa %ymm0,-0x2ab0(%rbp)
     8d5:	ff 
     8d6:	c7 85 a8 d1 ff ff 00 	movl   $0x8000,-0x2e58(%rbp)
     8dd:	80 00 00 
     8e0:	c4 e2 7d 58 85 a8 d1 	vpbroadcastd -0x2e58(%rbp),%ymm0
     8e7:	ff ff 
     8e9:	c5 fd 7f 85 70 d5 ff 	vmovdqa %ymm0,-0x2a90(%rbp)
     8f0:	ff 
     8f1:	48 8b 85 68 d1 ff ff 	mov    -0x2e98(%rbp),%rax
     8f8:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     8fc:	c5 f8 29 85 00 d3 ff 	vmovaps %xmm0,-0x2d00(%rbp)
     903:	ff 
     904:	c5 f9 6f 85 00 d3 ff 	vmovdqa -0x2d00(%rbp),%xmm0
     90b:	ff 
     90c:	c4 e2 7d 30 c0       	vpmovzxbw %xmm0,%ymm0
     911:	c5 fd 7f 85 10 d3 ff 	vmovdqa %ymm0,-0x2cf0(%rbp)
     918:	ff 
     919:	48 8b 85 68 d1 ff ff 	mov    -0x2e98(%rbp),%rax
     920:	48 83 c0 10          	add    $0x10,%rax
     924:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     928:	c5 f8 29 85 f0 d2 ff 	vmovaps %xmm0,-0x2d10(%rbp)
     92f:	ff 
     930:	c5 f9 6f 85 f0 d2 ff 	vmovdqa -0x2d10(%rbp),%xmm0
     937:	ff 
     938:	c4 e2 7d 30 c0       	vpmovzxbw %xmm0,%ymm0
     93d:	c5 fd 7f 85 30 d3 ff 	vmovdqa %ymm0,-0x2cd0(%rbp)
     944:	ff 
     945:	48 8b 85 68 d1 ff ff 	mov    -0x2e98(%rbp),%rax
     94c:	48 83 c0 20          	add    $0x20,%rax
     950:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     954:	c5 f8 29 85 e0 d2 ff 	vmovaps %xmm0,-0x2d20(%rbp)
     95b:	ff 
     95c:	c5 f9 6f 85 e0 d2 ff 	vmovdqa -0x2d20(%rbp),%xmm0
     963:	ff 
     964:	c4 e2 7d 30 c0       	vpmovzxbw %xmm0,%ymm0
     969:	c5 fd 7f 85 50 d3 ff 	vmovdqa %ymm0,-0x2cb0(%rbp)
     970:	ff 
     971:	48 8b 85 68 d1 ff ff 	mov    -0x2e98(%rbp),%rax
     978:	48 83 c0 30          	add    $0x30,%rax
     97c:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     980:	c5 f8 29 85 d0 d2 ff 	vmovaps %xmm0,-0x2d30(%rbp)
     987:	ff 
     988:	c5 f9 6f 85 d0 d2 ff 	vmovdqa -0x2d30(%rbp),%xmm0
     98f:	ff 
     990:	c4 e2 7d 30 c0       	vpmovzxbw %xmm0,%ymm0
     995:	c5 fd 7f 85 70 d3 ff 	vmovdqa %ymm0,-0x2c90(%rbp)
     99c:	ff 
     99d:	48 8d 85 10 d3 ff ff 	lea    -0x2cf0(%rbp),%rax
     9a4:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     9a8:	c5 f8 29 85 c0 d2 ff 	vmovaps %xmm0,-0x2d40(%rbp)
     9af:	ff 
     9b0:	c5 f9 6f 85 c0 d2 ff 	vmovdqa -0x2d40(%rbp),%xmm0
     9b7:	ff 
     9b8:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     9bd:	c5 fd 7f 85 90 d5 ff 	vmovdqa %ymm0,-0x2a70(%rbp)
     9c4:	ff 
     9c5:	48 8d 85 10 d3 ff ff 	lea    -0x2cf0(%rbp),%rax
     9cc:	48 83 c0 10          	add    $0x10,%rax
     9d0:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     9d4:	c5 f8 29 85 b0 d2 ff 	vmovaps %xmm0,-0x2d50(%rbp)
     9db:	ff 
     9dc:	c5 f9 6f 85 b0 d2 ff 	vmovdqa -0x2d50(%rbp),%xmm0
     9e3:	ff 
     9e4:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     9e9:	c5 fd 7f 85 b0 d5 ff 	vmovdqa %ymm0,-0x2a50(%rbp)
     9f0:	ff 
     9f1:	48 8d 85 30 d3 ff ff 	lea    -0x2cd0(%rbp),%rax
     9f8:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     9fc:	c5 f8 29 85 a0 d2 ff 	vmovaps %xmm0,-0x2d60(%rbp)
     a03:	ff 
     a04:	c5 f9 6f 85 a0 d2 ff 	vmovdqa -0x2d60(%rbp),%xmm0
     a0b:	ff 
     a0c:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     a11:	c5 fd 7f 85 d0 d5 ff 	vmovdqa %ymm0,-0x2a30(%rbp)
     a18:	ff 
     a19:	48 8d 85 30 d3 ff ff 	lea    -0x2cd0(%rbp),%rax
     a20:	48 83 c0 10          	add    $0x10,%rax
     a24:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     a28:	c5 f8 29 85 90 d2 ff 	vmovaps %xmm0,-0x2d70(%rbp)
     a2f:	ff 
     a30:	c5 f9 6f 85 90 d2 ff 	vmovdqa -0x2d70(%rbp),%xmm0
     a37:	ff 
     a38:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     a3d:	c5 fd 7f 85 f0 d5 ff 	vmovdqa %ymm0,-0x2a10(%rbp)
     a44:	ff 
     a45:	48 8d 85 50 d3 ff ff 	lea    -0x2cb0(%rbp),%rax
     a4c:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     a50:	c5 f8 29 85 80 d2 ff 	vmovaps %xmm0,-0x2d80(%rbp)
     a57:	ff 
     a58:	c5 f9 6f 85 80 d2 ff 	vmovdqa -0x2d80(%rbp),%xmm0
     a5f:	ff 
     a60:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     a65:	c5 fd 7f 85 10 d6 ff 	vmovdqa %ymm0,-0x29f0(%rbp)
     a6c:	ff 
     a6d:	48 8d 85 50 d3 ff ff 	lea    -0x2cb0(%rbp),%rax
     a74:	48 83 c0 10          	add    $0x10,%rax
     a78:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     a7c:	c5 f8 29 85 70 d2 ff 	vmovaps %xmm0,-0x2d90(%rbp)
     a83:	ff 
     a84:	c5 f9 6f 85 70 d2 ff 	vmovdqa -0x2d90(%rbp),%xmm0
     a8b:	ff 
     a8c:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     a91:	c5 fd 7f 85 30 d6 ff 	vmovdqa %ymm0,-0x29d0(%rbp)
     a98:	ff 
     a99:	48 8d 85 70 d3 ff ff 	lea    -0x2c90(%rbp),%rax
     aa0:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     aa4:	c5 f8 29 85 60 d2 ff 	vmovaps %xmm0,-0x2da0(%rbp)
     aab:	ff 
     aac:	c5 f9 6f 85 60 d2 ff 	vmovdqa -0x2da0(%rbp),%xmm0
     ab3:	ff 
     ab4:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     ab9:	c5 fd 7f 85 50 d6 ff 	vmovdqa %ymm0,-0x29b0(%rbp)
     ac0:	ff 
     ac1:	48 8d 85 70 d3 ff ff 	lea    -0x2c90(%rbp),%rax
     ac8:	48 83 c0 10          	add    $0x10,%rax
     acc:	c5 f9 6f 00          	vmovdqa (%rax),%xmm0
     ad0:	c5 f8 29 85 50 d2 ff 	vmovaps %xmm0,-0x2db0(%rbp)
     ad7:	ff 
     ad8:	c5 f9 6f 85 50 d2 ff 	vmovdqa -0x2db0(%rbp),%xmm0
     adf:	ff 
     ae0:	c4 e2 7d 23 c0       	vpmovsxwd %xmm0,%ymm0
     ae5:	c5 fd 7f 85 70 d6 ff 	vmovdqa %ymm0,-0x2990(%rbp)
     aec:	ff 
     aed:	c5 fd 6f 85 90 d5 ff 	vmovdqa -0x2a70(%rbp),%ymm0
     af4:	ff 
     af5:	c5 fd 7f 85 90 ee ff 	vmovdqa %ymm0,-0x1170(%rbp)
     afc:	ff 
     afd:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     b04:	ff 
     b05:	c5 fd 7f 85 b0 ee ff 	vmovdqa %ymm0,-0x1150(%rbp)
     b0c:	ff 
     b0d:	c5 fd 6f 85 90 ee ff 	vmovdqa -0x1170(%rbp),%ymm0
     b14:	ff 
     b15:	c5 fd 6f 8d b0 ee ff 	vmovdqa -0x1150(%rbp),%ymm1
     b1c:	ff 
     b1d:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     b21:	c5 fd 7f 85 10 d3 ff 	vmovdqa %ymm0,-0x2cf0(%rbp)
     b28:	ff 
     b29:	c5 fd 6f 85 b0 d5 ff 	vmovdqa -0x2a50(%rbp),%ymm0
     b30:	ff 
     b31:	c5 fd 7f 85 50 ee ff 	vmovdqa %ymm0,-0x11b0(%rbp)
     b38:	ff 
     b39:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     b40:	ff 
     b41:	c5 fd 7f 85 70 ee ff 	vmovdqa %ymm0,-0x1190(%rbp)
     b48:	ff 
     b49:	c5 fd 6f 85 50 ee ff 	vmovdqa -0x11b0(%rbp),%ymm0
     b50:	ff 
     b51:	c5 fd 6f 8d 70 ee ff 	vmovdqa -0x1190(%rbp),%ymm1
     b58:	ff 
     b59:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     b5d:	c5 fd 7f 85 30 d3 ff 	vmovdqa %ymm0,-0x2cd0(%rbp)
     b64:	ff 
     b65:	c5 fd 6f 85 d0 d5 ff 	vmovdqa -0x2a30(%rbp),%ymm0
     b6c:	ff 
     b6d:	c5 fd 7f 85 10 ee ff 	vmovdqa %ymm0,-0x11f0(%rbp)
     b74:	ff 
     b75:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     b7c:	ff 
     b7d:	c5 fd 7f 85 30 ee ff 	vmovdqa %ymm0,-0x11d0(%rbp)
     b84:	ff 
     b85:	c5 fd 6f 85 10 ee ff 	vmovdqa -0x11f0(%rbp),%ymm0
     b8c:	ff 
     b8d:	c5 fd 6f 8d 30 ee ff 	vmovdqa -0x11d0(%rbp),%ymm1
     b94:	ff 
     b95:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     b99:	c5 fd 7f 85 50 d3 ff 	vmovdqa %ymm0,-0x2cb0(%rbp)
     ba0:	ff 
     ba1:	c5 fd 6f 85 f0 d5 ff 	vmovdqa -0x2a10(%rbp),%ymm0
     ba8:	ff 
     ba9:	c5 fd 7f 85 d0 ed ff 	vmovdqa %ymm0,-0x1230(%rbp)
     bb0:	ff 
     bb1:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     bb8:	ff 
     bb9:	c5 fd 7f 85 f0 ed ff 	vmovdqa %ymm0,-0x1210(%rbp)
     bc0:	ff 
     bc1:	c5 fd 6f 85 d0 ed ff 	vmovdqa -0x1230(%rbp),%ymm0
     bc8:	ff 
     bc9:	c5 fd 6f 8d f0 ed ff 	vmovdqa -0x1210(%rbp),%ymm1
     bd0:	ff 
     bd1:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     bd5:	c5 fd 7f 85 70 d3 ff 	vmovdqa %ymm0,-0x2c90(%rbp)
     bdc:	ff 
     bdd:	c5 fd 6f 85 10 d6 ff 	vmovdqa -0x29f0(%rbp),%ymm0
     be4:	ff 
     be5:	c5 fd 7f 85 90 ed ff 	vmovdqa %ymm0,-0x1270(%rbp)
     bec:	ff 
     bed:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     bf4:	ff 
     bf5:	c5 fd 7f 85 b0 ed ff 	vmovdqa %ymm0,-0x1250(%rbp)
     bfc:	ff 
     bfd:	c5 fd 6f 85 90 ed ff 	vmovdqa -0x1270(%rbp),%ymm0
     c04:	ff 
     c05:	c5 fd 6f 8d b0 ed ff 	vmovdqa -0x1250(%rbp),%ymm1
     c0c:	ff 
     c0d:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     c11:	c5 fd 7f 85 90 d6 ff 	vmovdqa %ymm0,-0x2970(%rbp)
     c18:	ff 
     c19:	c5 fd 6f 85 30 d6 ff 	vmovdqa -0x29d0(%rbp),%ymm0
     c20:	ff 
     c21:	c5 fd 7f 85 50 ed ff 	vmovdqa %ymm0,-0x12b0(%rbp)
     c28:	ff 
     c29:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     c30:	ff 
     c31:	c5 fd 7f 85 70 ed ff 	vmovdqa %ymm0,-0x1290(%rbp)
     c38:	ff 
     c39:	c5 fd 6f 85 50 ed ff 	vmovdqa -0x12b0(%rbp),%ymm0
     c40:	ff 
     c41:	c5 fd 6f 8d 70 ed ff 	vmovdqa -0x1290(%rbp),%ymm1
     c48:	ff 
     c49:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     c4d:	c5 fd 7f 85 b0 d6 ff 	vmovdqa %ymm0,-0x2950(%rbp)
     c54:	ff 
     c55:	c5 fd 6f 85 50 d6 ff 	vmovdqa -0x29b0(%rbp),%ymm0
     c5c:	ff 
     c5d:	c5 fd 7f 85 10 ed ff 	vmovdqa %ymm0,-0x12f0(%rbp)
     c64:	ff 
     c65:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     c6c:	ff 
     c6d:	c5 fd 7f 85 30 ed ff 	vmovdqa %ymm0,-0x12d0(%rbp)
     c74:	ff 
     c75:	c5 fd 6f 85 10 ed ff 	vmovdqa -0x12f0(%rbp),%ymm0
     c7c:	ff 
     c7d:	c5 fd 6f 8d 30 ed ff 	vmovdqa -0x12d0(%rbp),%ymm1
     c84:	ff 
     c85:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     c89:	c5 fd 7f 85 d0 d6 ff 	vmovdqa %ymm0,-0x2930(%rbp)
     c90:	ff 
     c91:	c5 fd 6f 85 70 d6 ff 	vmovdqa -0x2990(%rbp),%ymm0
     c98:	ff 
     c99:	c5 fd 7f 85 d0 ec ff 	vmovdqa %ymm0,-0x1330(%rbp)
     ca0:	ff 
     ca1:	c5 fd 6f 85 30 d5 ff 	vmovdqa -0x2ad0(%rbp),%ymm0
     ca8:	ff 
     ca9:	c5 fd 7f 85 f0 ec ff 	vmovdqa %ymm0,-0x1310(%rbp)
     cb0:	ff 
     cb1:	c5 fd 6f 85 d0 ec ff 	vmovdqa -0x1330(%rbp),%ymm0
     cb8:	ff 
     cb9:	c5 fd 6f 8d f0 ec ff 	vmovdqa -0x1310(%rbp),%ymm1
     cc0:	ff 
     cc1:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     cc5:	c5 fd 7f 85 f0 d6 ff 	vmovdqa %ymm0,-0x2910(%rbp)
     ccc:	ff 
     ccd:	c5 fd 6f 85 10 d3 ff 	vmovdqa -0x2cf0(%rbp),%ymm0
     cd4:	ff 
     cd5:	c5 fd 7f 85 90 ec ff 	vmovdqa %ymm0,-0x1370(%rbp)
     cdc:	ff 
     cdd:	c5 fd 6f 85 f0 d6 ff 	vmovdqa -0x2910(%rbp),%ymm0
     ce4:	ff 
     ce5:	c5 fd 7f 85 b0 ec ff 	vmovdqa %ymm0,-0x1350(%rbp)
     cec:	ff 
     ced:	c5 fd 6f 8d 90 ec ff 	vmovdqa -0x1370(%rbp),%ymm1
     cf4:	ff 
     cf5:	c5 fd 6f 85 b0 ec ff 	vmovdqa -0x1350(%rbp),%ymm0
     cfc:	ff 
     cfd:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     d01:	c5 fd 7f 85 10 d7 ff 	vmovdqa %ymm0,-0x28f0(%rbp)
     d08:	ff 
     d09:	c5 fd 6f 85 30 d3 ff 	vmovdqa -0x2cd0(%rbp),%ymm0
     d10:	ff 
     d11:	c5 fd 7f 85 50 ec ff 	vmovdqa %ymm0,-0x13b0(%rbp)
     d18:	ff 
     d19:	c5 fd 6f 85 d0 d6 ff 	vmovdqa -0x2930(%rbp),%ymm0
     d20:	ff 
     d21:	c5 fd 7f 85 70 ec ff 	vmovdqa %ymm0,-0x1390(%rbp)
     d28:	ff 
     d29:	c5 fd 6f 8d 50 ec ff 	vmovdqa -0x13b0(%rbp),%ymm1
     d30:	ff 
     d31:	c5 fd 6f 85 70 ec ff 	vmovdqa -0x1390(%rbp),%ymm0
     d38:	ff 
     d39:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     d3d:	c5 fd 7f 85 30 d7 ff 	vmovdqa %ymm0,-0x28d0(%rbp)
     d44:	ff 
     d45:	c5 fd 6f 85 50 d3 ff 	vmovdqa -0x2cb0(%rbp),%ymm0
     d4c:	ff 
     d4d:	c5 fd 7f 85 10 ec ff 	vmovdqa %ymm0,-0x13f0(%rbp)
     d54:	ff 
     d55:	c5 fd 6f 85 b0 d6 ff 	vmovdqa -0x2950(%rbp),%ymm0
     d5c:	ff 
     d5d:	c5 fd 7f 85 30 ec ff 	vmovdqa %ymm0,-0x13d0(%rbp)
     d64:	ff 
     d65:	c5 fd 6f 8d 10 ec ff 	vmovdqa -0x13f0(%rbp),%ymm1
     d6c:	ff 
     d6d:	c5 fd 6f 85 30 ec ff 	vmovdqa -0x13d0(%rbp),%ymm0
     d74:	ff 
     d75:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     d79:	c5 fd 7f 85 50 d7 ff 	vmovdqa %ymm0,-0x28b0(%rbp)
     d80:	ff 
     d81:	c5 fd 6f 85 70 d3 ff 	vmovdqa -0x2c90(%rbp),%ymm0
     d88:	ff 
     d89:	c5 fd 7f 85 d0 eb ff 	vmovdqa %ymm0,-0x1430(%rbp)
     d90:	ff 
     d91:	c5 fd 6f 85 90 d6 ff 	vmovdqa -0x2970(%rbp),%ymm0
     d98:	ff 
     d99:	c5 fd 7f 85 f0 eb ff 	vmovdqa %ymm0,-0x1410(%rbp)
     da0:	ff 
     da1:	c5 fd 6f 8d d0 eb ff 	vmovdqa -0x1430(%rbp),%ymm1
     da8:	ff 
     da9:	c5 fd 6f 85 f0 eb ff 	vmovdqa -0x1410(%rbp),%ymm0
     db0:	ff 
     db1:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     db5:	c5 fd 7f 85 70 d7 ff 	vmovdqa %ymm0,-0x2890(%rbp)
     dbc:	ff 
     dbd:	c5 fd 6f 85 70 d3 ff 	vmovdqa -0x2c90(%rbp),%ymm0
     dc4:	ff 
     dc5:	c5 fd 7f 85 90 eb ff 	vmovdqa %ymm0,-0x1470(%rbp)
     dcc:	ff 
     dcd:	c5 fd 6f 85 90 d6 ff 	vmovdqa -0x2970(%rbp),%ymm0
     dd4:	ff 
     dd5:	c5 fd 7f 85 b0 eb ff 	vmovdqa %ymm0,-0x1450(%rbp)
     ddc:	ff 
     ddd:	c5 fd 6f 85 90 eb ff 	vmovdqa -0x1470(%rbp),%ymm0
     de4:	ff 
     de5:	c5 fd 6f 8d b0 eb ff 	vmovdqa -0x1450(%rbp),%ymm1
     dec:	ff 
     ded:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     df1:	c5 fd 7f 85 90 d7 ff 	vmovdqa %ymm0,-0x2870(%rbp)
     df8:	ff 
     df9:	c5 fd 6f 85 50 d3 ff 	vmovdqa -0x2cb0(%rbp),%ymm0
     e00:	ff 
     e01:	c5 fd 7f 85 50 eb ff 	vmovdqa %ymm0,-0x14b0(%rbp)
     e08:	ff 
     e09:	c5 fd 6f 85 b0 d6 ff 	vmovdqa -0x2950(%rbp),%ymm0
     e10:	ff 
     e11:	c5 fd 7f 85 70 eb ff 	vmovdqa %ymm0,-0x1490(%rbp)
     e18:	ff 
     e19:	c5 fd 6f 85 50 eb ff 	vmovdqa -0x14b0(%rbp),%ymm0
     e20:	ff 
     e21:	c5 fd 6f 8d 70 eb ff 	vmovdqa -0x1490(%rbp),%ymm1
     e28:	ff 
     e29:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     e2d:	c5 fd 7f 85 b0 d7 ff 	vmovdqa %ymm0,-0x2850(%rbp)
     e34:	ff 
     e35:	c5 fd 6f 85 30 d3 ff 	vmovdqa -0x2cd0(%rbp),%ymm0
     e3c:	ff 
     e3d:	c5 fd 7f 85 10 eb ff 	vmovdqa %ymm0,-0x14f0(%rbp)
     e44:	ff 
     e45:	c5 fd 6f 85 d0 d6 ff 	vmovdqa -0x2930(%rbp),%ymm0
     e4c:	ff 
     e4d:	c5 fd 7f 85 30 eb ff 	vmovdqa %ymm0,-0x14d0(%rbp)
     e54:	ff 
     e55:	c5 fd 6f 85 10 eb ff 	vmovdqa -0x14f0(%rbp),%ymm0
     e5c:	ff 
     e5d:	c5 fd 6f 8d 30 eb ff 	vmovdqa -0x14d0(%rbp),%ymm1
     e64:	ff 
     e65:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     e69:	c5 fd 7f 85 d0 d7 ff 	vmovdqa %ymm0,-0x2830(%rbp)
     e70:	ff 
     e71:	c5 fd 6f 85 10 d3 ff 	vmovdqa -0x2cf0(%rbp),%ymm0
     e78:	ff 
     e79:	c5 fd 7f 85 d0 ea ff 	vmovdqa %ymm0,-0x1530(%rbp)
     e80:	ff 
     e81:	c5 fd 6f 85 f0 d6 ff 	vmovdqa -0x2910(%rbp),%ymm0
     e88:	ff 
     e89:	c5 fd 7f 85 f0 ea ff 	vmovdqa %ymm0,-0x1510(%rbp)
     e90:	ff 
     e91:	c5 fd 6f 85 d0 ea ff 	vmovdqa -0x1530(%rbp),%ymm0
     e98:	ff 
     e99:	c5 fd 6f 8d f0 ea ff 	vmovdqa -0x1510(%rbp),%ymm1
     ea0:	ff 
     ea1:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     ea5:	c5 fd 7f 85 f0 d7 ff 	vmovdqa %ymm0,-0x2810(%rbp)
     eac:	ff 
     ead:	c5 fd 6f 85 10 d7 ff 	vmovdqa -0x28f0(%rbp),%ymm0
     eb4:	ff 
     eb5:	c5 fd 7f 85 90 ea ff 	vmovdqa %ymm0,-0x1570(%rbp)
     ebc:	ff 
     ebd:	c5 fd 6f 85 70 d7 ff 	vmovdqa -0x2890(%rbp),%ymm0
     ec4:	ff 
     ec5:	c5 fd 7f 85 b0 ea ff 	vmovdqa %ymm0,-0x1550(%rbp)
     ecc:	ff 
     ecd:	c5 fd 6f 8d 90 ea ff 	vmovdqa -0x1570(%rbp),%ymm1
     ed4:	ff 
     ed5:	c5 fd 6f 85 b0 ea ff 	vmovdqa -0x1550(%rbp),%ymm0
     edc:	ff 
     edd:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     ee1:	c5 fd 7f 85 10 d8 ff 	vmovdqa %ymm0,-0x27f0(%rbp)
     ee8:	ff 
     ee9:	c5 fd 6f 85 30 d7 ff 	vmovdqa -0x28d0(%rbp),%ymm0
     ef0:	ff 
     ef1:	c5 fd 7f 85 50 ea ff 	vmovdqa %ymm0,-0x15b0(%rbp)
     ef8:	ff 
     ef9:	c5 fd 6f 85 50 d7 ff 	vmovdqa -0x28b0(%rbp),%ymm0
     f00:	ff 
     f01:	c5 fd 7f 85 70 ea ff 	vmovdqa %ymm0,-0x1590(%rbp)
     f08:	ff 
     f09:	c5 fd 6f 8d 50 ea ff 	vmovdqa -0x15b0(%rbp),%ymm1
     f10:	ff 
     f11:	c5 fd 6f 85 70 ea ff 	vmovdqa -0x1590(%rbp),%ymm0
     f18:	ff 
     f19:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     f1d:	c5 fd 7f 85 30 d8 ff 	vmovdqa %ymm0,-0x27d0(%rbp)
     f24:	ff 
     f25:	c5 fd 6f 85 10 d7 ff 	vmovdqa -0x28f0(%rbp),%ymm0
     f2c:	ff 
     f2d:	c5 fd 7f 85 10 ea ff 	vmovdqa %ymm0,-0x15f0(%rbp)
     f34:	ff 
     f35:	c5 fd 6f 85 70 d7 ff 	vmovdqa -0x2890(%rbp),%ymm0
     f3c:	ff 
     f3d:	c5 fd 7f 85 30 ea ff 	vmovdqa %ymm0,-0x15d0(%rbp)
     f44:	ff 
     f45:	c5 fd 6f 85 10 ea ff 	vmovdqa -0x15f0(%rbp),%ymm0
     f4c:	ff 
     f4d:	c5 fd 6f 8d 30 ea ff 	vmovdqa -0x15d0(%rbp),%ymm1
     f54:	ff 
     f55:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     f59:	c5 fd 7f 85 50 d8 ff 	vmovdqa %ymm0,-0x27b0(%rbp)
     f60:	ff 
     f61:	c5 fd 6f 85 30 d7 ff 	vmovdqa -0x28d0(%rbp),%ymm0
     f68:	ff 
     f69:	c5 fd 7f 85 d0 e9 ff 	vmovdqa %ymm0,-0x1630(%rbp)
     f70:	ff 
     f71:	c5 fd 6f 85 50 d7 ff 	vmovdqa -0x28b0(%rbp),%ymm0
     f78:	ff 
     f79:	c5 fd 7f 85 f0 e9 ff 	vmovdqa %ymm0,-0x1610(%rbp)
     f80:	ff 
     f81:	c5 fd 6f 85 d0 e9 ff 	vmovdqa -0x1630(%rbp),%ymm0
     f88:	ff 
     f89:	c5 fd 6f 8d f0 e9 ff 	vmovdqa -0x1610(%rbp),%ymm1
     f90:	ff 
     f91:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
     f95:	c5 fd 7f 85 70 d8 ff 	vmovdqa %ymm0,-0x2790(%rbp)
     f9c:	ff 
     f9d:	c5 fd 6f 85 90 d7 ff 	vmovdqa -0x2870(%rbp),%ymm0
     fa4:	ff 
     fa5:	c5 fd 7f 85 90 e9 ff 	vmovdqa %ymm0,-0x1670(%rbp)
     fac:	ff 
     fad:	c5 fd 6f 85 90 d7 ff 	vmovdqa -0x2870(%rbp),%ymm0
     fb4:	ff 
     fb5:	c5 fd 7f 85 b0 e9 ff 	vmovdqa %ymm0,-0x1650(%rbp)
     fbc:	ff 
     fbd:	c5 fd 6f 8d 90 e9 ff 	vmovdqa -0x1670(%rbp),%ymm1
     fc4:	ff 
     fc5:	c5 fd 6f 85 b0 e9 ff 	vmovdqa -0x1650(%rbp),%ymm0
     fcc:	ff 
     fcd:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
     fd1:	c5 fd 7f 85 90 d8 ff 	vmovdqa %ymm0,-0x2770(%rbp)
     fd8:	ff 
     fd9:	c5 fd 6f 85 f0 d7 ff 	vmovdqa -0x2810(%rbp),%ymm0
     fe0:	ff 
     fe1:	c5 fd 7f 85 50 e9 ff 	vmovdqa %ymm0,-0x16b0(%rbp)
     fe8:	ff 
     fe9:	c5 fd 6f 85 f0 d7 ff 	vmovdqa -0x2810(%rbp),%ymm0
     ff0:	ff 
     ff1:	c5 fd 7f 85 70 e9 ff 	vmovdqa %ymm0,-0x1690(%rbp)
     ff8:	ff 
     ff9:	c5 fd 6f 8d 50 e9 ff 	vmovdqa -0x16b0(%rbp),%ymm1
    1000:	ff 
    1001:	c5 fd 6f 85 70 e9 ff 	vmovdqa -0x1690(%rbp),%ymm0
    1008:	ff 
    1009:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    100d:	c5 fd 7f 85 b0 d8 ff 	vmovdqa %ymm0,-0x2750(%rbp)
    1014:	ff 
    1015:	c5 fd 6f 85 90 d8 ff 	vmovdqa -0x2770(%rbp),%ymm0
    101c:	ff 
    101d:	c5 fd 7f 85 10 e9 ff 	vmovdqa %ymm0,-0x16f0(%rbp)
    1024:	ff 
    1025:	c5 fd 6f 85 10 d5 ff 	vmovdqa -0x2af0(%rbp),%ymm0
    102c:	ff 
    102d:	c5 fd 7f 85 30 e9 ff 	vmovdqa %ymm0,-0x16d0(%rbp)
    1034:	ff 
    1035:	c5 fd 6f 8d 10 e9 ff 	vmovdqa -0x16f0(%rbp),%ymm1
    103c:	ff 
    103d:	c5 fd 6f 85 30 e9 ff 	vmovdqa -0x16d0(%rbp),%ymm0
    1044:	ff 
    1045:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    104a:	c5 fd 7f 85 d0 e8 ff 	vmovdqa %ymm0,-0x1730(%rbp)
    1051:	ff 
    1052:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1059:	ff 
    105a:	c5 fd 7f 85 f0 e8 ff 	vmovdqa %ymm0,-0x1710(%rbp)
    1061:	ff 
    1062:	c5 fd 6f 8d d0 e8 ff 	vmovdqa -0x1730(%rbp),%ymm1
    1069:	ff 
    106a:	c5 fd 6f 85 f0 e8 ff 	vmovdqa -0x1710(%rbp),%ymm0
    1071:	ff 
    1072:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1076:	c5 fd 7f 85 b0 e8 ff 	vmovdqa %ymm0,-0x1750(%rbp)
    107d:	ff 
    107e:	c7 85 a4 d1 ff ff 10 	movl   $0x10,-0x2e5c(%rbp)
    1085:	00 00 00 
    1088:	c5 fd 6f 85 b0 e8 ff 	vmovdqa -0x1750(%rbp),%ymm0
    108f:	ff 
    1090:	8b 85 a4 d1 ff ff    	mov    -0x2e5c(%rbp),%eax
    1096:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    109d:	c5 fa 7e 9d 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm3
    10a4:	ff 
    10a5:	c5 fd e2 c3          	vpsrad %xmm3,%ymm0,%ymm0
    10a9:	c5 fd 7f 85 d0 d8 ff 	vmovdqa %ymm0,-0x2730(%rbp)
    10b0:	ff 
    10b1:	c5 fd 6f 85 b0 d8 ff 	vmovdqa -0x2750(%rbp),%ymm0
    10b8:	ff 
    10b9:	c5 fd 7f 85 70 e8 ff 	vmovdqa %ymm0,-0x1790(%rbp)
    10c0:	ff 
    10c1:	c5 fd 6f 85 10 d5 ff 	vmovdqa -0x2af0(%rbp),%ymm0
    10c8:	ff 
    10c9:	c5 fd 7f 85 90 e8 ff 	vmovdqa %ymm0,-0x1770(%rbp)
    10d0:	ff 
    10d1:	c5 fd 6f 8d 70 e8 ff 	vmovdqa -0x1790(%rbp),%ymm1
    10d8:	ff 
    10d9:	c5 fd 6f 85 90 e8 ff 	vmovdqa -0x1770(%rbp),%ymm0
    10e0:	ff 
    10e1:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    10e6:	c5 fd 7f 85 30 e8 ff 	vmovdqa %ymm0,-0x17d0(%rbp)
    10ed:	ff 
    10ee:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    10f5:	ff 
    10f6:	c5 fd 7f 85 50 e8 ff 	vmovdqa %ymm0,-0x17b0(%rbp)
    10fd:	ff 
    10fe:	c5 fd 6f 8d 30 e8 ff 	vmovdqa -0x17d0(%rbp),%ymm1
    1105:	ff 
    1106:	c5 fd 6f 85 50 e8 ff 	vmovdqa -0x17b0(%rbp),%ymm0
    110d:	ff 
    110e:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1112:	c5 fd 7f 85 10 e8 ff 	vmovdqa %ymm0,-0x17f0(%rbp)
    1119:	ff 
    111a:	c7 85 a0 d1 ff ff 10 	movl   $0x10,-0x2e60(%rbp)
    1121:	00 00 00 
    1124:	c5 fd 6f 85 10 e8 ff 	vmovdqa -0x17f0(%rbp),%ymm0
    112b:	ff 
    112c:	8b 85 a0 d1 ff ff    	mov    -0x2e60(%rbp),%eax
    1132:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    1139:	c5 fa 7e a5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm4
    1140:	ff 
    1141:	c5 fd e2 c4          	vpsrad %xmm4,%ymm0,%ymm0
    1145:	c5 fd 7f 85 f0 d8 ff 	vmovdqa %ymm0,-0x2710(%rbp)
    114c:	ff 
    114d:	c5 fd 6f 85 d0 d7 ff 	vmovdqa -0x2830(%rbp),%ymm0
    1154:	ff 
    1155:	c5 fd 7f 85 d0 e7 ff 	vmovdqa %ymm0,-0x1830(%rbp)
    115c:	ff 
    115d:	c5 fd 6f 85 b0 d7 ff 	vmovdqa -0x2850(%rbp),%ymm0
    1164:	ff 
    1165:	c5 fd 7f 85 f0 e7 ff 	vmovdqa %ymm0,-0x1810(%rbp)
    116c:	ff 
    116d:	c5 fd 6f 8d d0 e7 ff 	vmovdqa -0x1830(%rbp),%ymm1
    1174:	ff 
    1175:	c5 fd 6f 85 f0 e7 ff 	vmovdqa -0x1810(%rbp),%ymm0
    117c:	ff 
    117d:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1181:	c5 fd 7f 85 10 d9 ff 	vmovdqa %ymm0,-0x26f0(%rbp)
    1188:	ff 
    1189:	c5 fd 6f 85 d0 d7 ff 	vmovdqa -0x2830(%rbp),%ymm0
    1190:	ff 
    1191:	c5 fd 7f 85 90 e7 ff 	vmovdqa %ymm0,-0x1870(%rbp)
    1198:	ff 
    1199:	c5 fd 6f 85 b0 d7 ff 	vmovdqa -0x2850(%rbp),%ymm0
    11a0:	ff 
    11a1:	c5 fd 7f 85 b0 e7 ff 	vmovdqa %ymm0,-0x1850(%rbp)
    11a8:	ff 
    11a9:	c5 fd 6f 85 90 e7 ff 	vmovdqa -0x1870(%rbp),%ymm0
    11b0:	ff 
    11b1:	c5 fd 6f 8d b0 e7 ff 	vmovdqa -0x1850(%rbp),%ymm1
    11b8:	ff 
    11b9:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    11bd:	c5 fd 7f 85 30 d9 ff 	vmovdqa %ymm0,-0x26d0(%rbp)
    11c4:	ff 
    11c5:	c5 fd 6f 85 10 d8 ff 	vmovdqa -0x27f0(%rbp),%ymm0
    11cc:	ff 
    11cd:	c5 fd 7f 85 50 e7 ff 	vmovdqa %ymm0,-0x18b0(%rbp)
    11d4:	ff 
    11d5:	c5 fd 6f 85 30 d8 ff 	vmovdqa -0x27d0(%rbp),%ymm0
    11dc:	ff 
    11dd:	c5 fd 7f 85 70 e7 ff 	vmovdqa %ymm0,-0x1890(%rbp)
    11e4:	ff 
    11e5:	c5 fd 6f 8d 50 e7 ff 	vmovdqa -0x18b0(%rbp),%ymm1
    11ec:	ff 
    11ed:	c5 fd 6f 85 70 e7 ff 	vmovdqa -0x1890(%rbp),%ymm0
    11f4:	ff 
    11f5:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    11f9:	c5 fd 7f 85 50 d9 ff 	vmovdqa %ymm0,-0x26b0(%rbp)
    1200:	ff 
    1201:	c5 fd 6f 85 10 d8 ff 	vmovdqa -0x27f0(%rbp),%ymm0
    1208:	ff 
    1209:	c5 fd 7f 85 10 e7 ff 	vmovdqa %ymm0,-0x18f0(%rbp)
    1210:	ff 
    1211:	c5 fd 6f 85 30 d8 ff 	vmovdqa -0x27d0(%rbp),%ymm0
    1218:	ff 
    1219:	c5 fd 7f 85 30 e7 ff 	vmovdqa %ymm0,-0x18d0(%rbp)
    1220:	ff 
    1221:	c5 fd 6f 85 10 e7 ff 	vmovdqa -0x18f0(%rbp),%ymm0
    1228:	ff 
    1229:	c5 fd 6f 8d 30 e7 ff 	vmovdqa -0x18d0(%rbp),%ymm1
    1230:	ff 
    1231:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1235:	c5 fd 7f 85 70 d9 ff 	vmovdqa %ymm0,-0x2690(%rbp)
    123c:	ff 
    123d:	c5 fd 6f 85 50 d8 ff 	vmovdqa -0x27b0(%rbp),%ymm0
    1244:	ff 
    1245:	c5 fd 7f 85 d0 e6 ff 	vmovdqa %ymm0,-0x1930(%rbp)
    124c:	ff 
    124d:	c5 fd 6f 85 50 d8 ff 	vmovdqa -0x27b0(%rbp),%ymm0
    1254:	ff 
    1255:	c5 fd 7f 85 f0 e6 ff 	vmovdqa %ymm0,-0x1910(%rbp)
    125c:	ff 
    125d:	c5 fd 6f 8d d0 e6 ff 	vmovdqa -0x1930(%rbp),%ymm1
    1264:	ff 
    1265:	c5 fd 6f 85 f0 e6 ff 	vmovdqa -0x1910(%rbp),%ymm0
    126c:	ff 
    126d:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1271:	c5 fd 7f 85 90 d9 ff 	vmovdqa %ymm0,-0x2670(%rbp)
    1278:	ff 
    1279:	c5 fd 6f 85 70 d8 ff 	vmovdqa -0x2790(%rbp),%ymm0
    1280:	ff 
    1281:	c5 fd 7f 85 90 e6 ff 	vmovdqa %ymm0,-0x1970(%rbp)
    1288:	ff 
    1289:	c5 fd 6f 85 70 d8 ff 	vmovdqa -0x2790(%rbp),%ymm0
    1290:	ff 
    1291:	c5 fd 7f 85 b0 e6 ff 	vmovdqa %ymm0,-0x1950(%rbp)
    1298:	ff 
    1299:	c5 fd 6f 8d 90 e6 ff 	vmovdqa -0x1970(%rbp),%ymm1
    12a0:	ff 
    12a1:	c5 fd 6f 85 b0 e6 ff 	vmovdqa -0x1950(%rbp),%ymm0
    12a8:	ff 
    12a9:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    12ad:	c5 fd 7f 85 b0 d9 ff 	vmovdqa %ymm0,-0x2650(%rbp)
    12b4:	ff 
    12b5:	c5 fd 6f 85 d0 d4 ff 	vmovdqa -0x2b30(%rbp),%ymm0
    12bc:	ff 
    12bd:	c5 fd 7f 85 50 e6 ff 	vmovdqa %ymm0,-0x19b0(%rbp)
    12c4:	ff 
    12c5:	c5 fd 6f 85 90 d9 ff 	vmovdqa -0x2670(%rbp),%ymm0
    12cc:	ff 
    12cd:	c5 fd 7f 85 70 e6 ff 	vmovdqa %ymm0,-0x1990(%rbp)
    12d4:	ff 
    12d5:	c5 fd 6f 8d 50 e6 ff 	vmovdqa -0x19b0(%rbp),%ymm1
    12dc:	ff 
    12dd:	c5 fd 6f 85 70 e6 ff 	vmovdqa -0x1990(%rbp),%ymm0
    12e4:	ff 
    12e5:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    12ea:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    12ee:	c5 fd 6f 85 f0 d4 ff 	vmovdqa -0x2b10(%rbp),%ymm0
    12f5:	ff 
    12f6:	c5 fd 7f 85 10 e6 ff 	vmovdqa %ymm0,-0x19f0(%rbp)
    12fd:	ff 
    12fe:	c5 fd 6f 85 b0 d9 ff 	vmovdqa -0x2650(%rbp),%ymm0
    1305:	ff 
    1306:	c5 fd 7f 85 30 e6 ff 	vmovdqa %ymm0,-0x19d0(%rbp)
    130d:	ff 
    130e:	c5 fd 6f 8d 10 e6 ff 	vmovdqa -0x19f0(%rbp),%ymm1
    1315:	ff 
    1316:	c5 fd 6f 85 30 e6 ff 	vmovdqa -0x19d0(%rbp),%ymm0
    131d:	ff 
    131e:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1323:	c5 fd 7f 85 d0 e5 ff 	vmovdqa %ymm0,-0x1a30(%rbp)
    132a:	ff 
    132b:	c5 fd 7f 95 f0 e5 ff 	vmovdqa %ymm2,-0x1a10(%rbp)
    1332:	ff 
    1333:	c5 fd 6f 8d d0 e5 ff 	vmovdqa -0x1a30(%rbp),%ymm1
    133a:	ff 
    133b:	c5 fd 6f 85 f0 e5 ff 	vmovdqa -0x1a10(%rbp),%ymm0
    1342:	ff 
    1343:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1347:	c5 fd 7f 85 90 e5 ff 	vmovdqa %ymm0,-0x1a70(%rbp)
    134e:	ff 
    134f:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1356:	ff 
    1357:	c5 fd 7f 85 b0 e5 ff 	vmovdqa %ymm0,-0x1a50(%rbp)
    135e:	ff 
    135f:	c5 fd 6f 8d 90 e5 ff 	vmovdqa -0x1a70(%rbp),%ymm1
    1366:	ff 
    1367:	c5 fd 6f 85 b0 e5 ff 	vmovdqa -0x1a50(%rbp),%ymm0
    136e:	ff 
    136f:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1373:	c5 fd 7f 85 70 e5 ff 	vmovdqa %ymm0,-0x1a90(%rbp)
    137a:	ff 
    137b:	c7 85 9c d1 ff ff 10 	movl   $0x10,-0x2e64(%rbp)
    1382:	00 00 00 
    1385:	c5 fd 6f 85 70 e5 ff 	vmovdqa -0x1a90(%rbp),%ymm0
    138c:	ff 
    138d:	8b 85 9c d1 ff ff    	mov    -0x2e64(%rbp),%eax
    1393:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    139a:	c5 fa 7e ad 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm5
    13a1:	ff 
    13a2:	c5 fd e2 c5          	vpsrad %xmm5,%ymm0,%ymm0
    13a6:	c5 fd 7f 85 d0 d9 ff 	vmovdqa %ymm0,-0x2630(%rbp)
    13ad:	ff 
    13ae:	c5 fd 6f 85 d0 d4 ff 	vmovdqa -0x2b30(%rbp),%ymm0
    13b5:	ff 
    13b6:	c5 fd 7f 85 30 e5 ff 	vmovdqa %ymm0,-0x1ad0(%rbp)
    13bd:	ff 
    13be:	c5 fd 6f 85 b0 d9 ff 	vmovdqa -0x2650(%rbp),%ymm0
    13c5:	ff 
    13c6:	c5 fd 7f 85 50 e5 ff 	vmovdqa %ymm0,-0x1ab0(%rbp)
    13cd:	ff 
    13ce:	c5 fd 6f 8d 30 e5 ff 	vmovdqa -0x1ad0(%rbp),%ymm1
    13d5:	ff 
    13d6:	c5 fd 6f 85 50 e5 ff 	vmovdqa -0x1ab0(%rbp),%ymm0
    13dd:	ff 
    13de:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    13e3:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    13e7:	c5 fd 6f 85 f0 d4 ff 	vmovdqa -0x2b10(%rbp),%ymm0
    13ee:	ff 
    13ef:	c5 fd 7f 85 f0 e4 ff 	vmovdqa %ymm0,-0x1b10(%rbp)
    13f6:	ff 
    13f7:	c5 fd 6f 85 90 d9 ff 	vmovdqa -0x2670(%rbp),%ymm0
    13fe:	ff 
    13ff:	c5 fd 7f 85 10 e5 ff 	vmovdqa %ymm0,-0x1af0(%rbp)
    1406:	ff 
    1407:	c5 fd 6f 8d f0 e4 ff 	vmovdqa -0x1b10(%rbp),%ymm1
    140e:	ff 
    140f:	c5 fd 6f 85 10 e5 ff 	vmovdqa -0x1af0(%rbp),%ymm0
    1416:	ff 
    1417:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    141c:	c5 fd 7f 85 b0 e4 ff 	vmovdqa %ymm0,-0x1b50(%rbp)
    1423:	ff 
    1424:	c5 fd 7f 95 d0 e4 ff 	vmovdqa %ymm2,-0x1b30(%rbp)
    142b:	ff 
    142c:	c5 fd 6f 85 b0 e4 ff 	vmovdqa -0x1b50(%rbp),%ymm0
    1433:	ff 
    1434:	c5 fd 6f 8d d0 e4 ff 	vmovdqa -0x1b30(%rbp),%ymm1
    143b:	ff 
    143c:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1440:	c5 fd 7f 85 70 e4 ff 	vmovdqa %ymm0,-0x1b90(%rbp)
    1447:	ff 
    1448:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    144f:	ff 
    1450:	c5 fd 7f 85 90 e4 ff 	vmovdqa %ymm0,-0x1b70(%rbp)
    1457:	ff 
    1458:	c5 fd 6f 8d 70 e4 ff 	vmovdqa -0x1b90(%rbp),%ymm1
    145f:	ff 
    1460:	c5 fd 6f 85 90 e4 ff 	vmovdqa -0x1b70(%rbp),%ymm0
    1467:	ff 
    1468:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    146c:	c5 fd 7f 85 50 e4 ff 	vmovdqa %ymm0,-0x1bb0(%rbp)
    1473:	ff 
    1474:	c7 85 98 d1 ff ff 10 	movl   $0x10,-0x2e68(%rbp)
    147b:	00 00 00 
    147e:	c5 fd 6f 85 50 e4 ff 	vmovdqa -0x1bb0(%rbp),%ymm0
    1485:	ff 
    1486:	8b 85 98 d1 ff ff    	mov    -0x2e68(%rbp),%eax
    148c:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    1493:	c5 fa 7e b5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm6
    149a:	ff 
    149b:	c5 fd e2 c6          	vpsrad %xmm6,%ymm0,%ymm0
    149f:	c5 fd 7f 85 f0 d9 ff 	vmovdqa %ymm0,-0x2610(%rbp)
    14a6:	ff 
    14a7:	c5 fd 6f 85 d0 d8 ff 	vmovdqa -0x2730(%rbp),%ymm0
    14ae:	ff 
    14af:	c5 fd 7f 85 10 e4 ff 	vmovdqa %ymm0,-0x1bf0(%rbp)
    14b6:	ff 
    14b7:	c5 fd 6f 85 30 d9 ff 	vmovdqa -0x26d0(%rbp),%ymm0
    14be:	ff 
    14bf:	c5 fd 7f 85 30 e4 ff 	vmovdqa %ymm0,-0x1bd0(%rbp)
    14c6:	ff 
    14c7:	c5 fd 6f 8d 10 e4 ff 	vmovdqa -0x1bf0(%rbp),%ymm1
    14ce:	ff 
    14cf:	c5 fd 6f 85 30 e4 ff 	vmovdqa -0x1bd0(%rbp),%ymm0
    14d6:	ff 
    14d7:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    14db:	c5 fd 7f 85 10 da ff 	vmovdqa %ymm0,-0x25f0(%rbp)
    14e2:	ff 
    14e3:	c5 fd 6f 85 f0 d8 ff 	vmovdqa -0x2710(%rbp),%ymm0
    14ea:	ff 
    14eb:	c5 fd 7f 85 d0 e3 ff 	vmovdqa %ymm0,-0x1c30(%rbp)
    14f2:	ff 
    14f3:	c5 fd 6f 85 10 d9 ff 	vmovdqa -0x26f0(%rbp),%ymm0
    14fa:	ff 
    14fb:	c5 fd 7f 85 f0 e3 ff 	vmovdqa %ymm0,-0x1c10(%rbp)
    1502:	ff 
    1503:	c5 fd 6f 8d d0 e3 ff 	vmovdqa -0x1c30(%rbp),%ymm1
    150a:	ff 
    150b:	c5 fd 6f 85 f0 e3 ff 	vmovdqa -0x1c10(%rbp),%ymm0
    1512:	ff 
    1513:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1517:	c5 fd 7f 85 30 da ff 	vmovdqa %ymm0,-0x25d0(%rbp)
    151e:	ff 
    151f:	c5 fd 6f 85 d0 d8 ff 	vmovdqa -0x2730(%rbp),%ymm0
    1526:	ff 
    1527:	c5 fd 7f 85 90 e3 ff 	vmovdqa %ymm0,-0x1c70(%rbp)
    152e:	ff 
    152f:	c5 fd 6f 85 30 d9 ff 	vmovdqa -0x26d0(%rbp),%ymm0
    1536:	ff 
    1537:	c5 fd 7f 85 b0 e3 ff 	vmovdqa %ymm0,-0x1c50(%rbp)
    153e:	ff 
    153f:	c5 fd 6f 85 90 e3 ff 	vmovdqa -0x1c70(%rbp),%ymm0
    1546:	ff 
    1547:	c5 fd 6f 8d b0 e3 ff 	vmovdqa -0x1c50(%rbp),%ymm1
    154e:	ff 
    154f:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1553:	c5 fd 7f 85 50 da ff 	vmovdqa %ymm0,-0x25b0(%rbp)
    155a:	ff 
    155b:	c5 fd 6f 85 f0 d8 ff 	vmovdqa -0x2710(%rbp),%ymm0
    1562:	ff 
    1563:	c5 fd 7f 85 50 e3 ff 	vmovdqa %ymm0,-0x1cb0(%rbp)
    156a:	ff 
    156b:	c5 fd 6f 85 10 d9 ff 	vmovdqa -0x26f0(%rbp),%ymm0
    1572:	ff 
    1573:	c5 fd 7f 85 70 e3 ff 	vmovdqa %ymm0,-0x1c90(%rbp)
    157a:	ff 
    157b:	c5 fd 6f 85 50 e3 ff 	vmovdqa -0x1cb0(%rbp),%ymm0
    1582:	ff 
    1583:	c5 fd 6f 8d 70 e3 ff 	vmovdqa -0x1c90(%rbp),%ymm1
    158a:	ff 
    158b:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    158f:	c5 fd 7f 85 70 da ff 	vmovdqa %ymm0,-0x2590(%rbp)
    1596:	ff 
    1597:	c5 fd 6f 85 50 d9 ff 	vmovdqa -0x26b0(%rbp),%ymm0
    159e:	ff 
    159f:	c5 fd 7f 85 d0 fc ff 	vmovdqa %ymm0,-0x330(%rbp)
    15a6:	ff 
    15a7:	c5 fd 6f 85 70 d9 ff 	vmovdqa -0x2690(%rbp),%ymm0
    15ae:	ff 
    15af:	c5 fd 7f 85 50 fd ff 	vmovdqa %ymm0,-0x2b0(%rbp)
    15b6:	ff 
    15b7:	c5 fd 6f 85 d0 d9 ff 	vmovdqa -0x2630(%rbp),%ymm0
    15be:	ff 
    15bf:	c5 fd 7f 85 10 fd ff 	vmovdqa %ymm0,-0x2f0(%rbp)
    15c6:	ff 
    15c7:	c5 fd 6f 85 f0 d9 ff 	vmovdqa -0x2610(%rbp),%ymm0
    15ce:	ff 
    15cf:	c5 fd 7f 85 90 fd ff 	vmovdqa %ymm0,-0x270(%rbp)
    15d6:	ff 
    15d7:	c5 fd 6f 85 50 d4 ff 	vmovdqa -0x2bb0(%rbp),%ymm0
    15de:	ff 
    15df:	c5 fd 7f 85 10 e3 ff 	vmovdqa %ymm0,-0x1cf0(%rbp)
    15e6:	ff 
    15e7:	c5 fd 6f 85 30 da ff 	vmovdqa -0x25d0(%rbp),%ymm0
    15ee:	ff 
    15ef:	c5 fd 7f 85 30 e3 ff 	vmovdqa %ymm0,-0x1cd0(%rbp)
    15f6:	ff 
    15f7:	c5 fd 6f 8d 10 e3 ff 	vmovdqa -0x1cf0(%rbp),%ymm1
    15fe:	ff 
    15ff:	c5 fd 6f 85 30 e3 ff 	vmovdqa -0x1cd0(%rbp),%ymm0
    1606:	ff 
    1607:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    160c:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    1610:	c5 fd 6f 85 b0 d4 ff 	vmovdqa -0x2b50(%rbp),%ymm0
    1617:	ff 
    1618:	c5 fd 7f 85 d0 e2 ff 	vmovdqa %ymm0,-0x1d30(%rbp)
    161f:	ff 
    1620:	c5 fd 6f 85 10 da ff 	vmovdqa -0x25f0(%rbp),%ymm0
    1627:	ff 
    1628:	c5 fd 7f 85 f0 e2 ff 	vmovdqa %ymm0,-0x1d10(%rbp)
    162f:	ff 
    1630:	c5 fd 6f 8d d0 e2 ff 	vmovdqa -0x1d30(%rbp),%ymm1
    1637:	ff 
    1638:	c5 fd 6f 85 f0 e2 ff 	vmovdqa -0x1d10(%rbp),%ymm0
    163f:	ff 
    1640:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1645:	c5 fd 7f 85 90 e2 ff 	vmovdqa %ymm0,-0x1d70(%rbp)
    164c:	ff 
    164d:	c5 fd 7f 95 b0 e2 ff 	vmovdqa %ymm2,-0x1d50(%rbp)
    1654:	ff 
    1655:	c5 fd 6f 8d 90 e2 ff 	vmovdqa -0x1d70(%rbp),%ymm1
    165c:	ff 
    165d:	c5 fd 6f 85 b0 e2 ff 	vmovdqa -0x1d50(%rbp),%ymm0
    1664:	ff 
    1665:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1669:	c5 fd 7f 85 50 e2 ff 	vmovdqa %ymm0,-0x1db0(%rbp)
    1670:	ff 
    1671:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1678:	ff 
    1679:	c5 fd 7f 85 70 e2 ff 	vmovdqa %ymm0,-0x1d90(%rbp)
    1680:	ff 
    1681:	c5 fd 6f 8d 50 e2 ff 	vmovdqa -0x1db0(%rbp),%ymm1
    1688:	ff 
    1689:	c5 fd 6f 85 70 e2 ff 	vmovdqa -0x1d90(%rbp),%ymm0
    1690:	ff 
    1691:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1695:	c5 fd 7f 85 30 e2 ff 	vmovdqa %ymm0,-0x1dd0(%rbp)
    169c:	ff 
    169d:	c7 85 94 d1 ff ff 10 	movl   $0x10,-0x2e6c(%rbp)
    16a4:	00 00 00 
    16a7:	c5 fd 6f 85 30 e2 ff 	vmovdqa -0x1dd0(%rbp),%ymm0
    16ae:	ff 
    16af:	8b 85 94 d1 ff ff    	mov    -0x2e6c(%rbp),%eax
    16b5:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    16bc:	c5 fa 7e bd 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm7
    16c3:	ff 
    16c4:	c5 fd e2 c7          	vpsrad %xmm7,%ymm0,%ymm0
    16c8:	c5 fd 7f 85 f0 fc ff 	vmovdqa %ymm0,-0x310(%rbp)
    16cf:	ff 
    16d0:	c5 fd 6f 85 50 d4 ff 	vmovdqa -0x2bb0(%rbp),%ymm0
    16d7:	ff 
    16d8:	c5 fd 7f 85 f0 e1 ff 	vmovdqa %ymm0,-0x1e10(%rbp)
    16df:	ff 
    16e0:	c5 fd 6f 85 10 da ff 	vmovdqa -0x25f0(%rbp),%ymm0
    16e7:	ff 
    16e8:	c5 fd 7f 85 10 e2 ff 	vmovdqa %ymm0,-0x1df0(%rbp)
    16ef:	ff 
    16f0:	c5 fd 6f 8d f0 e1 ff 	vmovdqa -0x1e10(%rbp),%ymm1
    16f7:	ff 
    16f8:	c5 fd 6f 85 10 e2 ff 	vmovdqa -0x1df0(%rbp),%ymm0
    16ff:	ff 
    1700:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1705:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    1709:	c5 fd 6f 85 b0 d4 ff 	vmovdqa -0x2b50(%rbp),%ymm0
    1710:	ff 
    1711:	c5 fd 7f 85 b0 e1 ff 	vmovdqa %ymm0,-0x1e50(%rbp)
    1718:	ff 
    1719:	c5 fd 6f 85 30 da ff 	vmovdqa -0x25d0(%rbp),%ymm0
    1720:	ff 
    1721:	c5 fd 7f 85 d0 e1 ff 	vmovdqa %ymm0,-0x1e30(%rbp)
    1728:	ff 
    1729:	c5 fd 6f 8d b0 e1 ff 	vmovdqa -0x1e50(%rbp),%ymm1
    1730:	ff 
    1731:	c5 fd 6f 85 d0 e1 ff 	vmovdqa -0x1e30(%rbp),%ymm0
    1738:	ff 
    1739:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    173e:	c5 fd 7f 85 70 e1 ff 	vmovdqa %ymm0,-0x1e90(%rbp)
    1745:	ff 
    1746:	c5 fd 7f 95 90 e1 ff 	vmovdqa %ymm2,-0x1e70(%rbp)
    174d:	ff 
    174e:	c5 fd 6f 85 70 e1 ff 	vmovdqa -0x1e90(%rbp),%ymm0
    1755:	ff 
    1756:	c5 fd 6f 8d 90 e1 ff 	vmovdqa -0x1e70(%rbp),%ymm1
    175d:	ff 
    175e:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1762:	c5 fd 7f 85 30 e1 ff 	vmovdqa %ymm0,-0x1ed0(%rbp)
    1769:	ff 
    176a:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1771:	ff 
    1772:	c5 fd 7f 85 50 e1 ff 	vmovdqa %ymm0,-0x1eb0(%rbp)
    1779:	ff 
    177a:	c5 fd 6f 8d 30 e1 ff 	vmovdqa -0x1ed0(%rbp),%ymm1
    1781:	ff 
    1782:	c5 fd 6f 85 50 e1 ff 	vmovdqa -0x1eb0(%rbp),%ymm0
    1789:	ff 
    178a:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    178e:	c5 fd 7f 85 10 e1 ff 	vmovdqa %ymm0,-0x1ef0(%rbp)
    1795:	ff 
    1796:	c7 85 90 d1 ff ff 10 	movl   $0x10,-0x2e70(%rbp)
    179d:	00 00 00 
    17a0:	c5 fd 6f 85 10 e1 ff 	vmovdqa -0x1ef0(%rbp),%ymm0
    17a7:	ff 
    17a8:	8b 85 90 d1 ff ff    	mov    -0x2e70(%rbp),%eax
    17ae:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    17b5:	c5 fa 7e 9d 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm3
    17bc:	ff 
    17bd:	c5 fd e2 c3          	vpsrad %xmm3,%ymm0,%ymm0
    17c1:	c5 fd 7f 85 b0 fd ff 	vmovdqa %ymm0,-0x250(%rbp)
    17c8:	ff 
    17c9:	c5 fd 6f 85 90 d4 ff 	vmovdqa -0x2b70(%rbp),%ymm0
    17d0:	ff 
    17d1:	c5 fd 7f 85 d0 e0 ff 	vmovdqa %ymm0,-0x1f30(%rbp)
    17d8:	ff 
    17d9:	c5 fd 6f 85 70 da ff 	vmovdqa -0x2590(%rbp),%ymm0
    17e0:	ff 
    17e1:	c5 fd 7f 85 f0 e0 ff 	vmovdqa %ymm0,-0x1f10(%rbp)
    17e8:	ff 
    17e9:	c5 fd 6f 8d d0 e0 ff 	vmovdqa -0x1f30(%rbp),%ymm1
    17f0:	ff 
    17f1:	c5 fd 6f 85 f0 e0 ff 	vmovdqa -0x1f10(%rbp),%ymm0
    17f8:	ff 
    17f9:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    17fe:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    1802:	c5 fd 6f 85 70 d4 ff 	vmovdqa -0x2b90(%rbp),%ymm0
    1809:	ff 
    180a:	c5 fd 7f 85 90 e0 ff 	vmovdqa %ymm0,-0x1f70(%rbp)
    1811:	ff 
    1812:	c5 fd 6f 85 50 da ff 	vmovdqa -0x25b0(%rbp),%ymm0
    1819:	ff 
    181a:	c5 fd 7f 85 b0 e0 ff 	vmovdqa %ymm0,-0x1f50(%rbp)
    1821:	ff 
    1822:	c5 fd 6f 8d 90 e0 ff 	vmovdqa -0x1f70(%rbp),%ymm1
    1829:	ff 
    182a:	c5 fd 6f 85 b0 e0 ff 	vmovdqa -0x1f50(%rbp),%ymm0
    1831:	ff 
    1832:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1837:	c5 fd 7f 85 50 e0 ff 	vmovdqa %ymm0,-0x1fb0(%rbp)
    183e:	ff 
    183f:	c5 fd 7f 95 70 e0 ff 	vmovdqa %ymm2,-0x1f90(%rbp)
    1846:	ff 
    1847:	c5 fd 6f 8d 50 e0 ff 	vmovdqa -0x1fb0(%rbp),%ymm1
    184e:	ff 
    184f:	c5 fd 6f 85 70 e0 ff 	vmovdqa -0x1f90(%rbp),%ymm0
    1856:	ff 
    1857:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    185b:	c5 fd 7f 85 10 e0 ff 	vmovdqa %ymm0,-0x1ff0(%rbp)
    1862:	ff 
    1863:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    186a:	ff 
    186b:	c5 fd 7f 85 30 e0 ff 	vmovdqa %ymm0,-0x1fd0(%rbp)
    1872:	ff 
    1873:	c5 fd 6f 8d 10 e0 ff 	vmovdqa -0x1ff0(%rbp),%ymm1
    187a:	ff 
    187b:	c5 fd 6f 85 30 e0 ff 	vmovdqa -0x1fd0(%rbp),%ymm0
    1882:	ff 
    1883:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1887:	c5 fd 7f 85 f0 df ff 	vmovdqa %ymm0,-0x2010(%rbp)
    188e:	ff 
    188f:	c7 85 8c d1 ff ff 10 	movl   $0x10,-0x2e74(%rbp)
    1896:	00 00 00 
    1899:	c5 fd 6f 85 f0 df ff 	vmovdqa -0x2010(%rbp),%ymm0
    18a0:	ff 
    18a1:	8b 85 8c d1 ff ff    	mov    -0x2e74(%rbp),%eax
    18a7:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    18ae:	c5 fa 7e a5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm4
    18b5:	ff 
    18b6:	c5 fd e2 c4          	vpsrad %xmm4,%ymm0,%ymm0
    18ba:	c5 fd 7f 85 70 fd ff 	vmovdqa %ymm0,-0x290(%rbp)
    18c1:	ff 
    18c2:	c5 fd 6f 85 90 d4 ff 	vmovdqa -0x2b70(%rbp),%ymm0
    18c9:	ff 
    18ca:	c5 fd 7f 85 b0 df ff 	vmovdqa %ymm0,-0x2050(%rbp)
    18d1:	ff 
    18d2:	c5 fd 6f 85 50 da ff 	vmovdqa -0x25b0(%rbp),%ymm0
    18d9:	ff 
    18da:	c5 fd 7f 85 d0 df ff 	vmovdqa %ymm0,-0x2030(%rbp)
    18e1:	ff 
    18e2:	c5 fd 6f 8d b0 df ff 	vmovdqa -0x2050(%rbp),%ymm1
    18e9:	ff 
    18ea:	c5 fd 6f 85 d0 df ff 	vmovdqa -0x2030(%rbp),%ymm0
    18f1:	ff 
    18f2:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    18f7:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    18fb:	c5 fd 6f 85 70 d4 ff 	vmovdqa -0x2b90(%rbp),%ymm0
    1902:	ff 
    1903:	c5 fd 7f 85 70 df ff 	vmovdqa %ymm0,-0x2090(%rbp)
    190a:	ff 
    190b:	c5 fd 6f 85 70 da ff 	vmovdqa -0x2590(%rbp),%ymm0
    1912:	ff 
    1913:	c5 fd 7f 85 90 df ff 	vmovdqa %ymm0,-0x2070(%rbp)
    191a:	ff 
    191b:	c5 fd 6f 8d 70 df ff 	vmovdqa -0x2090(%rbp),%ymm1
    1922:	ff 
    1923:	c5 fd 6f 85 90 df ff 	vmovdqa -0x2070(%rbp),%ymm0
    192a:	ff 
    192b:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1930:	c5 fd 7f 85 30 df ff 	vmovdqa %ymm0,-0x20d0(%rbp)
    1937:	ff 
    1938:	c5 fd 7f 95 50 df ff 	vmovdqa %ymm2,-0x20b0(%rbp)
    193f:	ff 
    1940:	c5 fd 6f 85 30 df ff 	vmovdqa -0x20d0(%rbp),%ymm0
    1947:	ff 
    1948:	c5 fd 6f 8d 50 df ff 	vmovdqa -0x20b0(%rbp),%ymm1
    194f:	ff 
    1950:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1954:	c5 fd 7f 85 f0 de ff 	vmovdqa %ymm0,-0x2110(%rbp)
    195b:	ff 
    195c:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1963:	ff 
    1964:	c5 fd 7f 85 10 df ff 	vmovdqa %ymm0,-0x20f0(%rbp)
    196b:	ff 
    196c:	c5 fd 6f 8d f0 de ff 	vmovdqa -0x2110(%rbp),%ymm1
    1973:	ff 
    1974:	c5 fd 6f 85 10 df ff 	vmovdqa -0x20f0(%rbp),%ymm0
    197b:	ff 
    197c:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1980:	c5 fd 7f 85 d0 de ff 	vmovdqa %ymm0,-0x2130(%rbp)
    1987:	ff 
    1988:	c7 85 88 d1 ff ff 10 	movl   $0x10,-0x2e78(%rbp)
    198f:	00 00 00 
    1992:	c5 fd 6f 85 d0 de ff 	vmovdqa -0x2130(%rbp),%ymm0
    1999:	ff 
    199a:	8b 85 88 d1 ff ff    	mov    -0x2e78(%rbp),%eax
    19a0:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    19a7:	c5 fa 7e ad 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm5
    19ae:	ff 
    19af:	c5 fd e2 c5          	vpsrad %xmm5,%ymm0,%ymm0
    19b3:	c5 fd 7f 85 30 fd ff 	vmovdqa %ymm0,-0x2d0(%rbp)
    19ba:	ff 
    19bb:	c7 85 78 d1 ff ff 00 	movl   $0x0,-0x2e88(%rbp)
    19c2:	00 00 00 
    19c5:	eb 71                	jmp    1a38 <F_0_765+0x1ba>
    19c7:	c7 85 7c d1 ff ff 00 	movl   $0x0,-0x2e84(%rbp)
    19ce:	00 00 00 
    19d1:	eb 55                	jmp    1a28 <F_0_765+0x1aa>
    19d3:	8b 85 78 d1 ff ff    	mov    -0x2e88(%rbp),%eax
    19d9:	48 98                	cltq   
    19db:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
    19e2:	00 
    19e3:	8b 85 7c d1 ff ff    	mov    -0x2e84(%rbp),%eax
    19e9:	48 98                	cltq   
    19eb:	48 01 d0             	add    %rdx,%rax
    19ee:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx
    19f5:	00 
    19f6:	48 8d 85 d0 fc ff ff 	lea    -0x330(%rbp),%rax
    19fd:	48 01 d0             	add    %rdx,%rax
    1a00:	8b 95 7c d1 ff ff    	mov    -0x2e84(%rbp),%edx
    1a06:	8d 0c d5 00 00 00 00 	lea    0x0(,%rdx,8),%ecx
    1a0d:	8b 95 78 d1 ff ff    	mov    -0x2e88(%rbp),%edx
    1a13:	01 d1                	add    %edx,%ecx
    1a15:	8b 10                	mov    (%rax),%edx
    1a17:	48 63 c1             	movslq %ecx,%rax
    1a1a:	89 94 85 e0 fe ff ff 	mov    %edx,-0x120(%rbp,%rax,4)
    1a21:	83 85 7c d1 ff ff 01 	addl   $0x1,-0x2e84(%rbp)
    1a28:	83 bd 7c d1 ff ff 07 	cmpl   $0x7,-0x2e84(%rbp)
    1a2f:	7e a2                	jle    19d3 <F_0_765+0x155>
    1a31:	83 85 78 d1 ff ff 01 	addl   $0x1,-0x2e88(%rbp)
    1a38:	83 bd 78 d1 ff ff 07 	cmpl   $0x7,-0x2e88(%rbp)
    1a3f:	7e 86                	jle    19c7 <F_0_765+0x149>
    1a41:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1a48:	48 89 85 48 d2 ff ff 	mov    %rax,-0x2db8(%rbp)
    1a4f:	48 8b 85 48 d2 ff ff 	mov    -0x2db8(%rbp),%rax
    1a56:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1a5a:	c5 fd 7f 85 90 da ff 	vmovdqa %ymm0,-0x2570(%rbp)
    1a61:	ff 
    1a62:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1a69:	48 83 c0 20          	add    $0x20,%rax
    1a6d:	48 89 85 40 d2 ff ff 	mov    %rax,-0x2dc0(%rbp)
    1a74:	48 8b 85 40 d2 ff ff 	mov    -0x2dc0(%rbp),%rax
    1a7b:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1a7f:	c5 fd 7f 85 b0 da ff 	vmovdqa %ymm0,-0x2550(%rbp)
    1a86:	ff 
    1a87:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1a8e:	48 83 c0 40          	add    $0x40,%rax
    1a92:	48 89 85 38 d2 ff ff 	mov    %rax,-0x2dc8(%rbp)
    1a99:	48 8b 85 38 d2 ff ff 	mov    -0x2dc8(%rbp),%rax
    1aa0:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1aa4:	c5 fd 7f 85 d0 da ff 	vmovdqa %ymm0,-0x2530(%rbp)
    1aab:	ff 
    1aac:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1ab3:	48 83 c0 60          	add    $0x60,%rax
    1ab7:	48 89 85 30 d2 ff ff 	mov    %rax,-0x2dd0(%rbp)
    1abe:	48 8b 85 30 d2 ff ff 	mov    -0x2dd0(%rbp),%rax
    1ac5:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1ac9:	c5 fd 7f 85 f0 da ff 	vmovdqa %ymm0,-0x2510(%rbp)
    1ad0:	ff 
    1ad1:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1ad8:	48 83 e8 80          	sub    $0xffffffffffffff80,%rax
    1adc:	48 89 85 28 d2 ff ff 	mov    %rax,-0x2dd8(%rbp)
    1ae3:	48 8b 85 28 d2 ff ff 	mov    -0x2dd8(%rbp),%rax
    1aea:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1aee:	c5 fd 7f 85 10 db ff 	vmovdqa %ymm0,-0x24f0(%rbp)
    1af5:	ff 
    1af6:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1afd:	48 05 a0 00 00 00    	add    $0xa0,%rax
    1b03:	48 89 85 20 d2 ff ff 	mov    %rax,-0x2de0(%rbp)
    1b0a:	48 8b 85 20 d2 ff ff 	mov    -0x2de0(%rbp),%rax
    1b11:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1b15:	c5 fd 7f 85 30 db ff 	vmovdqa %ymm0,-0x24d0(%rbp)
    1b1c:	ff 
    1b1d:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1b24:	48 05 c0 00 00 00    	add    $0xc0,%rax
    1b2a:	48 89 85 18 d2 ff ff 	mov    %rax,-0x2de8(%rbp)
    1b31:	48 8b 85 18 d2 ff ff 	mov    -0x2de8(%rbp),%rax
    1b38:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1b3c:	c5 fd 7f 85 50 db ff 	vmovdqa %ymm0,-0x24b0(%rbp)
    1b43:	ff 
    1b44:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
    1b4b:	48 05 e0 00 00 00    	add    $0xe0,%rax
    1b51:	48 89 85 10 d2 ff ff 	mov    %rax,-0x2df0(%rbp)
    1b58:	48 8b 85 10 d2 ff ff 	mov    -0x2df0(%rbp),%rax
    1b5f:	c5 ff f0 00          	vlddqu (%rax),%ymm0
    1b63:	c5 fd 7f 85 70 db ff 	vmovdqa %ymm0,-0x2490(%rbp)
    1b6a:	ff 
    1b6b:	c5 fd 6f 85 90 da ff 	vmovdqa -0x2570(%rbp),%ymm0
    1b72:	ff 
    1b73:	c5 fd 7f 85 90 fc ff 	vmovdqa %ymm0,-0x370(%rbp)
    1b7a:	ff 
    1b7b:	c5 fd 6f 85 70 db ff 	vmovdqa -0x2490(%rbp),%ymm0
    1b82:	ff 
    1b83:	c5 fd 7f 85 b0 fc ff 	vmovdqa %ymm0,-0x350(%rbp)
    1b8a:	ff 
    1b8b:	c5 fd 6f 8d 90 fc ff 	vmovdqa -0x370(%rbp),%ymm1
    1b92:	ff 
    1b93:	c5 fd 6f 85 b0 fc ff 	vmovdqa -0x350(%rbp),%ymm0
    1b9a:	ff 
    1b9b:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1b9f:	c5 fd 7f 85 90 db ff 	vmovdqa %ymm0,-0x2470(%rbp)
    1ba6:	ff 
    1ba7:	c5 fd 6f 85 b0 da ff 	vmovdqa -0x2550(%rbp),%ymm0
    1bae:	ff 
    1baf:	c5 fd 7f 85 50 fc ff 	vmovdqa %ymm0,-0x3b0(%rbp)
    1bb6:	ff 
    1bb7:	c5 fd 6f 85 50 db ff 	vmovdqa -0x24b0(%rbp),%ymm0
    1bbe:	ff 
    1bbf:	c5 fd 7f 85 70 fc ff 	vmovdqa %ymm0,-0x390(%rbp)
    1bc6:	ff 
    1bc7:	c5 fd 6f 8d 50 fc ff 	vmovdqa -0x3b0(%rbp),%ymm1
    1bce:	ff 
    1bcf:	c5 fd 6f 85 70 fc ff 	vmovdqa -0x390(%rbp),%ymm0
    1bd6:	ff 
    1bd7:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1bdb:	c5 fd 7f 85 b0 db ff 	vmovdqa %ymm0,-0x2450(%rbp)
    1be2:	ff 
    1be3:	c5 fd 6f 85 d0 da ff 	vmovdqa -0x2530(%rbp),%ymm0
    1bea:	ff 
    1beb:	c5 fd 7f 85 10 fc ff 	vmovdqa %ymm0,-0x3f0(%rbp)
    1bf2:	ff 
    1bf3:	c5 fd 6f 85 30 db ff 	vmovdqa -0x24d0(%rbp),%ymm0
    1bfa:	ff 
    1bfb:	c5 fd 7f 85 30 fc ff 	vmovdqa %ymm0,-0x3d0(%rbp)
    1c02:	ff 
    1c03:	c5 fd 6f 8d 10 fc ff 	vmovdqa -0x3f0(%rbp),%ymm1
    1c0a:	ff 
    1c0b:	c5 fd 6f 85 30 fc ff 	vmovdqa -0x3d0(%rbp),%ymm0
    1c12:	ff 
    1c13:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1c17:	c5 fd 7f 85 d0 db ff 	vmovdqa %ymm0,-0x2430(%rbp)
    1c1e:	ff 
    1c1f:	c5 fd 6f 85 f0 da ff 	vmovdqa -0x2510(%rbp),%ymm0
    1c26:	ff 
    1c27:	c5 fd 7f 85 d0 fb ff 	vmovdqa %ymm0,-0x430(%rbp)
    1c2e:	ff 
    1c2f:	c5 fd 6f 85 10 db ff 	vmovdqa -0x24f0(%rbp),%ymm0
    1c36:	ff 
    1c37:	c5 fd 7f 85 f0 fb ff 	vmovdqa %ymm0,-0x410(%rbp)
    1c3e:	ff 
    1c3f:	c5 fd 6f 8d d0 fb ff 	vmovdqa -0x430(%rbp),%ymm1
    1c46:	ff 
    1c47:	c5 fd 6f 85 f0 fb ff 	vmovdqa -0x410(%rbp),%ymm0
    1c4e:	ff 
    1c4f:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1c53:	c5 fd 7f 85 f0 db ff 	vmovdqa %ymm0,-0x2410(%rbp)
    1c5a:	ff 
    1c5b:	c5 fd 6f 85 f0 da ff 	vmovdqa -0x2510(%rbp),%ymm0
    1c62:	ff 
    1c63:	c5 fd 7f 85 90 fb ff 	vmovdqa %ymm0,-0x470(%rbp)
    1c6a:	ff 
    1c6b:	c5 fd 6f 85 10 db ff 	vmovdqa -0x24f0(%rbp),%ymm0
    1c72:	ff 
    1c73:	c5 fd 7f 85 b0 fb ff 	vmovdqa %ymm0,-0x450(%rbp)
    1c7a:	ff 
    1c7b:	c5 fd 6f 85 90 fb ff 	vmovdqa -0x470(%rbp),%ymm0
    1c82:	ff 
    1c83:	c5 fd 6f 8d b0 fb ff 	vmovdqa -0x450(%rbp),%ymm1
    1c8a:	ff 
    1c8b:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1c8f:	c5 fd 7f 85 10 dc ff 	vmovdqa %ymm0,-0x23f0(%rbp)
    1c96:	ff 
    1c97:	c5 fd 6f 85 d0 da ff 	vmovdqa -0x2530(%rbp),%ymm0
    1c9e:	ff 
    1c9f:	c5 fd 7f 85 50 fb ff 	vmovdqa %ymm0,-0x4b0(%rbp)
    1ca6:	ff 
    1ca7:	c5 fd 6f 85 30 db ff 	vmovdqa -0x24d0(%rbp),%ymm0
    1cae:	ff 
    1caf:	c5 fd 7f 85 70 fb ff 	vmovdqa %ymm0,-0x490(%rbp)
    1cb6:	ff 
    1cb7:	c5 fd 6f 85 50 fb ff 	vmovdqa -0x4b0(%rbp),%ymm0
    1cbe:	ff 
    1cbf:	c5 fd 6f 8d 70 fb ff 	vmovdqa -0x490(%rbp),%ymm1
    1cc6:	ff 
    1cc7:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1ccb:	c5 fd 7f 85 30 dc ff 	vmovdqa %ymm0,-0x23d0(%rbp)
    1cd2:	ff 
    1cd3:	c5 fd 6f 85 b0 da ff 	vmovdqa -0x2550(%rbp),%ymm0
    1cda:	ff 
    1cdb:	c5 fd 7f 85 10 fb ff 	vmovdqa %ymm0,-0x4f0(%rbp)
    1ce2:	ff 
    1ce3:	c5 fd 6f 85 50 db ff 	vmovdqa -0x24b0(%rbp),%ymm0
    1cea:	ff 
    1ceb:	c5 fd 7f 85 30 fb ff 	vmovdqa %ymm0,-0x4d0(%rbp)
    1cf2:	ff 
    1cf3:	c5 fd 6f 85 10 fb ff 	vmovdqa -0x4f0(%rbp),%ymm0
    1cfa:	ff 
    1cfb:	c5 fd 6f 8d 30 fb ff 	vmovdqa -0x4d0(%rbp),%ymm1
    1d02:	ff 
    1d03:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1d07:	c5 fd 7f 85 50 dc ff 	vmovdqa %ymm0,-0x23b0(%rbp)
    1d0e:	ff 
    1d0f:	c5 fd 6f 85 90 da ff 	vmovdqa -0x2570(%rbp),%ymm0
    1d16:	ff 
    1d17:	c5 fd 7f 85 d0 fa ff 	vmovdqa %ymm0,-0x530(%rbp)
    1d1e:	ff 
    1d1f:	c5 fd 6f 85 70 db ff 	vmovdqa -0x2490(%rbp),%ymm0
    1d26:	ff 
    1d27:	c5 fd 7f 85 f0 fa ff 	vmovdqa %ymm0,-0x510(%rbp)
    1d2e:	ff 
    1d2f:	c5 fd 6f 85 d0 fa ff 	vmovdqa -0x530(%rbp),%ymm0
    1d36:	ff 
    1d37:	c5 fd 6f 8d f0 fa ff 	vmovdqa -0x510(%rbp),%ymm1
    1d3e:	ff 
    1d3f:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1d43:	c5 fd 7f 85 70 dc ff 	vmovdqa %ymm0,-0x2390(%rbp)
    1d4a:	ff 
    1d4b:	c5 fd 6f 85 90 db ff 	vmovdqa -0x2470(%rbp),%ymm0
    1d52:	ff 
    1d53:	c5 fd 7f 85 90 fa ff 	vmovdqa %ymm0,-0x570(%rbp)
    1d5a:	ff 
    1d5b:	c5 fd 6f 85 f0 db ff 	vmovdqa -0x2410(%rbp),%ymm0
    1d62:	ff 
    1d63:	c5 fd 7f 85 b0 fa ff 	vmovdqa %ymm0,-0x550(%rbp)
    1d6a:	ff 
    1d6b:	c5 fd 6f 8d 90 fa ff 	vmovdqa -0x570(%rbp),%ymm1
    1d72:	ff 
    1d73:	c5 fd 6f 85 b0 fa ff 	vmovdqa -0x550(%rbp),%ymm0
    1d7a:	ff 
    1d7b:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1d7f:	c5 fd 7f 85 50 fa ff 	vmovdqa %ymm0,-0x5b0(%rbp)
    1d86:	ff 
    1d87:	c5 fd 6f 85 50 d5 ff 	vmovdqa -0x2ab0(%rbp),%ymm0
    1d8e:	ff 
    1d8f:	c5 fd 7f 85 70 fa ff 	vmovdqa %ymm0,-0x590(%rbp)
    1d96:	ff 
    1d97:	c5 fd 6f 8d 50 fa ff 	vmovdqa -0x5b0(%rbp),%ymm1
    1d9e:	ff 
    1d9f:	c5 fd 6f 85 70 fa ff 	vmovdqa -0x590(%rbp),%ymm0
    1da6:	ff 
    1da7:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1dab:	c5 fd 7f 85 90 dc ff 	vmovdqa %ymm0,-0x2370(%rbp)
    1db2:	ff 
    1db3:	c5 fd 6f 85 b0 db ff 	vmovdqa -0x2450(%rbp),%ymm0
    1dba:	ff 
    1dbb:	c5 fd 7f 85 10 fa ff 	vmovdqa %ymm0,-0x5f0(%rbp)
    1dc2:	ff 
    1dc3:	c5 fd 6f 85 d0 db ff 	vmovdqa -0x2430(%rbp),%ymm0
    1dca:	ff 
    1dcb:	c5 fd 7f 85 30 fa ff 	vmovdqa %ymm0,-0x5d0(%rbp)
    1dd2:	ff 
    1dd3:	c5 fd 6f 8d 10 fa ff 	vmovdqa -0x5f0(%rbp),%ymm1
    1dda:	ff 
    1ddb:	c5 fd 6f 85 30 fa ff 	vmovdqa -0x5d0(%rbp),%ymm0
    1de2:	ff 
    1de3:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1de7:	c5 fd 7f 85 b0 dc ff 	vmovdqa %ymm0,-0x2350(%rbp)
    1dee:	ff 
    1def:	c5 fd 6f 85 90 db ff 	vmovdqa -0x2470(%rbp),%ymm0
    1df6:	ff 
    1df7:	c5 fd 7f 85 d0 f9 ff 	vmovdqa %ymm0,-0x630(%rbp)
    1dfe:	ff 
    1dff:	c5 fd 6f 85 f0 db ff 	vmovdqa -0x2410(%rbp),%ymm0
    1e06:	ff 
    1e07:	c5 fd 7f 85 f0 f9 ff 	vmovdqa %ymm0,-0x610(%rbp)
    1e0e:	ff 
    1e0f:	c5 fd 6f 85 d0 f9 ff 	vmovdqa -0x630(%rbp),%ymm0
    1e16:	ff 
    1e17:	c5 fd 6f 8d f0 f9 ff 	vmovdqa -0x610(%rbp),%ymm1
    1e1e:	ff 
    1e1f:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1e23:	c5 fd 7f 85 d0 dc ff 	vmovdqa %ymm0,-0x2330(%rbp)
    1e2a:	ff 
    1e2b:	c5 fd 6f 85 b0 db ff 	vmovdqa -0x2450(%rbp),%ymm0
    1e32:	ff 
    1e33:	c5 fd 7f 85 90 f9 ff 	vmovdqa %ymm0,-0x670(%rbp)
    1e3a:	ff 
    1e3b:	c5 fd 6f 85 d0 db ff 	vmovdqa -0x2430(%rbp),%ymm0
    1e42:	ff 
    1e43:	c5 fd 7f 85 b0 f9 ff 	vmovdqa %ymm0,-0x650(%rbp)
    1e4a:	ff 
    1e4b:	c5 fd 6f 85 90 f9 ff 	vmovdqa -0x670(%rbp),%ymm0
    1e52:	ff 
    1e53:	c5 fd 6f 8d b0 f9 ff 	vmovdqa -0x650(%rbp),%ymm1
    1e5a:	ff 
    1e5b:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    1e5f:	c5 fd 7f 85 f0 dc ff 	vmovdqa %ymm0,-0x2310(%rbp)
    1e66:	ff 
    1e67:	c5 fd 6f 85 10 dc ff 	vmovdqa -0x23f0(%rbp),%ymm0
    1e6e:	ff 
    1e6f:	c5 fd 7f 85 50 f9 ff 	vmovdqa %ymm0,-0x6b0(%rbp)
    1e76:	ff 
    1e77:	c5 fd 6f 85 10 d5 ff 	vmovdqa -0x2af0(%rbp),%ymm0
    1e7e:	ff 
    1e7f:	c5 fd 7f 85 70 f9 ff 	vmovdqa %ymm0,-0x690(%rbp)
    1e86:	ff 
    1e87:	c5 fd 6f 8d 50 f9 ff 	vmovdqa -0x6b0(%rbp),%ymm1
    1e8e:	ff 
    1e8f:	c5 fd 6f 85 70 f9 ff 	vmovdqa -0x690(%rbp),%ymm0
    1e96:	ff 
    1e97:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1e9c:	c5 fd 7f 85 10 f9 ff 	vmovdqa %ymm0,-0x6f0(%rbp)
    1ea3:	ff 
    1ea4:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1eab:	ff 
    1eac:	c5 fd 7f 85 30 f9 ff 	vmovdqa %ymm0,-0x6d0(%rbp)
    1eb3:	ff 
    1eb4:	c5 fd 6f 8d 10 f9 ff 	vmovdqa -0x6f0(%rbp),%ymm1
    1ebb:	ff 
    1ebc:	c5 fd 6f 85 30 f9 ff 	vmovdqa -0x6d0(%rbp),%ymm0
    1ec3:	ff 
    1ec4:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1ec8:	c5 fd 7f 85 f0 f8 ff 	vmovdqa %ymm0,-0x710(%rbp)
    1ecf:	ff 
    1ed0:	c7 85 0c d2 ff ff 10 	movl   $0x10,-0x2df4(%rbp)
    1ed7:	00 00 00 
    1eda:	c5 fd 6f 85 f0 f8 ff 	vmovdqa -0x710(%rbp),%ymm0
    1ee1:	ff 
    1ee2:	8b 85 0c d2 ff ff    	mov    -0x2df4(%rbp),%eax
    1ee8:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    1eef:	c5 fa 7e b5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm6
    1ef6:	ff 
    1ef7:	c5 fd e2 c6          	vpsrad %xmm6,%ymm0,%ymm0
    1efb:	c5 fd 7f 85 10 dd ff 	vmovdqa %ymm0,-0x22f0(%rbp)
    1f02:	ff 
    1f03:	c5 fd 6f 85 70 dc ff 	vmovdqa -0x2390(%rbp),%ymm0
    1f0a:	ff 
    1f0b:	c5 fd 7f 85 b0 f8 ff 	vmovdqa %ymm0,-0x750(%rbp)
    1f12:	ff 
    1f13:	c5 fd 6f 85 10 d5 ff 	vmovdqa -0x2af0(%rbp),%ymm0
    1f1a:	ff 
    1f1b:	c5 fd 7f 85 d0 f8 ff 	vmovdqa %ymm0,-0x730(%rbp)
    1f22:	ff 
    1f23:	c5 fd 6f 8d b0 f8 ff 	vmovdqa -0x750(%rbp),%ymm1
    1f2a:	ff 
    1f2b:	c5 fd 6f 85 d0 f8 ff 	vmovdqa -0x730(%rbp),%ymm0
    1f32:	ff 
    1f33:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    1f38:	c5 fd 7f 85 70 f8 ff 	vmovdqa %ymm0,-0x790(%rbp)
    1f3f:	ff 
    1f40:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    1f47:	ff 
    1f48:	c5 fd 7f 85 90 f8 ff 	vmovdqa %ymm0,-0x770(%rbp)
    1f4f:	ff 
    1f50:	c5 fd 6f 8d 70 f8 ff 	vmovdqa -0x790(%rbp),%ymm1
    1f57:	ff 
    1f58:	c5 fd 6f 85 90 f8 ff 	vmovdqa -0x770(%rbp),%ymm0
    1f5f:	ff 
    1f60:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1f64:	c5 fd 7f 85 50 f8 ff 	vmovdqa %ymm0,-0x7b0(%rbp)
    1f6b:	ff 
    1f6c:	c7 85 08 d2 ff ff 10 	movl   $0x10,-0x2df8(%rbp)
    1f73:	00 00 00 
    1f76:	c5 fd 6f 85 50 f8 ff 	vmovdqa -0x7b0(%rbp),%ymm0
    1f7d:	ff 
    1f7e:	8b 85 08 d2 ff ff    	mov    -0x2df8(%rbp),%eax
    1f84:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    1f8b:	c5 fa 7e bd 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm7
    1f92:	ff 
    1f93:	c5 fd e2 c7          	vpsrad %xmm7,%ymm0,%ymm0
    1f97:	c5 fd 7f 85 30 dd ff 	vmovdqa %ymm0,-0x22d0(%rbp)
    1f9e:	ff 
    1f9f:	c5 fd 6f 85 10 dd ff 	vmovdqa -0x22f0(%rbp),%ymm0
    1fa6:	ff 
    1fa7:	c5 fd 7f 85 10 f8 ff 	vmovdqa %ymm0,-0x7f0(%rbp)
    1fae:	ff 
    1faf:	c5 fd 6f 85 10 dd ff 	vmovdqa -0x22f0(%rbp),%ymm0
    1fb6:	ff 
    1fb7:	c5 fd 7f 85 30 f8 ff 	vmovdqa %ymm0,-0x7d0(%rbp)
    1fbe:	ff 
    1fbf:	c5 fd 6f 8d 10 f8 ff 	vmovdqa -0x7f0(%rbp),%ymm1
    1fc6:	ff 
    1fc7:	c5 fd 6f 85 30 f8 ff 	vmovdqa -0x7d0(%rbp),%ymm0
    1fce:	ff 
    1fcf:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    1fd3:	c5 fd 7f 85 50 dd ff 	vmovdqa %ymm0,-0x22b0(%rbp)
    1fda:	ff 
    1fdb:	c5 fd 6f 85 30 dd ff 	vmovdqa -0x22d0(%rbp),%ymm0
    1fe2:	ff 
    1fe3:	c5 fd 7f 85 d0 f7 ff 	vmovdqa %ymm0,-0x830(%rbp)
    1fea:	ff 
    1feb:	c5 fd 6f 85 30 dd ff 	vmovdqa -0x22d0(%rbp),%ymm0
    1ff2:	ff 
    1ff3:	c5 fd 7f 85 f0 f7 ff 	vmovdqa %ymm0,-0x810(%rbp)
    1ffa:	ff 
    1ffb:	c5 fd 6f 8d d0 f7 ff 	vmovdqa -0x830(%rbp),%ymm1
    2002:	ff 
    2003:	c5 fd 6f 85 f0 f7 ff 	vmovdqa -0x810(%rbp),%ymm0
    200a:	ff 
    200b:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    200f:	c5 fd 7f 85 70 dd ff 	vmovdqa %ymm0,-0x2290(%rbp)
    2016:	ff 
    2017:	c5 fd 6f 85 50 dc ff 	vmovdqa -0x23b0(%rbp),%ymm0
    201e:	ff 
    201f:	c5 fd 7f 85 90 f7 ff 	vmovdqa %ymm0,-0x870(%rbp)
    2026:	ff 
    2027:	c5 fd 6f 85 30 dc ff 	vmovdqa -0x23d0(%rbp),%ymm0
    202e:	ff 
    202f:	c5 fd 7f 85 b0 f7 ff 	vmovdqa %ymm0,-0x850(%rbp)
    2036:	ff 
    2037:	c5 fd 6f 8d 90 f7 ff 	vmovdqa -0x870(%rbp),%ymm1
    203e:	ff 
    203f:	c5 fd 6f 85 b0 f7 ff 	vmovdqa -0x850(%rbp),%ymm0
    2046:	ff 
    2047:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    204b:	c5 fd 7f 85 90 dd ff 	vmovdqa %ymm0,-0x2270(%rbp)
    2052:	ff 
    2053:	c5 fd 6f 85 50 dc ff 	vmovdqa -0x23b0(%rbp),%ymm0
    205a:	ff 
    205b:	c5 fd 7f 85 50 f7 ff 	vmovdqa %ymm0,-0x8b0(%rbp)
    2062:	ff 
    2063:	c5 fd 6f 85 30 dc ff 	vmovdqa -0x23d0(%rbp),%ymm0
    206a:	ff 
    206b:	c5 fd 7f 85 70 f7 ff 	vmovdqa %ymm0,-0x890(%rbp)
    2072:	ff 
    2073:	c5 fd 6f 85 50 f7 ff 	vmovdqa -0x8b0(%rbp),%ymm0
    207a:	ff 
    207b:	c5 fd 6f 8d 70 f7 ff 	vmovdqa -0x890(%rbp),%ymm1
    2082:	ff 
    2083:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    2087:	c5 fd 7f 85 b0 dd ff 	vmovdqa %ymm0,-0x2250(%rbp)
    208e:	ff 
    208f:	c5 fd 6f 85 90 dc ff 	vmovdqa -0x2370(%rbp),%ymm0
    2096:	ff 
    2097:	c5 fd 7f 85 10 f7 ff 	vmovdqa %ymm0,-0x8f0(%rbp)
    209e:	ff 
    209f:	c5 fd 6f 85 b0 dc ff 	vmovdqa -0x2350(%rbp),%ymm0
    20a6:	ff 
    20a7:	c5 fd 7f 85 30 f7 ff 	vmovdqa %ymm0,-0x8d0(%rbp)
    20ae:	ff 
    20af:	c5 fd 6f 8d 10 f7 ff 	vmovdqa -0x8f0(%rbp),%ymm1
    20b6:	ff 
    20b7:	c5 fd 6f 85 30 f7 ff 	vmovdqa -0x8d0(%rbp),%ymm0
    20be:	ff 
    20bf:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    20c3:	c5 fd 7f 85 f0 f6 ff 	vmovdqa %ymm0,-0x910(%rbp)
    20ca:	ff 
    20cb:	c7 85 04 d2 ff ff 03 	movl   $0x3,-0x2dfc(%rbp)
    20d2:	00 00 00 
    20d5:	c5 fd 6f 85 f0 f6 ff 	vmovdqa -0x910(%rbp),%ymm0
    20dc:	ff 
    20dd:	8b 85 04 d2 ff ff    	mov    -0x2dfc(%rbp),%eax
    20e3:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    20ea:	c5 fa 7e 9d 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm3
    20f1:	ff 
    20f2:	c5 fd e2 c3          	vpsrad %xmm3,%ymm0,%ymm0
    20f6:	c5 fd 7f 85 d0 dd ff 	vmovdqa %ymm0,-0x2230(%rbp)
    20fd:	ff 
    20fe:	c5 fd 6f 85 90 dc ff 	vmovdqa -0x2370(%rbp),%ymm0
    2105:	ff 
    2106:	c5 fd 7f 85 b0 f6 ff 	vmovdqa %ymm0,-0x950(%rbp)
    210d:	ff 
    210e:	c5 fd 6f 85 b0 dc ff 	vmovdqa -0x2350(%rbp),%ymm0
    2115:	ff 
    2116:	c5 fd 7f 85 d0 f6 ff 	vmovdqa %ymm0,-0x930(%rbp)
    211d:	ff 
    211e:	c5 fd 6f 85 b0 f6 ff 	vmovdqa -0x950(%rbp),%ymm0
    2125:	ff 
    2126:	c5 fd 6f 8d d0 f6 ff 	vmovdqa -0x930(%rbp),%ymm1
    212d:	ff 
    212e:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    2132:	c5 fd 7f 85 90 f6 ff 	vmovdqa %ymm0,-0x970(%rbp)
    2139:	ff 
    213a:	c7 85 00 d2 ff ff 03 	movl   $0x3,-0x2e00(%rbp)
    2141:	00 00 00 
    2144:	c5 fd 6f 85 90 f6 ff 	vmovdqa -0x970(%rbp),%ymm0
    214b:	ff 
    214c:	8b 85 00 d2 ff ff    	mov    -0x2e00(%rbp),%eax
    2152:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    2159:	c5 fa 7e a5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm4
    2160:	ff 
    2161:	c5 fd e2 c4          	vpsrad %xmm4,%ymm0,%ymm0
    2165:	c5 fd 7f 85 f0 dd ff 	vmovdqa %ymm0,-0x2210(%rbp)
    216c:	ff 
    216d:	c5 fd 6f 85 10 d4 ff 	vmovdqa -0x2bf0(%rbp),%ymm0
    2174:	ff 
    2175:	c5 fd 7f 85 50 f6 ff 	vmovdqa %ymm0,-0x9b0(%rbp)
    217c:	ff 
    217d:	c5 fd 6f 85 d0 dc ff 	vmovdqa -0x2330(%rbp),%ymm0
    2184:	ff 
    2185:	c5 fd 7f 85 70 f6 ff 	vmovdqa %ymm0,-0x990(%rbp)
    218c:	ff 
    218d:	c5 fd 6f 8d 50 f6 ff 	vmovdqa -0x9b0(%rbp),%ymm1
    2194:	ff 
    2195:	c5 fd 6f 85 70 f6 ff 	vmovdqa -0x990(%rbp),%ymm0
    219c:	ff 
    219d:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    21a2:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    21a6:	c5 fd 6f 85 30 d4 ff 	vmovdqa -0x2bd0(%rbp),%ymm0
    21ad:	ff 
    21ae:	c5 fd 7f 85 10 f6 ff 	vmovdqa %ymm0,-0x9f0(%rbp)
    21b5:	ff 
    21b6:	c5 fd 6f 85 f0 dc ff 	vmovdqa -0x2310(%rbp),%ymm0
    21bd:	ff 
    21be:	c5 fd 7f 85 30 f6 ff 	vmovdqa %ymm0,-0x9d0(%rbp)
    21c5:	ff 
    21c6:	c5 fd 6f 8d 10 f6 ff 	vmovdqa -0x9f0(%rbp),%ymm1
    21cd:	ff 
    21ce:	c5 fd 6f 85 30 f6 ff 	vmovdqa -0x9d0(%rbp),%ymm0
    21d5:	ff 
    21d6:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    21db:	c5 fd 7f 85 d0 f5 ff 	vmovdqa %ymm0,-0xa30(%rbp)
    21e2:	ff 
    21e3:	c5 fd 7f 95 f0 f5 ff 	vmovdqa %ymm2,-0xa10(%rbp)
    21ea:	ff 
    21eb:	c5 fd 6f 8d d0 f5 ff 	vmovdqa -0xa30(%rbp),%ymm1
    21f2:	ff 
    21f3:	c5 fd 6f 85 f0 f5 ff 	vmovdqa -0xa10(%rbp),%ymm0
    21fa:	ff 
    21fb:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    21ff:	c5 fd 7f 85 90 f5 ff 	vmovdqa %ymm0,-0xa70(%rbp)
    2206:	ff 
    2207:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    220e:	ff 
    220f:	c5 fd 7f 85 b0 f5 ff 	vmovdqa %ymm0,-0xa50(%rbp)
    2216:	ff 
    2217:	c5 fd 6f 8d 90 f5 ff 	vmovdqa -0xa70(%rbp),%ymm1
    221e:	ff 
    221f:	c5 fd 6f 85 b0 f5 ff 	vmovdqa -0xa50(%rbp),%ymm0
    2226:	ff 
    2227:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    222b:	c5 fd 7f 85 70 f5 ff 	vmovdqa %ymm0,-0xa90(%rbp)
    2232:	ff 
    2233:	c7 85 fc d1 ff ff 10 	movl   $0x10,-0x2e04(%rbp)
    223a:	00 00 00 
    223d:	c5 fd 6f 85 70 f5 ff 	vmovdqa -0xa90(%rbp),%ymm0
    2244:	ff 
    2245:	8b 85 fc d1 ff ff    	mov    -0x2e04(%rbp),%eax
    224b:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    2252:	c5 fa 7e ad 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm5
    2259:	ff 
    225a:	c5 fd e2 c5          	vpsrad %xmm5,%ymm0,%ymm0
    225e:	c5 fd 7f 85 10 de ff 	vmovdqa %ymm0,-0x21f0(%rbp)
    2265:	ff 
    2266:	c5 fd 6f 85 10 d4 ff 	vmovdqa -0x2bf0(%rbp),%ymm0
    226d:	ff 
    226e:	c5 fd 7f 85 30 f5 ff 	vmovdqa %ymm0,-0xad0(%rbp)
    2275:	ff 
    2276:	c5 fd 6f 85 f0 dc ff 	vmovdqa -0x2310(%rbp),%ymm0
    227d:	ff 
    227e:	c5 fd 7f 85 50 f5 ff 	vmovdqa %ymm0,-0xab0(%rbp)
    2285:	ff 
    2286:	c5 fd 6f 8d 30 f5 ff 	vmovdqa -0xad0(%rbp),%ymm1
    228d:	ff 
    228e:	c5 fd 6f 85 50 f5 ff 	vmovdqa -0xab0(%rbp),%ymm0
    2295:	ff 
    2296:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    229b:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    229f:	c5 fd 6f 85 30 d4 ff 	vmovdqa -0x2bd0(%rbp),%ymm0
    22a6:	ff 
    22a7:	c5 fd 7f 85 f0 f4 ff 	vmovdqa %ymm0,-0xb10(%rbp)
    22ae:	ff 
    22af:	c5 fd 6f 85 d0 dc ff 	vmovdqa -0x2330(%rbp),%ymm0
    22b6:	ff 
    22b7:	c5 fd 7f 85 10 f5 ff 	vmovdqa %ymm0,-0xaf0(%rbp)
    22be:	ff 
    22bf:	c5 fd 6f 8d f0 f4 ff 	vmovdqa -0xb10(%rbp),%ymm1
    22c6:	ff 
    22c7:	c5 fd 6f 85 10 f5 ff 	vmovdqa -0xaf0(%rbp),%ymm0
    22ce:	ff 
    22cf:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    22d4:	c5 fd 7f 85 b0 f4 ff 	vmovdqa %ymm0,-0xb50(%rbp)
    22db:	ff 
    22dc:	c5 fd 7f 95 d0 f4 ff 	vmovdqa %ymm2,-0xb30(%rbp)
    22e3:	ff 
    22e4:	c5 fd 6f 85 b0 f4 ff 	vmovdqa -0xb50(%rbp),%ymm0
    22eb:	ff 
    22ec:	c5 fd 6f 8d d0 f4 ff 	vmovdqa -0xb30(%rbp),%ymm1
    22f3:	ff 
    22f4:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    22f8:	c5 fd 7f 85 70 f4 ff 	vmovdqa %ymm0,-0xb90(%rbp)
    22ff:	ff 
    2300:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    2307:	ff 
    2308:	c5 fd 7f 85 90 f4 ff 	vmovdqa %ymm0,-0xb70(%rbp)
    230f:	ff 
    2310:	c5 fd 6f 8d 70 f4 ff 	vmovdqa -0xb90(%rbp),%ymm1
    2317:	ff 
    2318:	c5 fd 6f 85 90 f4 ff 	vmovdqa -0xb70(%rbp),%ymm0
    231f:	ff 
    2320:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2324:	c5 fd 7f 85 50 f4 ff 	vmovdqa %ymm0,-0xbb0(%rbp)
    232b:	ff 
    232c:	c7 85 f8 d1 ff ff 10 	movl   $0x10,-0x2e08(%rbp)
    2333:	00 00 00 
    2336:	c5 fd 6f 85 50 f4 ff 	vmovdqa -0xbb0(%rbp),%ymm0
    233d:	ff 
    233e:	8b 85 f8 d1 ff ff    	mov    -0x2e08(%rbp),%eax
    2344:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    234b:	c5 fa 7e b5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm6
    2352:	ff 
    2353:	c5 fd e2 c6          	vpsrad %xmm6,%ymm0,%ymm0
    2357:	c5 fd 7f 85 30 de ff 	vmovdqa %ymm0,-0x21d0(%rbp)
    235e:	ff 
    235f:	c5 fd 6f 85 50 dd ff 	vmovdqa -0x22b0(%rbp),%ymm0
    2366:	ff 
    2367:	c5 fd 7f 85 10 f4 ff 	vmovdqa %ymm0,-0xbf0(%rbp)
    236e:	ff 
    236f:	c5 fd 6f 85 b0 dd ff 	vmovdqa -0x2250(%rbp),%ymm0
    2376:	ff 
    2377:	c5 fd 7f 85 30 f4 ff 	vmovdqa %ymm0,-0xbd0(%rbp)
    237e:	ff 
    237f:	c5 fd 6f 8d 10 f4 ff 	vmovdqa -0xbf0(%rbp),%ymm1
    2386:	ff 
    2387:	c5 fd 6f 85 30 f4 ff 	vmovdqa -0xbd0(%rbp),%ymm0
    238e:	ff 
    238f:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2393:	c5 fd 7f 85 50 de ff 	vmovdqa %ymm0,-0x21b0(%rbp)
    239a:	ff 
    239b:	c5 fd 6f 85 50 dd ff 	vmovdqa -0x22b0(%rbp),%ymm0
    23a2:	ff 
    23a3:	c5 fd 7f 85 d0 f3 ff 	vmovdqa %ymm0,-0xc30(%rbp)
    23aa:	ff 
    23ab:	c5 fd 6f 85 b0 dd ff 	vmovdqa -0x2250(%rbp),%ymm0
    23b2:	ff 
    23b3:	c5 fd 7f 85 f0 f3 ff 	vmovdqa %ymm0,-0xc10(%rbp)
    23ba:	ff 
    23bb:	c5 fd 6f 85 d0 f3 ff 	vmovdqa -0xc30(%rbp),%ymm0
    23c2:	ff 
    23c3:	c5 fd 6f 8d f0 f3 ff 	vmovdqa -0xc10(%rbp),%ymm1
    23ca:	ff 
    23cb:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    23cf:	c5 fd 7f 85 70 de ff 	vmovdqa %ymm0,-0x2190(%rbp)
    23d6:	ff 
    23d7:	c5 fd 6f 85 70 dd ff 	vmovdqa -0x2290(%rbp),%ymm0
    23de:	ff 
    23df:	c5 fd 7f 85 90 f3 ff 	vmovdqa %ymm0,-0xc70(%rbp)
    23e6:	ff 
    23e7:	c5 fd 6f 85 90 dd ff 	vmovdqa -0x2270(%rbp),%ymm0
    23ee:	ff 
    23ef:	c5 fd 7f 85 b0 f3 ff 	vmovdqa %ymm0,-0xc50(%rbp)
    23f6:	ff 
    23f7:	c5 fd 6f 8d 90 f3 ff 	vmovdqa -0xc70(%rbp),%ymm1
    23fe:	ff 
    23ff:	c5 fd 6f 85 b0 f3 ff 	vmovdqa -0xc50(%rbp),%ymm0
    2406:	ff 
    2407:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    240b:	c5 fd 7f 85 90 de ff 	vmovdqa %ymm0,-0x2170(%rbp)
    2412:	ff 
    2413:	c5 fd 6f 85 70 dd ff 	vmovdqa -0x2290(%rbp),%ymm0
    241a:	ff 
    241b:	c5 fd 7f 85 50 f3 ff 	vmovdqa %ymm0,-0xcb0(%rbp)
    2422:	ff 
    2423:	c5 fd 6f 85 90 dd ff 	vmovdqa -0x2270(%rbp),%ymm0
    242a:	ff 
    242b:	c5 fd 7f 85 70 f3 ff 	vmovdqa %ymm0,-0xc90(%rbp)
    2432:	ff 
    2433:	c5 fd 6f 85 50 f3 ff 	vmovdqa -0xcb0(%rbp),%ymm0
    243a:	ff 
    243b:	c5 fd 6f 8d 70 f3 ff 	vmovdqa -0xc90(%rbp),%ymm1
    2442:	ff 
    2443:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    2447:	c5 fd 7f 85 b0 de ff 	vmovdqa %ymm0,-0x2150(%rbp)
    244e:	ff 
    244f:	c5 fd 6f 85 d0 dd ff 	vmovdqa -0x2230(%rbp),%ymm0
    2456:	ff 
    2457:	c5 fd 7f 85 d0 fd ff 	vmovdqa %ymm0,-0x230(%rbp)
    245e:	ff 
    245f:	c5 fd 6f 85 f0 dd ff 	vmovdqa -0x2210(%rbp),%ymm0
    2466:	ff 
    2467:	c5 fd 7f 85 50 fe ff 	vmovdqa %ymm0,-0x1b0(%rbp)
    246e:	ff 
    246f:	c5 fd 6f 85 10 de ff 	vmovdqa -0x21f0(%rbp),%ymm0
    2476:	ff 
    2477:	c5 fd 7f 85 10 fe ff 	vmovdqa %ymm0,-0x1f0(%rbp)
    247e:	ff 
    247f:	c5 fd 6f 85 30 de ff 	vmovdqa -0x21d0(%rbp),%ymm0
    2486:	ff 
    2487:	c5 fd 7f 85 90 fe ff 	vmovdqa %ymm0,-0x170(%rbp)
    248e:	ff 
    248f:	c5 fd 6f 85 90 d3 ff 	vmovdqa -0x2c70(%rbp),%ymm0
    2496:	ff 
    2497:	c5 fd 7f 85 10 f3 ff 	vmovdqa %ymm0,-0xcf0(%rbp)
    249e:	ff 
    249f:	c5 fd 6f 85 90 de ff 	vmovdqa -0x2170(%rbp),%ymm0
    24a6:	ff 
    24a7:	c5 fd 7f 85 30 f3 ff 	vmovdqa %ymm0,-0xcd0(%rbp)
    24ae:	ff 
    24af:	c5 fd 6f 8d 10 f3 ff 	vmovdqa -0xcf0(%rbp),%ymm1
    24b6:	ff 
    24b7:	c5 fd 6f 85 30 f3 ff 	vmovdqa -0xcd0(%rbp),%ymm0
    24be:	ff 
    24bf:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    24c4:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    24c8:	c5 fd 6f 85 f0 d3 ff 	vmovdqa -0x2c10(%rbp),%ymm0
    24cf:	ff 
    24d0:	c5 fd 7f 85 d0 f2 ff 	vmovdqa %ymm0,-0xd30(%rbp)
    24d7:	ff 
    24d8:	c5 fd 6f 85 50 de ff 	vmovdqa -0x21b0(%rbp),%ymm0
    24df:	ff 
    24e0:	c5 fd 7f 85 f0 f2 ff 	vmovdqa %ymm0,-0xd10(%rbp)
    24e7:	ff 
    24e8:	c5 fd 6f 8d d0 f2 ff 	vmovdqa -0xd30(%rbp),%ymm1
    24ef:	ff 
    24f0:	c5 fd 6f 85 f0 f2 ff 	vmovdqa -0xd10(%rbp),%ymm0
    24f7:	ff 
    24f8:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    24fd:	c5 fd 7f 85 90 f2 ff 	vmovdqa %ymm0,-0xd70(%rbp)
    2504:	ff 
    2505:	c5 fd 7f 95 b0 f2 ff 	vmovdqa %ymm2,-0xd50(%rbp)
    250c:	ff 
    250d:	c5 fd 6f 8d 90 f2 ff 	vmovdqa -0xd70(%rbp),%ymm1
    2514:	ff 
    2515:	c5 fd 6f 85 b0 f2 ff 	vmovdqa -0xd50(%rbp),%ymm0
    251c:	ff 
    251d:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2521:	c5 fd 7f 85 50 f2 ff 	vmovdqa %ymm0,-0xdb0(%rbp)
    2528:	ff 
    2529:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    2530:	ff 
    2531:	c5 fd 7f 85 70 f2 ff 	vmovdqa %ymm0,-0xd90(%rbp)
    2538:	ff 
    2539:	c5 fd 6f 8d 50 f2 ff 	vmovdqa -0xdb0(%rbp),%ymm1
    2540:	ff 
    2541:	c5 fd 6f 85 70 f2 ff 	vmovdqa -0xd90(%rbp),%ymm0
    2548:	ff 
    2549:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    254d:	c5 fd 7f 85 30 f2 ff 	vmovdqa %ymm0,-0xdd0(%rbp)
    2554:	ff 
    2555:	c7 85 f4 d1 ff ff 10 	movl   $0x10,-0x2e0c(%rbp)
    255c:	00 00 00 
    255f:	c5 fd 6f 85 30 f2 ff 	vmovdqa -0xdd0(%rbp),%ymm0
    2566:	ff 
    2567:	8b 85 f4 d1 ff ff    	mov    -0x2e0c(%rbp),%eax
    256d:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    2574:	c5 fa 7e bd 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm7
    257b:	ff 
    257c:	c5 fd e2 c7          	vpsrad %xmm7,%ymm0,%ymm0
    2580:	c5 fd 7f 85 f0 fd ff 	vmovdqa %ymm0,-0x210(%rbp)
    2587:	ff 
    2588:	c5 fd 6f 85 90 d3 ff 	vmovdqa -0x2c70(%rbp),%ymm0
    258f:	ff 
    2590:	c5 fd 7f 85 f0 f1 ff 	vmovdqa %ymm0,-0xe10(%rbp)
    2597:	ff 
    2598:	c5 fd 6f 85 50 de ff 	vmovdqa -0x21b0(%rbp),%ymm0
    259f:	ff 
    25a0:	c5 fd 7f 85 10 f2 ff 	vmovdqa %ymm0,-0xdf0(%rbp)
    25a7:	ff 
    25a8:	c5 fd 6f 8d f0 f1 ff 	vmovdqa -0xe10(%rbp),%ymm1
    25af:	ff 
    25b0:	c5 fd 6f 85 10 f2 ff 	vmovdqa -0xdf0(%rbp),%ymm0
    25b7:	ff 
    25b8:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    25bd:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    25c1:	c5 fd 6f 85 f0 d3 ff 	vmovdqa -0x2c10(%rbp),%ymm0
    25c8:	ff 
    25c9:	c5 fd 7f 85 b0 f1 ff 	vmovdqa %ymm0,-0xe50(%rbp)
    25d0:	ff 
    25d1:	c5 fd 6f 85 90 de ff 	vmovdqa -0x2170(%rbp),%ymm0
    25d8:	ff 
    25d9:	c5 fd 7f 85 d0 f1 ff 	vmovdqa %ymm0,-0xe30(%rbp)
    25e0:	ff 
    25e1:	c5 fd 6f 8d b0 f1 ff 	vmovdqa -0xe50(%rbp),%ymm1
    25e8:	ff 
    25e9:	c5 fd 6f 85 d0 f1 ff 	vmovdqa -0xe30(%rbp),%ymm0
    25f0:	ff 
    25f1:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    25f6:	c5 fd 7f 85 70 f1 ff 	vmovdqa %ymm0,-0xe90(%rbp)
    25fd:	ff 
    25fe:	c5 fd 7f 95 90 f1 ff 	vmovdqa %ymm2,-0xe70(%rbp)
    2605:	ff 
    2606:	c5 fd 6f 85 70 f1 ff 	vmovdqa -0xe90(%rbp),%ymm0
    260d:	ff 
    260e:	c5 fd 6f 8d 90 f1 ff 	vmovdqa -0xe70(%rbp),%ymm1
    2615:	ff 
    2616:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    261a:	c5 fd 7f 85 30 f1 ff 	vmovdqa %ymm0,-0xed0(%rbp)
    2621:	ff 
    2622:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    2629:	ff 
    262a:	c5 fd 7f 85 50 f1 ff 	vmovdqa %ymm0,-0xeb0(%rbp)
    2631:	ff 
    2632:	c5 fd 6f 8d 30 f1 ff 	vmovdqa -0xed0(%rbp),%ymm1
    2639:	ff 
    263a:	c5 fd 6f 85 50 f1 ff 	vmovdqa -0xeb0(%rbp),%ymm0
    2641:	ff 
    2642:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2646:	c5 fd 7f 85 10 f1 ff 	vmovdqa %ymm0,-0xef0(%rbp)
    264d:	ff 
    264e:	c7 85 f0 d1 ff ff 10 	movl   $0x10,-0x2e10(%rbp)
    2655:	00 00 00 
    2658:	c5 fd 6f 85 10 f1 ff 	vmovdqa -0xef0(%rbp),%ymm0
    265f:	ff 
    2660:	8b 85 f0 d1 ff ff    	mov    -0x2e10(%rbp),%eax
    2666:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    266d:	c5 fa 7e 9d 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm3
    2674:	ff 
    2675:	c5 fd e2 c3          	vpsrad %xmm3,%ymm0,%ymm0
    2679:	c5 fd 7f 85 b0 fe ff 	vmovdqa %ymm0,-0x150(%rbp)
    2680:	ff 
    2681:	c5 fd 6f 85 d0 d3 ff 	vmovdqa -0x2c30(%rbp),%ymm0
    2688:	ff 
    2689:	c5 fd 7f 85 d0 f0 ff 	vmovdqa %ymm0,-0xf30(%rbp)
    2690:	ff 
    2691:	c5 fd 6f 85 b0 de ff 	vmovdqa -0x2150(%rbp),%ymm0
    2698:	ff 
    2699:	c5 fd 7f 85 f0 f0 ff 	vmovdqa %ymm0,-0xf10(%rbp)
    26a0:	ff 
    26a1:	c5 fd 6f 8d d0 f0 ff 	vmovdqa -0xf30(%rbp),%ymm1
    26a8:	ff 
    26a9:	c5 fd 6f 85 f0 f0 ff 	vmovdqa -0xf10(%rbp),%ymm0
    26b0:	ff 
    26b1:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    26b6:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    26ba:	c5 fd 6f 85 b0 d3 ff 	vmovdqa -0x2c50(%rbp),%ymm0
    26c1:	ff 
    26c2:	c5 fd 7f 85 90 f0 ff 	vmovdqa %ymm0,-0xf70(%rbp)
    26c9:	ff 
    26ca:	c5 fd 6f 85 70 de ff 	vmovdqa -0x2190(%rbp),%ymm0
    26d1:	ff 
    26d2:	c5 fd 7f 85 b0 f0 ff 	vmovdqa %ymm0,-0xf50(%rbp)
    26d9:	ff 
    26da:	c5 fd 6f 8d 90 f0 ff 	vmovdqa -0xf70(%rbp),%ymm1
    26e1:	ff 
    26e2:	c5 fd 6f 85 b0 f0 ff 	vmovdqa -0xf50(%rbp),%ymm0
    26e9:	ff 
    26ea:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    26ef:	c5 fd 7f 85 50 f0 ff 	vmovdqa %ymm0,-0xfb0(%rbp)
    26f6:	ff 
    26f7:	c5 fd 7f 95 70 f0 ff 	vmovdqa %ymm2,-0xf90(%rbp)
    26fe:	ff 
    26ff:	c5 fd 6f 8d 50 f0 ff 	vmovdqa -0xfb0(%rbp),%ymm1
    2706:	ff 
    2707:	c5 fd 6f 85 70 f0 ff 	vmovdqa -0xf90(%rbp),%ymm0
    270e:	ff 
    270f:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2713:	c5 fd 7f 85 10 f0 ff 	vmovdqa %ymm0,-0xff0(%rbp)
    271a:	ff 
    271b:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    2722:	ff 
    2723:	c5 fd 7f 85 30 f0 ff 	vmovdqa %ymm0,-0xfd0(%rbp)
    272a:	ff 
    272b:	c5 fd 6f 8d 10 f0 ff 	vmovdqa -0xff0(%rbp),%ymm1
    2732:	ff 
    2733:	c5 fd 6f 85 30 f0 ff 	vmovdqa -0xfd0(%rbp),%ymm0
    273a:	ff 
    273b:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    273f:	c5 fd 7f 85 f0 ef ff 	vmovdqa %ymm0,-0x1010(%rbp)
    2746:	ff 
    2747:	c7 85 ec d1 ff ff 10 	movl   $0x10,-0x2e14(%rbp)
    274e:	00 00 00 
    2751:	c5 fd 6f 85 f0 ef ff 	vmovdqa -0x1010(%rbp),%ymm0
    2758:	ff 
    2759:	8b 85 ec d1 ff ff    	mov    -0x2e14(%rbp),%eax
    275f:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    2766:	c5 fa 7e a5 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm4
    276d:	ff 
    276e:	c5 fd e2 c4          	vpsrad %xmm4,%ymm0,%ymm0
    2772:	c5 fd 7f 85 70 fe ff 	vmovdqa %ymm0,-0x190(%rbp)
    2779:	ff 
    277a:	c5 fd 6f 85 d0 d3 ff 	vmovdqa -0x2c30(%rbp),%ymm0
    2781:	ff 
    2782:	c5 fd 7f 85 b0 ef ff 	vmovdqa %ymm0,-0x1050(%rbp)
    2789:	ff 
    278a:	c5 fd 6f 85 70 de ff 	vmovdqa -0x2190(%rbp),%ymm0
    2791:	ff 
    2792:	c5 fd 7f 85 d0 ef ff 	vmovdqa %ymm0,-0x1030(%rbp)
    2799:	ff 
    279a:	c5 fd 6f 8d b0 ef ff 	vmovdqa -0x1050(%rbp),%ymm1
    27a1:	ff 
    27a2:	c5 fd 6f 85 d0 ef ff 	vmovdqa -0x1030(%rbp),%ymm0
    27a9:	ff 
    27aa:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    27af:	c5 fd 6f d0          	vmovdqa %ymm0,%ymm2
    27b3:	c5 fd 6f 85 b0 d3 ff 	vmovdqa -0x2c50(%rbp),%ymm0
    27ba:	ff 
    27bb:	c5 fd 7f 85 70 ef ff 	vmovdqa %ymm0,-0x1090(%rbp)
    27c2:	ff 
    27c3:	c5 fd 6f 85 b0 de ff 	vmovdqa -0x2150(%rbp),%ymm0
    27ca:	ff 
    27cb:	c5 fd 7f 85 90 ef ff 	vmovdqa %ymm0,-0x1070(%rbp)
    27d2:	ff 
    27d3:	c5 fd 6f 8d 70 ef ff 	vmovdqa -0x1090(%rbp),%ymm1
    27da:	ff 
    27db:	c5 fd 6f 85 90 ef ff 	vmovdqa -0x1070(%rbp),%ymm0
    27e2:	ff 
    27e3:	c4 e2 75 40 c0       	vpmulld %ymm0,%ymm1,%ymm0
    27e8:	c5 fd 7f 85 30 ef ff 	vmovdqa %ymm0,-0x10d0(%rbp)
    27ef:	ff 
    27f0:	c5 fd 7f 95 50 ef ff 	vmovdqa %ymm2,-0x10b0(%rbp)
    27f7:	ff 
    27f8:	c5 fd 6f 85 30 ef ff 	vmovdqa -0x10d0(%rbp),%ymm0
    27ff:	ff 
    2800:	c5 fd 6f 8d 50 ef ff 	vmovdqa -0x10b0(%rbp),%ymm1
    2807:	ff 
    2808:	c5 fd fa c1          	vpsubd %ymm1,%ymm0,%ymm0
    280c:	c5 fd 7f 85 f0 ee ff 	vmovdqa %ymm0,-0x1110(%rbp)
    2813:	ff 
    2814:	c5 fd 6f 85 70 d5 ff 	vmovdqa -0x2a90(%rbp),%ymm0
    281b:	ff 
    281c:	c5 fd 7f 85 10 ef ff 	vmovdqa %ymm0,-0x10f0(%rbp)
    2823:	ff 
    2824:	c5 fd 6f 8d f0 ee ff 	vmovdqa -0x1110(%rbp),%ymm1
    282b:	ff 
    282c:	c5 fd 6f 85 10 ef ff 	vmovdqa -0x10f0(%rbp),%ymm0
    2833:	ff 
    2834:	c5 f5 fe c0          	vpaddd %ymm0,%ymm1,%ymm0
    2838:	c5 fd 7f 85 d0 ee ff 	vmovdqa %ymm0,-0x1130(%rbp)
    283f:	ff 
    2840:	c7 85 e8 d1 ff ff 10 	movl   $0x10,-0x2e18(%rbp)
    2847:	00 00 00 
    284a:	c5 fd 6f 85 d0 ee ff 	vmovdqa -0x1130(%rbp),%ymm0
    2851:	ff 
    2852:	8b 85 e8 d1 ff ff    	mov    -0x2e18(%rbp),%eax
    2858:	48 89 85 58 d1 ff ff 	mov    %rax,-0x2ea8(%rbp)
    285f:	c5 fa 7e ad 58 d1 ff 	vmovq  -0x2ea8(%rbp),%xmm5
    2866:	ff 
    2867:	c5 fd e2 c5          	vpsrad %xmm5,%ymm0,%ymm0
    286b:	c5 fd 7f 85 30 fe ff 	vmovdqa %ymm0,-0x1d0(%rbp)
    2872:	ff 
    2873:	c7 85 80 d1 ff ff 00 	movl   $0x0,-0x2e80(%rbp)
    287a:	00 00 00 
    287d:	eb 7a                	jmp    28f9 <F_1_175+0x358>
    287f:	c7 85 84 d1 ff ff 00 	movl   $0x0,-0x2e7c(%rbp)
    2886:	00 00 00 
    2889:	eb 5e                	jmp    28e9 <F_1_175+0x348>
    288b:	8b 85 80 d1 ff ff    	mov    -0x2e80(%rbp),%eax
    2891:	48 98                	cltq   
    2893:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
    289a:	00 
    289b:	8b 85 84 d1 ff ff    	mov    -0x2e7c(%rbp),%eax
    28a1:	48 98                	cltq   
    28a3:	48 01 d0             	add    %rdx,%rax
    28a6:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx
    28ad:	00 
    28ae:	48 8d 85 d0 fd ff ff 	lea    -0x230(%rbp),%rax
    28b5:	48 01 d0             	add    %rdx,%rax
    28b8:	8b 10                	mov    (%rax),%edx
    28ba:	8b 85 84 d1 ff ff    	mov    -0x2e7c(%rbp),%eax
    28c0:	8d 0c c5 00 00 00 00 	lea    0x0(,%rax,8),%ecx
    28c7:	8b 85 80 d1 ff ff    	mov    -0x2e80(%rbp),%eax
    28cd:	01 c8                	add    %ecx,%eax
    28cf:	48 98                	cltq   
    28d1:	48 8d 0c 00          	lea    (%rax,%rax,1),%rcx
    28d5:	48 8b 85 60 d1 ff ff 	mov    -0x2ea0(%rbp),%rax
    28dc:	48 01 c8             	add    %rcx,%rax
    28df:	66 89 10             	mov    %dx,(%rax)
    28e2:	83 85 84 d1 ff ff 01 	addl   $0x1,-0x2e7c(%rbp)
    28e9:	83 bd 84 d1 ff ff 07 	cmpl   $0x7,-0x2e7c(%rbp)
    28f0:	7e 99                	jle    288b <F_1_175+0x2ea>
    28f2:	83 85 80 d1 ff ff 01 	addl   $0x1,-0x2e80(%rbp)
    28f9:	83 bd 80 d1 ff ff 07 	cmpl   $0x7,-0x2e80(%rbp)
    2900:	0f 8e 79 ff ff ff    	jle    287f <F_1_175+0x2de>
    2906:	90                   	nop
    2907:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    290b:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    2912:	00 00 
    2914:	74 05                	je     291b <F_1_175+0x37a>
    2916:	e8 a5 dc ff ff       	callq  5c0 <__stack_chk_fail@plt>
    291b:	48 81 c4 a8 2e 00 00 	add    $0x2ea8,%rsp
    2922:	41 5a                	pop    %r10
    2924:	5d                   	pop    %rbp
    2925:	49 8d 62 f8          	lea    -0x8(%r10),%rsp
    2929:	c3                   	retq   

000000000000292a <fastDCT>:
    292a:	55                   	push   %rbp
    292b:	48 89 e5             	mov    %rsp,%rbp
    292e:	48 83 ec 30          	sub    $0x30,%rsp
    2932:	48 89 bd 68 ff ff ff 	mov    %rdi,-0x98(%rbp)
    2939:	48 89 b5 60 ff ff ff 	mov    %rsi,-0xa0(%rbp)
    2940:	89 95 5c ff ff ff    	mov    %edx,-0xa4(%rbp)
    2946:	66 c7 85 7e ff ff ff 	movw   $0x1f62,-0x82(%rbp)
    294d:	62 1f 
    294f:	66 c7 45 80 9b 1a    	movw   $0x1a9b,-0x80(%rbp)
    2955:	66 c7 45 82 c7 11    	movw   $0x11c7,-0x7e(%rbp)
    295b:	66 c7 45 84 3e 06    	movw   $0x63e,-0x7c(%rbp)
    2961:	66 c7 45 86 cf 29    	movw   $0x29cf,-0x7a(%rbp)
    2967:	66 c7 45 88 51 11    	movw   $0x1151,-0x78(%rbp)
    296d:	66 c7 45 8a 15 fb    	movw   $0xfb15,-0x76(%rbp)
    2973:	66 c7 45 8c db d4    	movw   $0xd4db,-0x74(%rbp)
    2979:	66 c7 45 8e 3a 8e    	movw   $0x8e3a,-0x72(%rbp)
    297f:	66 c7 45 90 f1 31    	movw   $0x31f1,-0x70(%rbp)
    2985:	66 c7 45 92 3d a7    	movw   $0xa73d,-0x6e(%rbp)
    298b:	66 c7 45 94 46 45    	movw   $0x4546,-0x6c(%rbp)
    2991:	66 c7 45 96 05 b5    	movw   $0xb505,-0x6a(%rbp)
    2997:	48 8b 85 60 ff ff ff 	mov    -0xa0(%rbp),%rax
    299e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    29a2:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%rbp)
    29a9:	e9 e8 03 00 00       	jmpq   2d96 <fastDCT+0x46c>
    29ae:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
    29b5:	e9 c1 03 00 00       	jmpq   2d7b <fastDCT+0x451>
    29ba:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    29c1:	0f b6 00             	movzbl (%rax),%eax
    29c4:	0f b6 c0             	movzbl %al,%eax
    29c7:	83 c0 80             	add    $0xffffff80,%eax
    29ca:	66 89 45 98          	mov    %ax,-0x68(%rbp)
    29ce:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    29d5:	48 83 c0 08          	add    $0x8,%rax
    29d9:	0f b6 00             	movzbl (%rax),%eax
    29dc:	0f b6 c0             	movzbl %al,%eax
    29df:	83 c0 80             	add    $0xffffff80,%eax
    29e2:	66 89 45 9a          	mov    %ax,-0x66(%rbp)
    29e6:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    29ed:	48 83 c0 10          	add    $0x10,%rax
    29f1:	0f b6 00             	movzbl (%rax),%eax
    29f4:	0f b6 c0             	movzbl %al,%eax
    29f7:	83 c0 80             	add    $0xffffff80,%eax
    29fa:	66 89 45 9c          	mov    %ax,-0x64(%rbp)
    29fe:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    2a05:	48 83 c0 18          	add    $0x18,%rax
    2a09:	0f b6 00             	movzbl (%rax),%eax
    2a0c:	0f b6 c0             	movzbl %al,%eax
    2a0f:	83 c0 80             	add    $0xffffff80,%eax
    2a12:	66 89 45 9e          	mov    %ax,-0x62(%rbp)
    2a16:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    2a1d:	48 83 c0 20          	add    $0x20,%rax
    2a21:	0f b6 00             	movzbl (%rax),%eax
    2a24:	0f b6 c0             	movzbl %al,%eax
    2a27:	83 c0 80             	add    $0xffffff80,%eax
    2a2a:	66 89 45 a0          	mov    %ax,-0x60(%rbp)
    2a2e:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    2a35:	48 83 c0 28          	add    $0x28,%rax
    2a39:	0f b6 00             	movzbl (%rax),%eax
    2a3c:	0f b6 c0             	movzbl %al,%eax
    2a3f:	83 c0 80             	add    $0xffffff80,%eax
    2a42:	66 89 45 a2          	mov    %ax,-0x5e(%rbp)
    2a46:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    2a4d:	48 83 c0 30          	add    $0x30,%rax
    2a51:	0f b6 00             	movzbl (%rax),%eax
    2a54:	0f b6 c0             	movzbl %al,%eax
    2a57:	83 c0 80             	add    $0xffffff80,%eax
    2a5a:	66 89 45 a4          	mov    %ax,-0x5c(%rbp)
    2a5e:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
    2a65:	48 83 c0 38          	add    $0x38,%rax
    2a69:	0f b6 00             	movzbl (%rax),%eax
    2a6c:	0f b6 c0             	movzbl %al,%eax
    2a6f:	83 c0 80             	add    $0xffffff80,%eax
    2a72:	66 89 45 a6          	mov    %ax,-0x5a(%rbp)
    2a76:	0f b7 55 98          	movzwl -0x68(%rbp),%edx
    2a7a:	0f b7 45 a6          	movzwl -0x5a(%rbp),%eax
    2a7e:	01 d0                	add    %edx,%eax
    2a80:	66 89 45 a8          	mov    %ax,-0x58(%rbp)
    2a84:	0f b7 55 9a          	movzwl -0x66(%rbp),%edx
    2a88:	0f b7 45 a4          	movzwl -0x5c(%rbp),%eax
    2a8c:	01 d0                	add    %edx,%eax
    2a8e:	66 89 45 aa          	mov    %ax,-0x56(%rbp)
    2a92:	0f b7 55 9c          	movzwl -0x64(%rbp),%edx
    2a96:	0f b7 45 a2          	movzwl -0x5e(%rbp),%eax
    2a9a:	01 d0                	add    %edx,%eax
    2a9c:	66 89 45 ac          	mov    %ax,-0x54(%rbp)
    2aa0:	0f b7 55 9e          	movzwl -0x62(%rbp),%edx
    2aa4:	0f b7 45 a0          	movzwl -0x60(%rbp),%eax
    2aa8:	01 d0                	add    %edx,%eax
    2aaa:	66 89 45 ae          	mov    %ax,-0x52(%rbp)
    2aae:	0f b7 55 9e          	movzwl -0x62(%rbp),%edx
    2ab2:	0f b7 45 a0          	movzwl -0x60(%rbp),%eax
    2ab6:	29 c2                	sub    %eax,%edx
    2ab8:	89 d0                	mov    %edx,%eax
    2aba:	66 89 45 b0          	mov    %ax,-0x50(%rbp)
    2abe:	0f b7 55 9c          	movzwl -0x64(%rbp),%edx
    2ac2:	0f b7 45 a2          	movzwl -0x5e(%rbp),%eax
    2ac6:	29 c2                	sub    %eax,%edx
    2ac8:	89 d0                	mov    %edx,%eax
    2aca:	66 89 45 b2          	mov    %ax,-0x4e(%rbp)
    2ace:	0f b7 55 9a          	movzwl -0x66(%rbp),%edx
    2ad2:	0f b7 45 a4          	movzwl -0x5c(%rbp),%eax
    2ad6:	29 c2                	sub    %eax,%edx
    2ad8:	89 d0                	mov    %edx,%eax
    2ada:	66 89 45 b4          	mov    %ax,-0x4c(%rbp)
    2ade:	0f b7 55 98          	movzwl -0x68(%rbp),%edx
    2ae2:	0f b7 45 a6          	movzwl -0x5a(%rbp),%eax
    2ae6:	29 c2                	sub    %eax,%edx
    2ae8:	89 d0                	mov    %edx,%eax
    2aea:	66 89 45 b6          	mov    %ax,-0x4a(%rbp)
    2aee:	0f b7 55 a8          	movzwl -0x58(%rbp),%edx
    2af2:	0f b7 45 ae          	movzwl -0x52(%rbp),%eax
    2af6:	01 d0                	add    %edx,%eax
    2af8:	66 89 45 b8          	mov    %ax,-0x48(%rbp)
    2afc:	0f b7 55 aa          	movzwl -0x56(%rbp),%edx
    2b00:	0f b7 45 ac          	movzwl -0x54(%rbp),%eax
    2b04:	01 d0                	add    %edx,%eax
    2b06:	66 89 45 ba          	mov    %ax,-0x46(%rbp)
    2b0a:	0f b7 55 a8          	movzwl -0x58(%rbp),%edx
    2b0e:	0f b7 45 ae          	movzwl -0x52(%rbp),%eax
    2b12:	29 c2                	sub    %eax,%edx
    2b14:	89 d0                	mov    %edx,%eax
    2b16:	66 89 45 bc          	mov    %ax,-0x44(%rbp)
    2b1a:	0f b7 55 aa          	movzwl -0x56(%rbp),%edx
    2b1e:	0f b7 45 ac          	movzwl -0x54(%rbp),%eax
    2b22:	29 c2                	sub    %eax,%edx
    2b24:	89 d0                	mov    %edx,%eax
    2b26:	66 89 45 be          	mov    %ax,-0x42(%rbp)
    2b2a:	0f b7 45 b0          	movzwl -0x50(%rbp),%eax
    2b2e:	01 c0                	add    %eax,%eax
    2b30:	66 89 45 c0          	mov    %ax,-0x40(%rbp)
    2b34:	0f b7 45 b6          	movzwl -0x4a(%rbp),%eax
    2b38:	01 c0                	add    %eax,%eax
    2b3a:	66 89 45 c2          	mov    %ax,-0x3e(%rbp)
    2b3e:	0f bf 55 c0          	movswl -0x40(%rbp),%edx
    2b42:	0f b7 45 96          	movzwl -0x6a(%rbp),%eax
    2b46:	0f af c2             	imul   %edx,%eax
    2b49:	05 00 80 00 00       	add    $0x8000,%eax
    2b4e:	c1 f8 10             	sar    $0x10,%eax
    2b51:	66 89 45 c4          	mov    %ax,-0x3c(%rbp)
    2b55:	0f bf 55 c2          	movswl -0x3e(%rbp),%edx
    2b59:	0f b7 45 96          	movzwl -0x6a(%rbp),%eax
    2b5d:	0f af c2             	imul   %edx,%eax
    2b60:	05 00 80 00 00       	add    $0x8000,%eax
    2b65:	c1 f8 10             	sar    $0x10,%eax
    2b68:	66 89 45 c6          	mov    %ax,-0x3a(%rbp)
    2b6c:	0f b7 55 b4          	movzwl -0x4c(%rbp),%edx
    2b70:	0f b7 45 b2          	movzwl -0x4e(%rbp),%eax
    2b74:	01 d0                	add    %edx,%eax
    2b76:	66 89 45 c8          	mov    %ax,-0x38(%rbp)
    2b7a:	0f b7 55 b4          	movzwl -0x4c(%rbp),%edx
    2b7e:	0f b7 45 b2          	movzwl -0x4e(%rbp),%eax
    2b82:	29 c2                	sub    %eax,%edx
    2b84:	89 d0                	mov    %edx,%eax
    2b86:	66 89 45 ca          	mov    %ax,-0x36(%rbp)
    2b8a:	0f b7 55 b8          	movzwl -0x48(%rbp),%edx
    2b8e:	0f b7 45 ba          	movzwl -0x46(%rbp),%eax
    2b92:	01 d0                	add    %edx,%eax
    2b94:	66 89 45 cc          	mov    %ax,-0x34(%rbp)
    2b98:	0f b7 55 b8          	movzwl -0x48(%rbp),%edx
    2b9c:	0f b7 45 ba          	movzwl -0x46(%rbp),%eax
    2ba0:	29 c2                	sub    %eax,%edx
    2ba2:	89 d0                	mov    %edx,%eax
    2ba4:	66 89 45 ce          	mov    %ax,-0x32(%rbp)
    2ba8:	0f b7 45 bc          	movzwl -0x44(%rbp),%eax
    2bac:	01 c0                	add    %eax,%eax
    2bae:	66 89 45 ec          	mov    %ax,-0x14(%rbp)
    2bb2:	0f b7 45 be          	movzwl -0x42(%rbp),%eax
    2bb6:	01 c0                	add    %eax,%eax
    2bb8:	66 89 45 ee          	mov    %ax,-0x12(%rbp)
    2bbc:	0f b7 55 94          	movzwl -0x6c(%rbp),%edx
    2bc0:	0f bf 45 ee          	movswl -0x12(%rbp),%eax
    2bc4:	0f af d0             	imul   %eax,%edx
    2bc7:	0f b7 4d 92          	movzwl -0x6e(%rbp),%ecx
    2bcb:	0f bf 45 ec          	movswl -0x14(%rbp),%eax
    2bcf:	0f af c1             	imul   %ecx,%eax
    2bd2:	01 d0                	add    %edx,%eax
    2bd4:	05 00 80 00 00       	add    $0x8000,%eax
    2bd9:	c1 f8 10             	sar    $0x10,%eax
    2bdc:	66 89 45 d0          	mov    %ax,-0x30(%rbp)
    2be0:	0f b7 55 94          	movzwl -0x6c(%rbp),%edx
    2be4:	0f bf 45 ec          	movswl -0x14(%rbp),%eax
    2be8:	0f af d0             	imul   %eax,%edx
    2beb:	0f b7 4d 92          	movzwl -0x6e(%rbp),%ecx
    2bef:	0f bf 45 ee          	movswl -0x12(%rbp),%eax
    2bf3:	0f af c1             	imul   %ecx,%eax
    2bf6:	29 c2                	sub    %eax,%edx
    2bf8:	89 d0                	mov    %edx,%eax
    2bfa:	05 00 80 00 00       	add    $0x8000,%eax
    2bff:	c1 f8 10             	sar    $0x10,%eax
    2c02:	66 89 45 d2          	mov    %ax,-0x2e(%rbp)
    2c06:	0f b7 55 c4          	movzwl -0x3c(%rbp),%edx
    2c0a:	0f b7 45 ca          	movzwl -0x36(%rbp),%eax
    2c0e:	01 d0                	add    %edx,%eax
    2c10:	66 89 45 d4          	mov    %ax,-0x2c(%rbp)
    2c14:	0f b7 55 c6          	movzwl -0x3a(%rbp),%edx
    2c18:	0f b7 45 c8          	movzwl -0x38(%rbp),%eax
    2c1c:	01 d0                	add    %edx,%eax
    2c1e:	66 89 45 d8          	mov    %ax,-0x28(%rbp)
    2c22:	0f b7 55 c4          	movzwl -0x3c(%rbp),%edx
    2c26:	0f b7 45 ca          	movzwl -0x36(%rbp),%eax
    2c2a:	29 c2                	sub    %eax,%edx
    2c2c:	89 d0                	mov    %edx,%eax
    2c2e:	66 89 45 d6          	mov    %ax,-0x2a(%rbp)
    2c32:	0f b7 55 c6          	movzwl -0x3a(%rbp),%edx
    2c36:	0f b7 45 c8          	movzwl -0x38(%rbp),%eax
    2c3a:	29 c2                	sub    %eax,%edx
    2c3c:	89 d0                	mov    %edx,%eax
    2c3e:	66 89 45 da          	mov    %ax,-0x26(%rbp)
    2c42:	0f b7 45 cc          	movzwl -0x34(%rbp),%eax
    2c46:	66 89 45 dc          	mov    %ax,-0x24(%rbp)
    2c4a:	0f b7 45 ce          	movzwl -0x32(%rbp),%eax
    2c4e:	66 89 45 de          	mov    %ax,-0x22(%rbp)
    2c52:	0f b7 45 d0          	movzwl -0x30(%rbp),%eax
    2c56:	66 89 45 e0          	mov    %ax,-0x20(%rbp)
    2c5a:	0f b7 45 d2          	movzwl -0x2e(%rbp),%eax
    2c5e:	66 89 45 e2          	mov    %ax,-0x1e(%rbp)
    2c62:	0f b7 55 90          	movzwl -0x70(%rbp),%edx
    2c66:	0f bf 45 d4          	movswl -0x2c(%rbp),%eax
    2c6a:	0f af d0             	imul   %eax,%edx
    2c6d:	0f b7 4d 8a          	movzwl -0x76(%rbp),%ecx
    2c71:	0f bf 45 d8          	movswl -0x28(%rbp),%eax
    2c75:	0f af c1             	imul   %ecx,%eax
    2c78:	01 d0                	add    %edx,%eax
    2c7a:	05 00 80 00 00       	add    $0x8000,%eax
    2c7f:	c1 f8 10             	sar    $0x10,%eax
    2c82:	66 89 45 e4          	mov    %ax,-0x1c(%rbp)
    2c86:	0f b7 55 90          	movzwl -0x70(%rbp),%edx
    2c8a:	0f bf 45 d8          	movswl -0x28(%rbp),%eax
    2c8e:	0f af d0             	imul   %eax,%edx
    2c91:	0f b7 4d 8a          	movzwl -0x76(%rbp),%ecx
    2c95:	0f bf 45 d4          	movswl -0x2c(%rbp),%eax
    2c99:	0f af c1             	imul   %ecx,%eax
    2c9c:	29 c2                	sub    %eax,%edx
    2c9e:	89 d0                	mov    %edx,%eax
    2ca0:	05 00 80 00 00       	add    $0x8000,%eax
    2ca5:	c1 f8 10             	sar    $0x10,%eax
    2ca8:	66 89 45 e6          	mov    %ax,-0x1a(%rbp)
    2cac:	0f b7 55 8c          	movzwl -0x74(%rbp),%edx
    2cb0:	0f bf 45 d6          	movswl -0x2a(%rbp),%eax
    2cb4:	0f af d0             	imul   %eax,%edx
    2cb7:	0f b7 4d 8e          	movzwl -0x72(%rbp),%ecx
    2cbb:	0f bf 45 da          	movswl -0x26(%rbp),%eax
    2cbf:	0f af c1             	imul   %ecx,%eax
    2cc2:	01 d0                	add    %edx,%eax
    2cc4:	05 00 80 00 00       	add    $0x8000,%eax
    2cc9:	c1 f8 10             	sar    $0x10,%eax
    2ccc:	66 89 45 e8          	mov    %ax,-0x18(%rbp)
    2cd0:	0f b7 55 8c          	movzwl -0x74(%rbp),%edx
    2cd4:	0f bf 45 da          	movswl -0x26(%rbp),%eax
    2cd8:	0f af d0             	imul   %eax,%edx
    2cdb:	0f b7 4d 8e          	movzwl -0x72(%rbp),%ecx
    2cdf:	0f bf 45 d6          	movswl -0x2a(%rbp),%eax
    2ce3:	0f af c1             	imul   %ecx,%eax
    2ce6:	29 c2                	sub    %eax,%edx
    2ce8:	89 d0                	mov    %edx,%eax
    2cea:	05 00 80 00 00       	add    $0x8000,%eax
    2cef:	c1 f8 10             	sar    $0x10,%eax
    2cf2:	66 89 45 ea          	mov    %ax,-0x16(%rbp)
    2cf6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2cfa:	0f b7 55 dc          	movzwl -0x24(%rbp),%edx
    2cfe:	66 89 10             	mov    %dx,(%rax)
    2d01:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d05:	48 8d 50 10          	lea    0x10(%rax),%rdx
    2d09:	0f b7 45 e4          	movzwl -0x1c(%rbp),%eax
    2d0d:	66 89 02             	mov    %ax,(%rdx)
    2d10:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d14:	48 8d 50 20          	lea    0x20(%rax),%rdx
    2d18:	0f b7 45 e0          	movzwl -0x20(%rbp),%eax
    2d1c:	66 89 02             	mov    %ax,(%rdx)
    2d1f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d23:	48 8d 50 30          	lea    0x30(%rax),%rdx
    2d27:	0f b7 45 ea          	movzwl -0x16(%rbp),%eax
    2d2b:	66 89 02             	mov    %ax,(%rdx)
    2d2e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d32:	48 8d 50 40          	lea    0x40(%rax),%rdx
    2d36:	0f b7 45 de          	movzwl -0x22(%rbp),%eax
    2d3a:	66 89 02             	mov    %ax,(%rdx)
    2d3d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d41:	48 8d 50 50          	lea    0x50(%rax),%rdx
    2d45:	0f b7 45 e8          	movzwl -0x18(%rbp),%eax
    2d49:	66 89 02             	mov    %ax,(%rdx)
    2d4c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d50:	48 8d 50 60          	lea    0x60(%rax),%rdx
    2d54:	0f b7 45 e2          	movzwl -0x1e(%rbp),%eax
    2d58:	66 89 02             	mov    %ax,(%rdx)
    2d5b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2d5f:	48 8d 50 70          	lea    0x70(%rax),%rdx
    2d63:	0f b7 45 e6          	movzwl -0x1a(%rbp),%eax
    2d67:	66 89 02             	mov    %ax,(%rdx)
    2d6a:	48 83 45 f8 02       	addq   $0x2,-0x8(%rbp)
    2d6f:	48 83 85 68 ff ff ff 	addq   $0x1,-0x98(%rbp)
    2d76:	01 
    2d77:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)
    2d7b:	83 7d f4 07          	cmpl   $0x7,-0xc(%rbp)
    2d7f:	0f 86 35 fc ff ff    	jbe    29ba <fastDCT+0x90>
    2d85:	48 83 45 f8 70       	addq   $0x70,-0x8(%rbp)
    2d8a:	48 83 85 68 ff ff ff 	addq   $0x38,-0x98(%rbp)
    2d91:	38 
    2d92:	83 45 f0 01          	addl   $0x1,-0x10(%rbp)
    2d96:	8b 45 f0             	mov    -0x10(%rbp),%eax
    2d99:	3b 85 5c ff ff ff    	cmp    -0xa4(%rbp),%eax
    2d9f:	0f 82 09 fc ff ff    	jb     29ae <fastDCT+0x84>
    2da5:	48 8b 85 60 ff ff ff 	mov    -0xa0(%rbp),%rax
    2dac:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    2db0:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%rbp)
    2db7:	e9 59 03 00 00       	jmpq   3115 <F_1_501+0x10a>
    2dbc:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2dc0:	0f b7 00             	movzwl (%rax),%eax
    2dc3:	66 89 45 98          	mov    %ax,-0x68(%rbp)
    2dc7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2dcb:	0f b7 40 02          	movzwl 0x2(%rax),%eax
    2dcf:	66 89 45 9a          	mov    %ax,-0x66(%rbp)
    2dd3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2dd7:	0f b7 40 04          	movzwl 0x4(%rax),%eax
    2ddb:	66 89 45 9c          	mov    %ax,-0x64(%rbp)
    2ddf:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2de3:	0f b7 40 06          	movzwl 0x6(%rax),%eax
    2de7:	66 89 45 9e          	mov    %ax,-0x62(%rbp)
    2deb:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2def:	0f b7 40 08          	movzwl 0x8(%rax),%eax
    2df3:	66 89 45 a0          	mov    %ax,-0x60(%rbp)
    2df7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2dfb:	0f b7 40 0a          	movzwl 0xa(%rax),%eax
    2dff:	66 89 45 a2          	mov    %ax,-0x5e(%rbp)
    2e03:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2e07:	0f b7 40 0c          	movzwl 0xc(%rax),%eax
    2e0b:	66 89 45 a4          	mov    %ax,-0x5c(%rbp)
    2e0f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    2e13:	0f b7 40 0e          	movzwl 0xe(%rax),%eax
    2e17:	66 89 45 a6          	mov    %ax,-0x5a(%rbp)
    2e1b:	0f b7 55 98          	movzwl -0x68(%rbp),%edx
    2e1f:	0f b7 45 a6          	movzwl -0x5a(%rbp),%eax
    2e23:	01 d0                	add    %edx,%eax
    2e25:	66 89 45 a8          	mov    %ax,-0x58(%rbp)
    2e29:	0f b7 55 9a          	movzwl -0x66(%rbp),%edx
    2e2d:	0f b7 45 a4          	movzwl -0x5c(%rbp),%eax
    2e31:	01 d0                	add    %edx,%eax
    2e33:	66 89 45 aa          	mov    %ax,-0x56(%rbp)
    2e37:	0f b7 55 9c          	movzwl -0x64(%rbp),%edx
    2e3b:	0f b7 45 a2          	movzwl -0x5e(%rbp),%eax
    2e3f:	01 d0                	add    %edx,%eax
    2e41:	66 89 45 ac          	mov    %ax,-0x54(%rbp)
    2e45:	0f b7 55 9e          	movzwl -0x62(%rbp),%edx
    2e49:	0f b7 45 a0          	movzwl -0x60(%rbp),%eax
    2e4d:	01 d0                	add    %edx,%eax
    2e4f:	66 89 45 ae          	mov    %ax,-0x52(%rbp)
    2e53:	0f b7 55 9e          	movzwl -0x62(%rbp),%edx
    2e57:	0f b7 45 a0          	movzwl -0x60(%rbp),%eax
    2e5b:	29 c2                	sub    %eax,%edx
    2e5d:	89 d0                	mov    %edx,%eax
    2e5f:	66 89 45 b0          	mov    %ax,-0x50(%rbp)
    2e63:	0f b7 55 9c          	movzwl -0x64(%rbp),%edx
    2e67:	0f b7 45 a2          	movzwl -0x5e(%rbp),%eax
    2e6b:	29 c2                	sub    %eax,%edx
    2e6d:	89 d0                	mov    %edx,%eax
    2e6f:	66 89 45 b2          	mov    %ax,-0x4e(%rbp)
    2e73:	0f b7 55 9a          	movzwl -0x66(%rbp),%edx
    2e77:	0f b7 45 a4          	movzwl -0x5c(%rbp),%eax
    2e7b:	29 c2                	sub    %eax,%edx
    2e7d:	89 d0                	mov    %edx,%eax
    2e7f:	66 89 45 b4          	mov    %ax,-0x4c(%rbp)
    2e83:	0f b7 55 98          	movzwl -0x68(%rbp),%edx
    2e87:	0f b7 45 a6          	movzwl -0x5a(%rbp),%eax
    2e8b:	29 c2                	sub    %eax,%edx
    2e8d:	89 d0                	mov    %edx,%eax
    2e8f:	66 89 45 b6          	mov    %ax,-0x4a(%rbp)
    2e93:	0f b7 55 a8          	movzwl -0x58(%rbp),%edx
    2e97:	0f b7 45 ae          	movzwl -0x52(%rbp),%eax
    2e9b:	01 d0                	add    %edx,%eax
    2e9d:	83 c0 04             	add    $0x4,%eax
    2ea0:	66 89 45 b8          	mov    %ax,-0x48(%rbp)
    2ea4:	0f b7 55 aa          	movzwl -0x56(%rbp),%edx
    2ea8:	0f b7 45 ac          	movzwl -0x54(%rbp),%eax
    2eac:	01 d0                	add    %edx,%eax
    2eae:	66 89 45 ba          	mov    %ax,-0x46(%rbp)
    2eb2:	0f b7 55 a8          	movzwl -0x58(%rbp),%edx
    2eb6:	0f b7 45 ae          	movzwl -0x52(%rbp),%eax
    2eba:	29 c2                	sub    %eax,%edx
    2ebc:	89 d0                	mov    %edx,%eax
    2ebe:	66 89 45 bc          	mov    %ax,-0x44(%rbp)
    2ec2:	0f b7 55 aa          	movzwl -0x56(%rbp),%edx
    2ec6:	0f b7 45 ac          	movzwl -0x54(%rbp),%eax
    2eca:	29 c2                	sub    %eax,%edx
    2ecc:	89 d0                	mov    %edx,%eax
    2ece:	66 89 45 be          	mov    %ax,-0x42(%rbp)
    2ed2:	0f bf 55 b0          	movswl -0x50(%rbp),%edx
    2ed6:	0f b7 45 96          	movzwl -0x6a(%rbp),%eax
    2eda:	0f af c2             	imul   %edx,%eax
    2edd:	05 00 80 00 00       	add    $0x8000,%eax
    2ee2:	c1 f8 10             	sar    $0x10,%eax
    2ee5:	66 89 45 c0          	mov    %ax,-0x40(%rbp)
    2ee9:	0f bf 55 b6          	movswl -0x4a(%rbp),%edx
    2eed:	0f b7 45 96          	movzwl -0x6a(%rbp),%eax
    2ef1:	0f af c2             	imul   %edx,%eax
    2ef4:	05 00 80 00 00       	add    $0x8000,%eax
    2ef9:	c1 f8 10             	sar    $0x10,%eax
    2efc:	66 89 45 c2          	mov    %ax,-0x3e(%rbp)
    2f00:	0f b7 45 c0          	movzwl -0x40(%rbp),%eax
    2f04:	01 c0                	add    %eax,%eax
    2f06:	66 89 45 c4          	mov    %ax,-0x3c(%rbp)
    2f0a:	0f b7 45 c2          	movzwl -0x3e(%rbp),%eax
    2f0e:	01 c0                	add    %eax,%eax
    2f10:	66 89 45 c6          	mov    %ax,-0x3a(%rbp)
    2f14:	0f b7 55 b4          	movzwl -0x4c(%rbp),%edx
    2f18:	0f b7 45 b2          	movzwl -0x4e(%rbp),%eax
    2f1c:	01 d0                	add    %edx,%eax
    2f1e:	66 89 45 c8          	mov    %ax,-0x38(%rbp)
    2f22:	0f b7 55 b4          	movzwl -0x4c(%rbp),%edx
    2f26:	0f b7 45 b2          	movzwl -0x4e(%rbp),%eax
    2f2a:	29 c2                	sub    %eax,%edx
    2f2c:	89 d0                	mov    %edx,%eax
    2f2e:	66 89 45 ca          	mov    %ax,-0x36(%rbp)
    2f32:	0f b7 55 b8          	movzwl -0x48(%rbp),%edx
    2f36:	0f b7 45 ba          	movzwl -0x46(%rbp),%eax
    2f3a:	01 d0                	add    %edx,%eax
    2f3c:	66 c1 f8 03          	sar    $0x3,%ax
    2f40:	66 89 45 cc          	mov    %ax,-0x34(%rbp)
    2f44:	0f b7 55 b8          	movzwl -0x48(%rbp),%edx
    2f48:	0f b7 45 ba          	movzwl -0x46(%rbp),%eax
    2f4c:	29 c2                	sub    %eax,%edx
    2f4e:	89 d0                	mov    %edx,%eax
    2f50:	66 c1 f8 03          	sar    $0x3,%ax
    2f54:	66 89 45 ce          	mov    %ax,-0x32(%rbp)
    2f58:	0f b7 55 88          	movzwl -0x78(%rbp),%edx
    2f5c:	0f bf 45 be          	movswl -0x42(%rbp),%eax
    2f60:	0f af d0             	imul   %eax,%edx
    2f63:	0f b7 4d 86          	movzwl -0x7a(%rbp),%ecx
    2f67:	0f bf 45 bc          	movswl -0x44(%rbp),%eax
    2f6b:	0f af c1             	imul   %ecx,%eax
    2f6e:	01 d0                	add    %edx,%eax
    2f70:	05 00 80 00 00       	add    $0x8000,%eax
    2f75:	c1 f8 10             	sar    $0x10,%eax
    2f78:	66 89 45 d0          	mov    %ax,-0x30(%rbp)
    2f7c:	0f b7 55 88          	movzwl -0x78(%rbp),%edx
    2f80:	0f bf 45 bc          	movswl -0x44(%rbp),%eax
    2f84:	0f af d0             	imul   %eax,%edx
    2f87:	0f b7 4d 86          	movzwl -0x7a(%rbp),%ecx
    2f8b:	0f bf 45 be          	movswl -0x42(%rbp),%eax
    2f8f:	0f af c1             	imul   %ecx,%eax
    2f92:	29 c2                	sub    %eax,%edx
    2f94:	89 d0                	mov    %edx,%eax
    2f96:	05 00 80 00 00       	add    $0x8000,%eax
    2f9b:	c1 f8 10             	sar    $0x10,%eax
    2f9e:	66 89 45 d2          	mov    %ax,-0x2e(%rbp)
    2fa2:	0f b7 55 c4          	movzwl -0x3c(%rbp),%edx
    2fa6:	0f b7 45 ca          	movzwl -0x36(%rbp),%eax
    2faa:	01 d0                	add    %edx,%eax
    2fac:	66 89 45 d4          	mov    %ax,-0x2c(%rbp)
    2fb0:	0f b7 55 c4          	movzwl -0x3c(%rbp),%edx
    2fb4:	0f b7 45 ca          	movzwl -0x36(%rbp),%eax
    2fb8:	29 c2                	sub    %eax,%edx
    2fba:	89 d0                	mov    %edx,%eax
    2fbc:	66 89 45 d6          	mov    %ax,-0x2a(%rbp)
    2fc0:	0f b7 55 c6          	movzwl -0x3a(%rbp),%edx
    2fc4:	0f b7 45 c8          	movzwl -0x38(%rbp),%eax
    2fc8:	01 d0                	add    %edx,%eax
    2fca:	66 89 45 d8          	mov    %ax,-0x28(%rbp)
    2fce:	0f b7 55 c6          	movzwl -0x3a(%rbp),%edx
    2fd2:	0f b7 45 c8          	movzwl -0x38(%rbp),%eax
    2fd6:	29 c2                	sub    %eax,%edx
    2fd8:	89 d0                	mov    %edx,%eax
    2fda:	66 89 45 da          	mov    %ax,-0x26(%rbp)
    2fde:	0f b7 45 cc          	movzwl -0x34(%rbp),%eax
    2fe2:	66 89 45 dc          	mov    %ax,-0x24(%rbp)
    2fe6:	0f b7 45 ce          	movzwl -0x32(%rbp),%eax
    2fea:	66 89 45 de          	mov    %ax,-0x22(%rbp)
    2fee:	0f b7 45 d0          	movzwl -0x30(%rbp),%eax
    2ff2:	66 89 45 e0          	mov    %ax,-0x20(%rbp)
    2ff6:	0f b7 45 d2          	movzwl -0x2e(%rbp),%eax
    2ffa:	66 89 45 e2          	mov    %ax,-0x1e(%rbp)
    2ffe:	0f b7 55 84          	movzwl -0x7c(%rbp),%edx
    3002:	0f bf 45 d4          	movswl -0x2c(%rbp),%eax
    3006:	0f af d0             	imul   %eax,%edx
    3009:	0f b7 8d 7e ff ff ff 	movzwl -0x82(%rbp),%ecx
    3010:	0f bf 45 d8          	movswl -0x28(%rbp),%eax
    3014:	0f af c1             	imul   %ecx,%eax
    3017:	01 d0                	add    %edx,%eax
    3019:	05 00 80 00 00       	add    $0x8000,%eax
    301e:	c1 f8 10             	sar    $0x10,%eax
    3021:	66 89 45 e4          	mov    %ax,-0x1c(%rbp)
    3025:	0f b7 55 84          	movzwl -0x7c(%rbp),%edx
    3029:	0f bf 45 d8          	movswl -0x28(%rbp),%eax
    302d:	0f af d0             	imul   %eax,%edx
    3030:	0f b7 8d 7e ff ff ff 	movzwl -0x82(%rbp),%ecx
    3037:	0f bf 45 d4          	movswl -0x2c(%rbp),%eax
    303b:	0f af c1             	imul   %ecx,%eax
    303e:	29 c2                	sub    %eax,%edx
    3040:	89 d0                	mov    %edx,%eax
    3042:	05 00 80 00 00       	add    $0x8000,%eax
    3047:	c1 f8 10             	sar    $0x10,%eax
    304a:	66 89 45 e6          	mov    %ax,-0x1a(%rbp)
    304e:	0f b7 55 80          	movzwl -0x80(%rbp),%edx
    3052:	0f bf 45 d6          	movswl -0x2a(%rbp),%eax
    3056:	0f af d0             	imul   %eax,%edx
    3059:	0f b7 4d 82          	movzwl -0x7e(%rbp),%ecx
    305d:	0f bf 45 da          	movswl -0x26(%rbp),%eax
    3061:	0f af c1             	imul   %ecx,%eax
    3064:	01 d0                	add    %edx,%eax
    3066:	05 00 80 00 00       	add    $0x8000,%eax
    306b:	c1 f8 10             	sar    $0x10,%eax
    306e:	66 89 45 e8          	mov    %ax,-0x18(%rbp)
    3072:	0f b7 55 80          	movzwl -0x80(%rbp),%edx
    3076:	0f bf 45 da          	movswl -0x26(%rbp),%eax
    307a:	0f af d0             	imul   %eax,%edx
    307d:	0f b7 4d 82          	movzwl -0x7e(%rbp),%ecx
    3081:	0f bf 45 d6          	movswl -0x2a(%rbp),%eax
    3085:	0f af c1             	imul   %ecx,%eax
    3088:	29 c2                	sub    %eax,%edx
    308a:	89 d0                	mov    %edx,%eax
    308c:	05 00 80 00 00       	add    $0x8000,%eax
    3091:	c1 f8 10             	sar    $0x10,%eax
    3094:	66 89 45 ea          	mov    %ax,-0x16(%rbp)
    3098:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    309c:	0f b7 55 dc          	movzwl -0x24(%rbp),%edx
    30a0:	66 89 10             	mov    %dx,(%rax)
    30a3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30a7:	48 8d 50 02          	lea    0x2(%rax),%rdx
    30ab:	0f b7 45 e4          	movzwl -0x1c(%rbp),%eax
    30af:	66 89 02             	mov    %ax,(%rdx)
    30b2:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30b6:	48 8d 50 04          	lea    0x4(%rax),%rdx
    30ba:	0f b7 45 e0          	movzwl -0x20(%rbp),%eax
    30be:	66 89 02             	mov    %ax,(%rdx)
    30c1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30c5:	48 8d 50 06          	lea    0x6(%rax),%rdx
    30c9:	0f b7 45 ea          	movzwl -0x16(%rbp),%eax
    30cd:	66 89 02             	mov    %ax,(%rdx)
    30d0:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30d4:	48 8d 50 08          	lea    0x8(%rax),%rdx
    30d8:	0f b7 45 de          	movzwl -0x22(%rbp),%eax
    30dc:	66 89 02             	mov    %ax,(%rdx)
    30df:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30e3:	48 8d 50 0a          	lea    0xa(%rax),%rdx
    30e7:	0f b7 45 e8          	movzwl -0x18(%rbp),%eax
    30eb:	66 89 02             	mov    %ax,(%rdx)
    30ee:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    30f2:	48 8d 50 0c          	lea    0xc(%rax),%rdx
    30f6:	0f b7 45 e2          	movzwl -0x1e(%rbp),%eax
    30fa:	66 89 02             	mov    %ax,(%rdx)
    30fd:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    3101:	48 8d 50 0e          	lea    0xe(%rax),%rdx
    3105:	0f b7 45 e6          	movzwl -0x1a(%rbp),%eax
    3109:	66 89 02             	mov    %ax,(%rdx)
    310c:	48 83 45 f8 10       	addq   $0x10,-0x8(%rbp)
    3111:	83 45 f0 01          	addl   $0x1,-0x10(%rbp)
    3115:	8b 85 5c ff ff ff    	mov    -0xa4(%rbp),%eax
    311b:	c1 e0 03             	shl    $0x3,%eax
    311e:	39 45 f0             	cmp    %eax,-0x10(%rbp)
    3121:	0f 82 95 fc ff ff    	jb     2dbc <fastDCT+0x492>
    3127:	90                   	nop
    3128:	c9                   	leaveq 
    3129:	c3                   	retq   

000000000000312a <fastIDCTInit>:
    312a:	55                   	push   %rbp
    312b:	48 89 e5             	mov    %rsp,%rbp
    312e:	48 83 ec 70          	sub    $0x70,%rsp
    3132:	48 89 7d 98          	mov    %rdi,-0x68(%rbp)
    3136:	48 89 75 90          	mov    %rsi,-0x70(%rbp)
    313a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    3141:	00 00 
    3143:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    3147:	31 c0                	xor    %eax,%eax
    3149:	c6 45 b0 00          	movb   $0x0,-0x50(%rbp)
    314d:	c6 45 b1 04          	movb   $0x4,-0x4f(%rbp)
    3151:	c6 45 b2 01          	movb   $0x1,-0x4e(%rbp)
    3155:	c6 45 b3 05          	movb   $0x5,-0x4d(%rbp)
    3159:	c6 45 b4 02          	movb   $0x2,-0x4c(%rbp)
    315d:	c6 45 b5 06          	movb   $0x6,-0x4b(%rbp)
    3161:	c6 45 b6 03          	movb   $0x3,-0x4a(%rbp)
    3165:	c6 45 b7 07          	movb   $0x7,-0x49(%rbp)
    3169:	c6 45 b8 08          	movb   $0x8,-0x48(%rbp)
    316d:	c6 45 b9 0c          	movb   $0xc,-0x47(%rbp)
    3171:	c6 45 ba 09          	movb   $0x9,-0x46(%rbp)
    3175:	c6 45 bb 0d          	movb   $0xd,-0x45(%rbp)
    3179:	c6 45 bc 0a          	movb   $0xa,-0x44(%rbp)
    317d:	c6 45 bd 0e          	movb   $0xe,-0x43(%rbp)
    3181:	c6 45 be 0b          	movb   $0xb,-0x42(%rbp)
    3185:	c6 45 bf 0f          	movb   $0xf,-0x41(%rbp)
    3189:	c6 45 c0 10          	movb   $0x10,-0x40(%rbp)
    318d:	c6 45 c1 14          	movb   $0x14,-0x3f(%rbp)
    3191:	c6 45 c2 11          	movb   $0x11,-0x3e(%rbp)
    3195:	c6 45 c3 15          	movb   $0x15,-0x3d(%rbp)
    3199:	c6 45 c4 12          	movb   $0x12,-0x3c(%rbp)
    319d:	c6 45 c5 16          	movb   $0x16,-0x3b(%rbp)
    31a1:	c6 45 c6 13          	movb   $0x13,-0x3a(%rbp)
    31a5:	c6 45 c7 17          	movb   $0x17,-0x39(%rbp)
    31a9:	c6 45 c8 18          	movb   $0x18,-0x38(%rbp)
    31ad:	c6 45 c9 1c          	movb   $0x1c,-0x37(%rbp)
    31b1:	c6 45 ca 19          	movb   $0x19,-0x36(%rbp)
    31b5:	c6 45 cb 1d          	movb   $0x1d,-0x35(%rbp)
    31b9:	c6 45 cc 1a          	movb   $0x1a,-0x34(%rbp)
    31bd:	c6 45 cd 1e          	movb   $0x1e,-0x33(%rbp)
    31c1:	c6 45 ce 1b          	movb   $0x1b,-0x32(%rbp)
    31c5:	c6 45 cf 1f          	movb   $0x1f,-0x31(%rbp)
    31c9:	c6 45 d0 20          	movb   $0x20,-0x30(%rbp)
    31cd:	c6 45 d1 24          	movb   $0x24,-0x2f(%rbp)
    31d1:	c6 45 d2 21          	movb   $0x21,-0x2e(%rbp)
    31d5:	c6 45 d3 25          	movb   $0x25,-0x2d(%rbp)
    31d9:	c6 45 d4 22          	movb   $0x22,-0x2c(%rbp)
    31dd:	c6 45 d5 26          	movb   $0x26,-0x2b(%rbp)
    31e1:	c6 45 d6 23          	movb   $0x23,-0x2a(%rbp)
    31e5:	c6 45 d7 27          	movb   $0x27,-0x29(%rbp)
    31e9:	c6 45 d8 28          	movb   $0x28,-0x28(%rbp)
    31ed:	c6 45 d9 2c          	movb   $0x2c,-0x27(%rbp)
    31f1:	c6 45 da 29          	movb   $0x29,-0x26(%rbp)
    31f5:	c6 45 db 2d          	movb   $0x2d,-0x25(%rbp)
    31f9:	c6 45 dc 2a          	movb   $0x2a,-0x24(%rbp)
    31fd:	c6 45 dd 2e          	movb   $0x2e,-0x23(%rbp)
    3201:	c6 45 de 2b          	movb   $0x2b,-0x22(%rbp)
    3205:	c6 45 df 2f          	movb   $0x2f,-0x21(%rbp)
    3209:	c6 45 e0 30          	movb   $0x30,-0x20(%rbp)
    320d:	c6 45 e1 34          	movb   $0x34,-0x1f(%rbp)
    3211:	c6 45 e2 31          	movb   $0x31,-0x1e(%rbp)
    3215:	c6 45 e3 35          	movb   $0x35,-0x1d(%rbp)
    3219:	c6 45 e4 32          	movb   $0x32,-0x1c(%rbp)
    321d:	c6 45 e5 36          	movb   $0x36,-0x1b(%rbp)
    3221:	c6 45 e6 33          	movb   $0x33,-0x1a(%rbp)
    3225:	c6 45 e7 37          	movb   $0x37,-0x19(%rbp)
    3229:	c6 45 e8 38          	movb   $0x38,-0x18(%rbp)
    322d:	c6 45 e9 3c          	movb   $0x3c,-0x17(%rbp)
    3231:	c6 45 ea 39          	movb   $0x39,-0x16(%rbp)
    3235:	c6 45 eb 3d          	movb   $0x3d,-0x15(%rbp)
    3239:	c6 45 ec 3a          	movb   $0x3a,-0x14(%rbp)
    323d:	c6 45 ed 3e          	movb   $0x3e,-0x13(%rbp)
    3241:	c6 45 ee 3b          	movb   $0x3b,-0x12(%rbp)
    3245:	c6 45 ef 3f          	movb   $0x3f,-0x11(%rbp)
    3249:	c7 45 ac 00 00 00 00 	movl   $0x0,-0x54(%rbp)
    3250:	eb 34                	jmp    3286 <fastIDCTInit+0x15c>
    3252:	8b 45 ac             	mov    -0x54(%rbp),%eax
    3255:	48 98                	cltq   
    3257:	48 8d 14 00          	lea    (%rax,%rax,1),%rdx
    325b:	48 8b 45 98          	mov    -0x68(%rbp),%rax
    325f:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx
    3263:	8b 45 ac             	mov    -0x54(%rbp),%eax
    3266:	48 98                	cltq   
    3268:	0f b6 44 05 b0       	movzbl -0x50(%rbp,%rax,1),%eax
    326d:	48 0f be c0          	movsbq %al,%rax
    3271:	48 8d 14 00          	lea    (%rax,%rax,1),%rdx
    3275:	48 8b 45 90          	mov    -0x70(%rbp),%rax
    3279:	48 01 c2             	add    %rax,%rdx
    327c:	0f b7 01             	movzwl (%rcx),%eax
    327f:	66 89 02             	mov    %ax,(%rdx)
    3282:	83 45 ac 01          	addl   $0x1,-0x54(%rbp)
    3286:	83 7d ac 3f          	cmpl   $0x3f,-0x54(%rbp)
    328a:	7e c6                	jle    3252 <fastIDCTInit+0x128>
    328c:	90                   	nop
    328d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    3291:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    3298:	00 00 
    329a:	74 05                	je     32a1 <fastIDCTInit+0x177>
    329c:	e8 1f d3 ff ff       	callq  5c0 <__stack_chk_fail@plt>
    32a1:	c9                   	leaveq 
    32a2:	c3                   	retq   

00000000000032a3 <fastIDCT>:
    32a3:	55                   	push   %rbp
    32a4:	48 89 e5             	mov    %rsp,%rbp
    32a7:	53                   	push   %rbx
    32a8:	48 89 7d 80          	mov    %rdi,-0x80(%rbp)
    32ac:	48 8b 5d 80          	mov    -0x80(%rbp),%rbx
    32b0:	c7 45 b4 07 00 00 00 	movl   $0x7,-0x4c(%rbp)
    32b7:	e9 13 0b 00 00       	jmpq   3dcf <F_1_847+0x2ae>
    32bc:	48 89 5d f0          	mov    %rbx,-0x10(%rbp)
    32c0:	0f b7 03             	movzwl (%rbx),%eax
    32c3:	98                   	cwtl   
    32c4:	89 45 b8             	mov    %eax,-0x48(%rbp)
    32c7:	48 8d 43 02          	lea    0x2(%rbx),%rax
    32cb:	0f b7 00             	movzwl (%rax),%eax
    32ce:	98                   	cwtl   
    32cf:	89 45 c0             	mov    %eax,-0x40(%rbp)
    32d2:	48 8d 43 04          	lea    0x4(%rbx),%rax
    32d6:	0f b7 00             	movzwl (%rax),%eax
    32d9:	98                   	cwtl   
    32da:	89 45 c8             	mov    %eax,-0x38(%rbp)
    32dd:	48 8d 43 06          	lea    0x6(%rbx),%rax
    32e1:	0f b7 00             	movzwl (%rax),%eax
    32e4:	98                   	cwtl   
    32e5:	89 45 d0             	mov    %eax,-0x30(%rbp)
    32e8:	48 8d 43 08          	lea    0x8(%rbx),%rax
    32ec:	0f b7 00             	movzwl (%rax),%eax
    32ef:	98                   	cwtl   
    32f0:	89 45 bc             	mov    %eax,-0x44(%rbp)
    32f3:	48 8d 43 0a          	lea    0xa(%rbx),%rax
    32f7:	0f b7 00             	movzwl (%rax),%eax
    32fa:	98                   	cwtl   
    32fb:	89 45 c4             	mov    %eax,-0x3c(%rbp)
    32fe:	48 8d 43 0c          	lea    0xc(%rbx),%rax
    3302:	0f b7 00             	movzwl (%rax),%eax
    3305:	98                   	cwtl   
    3306:	89 45 cc             	mov    %eax,-0x34(%rbp)
    3309:	48 8d 43 0e          	lea    0xe(%rbx),%rax
    330d:	0f b7 00             	movzwl (%rax),%eax
    3310:	98                   	cwtl   
    3311:	89 45 d4             	mov    %eax,-0x2c(%rbp)
    3314:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3317:	0b 45 c0             	or     -0x40(%rbp),%eax
    331a:	0b 45 c4             	or     -0x3c(%rbp),%eax
    331d:	0b 45 c8             	or     -0x38(%rbp),%eax
    3320:	0b 45 cc             	or     -0x34(%rbp),%eax
    3323:	0b 45 d0             	or     -0x30(%rbp),%eax
    3326:	0b 45 d4             	or     -0x2c(%rbp),%eax
    3329:	85 c0                	test   %eax,%eax
    332b:	75 5c                	jne    3389 <fastIDCT+0xe6>
    332d:	83 7d b8 00          	cmpl   $0x0,-0x48(%rbp)
    3331:	74 4d                	je     3380 <fastIDCT+0xdd>
    3333:	8b 45 b8             	mov    -0x48(%rbp),%eax
    3336:	c1 e0 02             	shl    $0x2,%eax
    3339:	66 89 45 92          	mov    %ax,-0x6e(%rbp)
    333d:	0f bf 45 92          	movswl -0x6e(%rbp),%eax
    3341:	0f b7 c0             	movzwl %ax,%eax
    3344:	0f bf 55 92          	movswl -0x6e(%rbp),%edx
    3348:	c1 e2 10             	shl    $0x10,%edx
    334b:	09 d0                	or     %edx,%eax
    334d:	89 45 ec             	mov    %eax,-0x14(%rbp)
    3350:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    3354:	8b 55 ec             	mov    -0x14(%rbp),%edx
    3357:	89 10                	mov    %edx,(%rax)
    3359:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    335d:	48 8d 50 04          	lea    0x4(%rax),%rdx
    3361:	8b 45 ec             	mov    -0x14(%rbp),%eax
    3364:	89 02                	mov    %eax,(%rdx)
    3366:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    336a:	48 8d 50 08          	lea    0x8(%rax),%rdx
    336e:	8b 45 ec             	mov    -0x14(%rbp),%eax
    3371:	89 02                	mov    %eax,(%rdx)
    3373:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    3377:	48 8d 50 0c          	lea    0xc(%rax),%rdx
    337b:	8b 45 ec             	mov    -0x14(%rbp),%eax
    337e:	89 02                	mov    %eax,(%rdx)
    3380:	48 83 c3 10          	add    $0x10,%rbx
    3384:	e9 42 0a 00 00       	jmpq   3dcb <F_1_847+0x2aa>
    3389:	83 7d d0 00          	cmpl   $0x0,-0x30(%rbp)
    338d:	0f 84 df 00 00 00    	je     3472 <fastIDCT+0x1cf>
    3393:	83 7d c0 00          	cmpl   $0x0,-0x40(%rbp)
    3397:	74 7a                	je     3413 <fastIDCT+0x170>
    3399:	8b 55 c0             	mov    -0x40(%rbp),%edx
    339c:	8b 45 d0             	mov    -0x30(%rbp),%eax
    339f:	01 d0                	add    %edx,%eax
    33a1:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    33a7:	89 45 d8             	mov    %eax,-0x28(%rbp)
    33aa:	8b 45 d0             	mov    -0x30(%rbp),%eax
    33ad:	69 d0 df c4 ff ff    	imul   $0xffffc4df,%eax,%edx
    33b3:	8b 45 d8             	mov    -0x28(%rbp),%eax
    33b6:	01 d0                	add    %edx,%eax
    33b8:	89 45 9c             	mov    %eax,-0x64(%rbp)
    33bb:	8b 45 c0             	mov    -0x40(%rbp),%eax
    33be:	69 d0 7e 18 00 00    	imul   $0x187e,%eax,%edx
    33c4:	8b 45 d8             	mov    -0x28(%rbp),%eax
    33c7:	01 d0                	add    %edx,%eax
    33c9:	89 45 a0             	mov    %eax,-0x60(%rbp)
    33cc:	8b 55 b8             	mov    -0x48(%rbp),%edx
    33cf:	8b 45 c8             	mov    -0x38(%rbp),%eax
    33d2:	01 d0                	add    %edx,%eax
    33d4:	c1 e0 0d             	shl    $0xd,%eax
    33d7:	89 45 94             	mov    %eax,-0x6c(%rbp)
    33da:	8b 45 b8             	mov    -0x48(%rbp),%eax
    33dd:	2b 45 c8             	sub    -0x38(%rbp),%eax
    33e0:	c1 e0 0d             	shl    $0xd,%eax
    33e3:	89 45 98             	mov    %eax,-0x68(%rbp)
    33e6:	8b 55 94             	mov    -0x6c(%rbp),%edx
    33e9:	8b 45 a0             	mov    -0x60(%rbp),%eax
    33ec:	01 d0                	add    %edx,%eax
    33ee:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    33f1:	8b 45 94             	mov    -0x6c(%rbp),%eax
    33f4:	2b 45 a0             	sub    -0x60(%rbp),%eax
    33f7:	89 45 b0             	mov    %eax,-0x50(%rbp)
    33fa:	8b 55 98             	mov    -0x68(%rbp),%edx
    33fd:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3400:	01 d0                	add    %edx,%eax
    3402:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3405:	8b 45 98             	mov    -0x68(%rbp),%eax
    3408:	2b 45 9c             	sub    -0x64(%rbp),%eax
    340b:	89 45 ac             	mov    %eax,-0x54(%rbp)
    340e:	e9 e7 00 00 00       	jmpq   34fa <fastIDCT+0x257>
    3413:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3416:	69 c0 31 d6 ff ff    	imul   $0xffffd631,%eax,%eax
    341c:	89 45 9c             	mov    %eax,-0x64(%rbp)
    341f:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3422:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    3428:	89 45 a0             	mov    %eax,-0x60(%rbp)
    342b:	8b 55 b8             	mov    -0x48(%rbp),%edx
    342e:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3431:	01 d0                	add    %edx,%eax
    3433:	c1 e0 0d             	shl    $0xd,%eax
    3436:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3439:	8b 45 b8             	mov    -0x48(%rbp),%eax
    343c:	2b 45 c8             	sub    -0x38(%rbp),%eax
    343f:	c1 e0 0d             	shl    $0xd,%eax
    3442:	89 45 98             	mov    %eax,-0x68(%rbp)
    3445:	8b 55 94             	mov    -0x6c(%rbp),%edx
    3448:	8b 45 a0             	mov    -0x60(%rbp),%eax
    344b:	01 d0                	add    %edx,%eax
    344d:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    3450:	8b 45 94             	mov    -0x6c(%rbp),%eax
    3453:	2b 45 a0             	sub    -0x60(%rbp),%eax
    3456:	89 45 b0             	mov    %eax,-0x50(%rbp)
    3459:	8b 55 98             	mov    -0x68(%rbp),%edx
    345c:	8b 45 9c             	mov    -0x64(%rbp),%eax
    345f:	01 d0                	add    %edx,%eax
    3461:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3464:	8b 45 98             	mov    -0x68(%rbp),%eax
    3467:	2b 45 9c             	sub    -0x64(%rbp),%eax
    346a:	89 45 ac             	mov    %eax,-0x54(%rbp)
    346d:	e9 88 00 00 00       	jmpq   34fa <fastIDCT+0x257>
    3472:	83 7d c0 00          	cmpl   $0x0,-0x40(%rbp)
    3476:	74 5c                	je     34d4 <fastIDCT+0x231>
    3478:	8b 45 c0             	mov    -0x40(%rbp),%eax
    347b:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    3481:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3484:	8b 45 c0             	mov    -0x40(%rbp),%eax
    3487:	69 c0 cf 29 00 00    	imul   $0x29cf,%eax,%eax
    348d:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3490:	8b 55 b8             	mov    -0x48(%rbp),%edx
    3493:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3496:	01 d0                	add    %edx,%eax
    3498:	c1 e0 0d             	shl    $0xd,%eax
    349b:	89 45 94             	mov    %eax,-0x6c(%rbp)
    349e:	8b 45 b8             	mov    -0x48(%rbp),%eax
    34a1:	2b 45 c8             	sub    -0x38(%rbp),%eax
    34a4:	c1 e0 0d             	shl    $0xd,%eax
    34a7:	89 45 98             	mov    %eax,-0x68(%rbp)
    34aa:	8b 55 94             	mov    -0x6c(%rbp),%edx
    34ad:	8b 45 a0             	mov    -0x60(%rbp),%eax
    34b0:	01 d0                	add    %edx,%eax
    34b2:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    34b5:	8b 45 94             	mov    -0x6c(%rbp),%eax
    34b8:	2b 45 a0             	sub    -0x60(%rbp),%eax
    34bb:	89 45 b0             	mov    %eax,-0x50(%rbp)
    34be:	8b 55 98             	mov    -0x68(%rbp),%edx
    34c1:	8b 45 9c             	mov    -0x64(%rbp),%eax
    34c4:	01 d0                	add    %edx,%eax
    34c6:	89 45 a8             	mov    %eax,-0x58(%rbp)
    34c9:	8b 45 98             	mov    -0x68(%rbp),%eax
    34cc:	2b 45 9c             	sub    -0x64(%rbp),%eax
    34cf:	89 45 ac             	mov    %eax,-0x54(%rbp)
    34d2:	eb 26                	jmp    34fa <fastIDCT+0x257>
    34d4:	8b 55 b8             	mov    -0x48(%rbp),%edx
    34d7:	8b 45 c8             	mov    -0x38(%rbp),%eax
    34da:	01 d0                	add    %edx,%eax
    34dc:	c1 e0 0d             	shl    $0xd,%eax
    34df:	89 45 b0             	mov    %eax,-0x50(%rbp)
    34e2:	8b 45 b0             	mov    -0x50(%rbp),%eax
    34e5:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    34e8:	8b 45 b8             	mov    -0x48(%rbp),%eax
    34eb:	2b 45 c8             	sub    -0x38(%rbp),%eax
    34ee:	c1 e0 0d             	shl    $0xd,%eax
    34f1:	89 45 ac             	mov    %eax,-0x54(%rbp)
    34f4:	8b 45 ac             	mov    -0x54(%rbp),%eax
    34f7:	89 45 a8             	mov    %eax,-0x58(%rbp)
    34fa:	83 7d d4 00          	cmpl   $0x0,-0x2c(%rbp)
    34fe:	0f 84 f9 04 00 00    	je     39fd <fastIDCT+0x75a>
    3504:	83 7d cc 00          	cmpl   $0x0,-0x34(%rbp)
    3508:	0f 84 f4 02 00 00    	je     3802 <fastIDCT+0x55f>
    350e:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    3512:	0f 84 9c 01 00 00    	je     36b4 <fastIDCT+0x411>
    3518:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    351c:	0f 84 da 00 00 00    	je     35fc <fastIDCT+0x359>
    3522:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3525:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3528:	01 d0                	add    %edx,%eax
    352a:	89 45 d8             	mov    %eax,-0x28(%rbp)
    352d:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3530:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3533:	01 d0                	add    %edx,%eax
    3535:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3538:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    353b:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    353e:	01 d0                	add    %edx,%eax
    3540:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3543:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3546:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3549:	01 d0                	add    %edx,%eax
    354b:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    354e:	8b 55 e8             	mov    -0x18(%rbp),%edx
    3551:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3554:	01 d0                	add    %edx,%eax
    3556:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    355c:	89 45 dc             	mov    %eax,-0x24(%rbp)
    355f:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3562:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    3568:	89 45 94             	mov    %eax,-0x6c(%rbp)
    356b:	8b 45 cc             	mov    -0x34(%rbp),%eax
    356e:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    3574:	89 45 98             	mov    %eax,-0x68(%rbp)
    3577:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    357a:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    3580:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3583:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3586:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    358c:	89 45 a0             	mov    %eax,-0x60(%rbp)
    358f:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3592:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    3598:	89 45 d8             	mov    %eax,-0x28(%rbp)
    359b:	8b 45 e0             	mov    -0x20(%rbp),%eax
    359e:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    35a4:	89 45 e0             	mov    %eax,-0x20(%rbp)
    35a7:	8b 45 e8             	mov    -0x18(%rbp),%eax
    35aa:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    35b0:	89 45 e8             	mov    %eax,-0x18(%rbp)
    35b3:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    35b6:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    35bc:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    35bf:	8b 45 dc             	mov    -0x24(%rbp),%eax
    35c2:	01 45 e8             	add    %eax,-0x18(%rbp)
    35c5:	8b 45 dc             	mov    -0x24(%rbp),%eax
    35c8:	01 45 e4             	add    %eax,-0x1c(%rbp)
    35cb:	8b 55 d8             	mov    -0x28(%rbp),%edx
    35ce:	8b 45 e8             	mov    -0x18(%rbp),%eax
    35d1:	01 d0                	add    %edx,%eax
    35d3:	01 45 94             	add    %eax,-0x6c(%rbp)
    35d6:	8b 55 e0             	mov    -0x20(%rbp),%edx
    35d9:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    35dc:	01 d0                	add    %edx,%eax
    35de:	01 45 98             	add    %eax,-0x68(%rbp)
    35e1:	8b 55 e0             	mov    -0x20(%rbp),%edx
    35e4:	8b 45 e8             	mov    -0x18(%rbp),%eax
    35e7:	01 d0                	add    %edx,%eax
    35e9:	01 45 9c             	add    %eax,-0x64(%rbp)
    35ec:	8b 55 d8             	mov    -0x28(%rbp),%edx
    35ef:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    35f2:	01 d0                	add    %edx,%eax
    35f4:	01 45 a0             	add    %eax,-0x60(%rbp)
    35f7:	e9 11 07 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    35fc:	8b 55 cc             	mov    -0x34(%rbp),%edx
    35ff:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3602:	01 d0                	add    %edx,%eax
    3604:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3607:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    360a:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    360d:	01 d0                	add    %edx,%eax
    360f:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3612:	8b 55 e8             	mov    -0x18(%rbp),%edx
    3615:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3618:	01 d0                	add    %edx,%eax
    361a:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3620:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3623:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3626:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    362c:	89 45 94             	mov    %eax,-0x6c(%rbp)
    362f:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3632:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    3638:	89 45 98             	mov    %eax,-0x68(%rbp)
    363b:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    363e:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    3644:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3647:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    364a:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    3650:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3653:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3656:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    365c:	89 45 e0             	mov    %eax,-0x20(%rbp)
    365f:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3662:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    3668:	89 45 e8             	mov    %eax,-0x18(%rbp)
    366b:	8b 45 cc             	mov    -0x34(%rbp),%eax
    366e:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    3674:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3677:	8b 45 dc             	mov    -0x24(%rbp),%eax
    367a:	01 45 e8             	add    %eax,-0x18(%rbp)
    367d:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3680:	01 45 e4             	add    %eax,-0x1c(%rbp)
    3683:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3686:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3689:	01 d0                	add    %edx,%eax
    368b:	01 45 94             	add    %eax,-0x6c(%rbp)
    368e:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3691:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3694:	01 d0                	add    %edx,%eax
    3696:	01 45 98             	add    %eax,-0x68(%rbp)
    3699:	8b 55 e0             	mov    -0x20(%rbp),%edx
    369c:	8b 45 e8             	mov    -0x18(%rbp),%eax
    369f:	01 d0                	add    %edx,%eax
    36a1:	01 45 9c             	add    %eax,-0x64(%rbp)
    36a4:	8b 55 d8             	mov    -0x28(%rbp),%edx
    36a7:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    36aa:	01 d0                	add    %edx,%eax
    36ac:	89 45 a0             	mov    %eax,-0x60(%rbp)
    36af:	e9 59 06 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    36b4:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    36b8:	0f 84 b8 00 00 00    	je     3776 <fastIDCT+0x4d3>
    36be:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    36c1:	8b 45 bc             	mov    -0x44(%rbp),%eax
    36c4:	01 d0                	add    %edx,%eax
    36c6:	89 45 d8             	mov    %eax,-0x28(%rbp)
    36c9:	8b 55 cc             	mov    -0x34(%rbp),%edx
    36cc:	8b 45 bc             	mov    -0x44(%rbp),%eax
    36cf:	01 d0                	add    %edx,%eax
    36d1:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    36d4:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    36d7:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    36da:	01 d0                	add    %edx,%eax
    36dc:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    36e2:	89 45 dc             	mov    %eax,-0x24(%rbp)
    36e5:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    36e8:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    36ee:	89 45 94             	mov    %eax,-0x6c(%rbp)
    36f1:	8b 45 cc             	mov    -0x34(%rbp),%eax
    36f4:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    36fa:	89 45 98             	mov    %eax,-0x68(%rbp)
    36fd:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3700:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    3706:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3709:	8b 45 d8             	mov    -0x28(%rbp),%eax
    370c:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    3712:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3715:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3718:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    371e:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3721:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3724:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    372a:	89 45 e8             	mov    %eax,-0x18(%rbp)
    372d:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3730:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    3736:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3739:	8b 45 dc             	mov    -0x24(%rbp),%eax
    373c:	01 45 e8             	add    %eax,-0x18(%rbp)
    373f:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3742:	01 45 e4             	add    %eax,-0x1c(%rbp)
    3745:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3748:	8b 45 e8             	mov    -0x18(%rbp),%eax
    374b:	01 d0                	add    %edx,%eax
    374d:	01 45 94             	add    %eax,-0x6c(%rbp)
    3750:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3753:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3756:	01 d0                	add    %edx,%eax
    3758:	01 45 98             	add    %eax,-0x68(%rbp)
    375b:	8b 55 e0             	mov    -0x20(%rbp),%edx
    375e:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3761:	01 d0                	add    %edx,%eax
    3763:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3766:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3769:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    376c:	01 d0                	add    %edx,%eax
    376e:	01 45 a0             	add    %eax,-0x60(%rbp)
    3771:	e9 97 05 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3776:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3779:	69 c0 c2 ec ff ff    	imul   $0xffffecc2,%eax,%eax
    377f:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3782:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3785:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    378b:	89 45 d8             	mov    %eax,-0x28(%rbp)
    378e:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3791:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    3797:	89 45 e8             	mov    %eax,-0x18(%rbp)
    379a:	8b 45 cc             	mov    -0x34(%rbp),%eax
    379d:	69 c0 b0 ef ff ff    	imul   $0xffffefb0,%eax,%eax
    37a3:	89 45 98             	mov    %eax,-0x68(%rbp)
    37a6:	8b 45 cc             	mov    -0x34(%rbp),%eax
    37a9:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    37af:	89 45 e0             	mov    %eax,-0x20(%rbp)
    37b2:	8b 45 cc             	mov    -0x34(%rbp),%eax
    37b5:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    37bb:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    37be:	8b 55 cc             	mov    -0x34(%rbp),%edx
    37c1:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    37c4:	01 d0                	add    %edx,%eax
    37c6:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    37cc:	89 45 dc             	mov    %eax,-0x24(%rbp)
    37cf:	8b 45 dc             	mov    -0x24(%rbp),%eax
    37d2:	01 45 e8             	add    %eax,-0x18(%rbp)
    37d5:	8b 45 dc             	mov    -0x24(%rbp),%eax
    37d8:	01 45 e4             	add    %eax,-0x1c(%rbp)
    37db:	8b 45 e8             	mov    -0x18(%rbp),%eax
    37de:	01 45 94             	add    %eax,-0x6c(%rbp)
    37e1:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    37e4:	01 45 98             	add    %eax,-0x68(%rbp)
    37e7:	8b 55 e0             	mov    -0x20(%rbp),%edx
    37ea:	8b 45 e8             	mov    -0x18(%rbp),%eax
    37ed:	01 d0                	add    %edx,%eax
    37ef:	89 45 9c             	mov    %eax,-0x64(%rbp)
    37f2:	8b 55 d8             	mov    -0x28(%rbp),%edx
    37f5:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    37f8:	01 d0                	add    %edx,%eax
    37fa:	89 45 a0             	mov    %eax,-0x60(%rbp)
    37fd:	e9 0b 05 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3802:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    3806:	0f 84 3c 01 00 00    	je     3948 <fastIDCT+0x6a5>
    380c:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3810:	0f 84 b8 00 00 00    	je     38ce <fastIDCT+0x62b>
    3816:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3819:	8b 45 bc             	mov    -0x44(%rbp),%eax
    381c:	01 d0                	add    %edx,%eax
    381e:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3821:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3824:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3827:	01 d0                	add    %edx,%eax
    3829:	89 45 e8             	mov    %eax,-0x18(%rbp)
    382c:	8b 55 e8             	mov    -0x18(%rbp),%edx
    382f:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3832:	01 d0                	add    %edx,%eax
    3834:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    383a:	89 45 dc             	mov    %eax,-0x24(%rbp)
    383d:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3840:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    3846:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3849:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    384c:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    3852:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3855:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3858:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    385e:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3861:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3864:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    386a:	89 45 d8             	mov    %eax,-0x28(%rbp)
    386d:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3870:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    3876:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3879:	8b 45 e8             	mov    -0x18(%rbp),%eax
    387c:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    3882:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3885:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3888:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    388e:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3891:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3894:	01 45 e8             	add    %eax,-0x18(%rbp)
    3897:	8b 45 dc             	mov    -0x24(%rbp),%eax
    389a:	01 45 e4             	add    %eax,-0x1c(%rbp)
    389d:	8b 55 d8             	mov    -0x28(%rbp),%edx
    38a0:	8b 45 e8             	mov    -0x18(%rbp),%eax
    38a3:	01 d0                	add    %edx,%eax
    38a5:	01 45 94             	add    %eax,-0x6c(%rbp)
    38a8:	8b 55 e0             	mov    -0x20(%rbp),%edx
    38ab:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    38ae:	01 d0                	add    %edx,%eax
    38b0:	89 45 98             	mov    %eax,-0x68(%rbp)
    38b3:	8b 55 e0             	mov    -0x20(%rbp),%edx
    38b6:	8b 45 e8             	mov    -0x18(%rbp),%eax
    38b9:	01 d0                	add    %edx,%eax
    38bb:	01 45 9c             	add    %eax,-0x64(%rbp)
    38be:	8b 55 d8             	mov    -0x28(%rbp),%edx
    38c1:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    38c4:	01 d0                	add    %edx,%eax
    38c6:	01 45 a0             	add    %eax,-0x60(%rbp)
    38c9:	e9 3f 04 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    38ce:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    38d1:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    38d4:	01 d0                	add    %edx,%eax
    38d6:	89 45 e8             	mov    %eax,-0x18(%rbp)
    38d9:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    38dc:	69 c0 c2 ec ff ff    	imul   $0xffffecc2,%eax,%eax
    38e2:	89 45 94             	mov    %eax,-0x6c(%rbp)
    38e5:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    38e8:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    38ee:	89 45 d8             	mov    %eax,-0x28(%rbp)
    38f1:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    38f4:	69 c0 50 10 00 00    	imul   $0x1050,%eax,%eax
    38fa:	89 45 9c             	mov    %eax,-0x64(%rbp)
    38fd:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3900:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    3906:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3909:	8b 45 e8             	mov    -0x18(%rbp),%eax
    390c:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3912:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3915:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3918:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    391e:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3921:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3924:	01 45 94             	add    %eax,-0x6c(%rbp)
    3927:	8b 55 e0             	mov    -0x20(%rbp),%edx
    392a:	8b 45 dc             	mov    -0x24(%rbp),%eax
    392d:	01 d0                	add    %edx,%eax
    392f:	89 45 98             	mov    %eax,-0x68(%rbp)
    3932:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3935:	01 45 9c             	add    %eax,-0x64(%rbp)
    3938:	8b 55 d8             	mov    -0x28(%rbp),%edx
    393b:	8b 45 dc             	mov    -0x24(%rbp),%eax
    393e:	01 d0                	add    %edx,%eax
    3940:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3943:	e9 c5 03 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3948:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    394c:	74 7a                	je     39c8 <fastIDCT+0x725>
    394e:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3951:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3954:	01 d0                	add    %edx,%eax
    3956:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3959:	8b 45 d8             	mov    -0x28(%rbp),%eax
    395c:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3962:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3965:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3968:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    396e:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3971:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3974:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    397a:	89 45 e8             	mov    %eax,-0x18(%rbp)
    397d:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    3980:	69 c0 c9 ca ff ff    	imul   $0xffffcac9,%eax,%eax
    3986:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3989:	8b 45 bc             	mov    -0x44(%rbp),%eax
    398c:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    3992:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3995:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3998:	69 c0 8e 23 00 00    	imul   $0x238e,%eax,%eax
    399e:	89 45 a0             	mov    %eax,-0x60(%rbp)
    39a1:	8b 45 d8             	mov    -0x28(%rbp),%eax
    39a4:	01 45 94             	add    %eax,-0x6c(%rbp)
    39a7:	8b 55 e4             	mov    -0x1c(%rbp),%edx
    39aa:	8b 45 dc             	mov    -0x24(%rbp),%eax
    39ad:	01 d0                	add    %edx,%eax
    39af:	89 45 98             	mov    %eax,-0x68(%rbp)
    39b2:	8b 55 e8             	mov    -0x18(%rbp),%edx
    39b5:	8b 45 dc             	mov    -0x24(%rbp),%eax
    39b8:	01 d0                	add    %edx,%eax
    39ba:	89 45 9c             	mov    %eax,-0x64(%rbp)
    39bd:	8b 45 d8             	mov    -0x28(%rbp),%eax
    39c0:	01 45 a0             	add    %eax,-0x60(%rbp)
    39c3:	e9 45 03 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    39c8:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    39cb:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    39d1:	89 45 94             	mov    %eax,-0x6c(%rbp)
    39d4:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    39d7:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    39dd:	89 45 98             	mov    %eax,-0x68(%rbp)
    39e0:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    39e3:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    39e9:	89 45 9c             	mov    %eax,-0x64(%rbp)
    39ec:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    39ef:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    39f5:	89 45 a0             	mov    %eax,-0x60(%rbp)
    39f8:	e9 10 03 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    39fd:	83 7d cc 00          	cmpl   $0x0,-0x34(%rbp)
    3a01:	0f 84 fb 01 00 00    	je     3c02 <F_1_847+0xe1>
    3a07:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    3a0b:	0f 84 3c 01 00 00    	je     3b4d <F_1_847+0x2c>
    3a11:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3a15:	0f 84 b8 00 00 00    	je     3ad3 <fastIDCT+0x830>
    3a1b:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3a1e:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3a21:	01 d0                	add    %edx,%eax
    3a23:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3a26:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3a29:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3a2c:	01 d0                	add    %edx,%eax
    3a2e:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3a31:	8b 55 c4             	mov    -0x3c(%rbp),%edx
    3a34:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3a37:	01 d0                	add    %edx,%eax
    3a39:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3a3f:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3a42:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3a45:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    3a4b:	89 45 98             	mov    %eax,-0x68(%rbp)
    3a4e:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3a51:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    3a57:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3a5a:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3a5d:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    3a63:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3a66:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3a69:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    3a6f:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3a72:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3a75:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    3a7b:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3a7e:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3a81:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    3a87:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3a8a:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3a8d:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    3a93:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3a96:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3a99:	01 45 e8             	add    %eax,-0x18(%rbp)
    3a9c:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3a9f:	01 45 e4             	add    %eax,-0x1c(%rbp)
    3aa2:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3aa5:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3aa8:	01 d0                	add    %edx,%eax
    3aaa:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3aad:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3ab0:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3ab3:	01 d0                	add    %edx,%eax
    3ab5:	01 45 98             	add    %eax,-0x68(%rbp)
    3ab8:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3abb:	8b 45 e8             	mov    -0x18(%rbp),%eax
    3abe:	01 d0                	add    %edx,%eax
    3ac0:	01 45 9c             	add    %eax,-0x64(%rbp)
    3ac3:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3ac6:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3ac9:	01 d0                	add    %edx,%eax
    3acb:	01 45 a0             	add    %eax,-0x60(%rbp)
    3ace:	e9 3a 02 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3ad3:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3ad6:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3ad9:	01 d0                	add    %edx,%eax
    3adb:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3ade:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3ae1:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3ae7:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3aea:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3aed:	69 c0 37 35 00 00    	imul   $0x3537,%eax,%eax
    3af3:	89 45 98             	mov    %eax,-0x68(%rbp)
    3af6:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3af9:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    3aff:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3b02:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3b05:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    3b0b:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3b0e:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3b11:	69 c0 8e 23 00 00    	imul   $0x238e,%eax,%eax
    3b17:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3b1a:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3b1d:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    3b23:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3b26:	8b 55 e8             	mov    -0x18(%rbp),%edx
    3b29:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3b2c:	01 d0                	add    %edx,%eax
    3b2e:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3b31:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3b34:	01 45 98             	add    %eax,-0x68(%rbp)
    3b37:	8b 45 e0             	mov    -0x20(%rbp),%eax
    3b3a:	01 45 9c             	add    %eax,-0x64(%rbp)
    3b3d:	8b 55 e4             	mov    -0x1c(%rbp),%edx
    3b40:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3b43:	01 d0                	add    %edx,%eax
    3b45:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3b48:	e9 c0 01 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3b4d:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3b51:	74 7a                	je     3bcd <F_1_847+0xac>
    3b53:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3b56:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3b59:	01 d0                	add    %edx,%eax
    3b5b:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3b5e:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3b61:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3b67:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3b6a:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3b6d:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    3b73:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3b76:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3b79:	69 c0 3e 13 00 00    	imul   $0x133e,%eax,%eax
    3b7f:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3b82:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3b85:	69 c0 b0 ef ff ff    	imul   $0xffffefb0,%eax,%eax
    3b8b:	89 45 98             	mov    %eax,-0x68(%rbp)
    3b8e:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3b91:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    3b97:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3b9a:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3b9d:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    3ba3:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3ba6:	8b 55 d8             	mov    -0x28(%rbp),%edx
    3ba9:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3bac:	01 d0                	add    %edx,%eax
    3bae:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3bb1:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3bb4:	01 45 98             	add    %eax,-0x68(%rbp)
    3bb7:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3bba:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3bbd:	01 d0                	add    %edx,%eax
    3bbf:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3bc2:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3bc5:	01 45 a0             	add    %eax,-0x60(%rbp)
    3bc8:	e9 40 01 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3bcd:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3bd0:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3bd6:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3bd9:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3bdc:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    3be2:	89 45 98             	mov    %eax,-0x68(%rbp)
    3be5:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3be8:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    3bee:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3bf1:	8b 45 cc             	mov    -0x34(%rbp),%eax
    3bf4:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    3bfa:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3bfd:	e9 0b 01 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3c02:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    3c06:	0f 84 b0 00 00 00    	je     3cbc <F_1_847+0x19b>
    3c0c:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3c10:	74 78                	je     3c8a <F_1_847+0x169>
    3c12:	8b 55 bc             	mov    -0x44(%rbp),%edx
    3c15:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3c18:	01 d0                	add    %edx,%eax
    3c1a:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3c1d:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3c20:	69 c0 c2 06 00 00    	imul   $0x6c2,%eax,%eax
    3c26:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3c29:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3c2c:	69 c0 8b d1 ff ff    	imul   $0xffffd18b,%eax,%eax
    3c32:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3c35:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3c38:	69 c0 f9 21 00 00    	imul   $0x21f9,%eax,%eax
    3c3e:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3c41:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3c44:	69 c0 79 ba ff ff    	imul   $0xffffba79,%eax,%eax
    3c4a:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3c4d:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3c50:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    3c56:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    3c59:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3c5c:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3c62:	89 45 dc             	mov    %eax,-0x24(%rbp)
    3c65:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3c68:	2b 45 e4             	sub    -0x1c(%rbp),%eax
    3c6b:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3c6e:	8b 55 e0             	mov    -0x20(%rbp),%edx
    3c71:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    3c74:	01 d0                	add    %edx,%eax
    3c76:	89 45 98             	mov    %eax,-0x68(%rbp)
    3c79:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3c7c:	01 45 9c             	add    %eax,-0x64(%rbp)
    3c7f:	8b 45 dc             	mov    -0x24(%rbp),%eax
    3c82:	01 45 a0             	add    %eax,-0x60(%rbp)
    3c85:	e9 83 00 00 00       	jmpq   3d0d <F_1_847+0x1ec>
    3c8a:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3c8d:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    3c93:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3c96:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3c99:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    3c9f:	89 45 98             	mov    %eax,-0x68(%rbp)
    3ca2:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3ca5:	69 c0 2c f7 ff ff    	imul   $0xfffff72c,%eax,%eax
    3cab:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3cae:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3cb1:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3cb7:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3cba:	eb 51                	jmp    3d0d <F_1_847+0x1ec>
    3cbc:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3cc0:	74 32                	je     3cf4 <F_1_847+0x1d3>
    3cc2:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3cc5:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    3ccb:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3cce:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3cd1:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    3cd7:	89 45 98             	mov    %eax,-0x68(%rbp)
    3cda:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3cdd:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    3ce3:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3ce6:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3ce9:	69 c0 63 2c 00 00    	imul   $0x2c63,%eax,%eax
    3cef:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3cf2:	eb 19                	jmp    3d0d <F_1_847+0x1ec>
    3cf4:	c7 45 a0 00 00 00 00 	movl   $0x0,-0x60(%rbp)
    3cfb:	8b 45 a0             	mov    -0x60(%rbp),%eax
    3cfe:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3d01:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3d04:	89 45 98             	mov    %eax,-0x68(%rbp)
    3d07:	8b 45 98             	mov    -0x68(%rbp),%eax
    3d0a:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3d0d:	8b 55 a4             	mov    -0x5c(%rbp),%edx
    3d10:	8b 45 a0             	mov    -0x60(%rbp),%eax
    3d13:	01 d0                	add    %edx,%eax
    3d15:	05 00 04 00 00       	add    $0x400,%eax
    3d1a:	c1 f8 0b             	sar    $0xb,%eax
    3d1d:	66 89 03             	mov    %ax,(%rbx)
    3d20:	8b 45 a4             	mov    -0x5c(%rbp),%eax
    3d23:	2b 45 a0             	sub    -0x60(%rbp),%eax
    3d26:	05 00 04 00 00       	add    $0x400,%eax
    3d2b:	c1 f8 0b             	sar    $0xb,%eax
    3d2e:	89 c2                	mov    %eax,%edx
    3d30:	48 8d 43 0e          	lea    0xe(%rbx),%rax
    3d34:	66 89 10             	mov    %dx,(%rax)
    3d37:	8b 55 a8             	mov    -0x58(%rbp),%edx
    3d3a:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3d3d:	01 d0                	add    %edx,%eax
    3d3f:	05 00 04 00 00       	add    $0x400,%eax
    3d44:	c1 f8 0b             	sar    $0xb,%eax
    3d47:	89 c2                	mov    %eax,%edx
    3d49:	48 8d 43 02          	lea    0x2(%rbx),%rax
    3d4d:	66 89 10             	mov    %dx,(%rax)
    3d50:	8b 45 a8             	mov    -0x58(%rbp),%eax
    3d53:	2b 45 9c             	sub    -0x64(%rbp),%eax
    3d56:	05 00 04 00 00       	add    $0x400,%eax
    3d5b:	c1 f8 0b             	sar    $0xb,%eax
    3d5e:	89 c2                	mov    %eax,%edx
    3d60:	48 8d 43 0c          	lea    0xc(%rbx),%rax
    3d64:	66 89 10             	mov    %dx,(%rax)
    3d67:	8b 55 ac             	mov    -0x54(%rbp),%edx
    3d6a:	8b 45 98             	mov    -0x68(%rbp),%eax
    3d6d:	01 d0                	add    %edx,%eax
    3d6f:	05 00 04 00 00       	add    $0x400,%eax
    3d74:	c1 f8 0b             	sar    $0xb,%eax
    3d77:	89 c2                	mov    %eax,%edx
    3d79:	48 8d 43 04          	lea    0x4(%rbx),%rax
    3d7d:	66 89 10             	mov    %dx,(%rax)
    3d80:	8b 45 ac             	mov    -0x54(%rbp),%eax
    3d83:	2b 45 98             	sub    -0x68(%rbp),%eax
    3d86:	05 00 04 00 00       	add    $0x400,%eax
    3d8b:	c1 f8 0b             	sar    $0xb,%eax
    3d8e:	89 c2                	mov    %eax,%edx
    3d90:	48 8d 43 0a          	lea    0xa(%rbx),%rax
    3d94:	66 89 10             	mov    %dx,(%rax)
    3d97:	8b 55 b0             	mov    -0x50(%rbp),%edx
    3d9a:	8b 45 94             	mov    -0x6c(%rbp),%eax
    3d9d:	01 d0                	add    %edx,%eax
    3d9f:	05 00 04 00 00       	add    $0x400,%eax
    3da4:	c1 f8 0b             	sar    $0xb,%eax
    3da7:	89 c2                	mov    %eax,%edx
    3da9:	48 8d 43 06          	lea    0x6(%rbx),%rax
    3dad:	66 89 10             	mov    %dx,(%rax)
    3db0:	8b 45 b0             	mov    -0x50(%rbp),%eax
    3db3:	2b 45 94             	sub    -0x6c(%rbp),%eax
    3db6:	05 00 04 00 00       	add    $0x400,%eax
    3dbb:	c1 f8 0b             	sar    $0xb,%eax
    3dbe:	89 c2                	mov    %eax,%edx
    3dc0:	48 8d 43 08          	lea    0x8(%rbx),%rax
    3dc4:	66 89 10             	mov    %dx,(%rax)
    3dc7:	48 83 c3 10          	add    $0x10,%rbx
    3dcb:	83 6d b4 01          	subl   $0x1,-0x4c(%rbp)
    3dcf:	83 7d b4 00          	cmpl   $0x0,-0x4c(%rbp)
    3dd3:	0f 89 e3 f4 ff ff    	jns    32bc <fastIDCT+0x19>
    3dd9:	48 8b 5d 80          	mov    -0x80(%rbp),%rbx
    3ddd:	c7 45 b4 07 00 00 00 	movl   $0x7,-0x4c(%rbp)
    3de4:	e9 c0 0a 00 00       	jmpq   48a9 <F_2_053+0x6f6>
    3de9:	0f b7 03             	movzwl (%rbx),%eax
    3dec:	98                   	cwtl   
    3ded:	89 45 b8             	mov    %eax,-0x48(%rbp)
    3df0:	48 8d 43 10          	lea    0x10(%rbx),%rax
    3df4:	0f b7 00             	movzwl (%rax),%eax
    3df7:	98                   	cwtl   
    3df8:	89 45 bc             	mov    %eax,-0x44(%rbp)
    3dfb:	48 8d 43 20          	lea    0x20(%rbx),%rax
    3dff:	0f b7 00             	movzwl (%rax),%eax
    3e02:	98                   	cwtl   
    3e03:	89 45 c0             	mov    %eax,-0x40(%rbp)
    3e06:	48 8d 43 30          	lea    0x30(%rbx),%rax
    3e0a:	0f b7 00             	movzwl (%rax),%eax
    3e0d:	98                   	cwtl   
    3e0e:	89 45 c4             	mov    %eax,-0x3c(%rbp)
    3e11:	48 8d 43 40          	lea    0x40(%rbx),%rax
    3e15:	0f b7 00             	movzwl (%rax),%eax
    3e18:	98                   	cwtl   
    3e19:	89 45 c8             	mov    %eax,-0x38(%rbp)
    3e1c:	48 8d 43 50          	lea    0x50(%rbx),%rax
    3e20:	0f b7 00             	movzwl (%rax),%eax
    3e23:	98                   	cwtl   
    3e24:	89 45 cc             	mov    %eax,-0x34(%rbp)
    3e27:	48 8d 43 60          	lea    0x60(%rbx),%rax
    3e2b:	0f b7 00             	movzwl (%rax),%eax
    3e2e:	98                   	cwtl   
    3e2f:	89 45 d0             	mov    %eax,-0x30(%rbp)
    3e32:	48 8d 43 70          	lea    0x70(%rbx),%rax
    3e36:	0f b7 00             	movzwl (%rax),%eax
    3e39:	98                   	cwtl   
    3e3a:	89 45 d4             	mov    %eax,-0x2c(%rbp)
    3e3d:	83 7d d0 00          	cmpl   $0x0,-0x30(%rbp)
    3e41:	0f 84 df 00 00 00    	je     3f26 <F_1_961+0x61>
    3e47:	83 7d c0 00          	cmpl   $0x0,-0x40(%rbp)
    3e4b:	74 7a                	je     3ec7 <F_1_961+0x2>
    3e4d:	8b 55 c0             	mov    -0x40(%rbp),%edx
    3e50:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3e53:	01 d0                	add    %edx,%eax
    3e55:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    3e5b:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3e5e:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3e61:	69 d0 df c4 ff ff    	imul   $0xffffc4df,%eax,%edx
    3e67:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3e6a:	01 d0                	add    %edx,%eax
    3e6c:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3e6f:	8b 45 c0             	mov    -0x40(%rbp),%eax
    3e72:	69 d0 7e 18 00 00    	imul   $0x187e,%eax,%edx
    3e78:	8b 45 d8             	mov    -0x28(%rbp),%eax
    3e7b:	01 d0                	add    %edx,%eax
    3e7d:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3e80:	8b 55 b8             	mov    -0x48(%rbp),%edx
    3e83:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3e86:	01 d0                	add    %edx,%eax
    3e88:	c1 e0 0d             	shl    $0xd,%eax
    3e8b:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3e8e:	8b 45 b8             	mov    -0x48(%rbp),%eax
    3e91:	2b 45 c8             	sub    -0x38(%rbp),%eax
    3e94:	c1 e0 0d             	shl    $0xd,%eax
    3e97:	89 45 98             	mov    %eax,-0x68(%rbp)
    3e9a:	8b 55 94             	mov    -0x6c(%rbp),%edx
    3e9d:	8b 45 a0             	mov    -0x60(%rbp),%eax
    3ea0:	01 d0                	add    %edx,%eax
    3ea2:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    3ea5:	8b 45 94             	mov    -0x6c(%rbp),%eax
    3ea8:	2b 45 a0             	sub    -0x60(%rbp),%eax
    3eab:	89 45 b0             	mov    %eax,-0x50(%rbp)
    3eae:	8b 55 98             	mov    -0x68(%rbp),%edx
    3eb1:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3eb4:	01 d0                	add    %edx,%eax
    3eb6:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3eb9:	8b 45 98             	mov    -0x68(%rbp),%eax
    3ebc:	2b 45 9c             	sub    -0x64(%rbp),%eax
    3ebf:	89 45 ac             	mov    %eax,-0x54(%rbp)
    3ec2:	e9 e7 00 00 00       	jmpq   3fae <F_1_961+0xe9>
    3ec7:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3eca:	69 c0 31 d6 ff ff    	imul   $0xffffd631,%eax,%eax
    3ed0:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3ed3:	8b 45 d0             	mov    -0x30(%rbp),%eax
    3ed6:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    3edc:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3edf:	8b 55 b8             	mov    -0x48(%rbp),%edx
    3ee2:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3ee5:	01 d0                	add    %edx,%eax
    3ee7:	c1 e0 0d             	shl    $0xd,%eax
    3eea:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3eed:	8b 45 b8             	mov    -0x48(%rbp),%eax
    3ef0:	2b 45 c8             	sub    -0x38(%rbp),%eax
    3ef3:	c1 e0 0d             	shl    $0xd,%eax
    3ef6:	89 45 98             	mov    %eax,-0x68(%rbp)
    3ef9:	8b 55 94             	mov    -0x6c(%rbp),%edx
    3efc:	8b 45 a0             	mov    -0x60(%rbp),%eax
    3eff:	01 d0                	add    %edx,%eax
    3f01:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    3f04:	8b 45 94             	mov    -0x6c(%rbp),%eax
    3f07:	2b 45 a0             	sub    -0x60(%rbp),%eax
    3f0a:	89 45 b0             	mov    %eax,-0x50(%rbp)
    3f0d:	8b 55 98             	mov    -0x68(%rbp),%edx
    3f10:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3f13:	01 d0                	add    %edx,%eax
    3f15:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3f18:	8b 45 98             	mov    -0x68(%rbp),%eax
    3f1b:	2b 45 9c             	sub    -0x64(%rbp),%eax
    3f1e:	89 45 ac             	mov    %eax,-0x54(%rbp)
    3f21:	e9 88 00 00 00       	jmpq   3fae <F_1_961+0xe9>
    3f26:	83 7d c0 00          	cmpl   $0x0,-0x40(%rbp)
    3f2a:	74 5c                	je     3f88 <F_1_961+0xc3>
    3f2c:	8b 45 c0             	mov    -0x40(%rbp),%eax
    3f2f:	69 c0 51 11 00 00    	imul   $0x1151,%eax,%eax
    3f35:	89 45 9c             	mov    %eax,-0x64(%rbp)
    3f38:	8b 45 c0             	mov    -0x40(%rbp),%eax
    3f3b:	69 c0 cf 29 00 00    	imul   $0x29cf,%eax,%eax
    3f41:	89 45 a0             	mov    %eax,-0x60(%rbp)
    3f44:	8b 55 b8             	mov    -0x48(%rbp),%edx
    3f47:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3f4a:	01 d0                	add    %edx,%eax
    3f4c:	c1 e0 0d             	shl    $0xd,%eax
    3f4f:	89 45 94             	mov    %eax,-0x6c(%rbp)
    3f52:	8b 45 b8             	mov    -0x48(%rbp),%eax
    3f55:	2b 45 c8             	sub    -0x38(%rbp),%eax
    3f58:	c1 e0 0d             	shl    $0xd,%eax
    3f5b:	89 45 98             	mov    %eax,-0x68(%rbp)
    3f5e:	8b 55 94             	mov    -0x6c(%rbp),%edx
    3f61:	8b 45 a0             	mov    -0x60(%rbp),%eax
    3f64:	01 d0                	add    %edx,%eax
    3f66:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    3f69:	8b 45 94             	mov    -0x6c(%rbp),%eax
    3f6c:	2b 45 a0             	sub    -0x60(%rbp),%eax
    3f6f:	89 45 b0             	mov    %eax,-0x50(%rbp)
    3f72:	8b 55 98             	mov    -0x68(%rbp),%edx
    3f75:	8b 45 9c             	mov    -0x64(%rbp),%eax
    3f78:	01 d0                	add    %edx,%eax
    3f7a:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3f7d:	8b 45 98             	mov    -0x68(%rbp),%eax
    3f80:	2b 45 9c             	sub    -0x64(%rbp),%eax
    3f83:	89 45 ac             	mov    %eax,-0x54(%rbp)
    3f86:	eb 26                	jmp    3fae <F_1_961+0xe9>
    3f88:	8b 55 b8             	mov    -0x48(%rbp),%edx
    3f8b:	8b 45 c8             	mov    -0x38(%rbp),%eax
    3f8e:	01 d0                	add    %edx,%eax
    3f90:	c1 e0 0d             	shl    $0xd,%eax
    3f93:	89 45 b0             	mov    %eax,-0x50(%rbp)
    3f96:	8b 45 b0             	mov    -0x50(%rbp),%eax
    3f99:	89 45 a4             	mov    %eax,-0x5c(%rbp)
    3f9c:	8b 45 b8             	mov    -0x48(%rbp),%eax
    3f9f:	2b 45 c8             	sub    -0x38(%rbp),%eax
    3fa2:	c1 e0 0d             	shl    $0xd,%eax
    3fa5:	89 45 ac             	mov    %eax,-0x54(%rbp)
    3fa8:	8b 45 ac             	mov    -0x54(%rbp),%eax
    3fab:	89 45 a8             	mov    %eax,-0x58(%rbp)
    3fae:	83 7d d4 00          	cmpl   $0x0,-0x2c(%rbp)
    3fb2:	0f 84 ff 04 00 00    	je     44b7 <F_2_053+0x304>
    3fb8:	83 7d cc 00          	cmpl   $0x0,-0x34(%rbp)
    3fbc:	0f 84 fa 02 00 00    	je     42bc <F_2_053+0x109>
    3fc2:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    3fc6:	0f 84 9c 01 00 00    	je     4168 <F_1_961+0x2a3>
    3fcc:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    3fd0:	0f 84 da 00 00 00    	je     40b0 <F_1_961+0x1eb>
    3fd6:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3fd9:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3fdc:	01 d0                	add    %edx,%eax
    3fde:	89 45 d8             	mov    %eax,-0x28(%rbp)
    3fe1:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3fe4:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3fe7:	01 d0                	add    %edx,%eax
    3fe9:	89 45 e0             	mov    %eax,-0x20(%rbp)
    3fec:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    3fef:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    3ff2:	01 d0                	add    %edx,%eax
    3ff4:	89 45 e8             	mov    %eax,-0x18(%rbp)
    3ff7:	8b 55 cc             	mov    -0x34(%rbp),%edx
    3ffa:	8b 45 bc             	mov    -0x44(%rbp),%eax
    3ffd:	01 d0                	add    %edx,%eax
    3fff:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4002:	8b 55 e8             	mov    -0x18(%rbp),%edx
    4005:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4008:	01 d0                	add    %edx,%eax
    400a:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4010:	89 45 dc             	mov    %eax,-0x24(%rbp)
    4013:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4016:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    401c:	89 45 94             	mov    %eax,-0x6c(%rbp)
    401f:	8b 45 cc             	mov    -0x34(%rbp),%eax
    4022:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    4028:	89 45 98             	mov    %eax,-0x68(%rbp)
    402b:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    402e:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    4034:	89 45 9c             	mov    %eax,-0x64(%rbp)
    4037:	8b 45 bc             	mov    -0x44(%rbp),%eax
    403a:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    4040:	89 45 a0             	mov    %eax,-0x60(%rbp)
    4043:	8b 45 d8             	mov    -0x28(%rbp),%eax
    4046:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    404c:	89 45 d8             	mov    %eax,-0x28(%rbp)
    404f:	8b 45 e0             	mov    -0x20(%rbp),%eax
    4052:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4058:	89 45 e0             	mov    %eax,-0x20(%rbp)
    405b:	8b 45 e8             	mov    -0x18(%rbp),%eax
    405e:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    4064:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4067:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    406a:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    4070:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4073:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4076:	01 45 e8             	add    %eax,-0x18(%rbp)
    4079:	8b 45 dc             	mov    -0x24(%rbp),%eax
    407c:	01 45 e4             	add    %eax,-0x1c(%rbp)
    407f:	8b 55 d8             	mov    -0x28(%rbp),%edx
    4082:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4085:	01 d0                	add    %edx,%eax
    4087:	01 45 94             	add    %eax,-0x6c(%rbp)
    408a:	8b 55 e0             	mov    -0x20(%rbp),%edx
    408d:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4090:	01 d0                	add    %edx,%eax
    4092:	01 45 98             	add    %eax,-0x68(%rbp)
    4095:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4098:	8b 45 e8             	mov    -0x18(%rbp),%eax
    409b:	01 d0                	add    %edx,%eax
    409d:	01 45 9c             	add    %eax,-0x64(%rbp)
    40a0:	8b 55 d8             	mov    -0x28(%rbp),%edx
    40a3:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    40a6:	01 d0                	add    %edx,%eax
    40a8:	01 45 a0             	add    %eax,-0x60(%rbp)
    40ab:	e9 17 07 00 00       	jmpq   47c7 <F_2_053+0x614>
    40b0:	8b 55 cc             	mov    -0x34(%rbp),%edx
    40b3:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    40b6:	01 d0                	add    %edx,%eax
    40b8:	89 45 e0             	mov    %eax,-0x20(%rbp)
    40bb:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    40be:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    40c1:	01 d0                	add    %edx,%eax
    40c3:	89 45 e8             	mov    %eax,-0x18(%rbp)
    40c6:	8b 55 e8             	mov    -0x18(%rbp),%edx
    40c9:	8b 45 cc             	mov    -0x34(%rbp),%eax
    40cc:	01 d0                	add    %edx,%eax
    40ce:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    40d4:	89 45 dc             	mov    %eax,-0x24(%rbp)
    40d7:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    40da:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    40e0:	89 45 94             	mov    %eax,-0x6c(%rbp)
    40e3:	8b 45 cc             	mov    -0x34(%rbp),%eax
    40e6:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    40ec:	89 45 98             	mov    %eax,-0x68(%rbp)
    40ef:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    40f2:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    40f8:	89 45 9c             	mov    %eax,-0x64(%rbp)
    40fb:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    40fe:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    4104:	89 45 d8             	mov    %eax,-0x28(%rbp)
    4107:	8b 45 e0             	mov    -0x20(%rbp),%eax
    410a:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4110:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4113:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4116:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    411c:	89 45 e8             	mov    %eax,-0x18(%rbp)
    411f:	8b 45 cc             	mov    -0x34(%rbp),%eax
    4122:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    4128:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    412b:	8b 45 dc             	mov    -0x24(%rbp),%eax
    412e:	01 45 e8             	add    %eax,-0x18(%rbp)
    4131:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4134:	01 45 e4             	add    %eax,-0x1c(%rbp)
    4137:	8b 55 d8             	mov    -0x28(%rbp),%edx
    413a:	8b 45 e8             	mov    -0x18(%rbp),%eax
    413d:	01 d0                	add    %edx,%eax
    413f:	01 45 94             	add    %eax,-0x6c(%rbp)
    4142:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4145:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4148:	01 d0                	add    %edx,%eax
    414a:	01 45 98             	add    %eax,-0x68(%rbp)
    414d:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4150:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4153:	01 d0                	add    %edx,%eax
    4155:	01 45 9c             	add    %eax,-0x64(%rbp)
    4158:	8b 55 d8             	mov    -0x28(%rbp),%edx
    415b:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    415e:	01 d0                	add    %edx,%eax
    4160:	89 45 a0             	mov    %eax,-0x60(%rbp)
    4163:	e9 5f 06 00 00       	jmpq   47c7 <F_2_053+0x614>
    4168:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    416c:	0f 84 be 00 00 00    	je     4230 <F_2_053+0x7d>
    4172:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    4175:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4178:	01 d0                	add    %edx,%eax
    417a:	89 45 d8             	mov    %eax,-0x28(%rbp)
    417d:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4180:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4183:	8b 55 cc             	mov    -0x34(%rbp),%edx
    4186:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4189:	01 d0                	add    %edx,%eax
    418b:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    418e:	8b 55 e8             	mov    -0x18(%rbp),%edx
    4191:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4194:	01 d0                	add    %edx,%eax
    4196:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    419c:	89 45 dc             	mov    %eax,-0x24(%rbp)
    419f:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    41a2:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    41a8:	89 45 94             	mov    %eax,-0x6c(%rbp)
    41ab:	8b 45 cc             	mov    -0x34(%rbp),%eax
    41ae:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    41b4:	89 45 98             	mov    %eax,-0x68(%rbp)
    41b7:	8b 45 bc             	mov    -0x44(%rbp),%eax
    41ba:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    41c0:	89 45 a0             	mov    %eax,-0x60(%rbp)
    41c3:	8b 45 d8             	mov    -0x28(%rbp),%eax
    41c6:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    41cc:	89 45 d8             	mov    %eax,-0x28(%rbp)
    41cf:	8b 45 cc             	mov    -0x34(%rbp),%eax
    41d2:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    41d8:	89 45 e0             	mov    %eax,-0x20(%rbp)
    41db:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    41de:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    41e4:	89 45 e8             	mov    %eax,-0x18(%rbp)
    41e7:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    41ea:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    41f0:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    41f3:	8b 45 dc             	mov    -0x24(%rbp),%eax
    41f6:	01 45 e8             	add    %eax,-0x18(%rbp)
    41f9:	8b 45 dc             	mov    -0x24(%rbp),%eax
    41fc:	01 45 e4             	add    %eax,-0x1c(%rbp)
    41ff:	8b 55 d8             	mov    -0x28(%rbp),%edx
    4202:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4205:	01 d0                	add    %edx,%eax
    4207:	01 45 94             	add    %eax,-0x6c(%rbp)
    420a:	8b 55 e0             	mov    -0x20(%rbp),%edx
    420d:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4210:	01 d0                	add    %edx,%eax
    4212:	01 45 98             	add    %eax,-0x68(%rbp)
    4215:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4218:	8b 45 e8             	mov    -0x18(%rbp),%eax
    421b:	01 d0                	add    %edx,%eax
    421d:	89 45 9c             	mov    %eax,-0x64(%rbp)
    4220:	8b 55 d8             	mov    -0x28(%rbp),%edx
    4223:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4226:	01 d0                	add    %edx,%eax
    4228:	01 45 a0             	add    %eax,-0x60(%rbp)
    422b:	e9 97 05 00 00       	jmpq   47c7 <F_2_053+0x614>
    4230:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4233:	69 c0 c2 ec ff ff    	imul   $0xffffecc2,%eax,%eax
    4239:	89 45 94             	mov    %eax,-0x6c(%rbp)
    423c:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    423f:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    4245:	89 45 d8             	mov    %eax,-0x28(%rbp)
    4248:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    424b:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    4251:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4254:	8b 45 cc             	mov    -0x34(%rbp),%eax
    4257:	69 c0 b0 ef ff ff    	imul   $0xffffefb0,%eax,%eax
    425d:	89 45 98             	mov    %eax,-0x68(%rbp)
    4260:	8b 45 cc             	mov    -0x34(%rbp),%eax
    4263:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4269:	89 45 e0             	mov    %eax,-0x20(%rbp)
    426c:	8b 45 cc             	mov    -0x34(%rbp),%eax
    426f:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    4275:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4278:	8b 55 cc             	mov    -0x34(%rbp),%edx
    427b:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    427e:	01 d0                	add    %edx,%eax
    4280:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4286:	89 45 dc             	mov    %eax,-0x24(%rbp)
    4289:	8b 45 dc             	mov    -0x24(%rbp),%eax
    428c:	01 45 e8             	add    %eax,-0x18(%rbp)
    428f:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4292:	01 45 e4             	add    %eax,-0x1c(%rbp)
    4295:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4298:	01 45 94             	add    %eax,-0x6c(%rbp)
    429b:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    429e:	01 45 98             	add    %eax,-0x68(%rbp)
    42a1:	8b 55 e0             	mov    -0x20(%rbp),%edx
    42a4:	8b 45 e8             	mov    -0x18(%rbp),%eax
    42a7:	01 d0                	add    %edx,%eax
    42a9:	89 45 9c             	mov    %eax,-0x64(%rbp)
    42ac:	8b 55 d8             	mov    -0x28(%rbp),%edx
    42af:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    42b2:	01 d0                	add    %edx,%eax
    42b4:	89 45 a0             	mov    %eax,-0x60(%rbp)
    42b7:	e9 0b 05 00 00       	jmpq   47c7 <F_2_053+0x614>
    42bc:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    42c0:	0f 84 3c 01 00 00    	je     4402 <F_2_053+0x24f>
    42c6:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    42ca:	0f 84 b8 00 00 00    	je     4388 <F_2_053+0x1d5>
    42d0:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    42d3:	8b 45 bc             	mov    -0x44(%rbp),%eax
    42d6:	01 d0                	add    %edx,%eax
    42d8:	89 45 d8             	mov    %eax,-0x28(%rbp)
    42db:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    42de:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    42e1:	01 d0                	add    %edx,%eax
    42e3:	89 45 e8             	mov    %eax,-0x18(%rbp)
    42e6:	8b 55 e8             	mov    -0x18(%rbp),%edx
    42e9:	8b 45 bc             	mov    -0x44(%rbp),%eax
    42ec:	01 d0                	add    %edx,%eax
    42ee:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    42f4:	89 45 dc             	mov    %eax,-0x24(%rbp)
    42f7:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    42fa:	69 c0 8e 09 00 00    	imul   $0x98e,%eax,%eax
    4300:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4303:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    4306:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    430c:	89 45 9c             	mov    %eax,-0x64(%rbp)
    430f:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4312:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    4318:	89 45 a0             	mov    %eax,-0x60(%rbp)
    431b:	8b 45 d8             	mov    -0x28(%rbp),%eax
    431e:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    4324:	89 45 d8             	mov    %eax,-0x28(%rbp)
    4327:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    432a:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4330:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4333:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4336:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    433c:	89 45 e8             	mov    %eax,-0x18(%rbp)
    433f:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4342:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    4348:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    434b:	8b 45 dc             	mov    -0x24(%rbp),%eax
    434e:	01 45 e8             	add    %eax,-0x18(%rbp)
    4351:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4354:	01 45 e4             	add    %eax,-0x1c(%rbp)
    4357:	8b 55 d8             	mov    -0x28(%rbp),%edx
    435a:	8b 45 e8             	mov    -0x18(%rbp),%eax
    435d:	01 d0                	add    %edx,%eax
    435f:	01 45 94             	add    %eax,-0x6c(%rbp)
    4362:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4365:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4368:	01 d0                	add    %edx,%eax
    436a:	89 45 98             	mov    %eax,-0x68(%rbp)
    436d:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4370:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4373:	01 d0                	add    %edx,%eax
    4375:	01 45 9c             	add    %eax,-0x64(%rbp)
    4378:	8b 55 d8             	mov    -0x28(%rbp),%edx
    437b:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    437e:	01 d0                	add    %edx,%eax
    4380:	01 45 a0             	add    %eax,-0x60(%rbp)
    4383:	e9 3f 04 00 00       	jmpq   47c7 <F_2_053+0x614>
    4388:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    438b:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    438e:	01 d0                	add    %edx,%eax
    4390:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4393:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4396:	69 c0 c2 ec ff ff    	imul   $0xffffecc2,%eax,%eax
    439c:	89 45 94             	mov    %eax,-0x6c(%rbp)
    439f:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    43a2:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    43a8:	89 45 d8             	mov    %eax,-0x28(%rbp)
    43ab:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    43ae:	69 c0 50 10 00 00    	imul   $0x1050,%eax,%eax
    43b4:	89 45 9c             	mov    %eax,-0x64(%rbp)
    43b7:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    43ba:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    43c0:	89 45 e0             	mov    %eax,-0x20(%rbp)
    43c3:	8b 45 e8             	mov    -0x18(%rbp),%eax
    43c6:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    43cc:	89 45 dc             	mov    %eax,-0x24(%rbp)
    43cf:	8b 45 e8             	mov    -0x18(%rbp),%eax
    43d2:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    43d8:	89 45 e8             	mov    %eax,-0x18(%rbp)
    43db:	8b 45 e8             	mov    -0x18(%rbp),%eax
    43de:	01 45 94             	add    %eax,-0x6c(%rbp)
    43e1:	8b 55 e0             	mov    -0x20(%rbp),%edx
    43e4:	8b 45 dc             	mov    -0x24(%rbp),%eax
    43e7:	01 d0                	add    %edx,%eax
    43e9:	89 45 98             	mov    %eax,-0x68(%rbp)
    43ec:	8b 45 e8             	mov    -0x18(%rbp),%eax
    43ef:	01 45 9c             	add    %eax,-0x64(%rbp)
    43f2:	8b 55 d8             	mov    -0x28(%rbp),%edx
    43f5:	8b 45 dc             	mov    -0x24(%rbp),%eax
    43f8:	01 d0                	add    %edx,%eax
    43fa:	89 45 a0             	mov    %eax,-0x60(%rbp)
    43fd:	e9 c5 03 00 00       	jmpq   47c7 <F_2_053+0x614>
    4402:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    4406:	74 7a                	je     4482 <F_2_053+0x2cf>
    4408:	8b 55 d4             	mov    -0x2c(%rbp),%edx
    440b:	8b 45 bc             	mov    -0x44(%rbp),%eax
    440e:	01 d0                	add    %edx,%eax
    4410:	89 45 d8             	mov    %eax,-0x28(%rbp)
    4413:	8b 45 d8             	mov    -0x28(%rbp),%eax
    4416:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    441c:	89 45 dc             	mov    %eax,-0x24(%rbp)
    441f:	8b 45 d8             	mov    -0x28(%rbp),%eax
    4422:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    4428:	89 45 d8             	mov    %eax,-0x28(%rbp)
    442b:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    442e:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    4434:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4437:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    443a:	69 c0 c9 ca ff ff    	imul   $0xffffcac9,%eax,%eax
    4440:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4443:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4446:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    444c:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    444f:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4452:	69 c0 8e 23 00 00    	imul   $0x238e,%eax,%eax
    4458:	89 45 a0             	mov    %eax,-0x60(%rbp)
    445b:	8b 45 d8             	mov    -0x28(%rbp),%eax
    445e:	01 45 94             	add    %eax,-0x6c(%rbp)
    4461:	8b 55 e4             	mov    -0x1c(%rbp),%edx
    4464:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4467:	01 d0                	add    %edx,%eax
    4469:	89 45 98             	mov    %eax,-0x68(%rbp)
    446c:	8b 55 e8             	mov    -0x18(%rbp),%edx
    446f:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4472:	01 d0                	add    %edx,%eax
    4474:	89 45 9c             	mov    %eax,-0x64(%rbp)
    4477:	8b 45 d8             	mov    -0x28(%rbp),%eax
    447a:	01 45 a0             	add    %eax,-0x60(%rbp)
    447d:	e9 45 03 00 00       	jmpq   47c7 <F_2_053+0x614>
    4482:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4485:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    448b:	89 45 94             	mov    %eax,-0x6c(%rbp)
    448e:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    4491:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4497:	89 45 98             	mov    %eax,-0x68(%rbp)
    449a:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    449d:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    44a3:	89 45 9c             	mov    %eax,-0x64(%rbp)
    44a6:	8b 45 d4             	mov    -0x2c(%rbp),%eax
    44a9:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    44af:	89 45 a0             	mov    %eax,-0x60(%rbp)
    44b2:	e9 10 03 00 00       	jmpq   47c7 <F_2_053+0x614>
    44b7:	83 7d cc 00          	cmpl   $0x0,-0x34(%rbp)
    44bb:	0f 84 fb 01 00 00    	je     46bc <F_2_053+0x509>
    44c1:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    44c5:	0f 84 3c 01 00 00    	je     4607 <F_2_053+0x454>
    44cb:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    44cf:	0f 84 b8 00 00 00    	je     458d <F_2_053+0x3da>
    44d5:	8b 55 cc             	mov    -0x34(%rbp),%edx
    44d8:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    44db:	01 d0                	add    %edx,%eax
    44dd:	89 45 e0             	mov    %eax,-0x20(%rbp)
    44e0:	8b 55 cc             	mov    -0x34(%rbp),%edx
    44e3:	8b 45 bc             	mov    -0x44(%rbp),%eax
    44e6:	01 d0                	add    %edx,%eax
    44e8:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    44eb:	8b 55 c4             	mov    -0x3c(%rbp),%edx
    44ee:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    44f1:	01 d0                	add    %edx,%eax
    44f3:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    44f9:	89 45 dc             	mov    %eax,-0x24(%rbp)
    44fc:	8b 45 cc             	mov    -0x34(%rbp),%eax
    44ff:	69 c0 b3 41 00 00    	imul   $0x41b3,%eax,%eax
    4505:	89 45 98             	mov    %eax,-0x68(%rbp)
    4508:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    450b:	69 c0 54 62 00 00    	imul   $0x6254,%eax,%eax
    4511:	89 45 9c             	mov    %eax,-0x64(%rbp)
    4514:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4517:	69 c0 0b 30 00 00    	imul   $0x300b,%eax,%eax
    451d:	89 45 a0             	mov    %eax,-0x60(%rbp)
    4520:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4523:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    4529:	89 45 d8             	mov    %eax,-0x28(%rbp)
    452c:	8b 45 e0             	mov    -0x20(%rbp),%eax
    452f:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4535:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4538:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    453b:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    4541:	89 45 e8             	mov    %eax,-0x18(%rbp)
    4544:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4547:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    454d:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4550:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4553:	01 45 e8             	add    %eax,-0x18(%rbp)
    4556:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4559:	01 45 e4             	add    %eax,-0x1c(%rbp)
    455c:	8b 55 d8             	mov    -0x28(%rbp),%edx
    455f:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4562:	01 d0                	add    %edx,%eax
    4564:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4567:	8b 55 e0             	mov    -0x20(%rbp),%edx
    456a:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    456d:	01 d0                	add    %edx,%eax
    456f:	01 45 98             	add    %eax,-0x68(%rbp)
    4572:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4575:	8b 45 e8             	mov    -0x18(%rbp),%eax
    4578:	01 d0                	add    %edx,%eax
    457a:	01 45 9c             	add    %eax,-0x64(%rbp)
    457d:	8b 55 d8             	mov    -0x28(%rbp),%edx
    4580:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4583:	01 d0                	add    %edx,%eax
    4585:	01 45 a0             	add    %eax,-0x60(%rbp)
    4588:	e9 3a 02 00 00       	jmpq   47c7 <F_2_053+0x614>
    458d:	8b 55 cc             	mov    -0x34(%rbp),%edx
    4590:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    4593:	01 d0                	add    %edx,%eax
    4595:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4598:	8b 45 e0             	mov    -0x20(%rbp),%eax
    459b:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    45a1:	89 45 dc             	mov    %eax,-0x24(%rbp)
    45a4:	8b 45 cc             	mov    -0x34(%rbp),%eax
    45a7:	69 c0 37 35 00 00    	imul   $0x3537,%eax,%eax
    45ad:	89 45 98             	mov    %eax,-0x68(%rbp)
    45b0:	8b 45 cc             	mov    -0x34(%rbp),%eax
    45b3:	69 c0 84 f3 ff ff    	imul   $0xfffff384,%eax,%eax
    45b9:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    45bc:	8b 45 e0             	mov    -0x20(%rbp),%eax
    45bf:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    45c5:	89 45 e0             	mov    %eax,-0x20(%rbp)
    45c8:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    45cb:	69 c0 8e 23 00 00    	imul   $0x238e,%eax,%eax
    45d1:	89 45 9c             	mov    %eax,-0x64(%rbp)
    45d4:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    45d7:	69 c0 3b c1 ff ff    	imul   $0xffffc13b,%eax,%eax
    45dd:	89 45 e8             	mov    %eax,-0x18(%rbp)
    45e0:	8b 55 e8             	mov    -0x18(%rbp),%edx
    45e3:	8b 45 dc             	mov    -0x24(%rbp),%eax
    45e6:	01 d0                	add    %edx,%eax
    45e8:	89 45 94             	mov    %eax,-0x6c(%rbp)
    45eb:	8b 45 e0             	mov    -0x20(%rbp),%eax
    45ee:	01 45 98             	add    %eax,-0x68(%rbp)
    45f1:	8b 45 e0             	mov    -0x20(%rbp),%eax
    45f4:	01 45 9c             	add    %eax,-0x64(%rbp)
    45f7:	8b 55 e4             	mov    -0x1c(%rbp),%edx
    45fa:	8b 45 dc             	mov    -0x24(%rbp),%eax
    45fd:	01 d0                	add    %edx,%eax
    45ff:	89 45 a0             	mov    %eax,-0x60(%rbp)
    4602:	e9 c0 01 00 00       	jmpq   47c7 <F_2_053+0x614>
    4607:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    460b:	74 7a                	je     4687 <F_2_053+0x4d4>
    460d:	8b 55 cc             	mov    -0x34(%rbp),%edx
    4610:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4613:	01 d0                	add    %edx,%eax
    4615:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4618:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    461b:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4621:	89 45 dc             	mov    %eax,-0x24(%rbp)
    4624:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4627:	69 c0 33 e3 ff ff    	imul   $0xffffe333,%eax,%eax
    462d:	89 45 d8             	mov    %eax,-0x28(%rbp)
    4630:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4633:	69 c0 3e 13 00 00    	imul   $0x133e,%eax,%eax
    4639:	89 45 a0             	mov    %eax,-0x60(%rbp)
    463c:	8b 45 cc             	mov    -0x34(%rbp),%eax
    463f:	69 c0 b0 ef ff ff    	imul   $0xffffefb0,%eax,%eax
    4645:	89 45 98             	mov    %eax,-0x68(%rbp)
    4648:	8b 45 cc             	mov    -0x34(%rbp),%eax
    464b:	69 c0 fd ad ff ff    	imul   $0xffffadfd,%eax,%eax
    4651:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4654:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    4657:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    465d:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4660:	8b 55 d8             	mov    -0x28(%rbp),%edx
    4663:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4666:	01 d0                	add    %edx,%eax
    4668:	89 45 94             	mov    %eax,-0x6c(%rbp)
    466b:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    466e:	01 45 98             	add    %eax,-0x68(%rbp)
    4671:	8b 55 e0             	mov    -0x20(%rbp),%edx
    4674:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4677:	01 d0                	add    %edx,%eax
    4679:	89 45 9c             	mov    %eax,-0x64(%rbp)
    467c:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    467f:	01 45 a0             	add    %eax,-0x60(%rbp)
    4682:	e9 40 01 00 00       	jmpq   47c7 <F_2_053+0x614>
    4687:	8b 45 cc             	mov    -0x34(%rbp),%eax
    468a:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4690:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4693:	8b 45 cc             	mov    -0x34(%rbp),%eax
    4696:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    469c:	89 45 98             	mov    %eax,-0x68(%rbp)
    469f:	8b 45 cc             	mov    -0x34(%rbp),%eax
    46a2:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    46a8:	89 45 9c             	mov    %eax,-0x64(%rbp)
    46ab:	8b 45 cc             	mov    -0x34(%rbp),%eax
    46ae:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    46b4:	89 45 a0             	mov    %eax,-0x60(%rbp)
    46b7:	e9 0b 01 00 00       	jmpq   47c7 <F_2_053+0x614>
    46bc:	83 7d c4 00          	cmpl   $0x0,-0x3c(%rbp)
    46c0:	0f 84 b0 00 00 00    	je     4776 <F_2_053+0x5c3>
    46c6:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    46ca:	74 78                	je     4744 <F_2_053+0x591>
    46cc:	8b 55 bc             	mov    -0x44(%rbp),%edx
    46cf:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    46d2:	01 d0                	add    %edx,%eax
    46d4:	89 45 dc             	mov    %eax,-0x24(%rbp)
    46d7:	8b 45 bc             	mov    -0x44(%rbp),%eax
    46da:	69 c0 c2 06 00 00    	imul   $0x6c2,%eax,%eax
    46e0:	89 45 a0             	mov    %eax,-0x60(%rbp)
    46e3:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    46e6:	69 c0 8b d1 ff ff    	imul   $0xffffd18b,%eax,%eax
    46ec:	89 45 9c             	mov    %eax,-0x64(%rbp)
    46ef:	8b 45 bc             	mov    -0x44(%rbp),%eax
    46f2:	69 c0 f9 21 00 00    	imul   $0x21f9,%eax,%eax
    46f8:	89 45 d8             	mov    %eax,-0x28(%rbp)
    46fb:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    46fe:	69 c0 79 ba ff ff    	imul   $0xffffba79,%eax,%eax
    4704:	89 45 e0             	mov    %eax,-0x20(%rbp)
    4707:	8b 45 dc             	mov    -0x24(%rbp),%eax
    470a:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    4710:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    4713:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4716:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    471c:	89 45 dc             	mov    %eax,-0x24(%rbp)
    471f:	8b 45 d8             	mov    -0x28(%rbp),%eax
    4722:	2b 45 e4             	sub    -0x1c(%rbp),%eax
    4725:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4728:	8b 55 e0             	mov    -0x20(%rbp),%edx
    472b:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    472e:	01 d0                	add    %edx,%eax
    4730:	89 45 98             	mov    %eax,-0x68(%rbp)
    4733:	8b 45 dc             	mov    -0x24(%rbp),%eax
    4736:	01 45 9c             	add    %eax,-0x64(%rbp)
    4739:	8b 45 dc             	mov    -0x24(%rbp),%eax
    473c:	01 45 a0             	add    %eax,-0x60(%rbp)
    473f:	e9 83 00 00 00       	jmpq   47c7 <F_2_053+0x614>
    4744:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    4747:	69 c0 dc e6 ff ff    	imul   $0xffffe6dc,%eax,%eax
    474d:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4750:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    4753:	69 c0 9d d3 ff ff    	imul   $0xffffd39d,%eax,%eax
    4759:	89 45 98             	mov    %eax,-0x68(%rbp)
    475c:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    475f:	69 c0 2c f7 ff ff    	imul   $0xfffff72c,%eax,%eax
    4765:	89 45 9c             	mov    %eax,-0x64(%rbp)
    4768:	8b 45 c4             	mov    -0x3c(%rbp),%eax
    476b:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    4771:	89 45 a0             	mov    %eax,-0x60(%rbp)
    4774:	eb 51                	jmp    47c7 <F_2_053+0x614>
    4776:	83 7d bc 00          	cmpl   $0x0,-0x44(%rbp)
    477a:	74 32                	je     47ae <F_2_053+0x5fb>
    477c:	8b 45 bc             	mov    -0x44(%rbp),%eax
    477f:	69 c0 d4 08 00 00    	imul   $0x8d4,%eax,%eax
    4785:	89 45 94             	mov    %eax,-0x6c(%rbp)
    4788:	8b 45 bc             	mov    -0x44(%rbp),%eax
    478b:	69 c0 24 19 00 00    	imul   $0x1924,%eax,%eax
    4791:	89 45 98             	mov    %eax,-0x68(%rbp)
    4794:	8b 45 bc             	mov    -0x44(%rbp),%eax
    4797:	69 c0 a1 25 00 00    	imul   $0x25a1,%eax,%eax
    479d:	89 45 9c             	mov    %eax,-0x64(%rbp)
    47a0:	8b 45 bc             	mov    -0x44(%rbp),%eax
    47a3:	69 c0 63 2c 00 00    	imul   $0x2c63,%eax,%eax
    47a9:	89 45 a0             	mov    %eax,-0x60(%rbp)
    47ac:	eb 19                	jmp    47c7 <F_2_053+0x614>
    47ae:	c7 45 a0 00 00 00 00 	movl   $0x0,-0x60(%rbp)
    47b5:	8b 45 a0             	mov    -0x60(%rbp),%eax
    47b8:	89 45 9c             	mov    %eax,-0x64(%rbp)
    47bb:	8b 45 9c             	mov    -0x64(%rbp),%eax
    47be:	89 45 98             	mov    %eax,-0x68(%rbp)
    47c1:	8b 45 98             	mov    -0x68(%rbp),%eax
    47c4:	89 45 94             	mov    %eax,-0x6c(%rbp)
    47c7:	8b 55 a4             	mov    -0x5c(%rbp),%edx
    47ca:	8b 45 a0             	mov    -0x60(%rbp),%eax
    47cd:	01 d0                	add    %edx,%eax
    47cf:	05 00 00 02 00       	add    $0x20000,%eax
    47d4:	c1 f8 12             	sar    $0x12,%eax
    47d7:	66 83 e8 80          	sub    $0xff80,%ax
    47db:	66 89 03             	mov    %ax,(%rbx)
    47de:	8b 45 a4             	mov    -0x5c(%rbp),%eax
    47e1:	2b 45 a0             	sub    -0x60(%rbp),%eax
    47e4:	05 00 00 02 00       	add    $0x20000,%eax
    47e9:	c1 f8 12             	sar    $0x12,%eax
    47ec:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    47f2:	48 8d 43 70          	lea    0x70(%rbx),%rax
    47f6:	66 89 10             	mov    %dx,(%rax)
    47f9:	8b 55 a8             	mov    -0x58(%rbp),%edx
    47fc:	8b 45 9c             	mov    -0x64(%rbp),%eax
    47ff:	01 d0                	add    %edx,%eax
    4801:	05 00 00 02 00       	add    $0x20000,%eax
    4806:	c1 f8 12             	sar    $0x12,%eax
    4809:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    480f:	48 8d 43 10          	lea    0x10(%rbx),%rax
    4813:	66 89 10             	mov    %dx,(%rax)
    4816:	8b 45 a8             	mov    -0x58(%rbp),%eax
    4819:	2b 45 9c             	sub    -0x64(%rbp),%eax
    481c:	05 00 00 02 00       	add    $0x20000,%eax
    4821:	c1 f8 12             	sar    $0x12,%eax
    4824:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    482a:	48 8d 43 60          	lea    0x60(%rbx),%rax
    482e:	66 89 10             	mov    %dx,(%rax)
    4831:	8b 55 ac             	mov    -0x54(%rbp),%edx
    4834:	8b 45 98             	mov    -0x68(%rbp),%eax
    4837:	01 d0                	add    %edx,%eax
    4839:	05 00 00 02 00       	add    $0x20000,%eax
    483e:	c1 f8 12             	sar    $0x12,%eax
    4841:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    4847:	48 8d 43 20          	lea    0x20(%rbx),%rax
    484b:	66 89 10             	mov    %dx,(%rax)
    484e:	8b 45 ac             	mov    -0x54(%rbp),%eax
    4851:	2b 45 98             	sub    -0x68(%rbp),%eax
    4854:	05 00 00 02 00       	add    $0x20000,%eax
    4859:	c1 f8 12             	sar    $0x12,%eax
    485c:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    4862:	48 8d 43 50          	lea    0x50(%rbx),%rax
    4866:	66 89 10             	mov    %dx,(%rax)
    4869:	8b 55 b0             	mov    -0x50(%rbp),%edx
    486c:	8b 45 94             	mov    -0x6c(%rbp),%eax
    486f:	01 d0                	add    %edx,%eax
    4871:	05 00 00 02 00       	add    $0x20000,%eax
    4876:	c1 f8 12             	sar    $0x12,%eax
    4879:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    487f:	48 8d 43 30          	lea    0x30(%rbx),%rax
    4883:	66 89 10             	mov    %dx,(%rax)
    4886:	8b 45 b0             	mov    -0x50(%rbp),%eax
    4889:	2b 45 94             	sub    -0x6c(%rbp),%eax
    488c:	05 00 00 02 00       	add    $0x20000,%eax
    4891:	c1 f8 12             	sar    $0x12,%eax
    4894:	8d 90 80 00 00 00    	lea    0x80(%rax),%edx
    489a:	48 8d 43 40          	lea    0x40(%rbx),%rax
    489e:	66 89 10             	mov    %dx,(%rax)
    48a1:	48 83 c3 02          	add    $0x2,%rbx
    48a5:	83 6d b4 01          	subl   $0x1,-0x4c(%rbp)
    48a9:	83 7d b4 00          	cmpl   $0x0,-0x4c(%rbp)
    48ad:	0f 89 36 f5 ff ff    	jns    3de9 <F_1_847+0x2c8>
    48b3:	90                   	nop
    48b4:	5b                   	pop    %rbx
    48b5:	5d                   	pop    %rbp
    48b6:	c3                   	retq   

00000000000048b7 <dump_uint8>:
    48b7:	55                   	push   %rbp
    48b8:	48 89 e5             	mov    %rsp,%rbp
    48bb:	48 83 ec 20          	sub    $0x20,%rsp
    48bf:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    48c3:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    48ca:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    48d1:	eb 41                	jmp    4914 <dump_uint8+0x5d>
    48d3:	8b 45 fc             	mov    -0x4(%rbp),%eax
    48d6:	48 63 d0             	movslq %eax,%rdx
    48d9:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    48dd:	48 01 d0             	add    %rdx,%rax
    48e0:	0f b6 00             	movzbl (%rax),%eax
    48e3:	0f b6 c0             	movzbl %al,%eax
    48e6:	89 c6                	mov    %eax,%esi
    48e8:	48 8d 3d 75 0a 00 00 	lea    0xa75(%rip),%rdi        # 5364 <_IO_stdin_used+0x4>
    48ef:	b8 00 00 00 00       	mov    $0x0,%eax
    48f4:	e8 d7 bc ff ff       	callq  5d0 <printf@plt>
    48f9:	8b 45 fc             	mov    -0x4(%rbp),%eax
    48fc:	83 c0 01             	add    $0x1,%eax
    48ff:	83 e0 07             	and    $0x7,%eax
    4902:	85 c0                	test   %eax,%eax
    4904:	75 0a                	jne    4910 <dump_uint8+0x59>
    4906:	bf 0a 00 00 00       	mov    $0xa,%edi
    490b:	e8 a0 bc ff ff       	callq  5b0 <putchar@plt>
    4910:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    4914:	83 7d fc 3f          	cmpl   $0x3f,-0x4(%rbp)
    4918:	7e b9                	jle    48d3 <dump_uint8+0x1c>
    491a:	bf 0a 00 00 00       	mov    $0xa,%edi
    491f:	e8 8c bc ff ff       	callq  5b0 <putchar@plt>
    4924:	90                   	nop
    4925:	c9                   	leaveq 
    4926:	c3                   	retq   

0000000000004927 <dump_int16>:
    4927:	55                   	push   %rbp
    4928:	48 89 e5             	mov    %rsp,%rbp
    492b:	48 83 ec 20          	sub    $0x20,%rsp
    492f:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    4933:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    493a:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    4941:	eb 42                	jmp    4985 <dump_int16+0x5e>
    4943:	8b 45 fc             	mov    -0x4(%rbp),%eax
    4946:	48 98                	cltq   
    4948:	48 8d 14 00          	lea    (%rax,%rax,1),%rdx
    494c:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    4950:	48 01 d0             	add    %rdx,%rax
    4953:	0f b7 00             	movzwl (%rax),%eax
    4956:	98                   	cwtl   
    4957:	89 c6                	mov    %eax,%esi
    4959:	48 8d 3d 04 0a 00 00 	lea    0xa04(%rip),%rdi        # 5364 <_IO_stdin_used+0x4>
    4960:	b8 00 00 00 00       	mov    $0x0,%eax
    4965:	e8 66 bc ff ff       	callq  5d0 <printf@plt>
    496a:	8b 45 fc             	mov    -0x4(%rbp),%eax
    496d:	83 c0 01             	add    $0x1,%eax
    4970:	83 e0 07             	and    $0x7,%eax
    4973:	85 c0                	test   %eax,%eax
    4975:	75 0a                	jne    4981 <dump_int16+0x5a>
    4977:	bf 0a 00 00 00       	mov    $0xa,%edi
    497c:	e8 2f bc ff ff       	callq  5b0 <putchar@plt>
    4981:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    4985:	83 7d fc 3f          	cmpl   $0x3f,-0x4(%rbp)
    4989:	7e b8                	jle    4943 <dump_int16+0x1c>
    498b:	bf 0a 00 00 00       	mov    $0xa,%edi
    4990:	e8 1b bc ff ff       	callq  5b0 <putchar@plt>
    4995:	90                   	nop
    4996:	c9                   	leaveq 
    4997:	c3                   	retq   

0000000000004998 <main>:
    4998:	55                   	push   %rbp
    4999:	48 89 e5             	mov    %rsp,%rbp
    499c:	48 81 ec 70 01 00 00 	sub    $0x170,%rsp
    49a3:	89 bd 9c fe ff ff    	mov    %edi,-0x164(%rbp)
    49a9:	48 89 b5 90 fe ff ff 	mov    %rsi,-0x170(%rbp)
    49b0:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    49b7:	00 00 
    49b9:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    49bd:	31 c0                	xor    %eax,%eax
    49bf:	c7 85 ac fe ff ff 00 	movl   $0x0,-0x154(%rbp)
    49c6:	00 00 00 
    49c9:	48 c7 45 b0 00 00 00 	movq   $0x0,-0x50(%rbp)
    49d0:	00 
    49d1:	48 c7 45 b8 00 00 00 	movq   $0x0,-0x48(%rbp)
    49d8:	00 
    49d9:	48 c7 45 c0 00 00 00 	movq   $0x0,-0x40(%rbp)
    49e0:	00 
    49e1:	48 c7 45 c8 00 00 00 	movq   $0x0,-0x38(%rbp)
    49e8:	00 
    49e9:	48 c7 45 d0 00 00 00 	movq   $0x0,-0x30(%rbp)
    49f0:	00 
    49f1:	48 c7 45 d8 00 00 00 	movq   $0x0,-0x28(%rbp)
    49f8:	00 
    49f9:	48 c7 45 e0 00 00 00 	movq   $0x0,-0x20(%rbp)
    4a00:	00 
    4a01:	48 c7 45 e8 00 00 00 	movq   $0x0,-0x18(%rbp)
    4a08:	00 
    4a09:	48 8d 95 b0 fe ff ff 	lea    -0x150(%rbp),%rdx
    4a10:	b8 00 00 00 00       	mov    $0x0,%eax
    4a15:	b9 10 00 00 00       	mov    $0x10,%ecx
    4a1a:	48 89 d7             	mov    %rdx,%rdi
    4a1d:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    4a20:	48 8d 95 30 ff ff ff 	lea    -0xd0(%rbp),%rdx
    4a27:	b8 00 00 00 00       	mov    $0x0,%eax
    4a2c:	b9 10 00 00 00       	mov    $0x10,%ecx
    4a31:	48 89 d7             	mov    %rdx,%rdi
    4a34:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    4a37:	c7 85 ac fe ff ff 00 	movl   $0x0,-0x154(%rbp)
    4a3e:	00 00 00 
    4a41:	eb 1b                	jmp    4a5e <main+0xc6>
    4a43:	8b 85 ac fe ff ff    	mov    -0x154(%rbp),%eax
    4a49:	89 c2                	mov    %eax,%edx
    4a4b:	8b 85 ac fe ff ff    	mov    -0x154(%rbp),%eax
    4a51:	48 98                	cltq   
    4a53:	88 54 05 b0          	mov    %dl,-0x50(%rbp,%rax,1)
    4a57:	83 85 ac fe ff ff 01 	addl   $0x1,-0x154(%rbp)
    4a5e:	83 bd ac fe ff ff 3f 	cmpl   $0x3f,-0x154(%rbp)
    4a65:	7e dc                	jle    4a43 <main+0xab>
    4a67:	48 8d 45 b0          	lea    -0x50(%rbp),%rax
    4a6b:	48 89 c7             	mov    %rax,%rdi
    4a6e:	e8 44 fe ff ff       	callq  48b7 <dump_uint8>
    4a73:	48 8d 45 b0          	lea    -0x50(%rbp),%rax
    4a77:	48 89 c7             	mov    %rax,%rdi
    4a7a:	e8 38 fe ff ff       	callq  48b7 <dump_uint8>
    4a7f:	b8 00 00 00 00       	mov    $0x0,%eax
    4a84:	48 8b 75 f8          	mov    -0x8(%rbp),%rsi
    4a88:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
    4a8f:	00 00 
    4a91:	74 05                	je     4a98 <main+0x100>
    4a93:	e8 28 bb ff ff       	callq  5c0 <__stack_chk_fail@plt>
    4a98:	c9                   	leaveq 
    4a99:	c3                   	retq   
    4a9a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000004aa0 <_jsimd_fdct_islow_avx2>:
    4aa0:	55                   	push   %rbp
    4aa1:	48 89 e0             	mov    %rsp,%rax
    4aa4:	48 89 e5             	mov    %rsp,%rbp
    4aa7:	41 52                	push   %r10
    4aa9:	49 89 fa             	mov    %rdi,%r10
    4aac:	c4 c1 7e 6f 22       	vmovdqu (%r10),%ymm4
    4ab1:	c4 c1 7e 6f 6a 20    	vmovdqu 0x20(%r10),%ymm5
    4ab7:	c4 c1 7e 6f 72 40    	vmovdqu 0x40(%r10),%ymm6
    4abd:	c4 c1 7e 6f 7a 60    	vmovdqu 0x60(%r10),%ymm7
    4ac3:	c4 e3 5d 46 c6 20    	vperm2i128 $0x20,%ymm6,%ymm4,%ymm0
    4ac9:	c4 e3 5d 46 ce 31    	vperm2i128 $0x31,%ymm6,%ymm4,%ymm1
    4acf:	c4 e3 55 46 d7 20    	vperm2i128 $0x20,%ymm7,%ymm5,%ymm2
    4ad5:	c4 e3 55 46 df 31    	vperm2i128 $0x31,%ymm7,%ymm5,%ymm3
    4adb:	c5 fd 61 e1          	vpunpcklwd %ymm1,%ymm0,%ymm4
    4adf:	c5 fd 69 e9          	vpunpckhwd %ymm1,%ymm0,%ymm5
    4ae3:	c5 ed 61 f3          	vpunpcklwd %ymm3,%ymm2,%ymm6
    4ae7:	c5 ed 69 fb          	vpunpckhwd %ymm3,%ymm2,%ymm7
    4aeb:	c5 dd 62 c6          	vpunpckldq %ymm6,%ymm4,%ymm0
    4aef:	c5 dd 6a ce          	vpunpckhdq %ymm6,%ymm4,%ymm1
    4af3:	c5 d5 62 d7          	vpunpckldq %ymm7,%ymm5,%ymm2
    4af7:	c5 d5 6a df          	vpunpckhdq %ymm7,%ymm5,%ymm3
    4afb:	c4 e3 fd 00 c0 8d    	vpermq $0x8d,%ymm0,%ymm0
    4b01:	c4 e3 fd 00 c9 8d    	vpermq $0x8d,%ymm1,%ymm1
    4b07:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
    4b0d:	c4 e3 fd 00 db d8    	vpermq $0xd8,%ymm3,%ymm3
    4b13:	c5 fd f9 e3          	vpsubw %ymm3,%ymm0,%ymm4
    4b17:	c5 fd fd eb          	vpaddw %ymm3,%ymm0,%ymm5
    4b1b:	c5 f5 fd f2          	vpaddw %ymm2,%ymm1,%ymm6
    4b1f:	c5 f5 f9 fa          	vpsubw %ymm2,%ymm1,%ymm7
    4b23:	c4 e3 55 46 ed 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm5
    4b29:	c5 d5 fd c6          	vpaddw %ymm6,%ymm5,%ymm0
    4b2d:	c5 d5 f9 ee          	vpsubw %ymm6,%ymm5,%ymm5
    4b31:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    4b37:	c4 e2 7d 09 05 c0 15 	vpsignw 0x2015c0(%rip),%ymm0,%ymm0        # 206100 <PW_1_NEG1>
    4b3e:	20 00 
    4b40:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    4b44:	c5 fd 71 f6 02       	vpsllw $0x2,%ymm6,%ymm0
    4b49:	c4 e3 55 46 f5 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm6
    4b4f:	c5 d5 61 ce          	vpunpcklwd %ymm6,%ymm5,%ymm1
    4b53:	c5 d5 69 ee          	vpunpckhwd %ymm6,%ymm5,%ymm5
    4b57:	c5 f5 f5 0d c1 14 20 	vpmaddwd 0x2014c1(%rip),%ymm1,%ymm1        # 206020 <_jconst_fdct_islow_avx2>
    4b5e:	00 
    4b5f:	c5 d5 f5 2d b9 14 20 	vpmaddwd 0x2014b9(%rip),%ymm5,%ymm5        # 206020 <_jconst_fdct_islow_avx2>
    4b66:	00 
    4b67:	c5 f5 fe 0d 31 15 20 	vpaddd 0x201531(%rip),%ymm1,%ymm1        # 2060a0 <PD_DESCALE_P1>
    4b6e:	00 
    4b6f:	c5 d5 fe 2d 29 15 20 	vpaddd 0x201529(%rip),%ymm5,%ymm5        # 2060a0 <PD_DESCALE_P1>
    4b76:	00 
    4b77:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
    4b7c:	c5 d5 72 e5 0b       	vpsrad $0xb,%ymm5,%ymm5
    4b81:	c5 f5 6b d5          	vpackssdw %ymm5,%ymm1,%ymm2
    4b85:	c5 c5 fd f4          	vpaddw %ymm4,%ymm7,%ymm6
    4b89:	c4 e3 4d 46 ce 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm1
    4b8f:	c5 cd 61 e9          	vpunpcklwd %ymm1,%ymm6,%ymm5
    4b93:	c5 cd 69 f1          	vpunpckhwd %ymm1,%ymm6,%ymm6
    4b97:	c5 d5 f5 2d a1 14 20 	vpmaddwd 0x2014a1(%rip),%ymm5,%ymm5        # 206040 <PW_MF078_F117_F078_F117>
    4b9e:	00 
    4b9f:	c5 cd f5 35 99 14 20 	vpmaddwd 0x201499(%rip),%ymm6,%ymm6        # 206040 <PW_MF078_F117_F078_F117>
    4ba6:	00 
    4ba7:	c4 e3 5d 46 dc 01    	vperm2i128 $0x1,%ymm4,%ymm4,%ymm3
    4bad:	c5 c5 61 cb          	vpunpcklwd %ymm3,%ymm7,%ymm1
    4bb1:	c5 c5 69 db          	vpunpckhwd %ymm3,%ymm7,%ymm3
    4bb5:	c5 f5 f5 0d a3 14 20 	vpmaddwd 0x2014a3(%rip),%ymm1,%ymm1        # 206060 <PW_MF060_MF089_MF050_MF256>
    4bbc:	00 
    4bbd:	c5 e5 f5 1d 9b 14 20 	vpmaddwd 0x20149b(%rip),%ymm3,%ymm3        # 206060 <PW_MF060_MF089_MF050_MF256>
    4bc4:	00 
    4bc5:	c5 f5 fe cd          	vpaddd %ymm5,%ymm1,%ymm1
    4bc9:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
    4bcd:	c5 f5 fe 0d cb 14 20 	vpaddd 0x2014cb(%rip),%ymm1,%ymm1        # 2060a0 <PD_DESCALE_P1>
    4bd4:	00 
    4bd5:	c5 e5 fe 1d c3 14 20 	vpaddd 0x2014c3(%rip),%ymm3,%ymm3        # 2060a0 <PD_DESCALE_P1>
    4bdc:	00 
    4bdd:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
    4be2:	c5 e5 72 e3 0b       	vpsrad $0xb,%ymm3,%ymm3
    4be7:	c5 f5 6b db          	vpackssdw %ymm3,%ymm1,%ymm3
    4beb:	c4 e3 45 46 cf 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm1
    4bf1:	c5 dd 61 f9          	vpunpcklwd %ymm1,%ymm4,%ymm7
    4bf5:	c5 dd 69 e1          	vpunpckhwd %ymm1,%ymm4,%ymm4
    4bf9:	c5 c5 f5 3d 7f 14 20 	vpmaddwd 0x20147f(%rip),%ymm7,%ymm7        # 206080 <PW_F050_MF256_F060_MF089>
    4c00:	00 
    4c01:	c5 dd f5 25 77 14 20 	vpmaddwd 0x201477(%rip),%ymm4,%ymm4        # 206080 <PW_F050_MF256_F060_MF089>
    4c08:	00 
    4c09:	c5 c5 fe fd          	vpaddd %ymm5,%ymm7,%ymm7
    4c0d:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
    4c11:	c5 c5 fe 3d 87 14 20 	vpaddd 0x201487(%rip),%ymm7,%ymm7        # 2060a0 <PD_DESCALE_P1>
    4c18:	00 
    4c19:	c5 dd fe 25 7f 14 20 	vpaddd 0x20147f(%rip),%ymm4,%ymm4        # 2060a0 <PD_DESCALE_P1>
    4c20:	00 
    4c21:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
    4c26:	c5 dd 72 e4 0b       	vpsrad $0xb,%ymm4,%ymm4
    4c2b:	c5 c5 6b cc          	vpackssdw %ymm4,%ymm7,%ymm1
    4c2f:	c4 e3 75 46 e3 20    	vperm2i128 $0x20,%ymm3,%ymm1,%ymm4
    4c35:	c4 e3 75 46 cb 31    	vperm2i128 $0x31,%ymm3,%ymm1,%ymm1
    4c3b:	c5 fd 61 d9          	vpunpcklwd %ymm1,%ymm0,%ymm3
    4c3f:	c5 fd 69 e9          	vpunpckhwd %ymm1,%ymm0,%ymm5
    4c43:	c5 ed 61 f4          	vpunpcklwd %ymm4,%ymm2,%ymm6
    4c47:	c5 ed 69 fc          	vpunpckhwd %ymm4,%ymm2,%ymm7
    4c4b:	c5 e5 62 c6          	vpunpckldq %ymm6,%ymm3,%ymm0
    4c4f:	c5 e5 6a ce          	vpunpckhdq %ymm6,%ymm3,%ymm1
    4c53:	c5 d5 62 d7          	vpunpckldq %ymm7,%ymm5,%ymm2
    4c57:	c5 d5 6a e7          	vpunpckhdq %ymm7,%ymm5,%ymm4
    4c5b:	c4 e3 fd 00 c0 8d    	vpermq $0x8d,%ymm0,%ymm0
    4c61:	c4 e3 fd 00 c9 8d    	vpermq $0x8d,%ymm1,%ymm1
    4c67:	c4 e3 fd 00 d2 d8    	vpermq $0xd8,%ymm2,%ymm2
    4c6d:	c4 e3 fd 00 e4 d8    	vpermq $0xd8,%ymm4,%ymm4
    4c73:	c5 fd f9 dc          	vpsubw %ymm4,%ymm0,%ymm3
    4c77:	c5 fd fd ec          	vpaddw %ymm4,%ymm0,%ymm5
    4c7b:	c5 f5 fd f2          	vpaddw %ymm2,%ymm1,%ymm6
    4c7f:	c5 f5 f9 fa          	vpsubw %ymm2,%ymm1,%ymm7
    4c83:	c4 e3 55 46 ed 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm5
    4c89:	c5 d5 fd c6          	vpaddw %ymm6,%ymm5,%ymm0
    4c8d:	c5 d5 f9 ee          	vpsubw %ymm6,%ymm5,%ymm5
    4c91:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    4c97:	c4 e2 7d 09 05 60 14 	vpsignw 0x201460(%rip),%ymm0,%ymm0        # 206100 <PW_1_NEG1>
    4c9e:	20 00 
    4ca0:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    4ca4:	c5 cd fd 35 34 14 20 	vpaddw 0x201434(%rip),%ymm6,%ymm6        # 2060e0 <PW_DESCALE_P2X>
    4cab:	00 
    4cac:	c5 fd 71 e6 02       	vpsraw $0x2,%ymm6,%ymm0
    4cb1:	c4 e3 55 46 f5 01    	vperm2i128 $0x1,%ymm5,%ymm5,%ymm6
    4cb7:	c5 d5 61 ce          	vpunpcklwd %ymm6,%ymm5,%ymm1
    4cbb:	c5 d5 69 ee          	vpunpckhwd %ymm6,%ymm5,%ymm5
    4cbf:	c5 f5 f5 0d 59 13 20 	vpmaddwd 0x201359(%rip),%ymm1,%ymm1        # 206020 <_jconst_fdct_islow_avx2>
    4cc6:	00 
    4cc7:	c5 d5 f5 2d 51 13 20 	vpmaddwd 0x201351(%rip),%ymm5,%ymm5        # 206020 <_jconst_fdct_islow_avx2>
    4cce:	00 
    4ccf:	c5 f5 fe 0d e9 13 20 	vpaddd 0x2013e9(%rip),%ymm1,%ymm1        # 2060c0 <PD_DESCALE_P2>
    4cd6:	00 
    4cd7:	c5 d5 fe 2d e1 13 20 	vpaddd 0x2013e1(%rip),%ymm5,%ymm5        # 2060c0 <PD_DESCALE_P2>
    4cde:	00 
    4cdf:	c5 f5 72 e1 0f       	vpsrad $0xf,%ymm1,%ymm1
    4ce4:	c5 d5 72 e5 0f       	vpsrad $0xf,%ymm5,%ymm5
    4ce9:	c5 f5 6b d5          	vpackssdw %ymm5,%ymm1,%ymm2
    4ced:	c5 c5 fd f3          	vpaddw %ymm3,%ymm7,%ymm6
    4cf1:	c4 e3 4d 46 ce 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm1
    4cf7:	c5 cd 61 e9          	vpunpcklwd %ymm1,%ymm6,%ymm5
    4cfb:	c5 cd 69 f1          	vpunpckhwd %ymm1,%ymm6,%ymm6
    4cff:	c5 d5 f5 2d 39 13 20 	vpmaddwd 0x201339(%rip),%ymm5,%ymm5        # 206040 <PW_MF078_F117_F078_F117>
    4d06:	00 
    4d07:	c5 cd f5 35 31 13 20 	vpmaddwd 0x201331(%rip),%ymm6,%ymm6        # 206040 <PW_MF078_F117_F078_F117>
    4d0e:	00 
    4d0f:	c4 e3 65 46 e3 01    	vperm2i128 $0x1,%ymm3,%ymm3,%ymm4
    4d15:	c5 c5 61 cc          	vpunpcklwd %ymm4,%ymm7,%ymm1
    4d19:	c5 c5 69 e4          	vpunpckhwd %ymm4,%ymm7,%ymm4
    4d1d:	c5 f5 f5 0d 3b 13 20 	vpmaddwd 0x20133b(%rip),%ymm1,%ymm1        # 206060 <PW_MF060_MF089_MF050_MF256>
    4d24:	00 
    4d25:	c5 dd f5 25 33 13 20 	vpmaddwd 0x201333(%rip),%ymm4,%ymm4        # 206060 <PW_MF060_MF089_MF050_MF256>
    4d2c:	00 
    4d2d:	c5 f5 fe cd          	vpaddd %ymm5,%ymm1,%ymm1
    4d31:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
    4d35:	c5 f5 fe 0d 83 13 20 	vpaddd 0x201383(%rip),%ymm1,%ymm1        # 2060c0 <PD_DESCALE_P2>
    4d3c:	00 
    4d3d:	c5 dd fe 25 7b 13 20 	vpaddd 0x20137b(%rip),%ymm4,%ymm4        # 2060c0 <PD_DESCALE_P2>
    4d44:	00 
    4d45:	c5 f5 72 e1 0f       	vpsrad $0xf,%ymm1,%ymm1
    4d4a:	c5 dd 72 e4 0f       	vpsrad $0xf,%ymm4,%ymm4
    4d4f:	c5 f5 6b e4          	vpackssdw %ymm4,%ymm1,%ymm4
    4d53:	c4 e3 45 46 cf 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm1
    4d59:	c5 e5 61 f9          	vpunpcklwd %ymm1,%ymm3,%ymm7
    4d5d:	c5 e5 69 d9          	vpunpckhwd %ymm1,%ymm3,%ymm3
    4d61:	c5 c5 f5 3d 17 13 20 	vpmaddwd 0x201317(%rip),%ymm7,%ymm7        # 206080 <PW_F050_MF256_F060_MF089>
    4d68:	00 
    4d69:	c5 e5 f5 1d 0f 13 20 	vpmaddwd 0x20130f(%rip),%ymm3,%ymm3        # 206080 <PW_F050_MF256_F060_MF089>
    4d70:	00 
    4d71:	c5 c5 fe fd          	vpaddd %ymm5,%ymm7,%ymm7
    4d75:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
    4d79:	c5 c5 fe 3d 3f 13 20 	vpaddd 0x20133f(%rip),%ymm7,%ymm7        # 2060c0 <PD_DESCALE_P2>
    4d80:	00 
    4d81:	c5 e5 fe 1d 37 13 20 	vpaddd 0x201337(%rip),%ymm3,%ymm3        # 2060c0 <PD_DESCALE_P2>
    4d88:	00 
    4d89:	c5 c5 72 e7 0f       	vpsrad $0xf,%ymm7,%ymm7
    4d8e:	c5 e5 72 e3 0f       	vpsrad $0xf,%ymm3,%ymm3
    4d93:	c5 c5 6b cb          	vpackssdw %ymm3,%ymm7,%ymm1
    4d97:	c4 e3 7d 46 d9 30    	vperm2i128 $0x30,%ymm1,%ymm0,%ymm3
    4d9d:	c4 e3 6d 46 e9 20    	vperm2i128 $0x20,%ymm1,%ymm2,%ymm5
    4da3:	c4 e3 7d 46 f4 31    	vperm2i128 $0x31,%ymm4,%ymm0,%ymm6
    4da9:	c4 e3 6d 46 fc 21    	vperm2i128 $0x21,%ymm4,%ymm2,%ymm7
    4daf:	c4 c1 7e 7f 1a       	vmovdqu %ymm3,(%r10)
    4db4:	c4 c1 7e 7f 6a 20    	vmovdqu %ymm5,0x20(%r10)
    4dba:	c4 c1 7e 7f 72 40    	vmovdqu %ymm6,0x40(%r10)
    4dc0:	c4 c1 7e 7f 7a 60    	vmovdqu %ymm7,0x60(%r10)
    4dc6:	c5 f8 77             	vzeroupper 
    4dc9:	41 5a                	pop    %r10
    4dcb:	5d                   	pop    %rbp
    4dcc:	c3                   	retq   
    4dcd:	90                   	nop
    4dce:	90                   	nop
    4dcf:	90                   	nop
    4dd0:	90                   	nop
    4dd1:	90                   	nop
    4dd2:	90                   	nop
    4dd3:	90                   	nop
    4dd4:	90                   	nop
    4dd5:	90                   	nop
    4dd6:	90                   	nop
    4dd7:	90                   	nop
    4dd8:	90                   	nop
    4dd9:	90                   	nop
    4dda:	90                   	nop
    4ddb:	90                   	nop
    4ddc:	90                   	nop
    4ddd:	90                   	nop
    4dde:	90                   	nop
    4ddf:	90                   	nop

0000000000004de0 <_jsimd_idct_islow_avx2>:
    4de0:	55                   	push   %rbp
    4de1:	48 89 e0             	mov    %rsp,%rax
    4de4:	48 89 e5             	mov    %rsp,%rbp
    4de7:	41 52                	push   %r10
    4de9:	49 89 fa             	mov    %rdi,%r10
    4dec:	41 53                	push   %r11
    4dee:	49 89 f3             	mov    %rsi,%r11
    4df1:	41 54                	push   %r12
    4df3:	49 89 d4             	mov    %rdx,%r12
    4df6:	41 55                	push   %r13
    4df8:	49 89 cd             	mov    %rcx,%r13
    4dfb:	41 8b 43 10          	mov    0x10(%r11),%eax
    4dff:	41 0b 43 20          	or     0x20(%r11),%eax
    4e03:	0f 85 75 00 00 00    	jne    4e7e <_jsimd_idct_islow_avx2.columnDCT>
    4e09:	66 41 0f 6f 43 10    	movdqa 0x10(%r11),%xmm0
    4e0f:	66 41 0f 6f 4b 20    	movdqa 0x20(%r11),%xmm1
    4e15:	c4 c1 79 eb 43 30    	vpor   0x30(%r11),%xmm0,%xmm0
    4e1b:	c4 c1 71 eb 4b 40    	vpor   0x40(%r11),%xmm1,%xmm1
    4e21:	c4 c1 79 eb 43 50    	vpor   0x50(%r11),%xmm0,%xmm0
    4e27:	c4 c1 71 eb 4b 60    	vpor   0x60(%r11),%xmm1,%xmm1
    4e2d:	c4 c1 79 eb 43 70    	vpor   0x70(%r11),%xmm0,%xmm0
    4e33:	c5 f1 eb c8          	vpor   %xmm0,%xmm1,%xmm1
    4e37:	c5 f1 63 c9          	vpacksswb %xmm1,%xmm1,%xmm1
    4e3b:	c5 f1 63 c9          	vpacksswb %xmm1,%xmm1,%xmm1
    4e3f:	66 0f 7e c8          	movd   %xmm1,%eax
    4e43:	48 85 c0             	test   %rax,%rax
    4e46:	75 36                	jne    4e7e <_jsimd_idct_islow_avx2.columnDCT>
    4e48:	66 41 0f 6f 2b       	movdqa (%r11),%xmm5
    4e4d:	c4 c1 51 d5 2a       	vpmullw (%r10),%xmm5,%xmm5
    4e52:	c5 d1 71 f5 02       	vpsllw $0x2,%xmm5,%xmm5
    4e57:	c5 d1 61 e5          	vpunpcklwd %xmm5,%xmm5,%xmm4
    4e5b:	c5 d1 69 ed          	vpunpckhwd %xmm5,%xmm5,%xmm5
    4e5f:	c4 e3 5d 38 e5 01    	vinserti128 $0x1,%xmm5,%ymm4,%ymm4
    4e65:	c5 fd 70 c4 00       	vpshufd $0x0,%ymm4,%ymm0
    4e6a:	c5 fd 70 cc 55       	vpshufd $0x55,%ymm4,%ymm1
    4e6f:	c5 fd 70 d4 aa       	vpshufd $0xaa,%ymm4,%ymm2
    4e74:	c5 fd 70 dc ff       	vpshufd $0xff,%ymm4,%ymm3
    4e79:	e9 e9 01 00 00       	jmpq   5067 <_jsimd_idct_islow_avx2.column_end>

0000000000004e7e <_jsimd_idct_islow_avx2.columnDCT>:
    4e7e:	c4 c1 7e 6f 23       	vmovdqu (%r11),%ymm4
    4e83:	c4 c1 7e 6f 6b 20    	vmovdqu 0x20(%r11),%ymm5
    4e89:	c4 c1 7e 6f 73 40    	vmovdqu 0x40(%r11),%ymm6
    4e8f:	c4 c1 7e 6f 7b 60    	vmovdqu 0x60(%r11),%ymm7
    4e95:	c4 c1 5d d5 22       	vpmullw (%r10),%ymm4,%ymm4
    4e9a:	c4 c1 55 d5 6a 20    	vpmullw 0x20(%r10),%ymm5,%ymm5
    4ea0:	c4 c1 4d d5 72 40    	vpmullw 0x40(%r10),%ymm6,%ymm6
    4ea6:	c4 c1 45 d5 7a 60    	vpmullw 0x60(%r10),%ymm7,%ymm7
    4eac:	c4 e3 5d 46 c6 20    	vperm2i128 $0x20,%ymm6,%ymm4,%ymm0
    4eb2:	c4 e3 55 46 cc 31    	vperm2i128 $0x31,%ymm4,%ymm5,%ymm1
    4eb8:	c4 e3 55 46 d7 20    	vperm2i128 $0x20,%ymm7,%ymm5,%ymm2
    4ebe:	c4 e3 45 46 de 31    	vperm2i128 $0x31,%ymm6,%ymm7,%ymm3
    4ec4:	c4 e3 6d 46 ea 01    	vperm2i128 $0x1,%ymm2,%ymm2,%ymm5
    4eca:	c5 ed 61 e5          	vpunpcklwd %ymm5,%ymm2,%ymm4
    4ece:	c5 ed 69 ed          	vpunpckhwd %ymm5,%ymm2,%ymm5
    4ed2:	c5 dd f5 25 46 12 20 	vpmaddwd 0x201246(%rip),%ymm4,%ymm4        # 206120 <_jconst_idct_islow_avx2>
    4ed9:	00 
    4eda:	c5 d5 f5 2d 3e 12 20 	vpmaddwd 0x20123e(%rip),%ymm5,%ymm5        # 206120 <_jconst_idct_islow_avx2>
    4ee1:	00 
    4ee2:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    4ee8:	c4 e2 7d 09 05 0f 13 	vpsignw 0x20130f(%rip),%ymm0,%ymm0        # 206200 <PW_1_NEG1>
    4eef:	20 00 
    4ef1:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    4ef5:	c5 fd ef c0          	vpxor  %ymm0,%ymm0,%ymm0
    4ef9:	c5 fd 61 fe          	vpunpcklwd %ymm6,%ymm0,%ymm7
    4efd:	c5 fd 69 c6          	vpunpckhwd %ymm6,%ymm0,%ymm0
    4f01:	c5 c5 72 e7 03       	vpsrad $0x3,%ymm7,%ymm7
    4f06:	c5 fd 72 e0 03       	vpsrad $0x3,%ymm0,%ymm0
    4f0b:	c5 45 fa d4          	vpsubd %ymm4,%ymm7,%ymm10
    4f0f:	c5 45 fe c4          	vpaddd %ymm4,%ymm7,%ymm8
    4f13:	c5 7d fa dd          	vpsubd %ymm5,%ymm0,%ymm11
    4f17:	c5 7d fe cd          	vpaddd %ymm5,%ymm0,%ymm9
    4f1b:	c5 e5 fd c1          	vpaddw %ymm1,%ymm3,%ymm0
    4f1f:	c4 e3 7d 46 f8 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm7
    4f25:	c5 fd 61 f7          	vpunpcklwd %ymm7,%ymm0,%ymm6
    4f29:	c5 fd 69 ff          	vpunpckhwd %ymm7,%ymm0,%ymm7
    4f2d:	c5 cd f5 35 0b 12 20 	vpmaddwd 0x20120b(%rip),%ymm6,%ymm6        # 206140 <PW_MF078_F117_F078_F117>
    4f34:	00 
    4f35:	c5 c5 f5 3d 03 12 20 	vpmaddwd 0x201203(%rip),%ymm7,%ymm7        # 206140 <PW_MF078_F117_F078_F117>
    4f3c:	00 
    4f3d:	c4 e3 75 46 c9 01    	vperm2i128 $0x1,%ymm1,%ymm1,%ymm1
    4f43:	c5 e5 61 d1          	vpunpcklwd %ymm1,%ymm3,%ymm2
    4f47:	c5 e5 69 d9          	vpunpckhwd %ymm1,%ymm3,%ymm3
    4f4b:	c5 ed f5 25 0d 12 20 	vpmaddwd 0x20120d(%rip),%ymm2,%ymm4        # 206160 <PW_MF060_MF089_MF050_MF256>
    4f52:	00 
    4f53:	c5 e5 f5 2d 05 12 20 	vpmaddwd 0x201205(%rip),%ymm3,%ymm5        # 206160 <PW_MF060_MF089_MF050_MF256>
    4f5a:	00 
    4f5b:	c5 dd fe e6          	vpaddd %ymm6,%ymm4,%ymm4
    4f5f:	c5 d5 fe ef          	vpaddd %ymm7,%ymm5,%ymm5
    4f63:	c5 ed f5 15 15 12 20 	vpmaddwd 0x201215(%rip),%ymm2,%ymm2        # 206180 <PW_MF089_F060_MF256_F050>
    4f6a:	00 
    4f6b:	c5 e5 f5 1d 0d 12 20 	vpmaddwd 0x20120d(%rip),%ymm3,%ymm3        # 206180 <PW_MF089_F060_MF256_F050>
    4f72:	00 
    4f73:	c4 e3 4d 46 f6 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm6
    4f79:	c4 e3 45 46 ff 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm7
    4f7f:	c5 ed fe f6          	vpaddd %ymm6,%ymm2,%ymm6
    4f83:	c5 e5 fe ff          	vpaddd %ymm7,%ymm3,%ymm7
    4f87:	c5 bd fe c6          	vpaddd %ymm6,%ymm8,%ymm0
    4f8b:	c5 b5 fe cf          	vpaddd %ymm7,%ymm9,%ymm1
    4f8f:	c5 fd fe 05 09 12 20 	vpaddd 0x201209(%rip),%ymm0,%ymm0        # 2061a0 <PD_DESCALE_P1>
    4f96:	00 
    4f97:	c5 f5 fe 0d 01 12 20 	vpaddd 0x201201(%rip),%ymm1,%ymm1        # 2061a0 <PD_DESCALE_P1>
    4f9e:	00 
    4f9f:	c5 fd 72 e0 0b       	vpsrad $0xb,%ymm0,%ymm0
    4fa4:	c5 f5 72 e1 0b       	vpsrad $0xb,%ymm1,%ymm1
    4fa9:	c5 fd 6b c1          	vpackssdw %ymm1,%ymm0,%ymm0
    4fad:	c5 bd fa d6          	vpsubd %ymm6,%ymm8,%ymm2
    4fb1:	c5 b5 fa df          	vpsubd %ymm7,%ymm9,%ymm3
    4fb5:	c5 ed fe 15 e3 11 20 	vpaddd 0x2011e3(%rip),%ymm2,%ymm2        # 2061a0 <PD_DESCALE_P1>
    4fbc:	00 
    4fbd:	c5 e5 fe 1d db 11 20 	vpaddd 0x2011db(%rip),%ymm3,%ymm3        # 2061a0 <PD_DESCALE_P1>
    4fc4:	00 
    4fc5:	c5 ed 72 e2 0b       	vpsrad $0xb,%ymm2,%ymm2
    4fca:	c5 e5 72 e3 0b       	vpsrad $0xb,%ymm3,%ymm3
    4fcf:	c5 ed 6b db          	vpackssdw %ymm3,%ymm2,%ymm3
    4fd3:	c5 ad fe f4          	vpaddd %ymm4,%ymm10,%ymm6
    4fd7:	c5 a5 fe fd          	vpaddd %ymm5,%ymm11,%ymm7
    4fdb:	c5 cd fe 35 bd 11 20 	vpaddd 0x2011bd(%rip),%ymm6,%ymm6        # 2061a0 <PD_DESCALE_P1>
    4fe2:	00 
    4fe3:	c5 c5 fe 3d b5 11 20 	vpaddd 0x2011b5(%rip),%ymm7,%ymm7        # 2061a0 <PD_DESCALE_P1>
    4fea:	00 
    4feb:	c5 cd 72 e6 0b       	vpsrad $0xb,%ymm6,%ymm6
    4ff0:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
    4ff5:	c5 cd 6b cf          	vpackssdw %ymm7,%ymm6,%ymm1
    4ff9:	c5 ad fa f4          	vpsubd %ymm4,%ymm10,%ymm6
    4ffd:	c5 a5 fa fd          	vpsubd %ymm5,%ymm11,%ymm7
    5001:	c5 cd fe 35 97 11 20 	vpaddd 0x201197(%rip),%ymm6,%ymm6        # 2061a0 <PD_DESCALE_P1>
    5008:	00 
    5009:	c5 c5 fe 3d 8f 11 20 	vpaddd 0x20118f(%rip),%ymm7,%ymm7        # 2061a0 <PD_DESCALE_P1>
    5010:	00 
    5011:	c5 cd 72 e6 0b       	vpsrad $0xb,%ymm6,%ymm6
    5016:	c5 c5 72 e7 0b       	vpsrad $0xb,%ymm7,%ymm7
    501b:	c5 cd 6b d7          	vpackssdw %ymm7,%ymm6,%ymm2
    501f:	c4 e3 fd 00 e0 d8    	vpermq $0xd8,%ymm0,%ymm4
    5025:	c4 e3 fd 00 e9 72    	vpermq $0x72,%ymm1,%ymm5
    502b:	c4 e3 fd 00 f2 d8    	vpermq $0xd8,%ymm2,%ymm6
    5031:	c4 e3 fd 00 fb 72    	vpermq $0x72,%ymm3,%ymm7
    5037:	c5 dd 61 c5          	vpunpcklwd %ymm5,%ymm4,%ymm0
    503b:	c5 dd 69 cd          	vpunpckhwd %ymm5,%ymm4,%ymm1
    503f:	c5 cd 61 d7          	vpunpcklwd %ymm7,%ymm6,%ymm2
    5043:	c5 cd 69 df          	vpunpckhwd %ymm7,%ymm6,%ymm3
    5047:	c5 fd 61 e1          	vpunpcklwd %ymm1,%ymm0,%ymm4
    504b:	c5 ed 61 eb          	vpunpcklwd %ymm3,%ymm2,%ymm5
    504f:	c5 fd 69 f1          	vpunpckhwd %ymm1,%ymm0,%ymm6
    5053:	c5 ed 69 fb          	vpunpckhwd %ymm3,%ymm2,%ymm7
    5057:	c5 dd 6c c5          	vpunpcklqdq %ymm5,%ymm4,%ymm0
    505b:	c5 dd 6d cd          	vpunpckhqdq %ymm5,%ymm4,%ymm1
    505f:	c5 cd 6c d7          	vpunpcklqdq %ymm7,%ymm6,%ymm2
    5063:	c5 cd 6d df          	vpunpckhqdq %ymm7,%ymm6,%ymm3

0000000000005067 <_jsimd_idct_islow_avx2.column_end>:
    5067:	41 0f 18 83 80 00 00 	prefetchnta 0x80(%r11)
    506e:	00 
    506f:	41 0f 18 83 a0 00 00 	prefetchnta 0xa0(%r11)
    5076:	00 
    5077:	41 0f 18 83 c0 00 00 	prefetchnta 0xc0(%r11)
    507e:	00 
    507f:	41 0f 18 83 e0 00 00 	prefetchnta 0xe0(%r11)
    5086:	00 
    5087:	c4 e3 65 46 e1 31    	vperm2i128 $0x31,%ymm1,%ymm3,%ymm4
    508d:	c4 e3 65 46 c9 20    	vperm2i128 $0x20,%ymm1,%ymm3,%ymm1
    5093:	c4 e3 6d 46 ea 01    	vperm2i128 $0x1,%ymm2,%ymm2,%ymm5
    5099:	c5 ed 61 dd          	vpunpcklwd %ymm5,%ymm2,%ymm3
    509d:	c5 ed 69 ed          	vpunpckhwd %ymm5,%ymm2,%ymm5
    50a1:	c5 e5 f5 1d 77 10 20 	vpmaddwd 0x201077(%rip),%ymm3,%ymm3        # 206120 <_jconst_idct_islow_avx2>
    50a8:	00 
    50a9:	c5 d5 f5 2d 6f 10 20 	vpmaddwd 0x20106f(%rip),%ymm5,%ymm5        # 206120 <_jconst_idct_islow_avx2>
    50b0:	00 
    50b1:	c4 e3 7d 46 f0 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm6
    50b7:	c4 e2 7d 09 05 40 11 	vpsignw 0x201140(%rip),%ymm0,%ymm0        # 206200 <PW_1_NEG1>
    50be:	20 00 
    50c0:	c5 cd fd f0          	vpaddw %ymm0,%ymm6,%ymm6
    50c4:	c5 fd ef c0          	vpxor  %ymm0,%ymm0,%ymm0
    50c8:	c5 fd 61 fe          	vpunpcklwd %ymm6,%ymm0,%ymm7
    50cc:	c5 fd 69 c6          	vpunpckhwd %ymm6,%ymm0,%ymm0
    50d0:	c5 c5 72 e7 03       	vpsrad $0x3,%ymm7,%ymm7
    50d5:	c5 fd 72 e0 03       	vpsrad $0x3,%ymm0,%ymm0
    50da:	c5 45 fa d3          	vpsubd %ymm3,%ymm7,%ymm10
    50de:	c5 45 fe c3          	vpaddd %ymm3,%ymm7,%ymm8
    50e2:	c5 7d fa dd          	vpsubd %ymm5,%ymm0,%ymm11
    50e6:	c5 7d fe cd          	vpaddd %ymm5,%ymm0,%ymm9
    50ea:	c5 dd fd c1          	vpaddw %ymm1,%ymm4,%ymm0
    50ee:	c4 e3 7d 46 f8 01    	vperm2i128 $0x1,%ymm0,%ymm0,%ymm7
    50f4:	c5 fd 61 f7          	vpunpcklwd %ymm7,%ymm0,%ymm6
    50f8:	c5 fd 69 ff          	vpunpckhwd %ymm7,%ymm0,%ymm7
    50fc:	c5 cd f5 35 3c 10 20 	vpmaddwd 0x20103c(%rip),%ymm6,%ymm6        # 206140 <PW_MF078_F117_F078_F117>
    5103:	00 
    5104:	c5 c5 f5 3d 34 10 20 	vpmaddwd 0x201034(%rip),%ymm7,%ymm7        # 206140 <PW_MF078_F117_F078_F117>
    510b:	00 
    510c:	c4 e3 75 46 c9 01    	vperm2i128 $0x1,%ymm1,%ymm1,%ymm1
    5112:	c5 dd 61 d1          	vpunpcklwd %ymm1,%ymm4,%ymm2
    5116:	c5 dd 69 e1          	vpunpckhwd %ymm1,%ymm4,%ymm4
    511a:	c5 ed f5 1d 3e 10 20 	vpmaddwd 0x20103e(%rip),%ymm2,%ymm3        # 206160 <PW_MF060_MF089_MF050_MF256>
    5121:	00 
    5122:	c5 dd f5 2d 36 10 20 	vpmaddwd 0x201036(%rip),%ymm4,%ymm5        # 206160 <PW_MF060_MF089_MF050_MF256>
    5129:	00 
    512a:	c5 e5 fe de          	vpaddd %ymm6,%ymm3,%ymm3
    512e:	c5 d5 fe ef          	vpaddd %ymm7,%ymm5,%ymm5
    5132:	c5 ed f5 15 46 10 20 	vpmaddwd 0x201046(%rip),%ymm2,%ymm2        # 206180 <PW_MF089_F060_MF256_F050>
    5139:	00 
    513a:	c5 dd f5 25 3e 10 20 	vpmaddwd 0x20103e(%rip),%ymm4,%ymm4        # 206180 <PW_MF089_F060_MF256_F050>
    5141:	00 
    5142:	c4 e3 4d 46 f6 01    	vperm2i128 $0x1,%ymm6,%ymm6,%ymm6
    5148:	c4 e3 45 46 ff 01    	vperm2i128 $0x1,%ymm7,%ymm7,%ymm7
    514e:	c5 ed fe f6          	vpaddd %ymm6,%ymm2,%ymm6
    5152:	c5 dd fe ff          	vpaddd %ymm7,%ymm4,%ymm7
    5156:	c5 bd fe c6          	vpaddd %ymm6,%ymm8,%ymm0
    515a:	c5 b5 fe cf          	vpaddd %ymm7,%ymm9,%ymm1
    515e:	c5 fd fe 05 5a 10 20 	vpaddd 0x20105a(%rip),%ymm0,%ymm0        # 2061c0 <PD_DESCALE_P2>
    5165:	00 
    5166:	c5 f5 fe 0d 52 10 20 	vpaddd 0x201052(%rip),%ymm1,%ymm1        # 2061c0 <PD_DESCALE_P2>
    516d:	00 
    516e:	c5 fd 72 e0 12       	vpsrad $0x12,%ymm0,%ymm0
    5173:	c5 f5 72 e1 12       	vpsrad $0x12,%ymm1,%ymm1
    5178:	c5 fd 6b c1          	vpackssdw %ymm1,%ymm0,%ymm0
    517c:	c5 bd fa d6          	vpsubd %ymm6,%ymm8,%ymm2
    5180:	c5 b5 fa e7          	vpsubd %ymm7,%ymm9,%ymm4
    5184:	c5 ed fe 15 34 10 20 	vpaddd 0x201034(%rip),%ymm2,%ymm2        # 2061c0 <PD_DESCALE_P2>
    518b:	00 
    518c:	c5 dd fe 25 2c 10 20 	vpaddd 0x20102c(%rip),%ymm4,%ymm4        # 2061c0 <PD_DESCALE_P2>
    5193:	00 
    5194:	c5 ed 72 e2 12       	vpsrad $0x12,%ymm2,%ymm2
    5199:	c5 dd 72 e4 12       	vpsrad $0x12,%ymm4,%ymm4
    519e:	c5 ed 6b e4          	vpackssdw %ymm4,%ymm2,%ymm4
    51a2:	c5 ad fe f3          	vpaddd %ymm3,%ymm10,%ymm6
    51a6:	c5 a5 fe fd          	vpaddd %ymm5,%ymm11,%ymm7
    51aa:	c5 cd fe 35 0e 10 20 	vpaddd 0x20100e(%rip),%ymm6,%ymm6        # 2061c0 <PD_DESCALE_P2>
    51b1:	00 
    51b2:	c5 c5 fe 3d 06 10 20 	vpaddd 0x201006(%rip),%ymm7,%ymm7        # 2061c0 <PD_DESCALE_P2>
    51b9:	00 
    51ba:	c5 cd 72 e6 12       	vpsrad $0x12,%ymm6,%ymm6
    51bf:	c5 c5 72 e7 12       	vpsrad $0x12,%ymm7,%ymm7
    51c4:	c5 cd 6b cf          	vpackssdw %ymm7,%ymm6,%ymm1
    51c8:	c5 ad fa f3          	vpsubd %ymm3,%ymm10,%ymm6
    51cc:	c5 a5 fa fd          	vpsubd %ymm5,%ymm11,%ymm7
    51d0:	c5 cd fe 35 e8 0f 20 	vpaddd 0x200fe8(%rip),%ymm6,%ymm6        # 2061c0 <PD_DESCALE_P2>
    51d7:	00 
    51d8:	c5 c5 fe 3d e0 0f 20 	vpaddd 0x200fe0(%rip),%ymm7,%ymm7        # 2061c0 <PD_DESCALE_P2>
    51df:	00 
    51e0:	c5 cd 72 e6 12       	vpsrad $0x12,%ymm6,%ymm6
    51e5:	c5 c5 72 e7 12       	vpsrad $0x12,%ymm7,%ymm7
    51ea:	c5 cd 6b d7          	vpackssdw %ymm7,%ymm6,%ymm2
    51ee:	c4 e3 fd 00 d8 d8    	vpermq $0xd8,%ymm0,%ymm3
    51f4:	c4 e3 fd 00 e9 72    	vpermq $0x72,%ymm1,%ymm5
    51fa:	c4 e3 fd 00 f2 d8    	vpermq $0xd8,%ymm2,%ymm6
    5200:	c4 e3 fd 00 fc 72    	vpermq $0x72,%ymm4,%ymm7
    5206:	c5 e5 61 c5          	vpunpcklwd %ymm5,%ymm3,%ymm0
    520a:	c5 e5 69 cd          	vpunpckhwd %ymm5,%ymm3,%ymm1
    520e:	c5 cd 61 d7          	vpunpcklwd %ymm7,%ymm6,%ymm2
    5212:	c5 cd 69 e7          	vpunpckhwd %ymm7,%ymm6,%ymm4
    5216:	c5 fd 61 d9          	vpunpcklwd %ymm1,%ymm0,%ymm3
    521a:	c5 ed 61 ec          	vpunpcklwd %ymm4,%ymm2,%ymm5
    521e:	c5 fd 69 f1          	vpunpckhwd %ymm1,%ymm0,%ymm6
    5222:	c5 ed 69 fc          	vpunpckhwd %ymm4,%ymm2,%ymm7
    5226:	c5 e5 6c c5          	vpunpcklqdq %ymm5,%ymm3,%ymm0
    522a:	c5 e5 6d cd          	vpunpckhqdq %ymm5,%ymm3,%ymm1
    522e:	c5 cd 6c d7          	vpunpcklqdq %ymm7,%ymm6,%ymm2
    5232:	c5 cd 6d e7          	vpunpckhqdq %ymm7,%ymm6,%ymm4
    5236:	c5 fd 63 c1          	vpacksswb %ymm1,%ymm0,%ymm0
    523a:	c5 ed 63 cc          	vpacksswb %ymm4,%ymm2,%ymm1
    523e:	c5 fd fc 05 9a 0f 20 	vpaddb 0x200f9a(%rip),%ymm0,%ymm0        # 2061e0 <PB_CENTERJSAMP>
    5245:	00 
    5246:	c5 f5 fc 0d 92 0f 20 	vpaddb 0x200f92(%rip),%ymm1,%ymm1        # 2061e0 <PB_CENTERJSAMP>
    524d:	00 
    524e:	c4 e3 7d 39 ce 01    	vextracti128 $0x1,%ymm1,%xmm6
    5254:	c4 e3 7d 39 c4 01    	vextracti128 $0x1,%ymm0,%xmm4
    525a:	c4 e3 7d 39 ca 00    	vextracti128 $0x0,%ymm1,%xmm2
    5260:	c4 e3 7d 39 c0 00    	vextracti128 $0x0,%ymm0,%xmm0
    5266:	c5 f9 70 c8 4e       	vpshufd $0x4e,%xmm0,%xmm1
    526b:	c5 f9 70 da 4e       	vpshufd $0x4e,%xmm2,%xmm3
    5270:	c5 f9 70 ec 4e       	vpshufd $0x4e,%xmm4,%xmm5
    5275:	c5 f9 70 fe 4e       	vpshufd $0x4e,%xmm6,%xmm7
    527a:	c5 f8 77             	vzeroupper 
    527d:	44 89 e8             	mov    %r13d,%eax
    5280:	49 8b 14 24          	mov    (%r12),%rdx
    5284:	49 8b 74 24 08       	mov    0x8(%r12),%rsi
    5289:	66 0f d6 04 02       	movq   %xmm0,(%rdx,%rax,1)
    528e:	66 0f d6 0c 06       	movq   %xmm1,(%rsi,%rax,1)
    5293:	49 8b 54 24 10       	mov    0x10(%r12),%rdx
    5298:	49 8b 74 24 18       	mov    0x18(%r12),%rsi
    529d:	66 0f d6 14 02       	movq   %xmm2,(%rdx,%rax,1)
    52a2:	66 0f d6 1c 06       	movq   %xmm3,(%rsi,%rax,1)
    52a7:	49 8b 54 24 20       	mov    0x20(%r12),%rdx
    52ac:	49 8b 74 24 28       	mov    0x28(%r12),%rsi
    52b1:	66 0f d6 24 02       	movq   %xmm4,(%rdx,%rax,1)
    52b6:	66 0f d6 2c 06       	movq   %xmm5,(%rsi,%rax,1)
    52bb:	49 8b 54 24 30       	mov    0x30(%r12),%rdx
    52c0:	49 8b 74 24 38       	mov    0x38(%r12),%rsi
    52c5:	66 0f d6 34 02       	movq   %xmm6,(%rdx,%rax,1)
    52ca:	66 0f d6 3c 06       	movq   %xmm7,(%rsi,%rax,1)
    52cf:	41 5d                	pop    %r13
    52d1:	41 5c                	pop    %r12
    52d3:	41 5b                	pop    %r11
    52d5:	41 5a                	pop    %r10
    52d7:	5d                   	pop    %rbp
    52d8:	c3                   	retq   
    52d9:	90                   	nop
    52da:	90                   	nop
    52db:	90                   	nop
    52dc:	90                   	nop
    52dd:	90                   	nop
    52de:	90                   	nop
    52df:	90                   	nop

00000000000052e0 <__libc_csu_init>:
    52e0:	41 57                	push   %r15
    52e2:	41 56                	push   %r14
    52e4:	49 89 d7             	mov    %rdx,%r15
    52e7:	41 55                	push   %r13
    52e9:	41 54                	push   %r12
    52eb:	4c 8d 25 b6 0a 20 00 	lea    0x200ab6(%rip),%r12        # 205da8 <__frame_dummy_init_array_entry>
    52f2:	55                   	push   %rbp
    52f3:	48 8d 2d b6 0a 20 00 	lea    0x200ab6(%rip),%rbp        # 205db0 <__init_array_end>
    52fa:	53                   	push   %rbx
    52fb:	41 89 fd             	mov    %edi,%r13d
    52fe:	49 89 f6             	mov    %rsi,%r14
    5301:	4c 29 e5             	sub    %r12,%rbp
    5304:	48 83 ec 08          	sub    $0x8,%rsp
    5308:	48 c1 fd 03          	sar    $0x3,%rbp
    530c:	e8 77 b2 ff ff       	callq  588 <_init>
    5311:	48 85 ed             	test   %rbp,%rbp
    5314:	74 20                	je     5336 <__libc_csu_init+0x56>
    5316:	31 db                	xor    %ebx,%ebx
    5318:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    531f:	00 
    5320:	4c 89 fa             	mov    %r15,%rdx
    5323:	4c 89 f6             	mov    %r14,%rsi
    5326:	44 89 ef             	mov    %r13d,%edi
    5329:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
    532d:	48 83 c3 01          	add    $0x1,%rbx
    5331:	48 39 dd             	cmp    %rbx,%rbp
    5334:	75 ea                	jne    5320 <__libc_csu_init+0x40>
    5336:	48 83 c4 08          	add    $0x8,%rsp
    533a:	5b                   	pop    %rbx
    533b:	5d                   	pop    %rbp
    533c:	41 5c                	pop    %r12
    533e:	41 5d                	pop    %r13
    5340:	41 5e                	pop    %r14
    5342:	41 5f                	pop    %r15
    5344:	c3                   	retq   
    5345:	90                   	nop
    5346:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    534d:	00 00 00 

0000000000005350 <__libc_csu_fini>:
    5350:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000005354 <_fini>:
    5354:	48 83 ec 08          	sub    $0x8,%rsp
    5358:	48 83 c4 08          	add    $0x8,%rsp
    535c:	c3                   	retq   
