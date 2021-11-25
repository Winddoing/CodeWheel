.globl _start

#define TEXT_BASE		0x300000
#define	SP_POINTER		0x100000
#define LOC_OFFS		0x100008
#define	TARGET_LOC		0x3B600000

#define UARTTX			0x0
#define UARTLSR			0x14
#define UARTLSR_TEMT		(1 << 6)	/* Tx Shift Register Empty */
#define UARTLSR_THRE		(1 << 5)	/* Tx Holding Register Empty */

.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm

_start:
	mov	sp,		SP_POINTER
	mov	x0,		TARGET_LOC
	str	x0,		[sp,	#8]
//	adr	x0,		hello_tips
//	bl	show_msg
//	adr	x0,		prep_reloc_tips
//	ldr	x1,		[sp,	#8]
//	bl	mprintf

/* begin relocate */
1:	ldr	x0,		[sp,	#8]
	mov	lr,		x0
	add	lr,		lr,	relocation_return
	b	relocate_code
relocation_return:
	bl	enable_caches
//	adr	x0,		reloc_ok_tips
//	bl	show_msg
	ldr	x1,		[sp,	#8]
	subs	x1,		x1,	#0x100000
	str	x1,		[sp,	#8]
	cmp	x1,		#0x300000
	bne	1b
	b	.

.globl hang
hang:
	adr	x0,		hang_tips
	bl	show_msg
	b	.

__asm_dcache_level:
	lsl	x12, x0, #1
	msr	csselr_el1, x12		/* select cache level */
	isb				/* sync change of cssidr_el1 */
	mrs	x6, ccsidr_el1		/* read the new cssidr_el1 */
	and	x2, x6, #7		/* x2 <- log2(cache line size)-4 */
	add	x2, x2, #4		/* x2 <- log2(cache line size) */
	mov	x3, #0x3ff
	and	x3, x3, x6, lsr #3	/* x3 <- max number of #ways */
	clz	w5, w3			/* bit position of #ways */
	mov	x4, #0x7fff
	and	x4, x4, x6, lsr #13	/* x4 <- max number of #sets */
	/* x12 <- cache level << 1 */
	/* x2 <- line length offset */
	/* x3 <- number of cache ways - 1 */
	/* x4 <- number of cache sets - 1 */
	/* x5 <- bit position of #ways */

loop_set:
	mov	x6, x3			/* x6 <- working copy of #ways */
loop_way:
	lsl	x7, x6, x5
	orr	x9, x12, x7		/* map way and level to cisw value */
	lsl	x7, x4, x2
	orr	x9, x9, x7		/* map set number to cisw value */
	tbz	w1, #0, 1f
	dc	isw, x9
	b	2f
1:	dc	cisw, x9		/* clean & invalidate by set/way */
2:	subs	x6, x6, #1		/* decrement the way */
	b.ge	loop_way
	subs	x4, x4, #1		/* decrement the set */
	b.ge	loop_set

	ret

__asm_dcache_all:
	mov	x1, x0
	dsb	sy
	mrs	x10, clidr_el1		/* read clidr_el1 */
	lsr	x11, x10, #24
	and	x11, x11, #0x7		/* x11 <- loc */
	cbz	x11, finished		/* if loc is 0, exit */
	mov	x15, lr
	mov	x0, #0			/* start flush at cache level 0 */
	/* x0  <- cache level */
	/* x10 <- clidr_el1 */
	/* x11 <- loc */
	/* x15 <- return address */

loop_level:
	lsl	x12, x0, #1
	add	x12, x12, x0		/* x0 <- tripled cache level */
	lsr	x12, x10, x12
	and	x12, x12, #7		/* x12 <- cache type */
	cmp	x12, #2
	b.lt	skip			/* skip if no cache or icache */
	bl	__asm_dcache_level	/* x1 = 0 flush, 1 invalidate */
skip:
	add	x0, x0, #1		/* increment cache level */
	cmp	x11, x0
	b.gt	loop_level

	mov	x0, #0
	msr	csselr_el1, x0		/* restore csselr_el1 */
	dsb	sy
	isb
	mov	lr, x15

finished:
	ret

.globl __asm_flush_dcache_all
__asm_flush_dcache_all:
	mov	x0, #0
	b	__asm_dcache_all

.globl __asm_invalidate_icache_all
__asm_invalidate_icache_all:
	ic	ialluis
	isb	sy
	ret

.globl __asm_invalidate_tlb_all
__asm_invalidate_tlb_all:
	switch_el x9, 3f, 2f, 1f
3:	tlbi	alle3
	dsb	sy
	isb
	b	0f
2:	tlbi	alle2
	dsb	sy
	isb
	b	0f
1:	tlbi	vmalle1
	dsb	sy
	isb
0:
	ret

__asm_flush_dcache_range:
	mrs	x3,		ctr_el0
	lsr	x3,		x3,	#16
	and	x3,		x3,	#0xf
	mov	x2,		#4
	lsl	x2,		x2,	x3		/* cache line size */

	/* x2 <- minimal cache line size in cache system */
	sub	x3,		x2,	#1
	bic	x0,		x0,	x3
1:	dc	civac,		x0
	add	x0,		x0,	x2
	cmp	x0,		x1
	b.lo	1b
	dsb	sy
	ret

.globl __asm_invalidate_dcache_all
__asm_invalidate_dcache_all:
	mov	x0, #0x1
	b	__asm_dcache_all

/* x0 holds the destination address. */
relocate_code:
	stp	x29,		x30,	[sp,	#-32]!
	mov	x29,		sp
	str	x0,		[sp,	#16]
	
	adrp	x1,		image_copy_start
	add	x1,		x1,	:lo12:image_copy_start
	subs	x9,		x0,	x1
	b.eq	relocate_done

	mov	x1,		TEXT_BASE
	subs	x9,		x0,	x1

	adrp	x1,		image_copy_start
	add	x1,		x1,	:lo12:image_copy_start
	adrp	x2,		image_copy_end
	add	x2,		x2,	:lo12:image_copy_end
copy_loop:
	ldp	x10,		x11,	[x1],	#16
	stp	x10,		x11,	[x0],	#16
	cmp	x1,		x2
	b.lo	copy_loop
	str	x0,		[sp,	#24]

relocate_done:
	switch_el x1, 3f, 2f, 1f
	bl	hang
3:	mrs	x0,		sctlr_el3
	b	0f
2:	mrs	x0,		sctlr_el2
	b	0f
1:	mrs	x0,		sctlr_el1
0:	tbz	w0,		#2,	5f /* skip flushing cache if disabled */
	tbz	w0,		#12,	4f /* skip invalidating i-cache if disabled */
	ic	iallu		/* i-cache invalidate all */
	isb	sy
4:	ldp	x0,		x1,	[sp,	#16]
	bl	__asm_flush_dcache_range
5:	ldp	x29,		x30,	[sp],	#32
	ret

/*
 * void _uart_putchar(int c);
 */

.globl _uart_putchar
_uart_putchar:
	stp	x29,	x30,	[sp,	#-32]!
	mov	x29,	sp
	stp	x19,	x20,	[sp,	#16]	/* save x19, x20*/
	isb
1:	mov	x19,	0xFC200000
	ldr	w20,	[x19,	#UARTLSR]
	and	w20,	w20,	#(UARTLSR_TEMT | UARTLSR_THRE)
	cmp	w20,	#(UARTLSR_TEMT | UARTLSR_THRE)
	b.ne	1b
	str	w0,	[x19,	#UARTTX]
	ldp	x19,	x20,	[sp,	#16]	/* restore x19 */
	ldp	x29,	x30,	[sp],	#32
	ret

.section .rodata
hello_tips:		.asciz		"\nBaremetal test...\n"
prep_reloc_tips:	.asciz		"Relocate to %x...\n"
reloc_ok_tips:		.asciz		"Relocate ok...\n"
hang_tips:		.asciz		"System HALT!\n"

