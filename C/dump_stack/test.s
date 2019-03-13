
test.out:     file format elf64-x86-64


Disassembly of section .init:

00000000000008c8 <_init>:
 8c8:	48 83 ec 08          	sub    $0x8,%rsp
 8cc:	48 8b 05 15 17 20 00 	mov    0x201715(%rip),%rax        # 201fe8 <__gmon_start__>
 8d3:	48 85 c0             	test   %rax,%rax
 8d6:	74 02                	je     8da <_init+0x12>
 8d8:	ff d0                	callq  *%rax
 8da:	48 83 c4 08          	add    $0x8,%rsp
 8de:	c3                   	retq   

Disassembly of section .plt:

00000000000008e0 <.plt>:
 8e0:	ff 35 aa 16 20 00    	pushq  0x2016aa(%rip)        # 201f90 <_GLOBAL_OFFSET_TABLE_+0x8>
 8e6:	ff 25 ac 16 20 00    	jmpq   *0x2016ac(%rip)        # 201f98 <_GLOBAL_OFFSET_TABLE_+0x10>
 8ec:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000008f0 <free@plt>:
 8f0:	ff 25 aa 16 20 00    	jmpq   *0x2016aa(%rip)        # 201fa0 <free@GLIBC_2.2.5>
 8f6:	68 00 00 00 00       	pushq  $0x0
 8fb:	e9 e0 ff ff ff       	jmpq   8e0 <.plt>

0000000000000900 <puts@plt>:
 900:	ff 25 a2 16 20 00    	jmpq   *0x2016a2(%rip)        # 201fa8 <puts@GLIBC_2.2.5>
 906:	68 01 00 00 00       	pushq  $0x1
 90b:	e9 d0 ff ff ff       	jmpq   8e0 <.plt>

0000000000000910 <backtrace_symbols@plt>:
 910:	ff 25 9a 16 20 00    	jmpq   *0x20169a(%rip)        # 201fb0 <backtrace_symbols@GLIBC_2.2.5>
 916:	68 02 00 00 00       	pushq  $0x2
 91b:	e9 c0 ff ff ff       	jmpq   8e0 <.plt>

0000000000000920 <backtrace@plt>:
 920:	ff 25 92 16 20 00    	jmpq   *0x201692(%rip)        # 201fb8 <backtrace@GLIBC_2.2.5>
 926:	68 03 00 00 00       	pushq  $0x3
 92b:	e9 b0 ff ff ff       	jmpq   8e0 <.plt>

0000000000000930 <fprintf@plt>:
 930:	ff 25 8a 16 20 00    	jmpq   *0x20168a(%rip)        # 201fc0 <fprintf@GLIBC_2.2.5>
 936:	68 04 00 00 00       	pushq  $0x4
 93b:	e9 a0 ff ff ff       	jmpq   8e0 <.plt>

0000000000000940 <perror@plt>:
 940:	ff 25 82 16 20 00    	jmpq   *0x201682(%rip)        # 201fc8 <perror@GLIBC_2.2.5>
 946:	68 05 00 00 00       	pushq  $0x5
 94b:	e9 90 ff ff ff       	jmpq   8e0 <.plt>

0000000000000950 <exit@plt>:
 950:	ff 25 7a 16 20 00    	jmpq   *0x20167a(%rip)        # 201fd0 <exit@GLIBC_2.2.5>
 956:	68 06 00 00 00       	pushq  $0x6
 95b:	e9 80 ff ff ff       	jmpq   8e0 <.plt>

Disassembly of section .plt.got:

0000000000000960 <__cxa_finalize@plt>:
 960:	ff 25 92 16 20 00    	jmpq   *0x201692(%rip)        # 201ff8 <__cxa_finalize@GLIBC_2.2.5>
 966:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000970 <_start>:
 970:	31 ed                	xor    %ebp,%ebp
 972:	49 89 d1             	mov    %rdx,%r9
 975:	5e                   	pop    %rsi
 976:	48 89 e2             	mov    %rsp,%rdx
 979:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 97d:	50                   	push   %rax
 97e:	54                   	push   %rsp
 97f:	4c 8d 05 fa 02 00 00 	lea    0x2fa(%rip),%r8        # c80 <__libc_csu_fini>
 986:	48 8d 0d 83 02 00 00 	lea    0x283(%rip),%rcx        # c10 <__libc_csu_init>
 98d:	48 8d 3d 5c 02 00 00 	lea    0x25c(%rip),%rdi        # bf0 <main>
 994:	ff 15 46 16 20 00    	callq  *0x201646(%rip)        # 201fe0 <__libc_start_main@GLIBC_2.2.5>
 99a:	f4                   	hlt    
 99b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000009a0 <deregister_tm_clones>:
 9a0:	48 8d 3d 69 16 20 00 	lea    0x201669(%rip),%rdi        # 202010 <stdout@@GLIBC_2.2.5>
 9a7:	55                   	push   %rbp
 9a8:	48 8d 05 61 16 20 00 	lea    0x201661(%rip),%rax        # 202010 <stdout@@GLIBC_2.2.5>
 9af:	48 39 f8             	cmp    %rdi,%rax
 9b2:	48 89 e5             	mov    %rsp,%rbp
 9b5:	74 19                	je     9d0 <deregister_tm_clones+0x30>
 9b7:	48 8b 05 1a 16 20 00 	mov    0x20161a(%rip),%rax        # 201fd8 <_ITM_deregisterTMCloneTable>
 9be:	48 85 c0             	test   %rax,%rax
 9c1:	74 0d                	je     9d0 <deregister_tm_clones+0x30>
 9c3:	5d                   	pop    %rbp
 9c4:	ff e0                	jmpq   *%rax
 9c6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 9cd:	00 00 00 
 9d0:	5d                   	pop    %rbp
 9d1:	c3                   	retq   
 9d2:	0f 1f 40 00          	nopl   0x0(%rax)
 9d6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 9dd:	00 00 00 

00000000000009e0 <register_tm_clones>:
 9e0:	48 8d 3d 29 16 20 00 	lea    0x201629(%rip),%rdi        # 202010 <stdout@@GLIBC_2.2.5>
 9e7:	48 8d 35 22 16 20 00 	lea    0x201622(%rip),%rsi        # 202010 <stdout@@GLIBC_2.2.5>
 9ee:	55                   	push   %rbp
 9ef:	48 29 fe             	sub    %rdi,%rsi
 9f2:	48 89 e5             	mov    %rsp,%rbp
 9f5:	48 c1 fe 03          	sar    $0x3,%rsi
 9f9:	48 89 f0             	mov    %rsi,%rax
 9fc:	48 c1 e8 3f          	shr    $0x3f,%rax
 a00:	48 01 c6             	add    %rax,%rsi
 a03:	48 d1 fe             	sar    %rsi
 a06:	74 18                	je     a20 <register_tm_clones+0x40>
 a08:	48 8b 05 e1 15 20 00 	mov    0x2015e1(%rip),%rax        # 201ff0 <_ITM_registerTMCloneTable>
 a0f:	48 85 c0             	test   %rax,%rax
 a12:	74 0c                	je     a20 <register_tm_clones+0x40>
 a14:	5d                   	pop    %rbp
 a15:	ff e0                	jmpq   *%rax
 a17:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 a1e:	00 00 
 a20:	5d                   	pop    %rbp
 a21:	c3                   	retq   
 a22:	0f 1f 40 00          	nopl   0x0(%rax)
 a26:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 a2d:	00 00 00 

0000000000000a30 <__do_global_dtors_aux>:
 a30:	80 3d e1 15 20 00 00 	cmpb   $0x0,0x2015e1(%rip)        # 202018 <completed.7696>
 a37:	75 2f                	jne    a68 <__do_global_dtors_aux+0x38>
 a39:	48 83 3d b7 15 20 00 	cmpq   $0x0,0x2015b7(%rip)        # 201ff8 <__cxa_finalize@GLIBC_2.2.5>
 a40:	00 
 a41:	55                   	push   %rbp
 a42:	48 89 e5             	mov    %rsp,%rbp
 a45:	74 0c                	je     a53 <__do_global_dtors_aux+0x23>
 a47:	48 8b 3d ba 15 20 00 	mov    0x2015ba(%rip),%rdi        # 202008 <__dso_handle>
 a4e:	e8 0d ff ff ff       	callq  960 <__cxa_finalize@plt>
 a53:	e8 48 ff ff ff       	callq  9a0 <deregister_tm_clones>
 a58:	c6 05 b9 15 20 00 01 	movb   $0x1,0x2015b9(%rip)        # 202018 <completed.7696>
 a5f:	5d                   	pop    %rbp
 a60:	c3                   	retq   
 a61:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 a68:	f3 c3                	repz retq 
 a6a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000a70 <frame_dummy>:
 a70:	55                   	push   %rbp
 a71:	48 89 e5             	mov    %rsp,%rbp
 a74:	5d                   	pop    %rbp
 a75:	e9 66 ff ff ff       	jmpq   9e0 <register_tm_clones>

0000000000000a7a <dump>:
 a7a:	55                   	push   %rbp
 a7b:	48 89 e5             	mov    %rsp,%rbp
 a7e:	48 81 ec 20 01 00 00 	sub    $0x120,%rsp
 a85:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 a8c:	00 00 
 a8e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 a92:	31 c0                	xor    %eax,%eax
 a94:	48 8d 95 00 ff ff ff 	lea    -0x100(%rbp),%rdx
 a9b:	b8 00 00 00 00       	mov    $0x0,%eax
 aa0:	b9 1e 00 00 00       	mov    $0x1e,%ecx
 aa5:	48 89 d7             	mov    %rdx,%rdi
 aa8:	f3 48 ab             	rep stos %rax,%es:(%rdi)
 aab:	48 c7 85 f0 fe ff ff 	movq   $0x0,-0x110(%rbp)
 ab2:	00 00 00 00 
 ab6:	48 c7 85 e8 fe ff ff 	movq   $0x0,-0x118(%rbp)
 abd:	00 00 00 00 
 ac1:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
 ac8:	be 1e 00 00 00       	mov    $0x1e,%esi
 acd:	48 89 c7             	mov    %rax,%rdi
 ad0:	e8 4b fe ff ff       	callq  920 <backtrace@plt>
 ad5:	48 98                	cltq   
 ad7:	48 89 85 f8 fe ff ff 	mov    %rax,-0x108(%rbp)
 ade:	48 8b 85 f8 fe ff ff 	mov    -0x108(%rbp),%rax
 ae5:	89 c2                	mov    %eax,%edx
 ae7:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
 aee:	89 d6                	mov    %edx,%esi
 af0:	48 89 c7             	mov    %rax,%rdi
 af3:	e8 18 fe ff ff       	callq  910 <backtrace_symbols@plt>
 af8:	48 89 85 f0 fe ff ff 	mov    %rax,-0x110(%rbp)
 aff:	48 83 bd f0 fe ff ff 	cmpq   $0x0,-0x110(%rbp)
 b06:	00 
 b07:	75 16                	jne    b1f <dump+0xa5>
 b09:	48 8d 3d 84 01 00 00 	lea    0x184(%rip),%rdi        # c94 <_IO_stdin_used+0x4>
 b10:	e8 2b fe ff ff       	callq  940 <perror@plt>
 b15:	bf 01 00 00 00       	mov    $0x1,%edi
 b1a:	e8 31 fe ff ff       	callq  950 <exit@plt>
 b1f:	48 8b 05 ea 14 20 00 	mov    0x2014ea(%rip),%rax        # 202010 <stdout@@GLIBC_2.2.5>
 b26:	48 8b 95 f8 fe ff ff 	mov    -0x108(%rbp),%rdx
 b2d:	48 8d 35 73 01 00 00 	lea    0x173(%rip),%rsi        # ca7 <_IO_stdin_used+0x17>
 b34:	48 89 c7             	mov    %rax,%rdi
 b37:	b8 00 00 00 00       	mov    $0x0,%eax
 b3c:	e8 ef fd ff ff       	callq  930 <fprintf@plt>
 b41:	48 c7 85 e8 fe ff ff 	movq   $0x0,-0x118(%rbp)
 b48:	00 00 00 00 
 b4c:	eb 3f                	jmp    b8d <dump+0x113>
 b4e:	48 8b 85 e8 fe ff ff 	mov    -0x118(%rbp),%rax
 b55:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
 b5c:	00 
 b5d:	48 8b 85 f0 fe ff ff 	mov    -0x110(%rbp),%rax
 b64:	48 01 d0             	add    %rdx,%rax
 b67:	48 8b 10             	mov    (%rax),%rdx
 b6a:	48 8b 05 9f 14 20 00 	mov    0x20149f(%rip),%rax        # 202010 <stdout@@GLIBC_2.2.5>
 b71:	48 8d 35 4a 01 00 00 	lea    0x14a(%rip),%rsi        # cc2 <_IO_stdin_used+0x32>
 b78:	48 89 c7             	mov    %rax,%rdi
 b7b:	b8 00 00 00 00       	mov    $0x0,%eax
 b80:	e8 ab fd ff ff       	callq  930 <fprintf@plt>
 b85:	48 83 85 e8 fe ff ff 	addq   $0x1,-0x118(%rbp)
 b8c:	01 
 b8d:	48 8b 85 e8 fe ff ff 	mov    -0x118(%rbp),%rax
 b94:	48 3b 85 f8 fe ff ff 	cmp    -0x108(%rbp),%rax
 b9b:	72 b1                	jb     b4e <dump+0xd4>
 b9d:	48 8b 85 f0 fe ff ff 	mov    -0x110(%rbp),%rax
 ba4:	48 89 c7             	mov    %rax,%rdi
 ba7:	e8 44 fd ff ff       	callq  8f0 <free@plt>
 bac:	bf 00 00 00 00       	mov    $0x0,%edi
 bb1:	e8 9a fd ff ff       	callq  950 <exit@plt>

0000000000000bb6 <func_c>:
 bb6:	55                   	push   %rbp
 bb7:	48 89 e5             	mov    %rsp,%rbp
 bba:	48 8d 3d 05 01 00 00 	lea    0x105(%rip),%rdi        # cc6 <_IO_stdin_used+0x36>
 bc1:	e8 3a fd ff ff       	callq  900 <puts@plt>
 bc6:	e8 af fe ff ff       	callq  a7a <dump>
 bcb:	90                   	nop
 bcc:	5d                   	pop    %rbp
 bcd:	c3                   	retq   

0000000000000bce <func_b>:
 bce:	55                   	push   %rbp
 bcf:	48 89 e5             	mov    %rsp,%rbp
 bd2:	b8 00 00 00 00       	mov    $0x0,%eax
 bd7:	e8 da ff ff ff       	callq  bb6 <func_c>
 bdc:	90                   	nop
 bdd:	5d                   	pop    %rbp
 bde:	c3                   	retq   

0000000000000bdf <func_a>:
 bdf:	55                   	push   %rbp
 be0:	48 89 e5             	mov    %rsp,%rbp
 be3:	b8 00 00 00 00       	mov    $0x0,%eax
 be8:	e8 e1 ff ff ff       	callq  bce <func_b>
 bed:	90                   	nop
 bee:	5d                   	pop    %rbp
 bef:	c3                   	retq   

0000000000000bf0 <main>:
 bf0:	55                   	push   %rbp
 bf1:	48 89 e5             	mov    %rsp,%rbp
 bf4:	48 83 ec 10          	sub    $0x10,%rsp
 bf8:	89 7d fc             	mov    %edi,-0x4(%rbp)
 bfb:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
 bff:	b8 00 00 00 00       	mov    $0x0,%eax
 c04:	e8 d6 ff ff ff       	callq  bdf <func_a>
 c09:	b8 00 00 00 00       	mov    $0x0,%eax
 c0e:	c9                   	leaveq 
 c0f:	c3                   	retq   

0000000000000c10 <__libc_csu_init>:
 c10:	41 57                	push   %r15
 c12:	41 56                	push   %r14
 c14:	49 89 d7             	mov    %rdx,%r15
 c17:	41 55                	push   %r13
 c19:	41 54                	push   %r12
 c1b:	4c 8d 25 66 11 20 00 	lea    0x201166(%rip),%r12        # 201d88 <__frame_dummy_init_array_entry>
 c22:	55                   	push   %rbp
 c23:	48 8d 2d 66 11 20 00 	lea    0x201166(%rip),%rbp        # 201d90 <__init_array_end>
 c2a:	53                   	push   %rbx
 c2b:	41 89 fd             	mov    %edi,%r13d
 c2e:	49 89 f6             	mov    %rsi,%r14
 c31:	4c 29 e5             	sub    %r12,%rbp
 c34:	48 83 ec 08          	sub    $0x8,%rsp
 c38:	48 c1 fd 03          	sar    $0x3,%rbp
 c3c:	e8 87 fc ff ff       	callq  8c8 <_init>
 c41:	48 85 ed             	test   %rbp,%rbp
 c44:	74 20                	je     c66 <__libc_csu_init+0x56>
 c46:	31 db                	xor    %ebx,%ebx
 c48:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 c4f:	00 
 c50:	4c 89 fa             	mov    %r15,%rdx
 c53:	4c 89 f6             	mov    %r14,%rsi
 c56:	44 89 ef             	mov    %r13d,%edi
 c59:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 c5d:	48 83 c3 01          	add    $0x1,%rbx
 c61:	48 39 dd             	cmp    %rbx,%rbp
 c64:	75 ea                	jne    c50 <__libc_csu_init+0x40>
 c66:	48 83 c4 08          	add    $0x8,%rsp
 c6a:	5b                   	pop    %rbx
 c6b:	5d                   	pop    %rbp
 c6c:	41 5c                	pop    %r12
 c6e:	41 5d                	pop    %r13
 c70:	41 5e                	pop    %r14
 c72:	41 5f                	pop    %r15
 c74:	c3                   	retq   
 c75:	90                   	nop
 c76:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 c7d:	00 00 00 

0000000000000c80 <__libc_csu_fini>:
 c80:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000c84 <_fini>:
 c84:	48 83 ec 08          	sub    $0x8,%rsp
 c88:	48 83 c4 08          	add    $0x8,%rsp
 c8c:	c3                   	retq   
