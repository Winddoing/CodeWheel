/*
 * Copyright (c) 2009
 *      MIPS Technologies, Inc., California.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the MIPS Technologies, Inc., nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE MIPS TECHNOLOGIES, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE MIPS TECHNOLOGIES, INC. BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/************************************************************************
 *
 *  memcpy.S
 *  Version: "043009"
 *
 ************************************************************************/


/************************************************************************
 *  Include files
 ************************************************************************/

/*#include "machine/asm.h"*/
#include "asm.h"


/*
 * This routine could be optimized for MIPS64. The current code only
 * uses MIPS32 instructions.
 */

#if defined(__MIPSEB__)
#  define LWHI	lwl		/* high part is left in big-endian	*/
#  define SWHI	swl		/* high part is left in big-endian	*/
#  define LWLO	lwr		/* low part is right in big-endian	*/
#  define SWLO	swr		/* low part is right in big-endian	*/
#endif

#if defined(__MIPSEL__)
#  define LWHI	lwr		/* high part is right in little-endian	*/
#  define SWHI	swr		/* high part is right in little-endian	*/
#  define LWLO	lwl		/* low part is left in big-endian	*/
#  define SWLO	swl		/* low part is left in big-endian	*/
#endif

#if !(defined(XGPROF) || defined(XPROF))
#undef SETUP_GP
#define SETUP_GP
#endif

# SIMD memcpy by dzwei 2017/1/3
LEAF(simd_memcpy,0)
/*LEAF(simd_memcpy)*/

	.set	noreorder
	.set	noat

	pref	0,0(a1)
	addiu	sp,sp,-84
	st.b	$w0,16(sp)
	st.b	$w1,32(sp)
	st.b	$w2,48(sp)
	st.b	$w3,64(sp)

	slti	AT,a2,16
	bne	AT,zero,.Llast16
	move	v0,a0		# memcpy returns the dst pointer

# Test if the src is 4word-aligned address
	negu	t8,a1
	andi	t8,t8,0xf
	beq	t8,zero,.Lchkaligned8w
	subu	a2,a2,t8
	addu	a3,a1,t8	# now a3 is the src is 4word-aligned address

# Copy the first (<16) bytes for the src is 4word-aligned address
.Lfirst16loop:
	lb	t3,0(a1)
	addiu	a1,a1,1
	addiu	a0,a0,1
	bne	a1,a3,.Lfirst16loop
	sb	t3,-1(a0)

	andi	t8,a2,0xf
	beq	t8,a2,.Llast16
	nop

# Test if the src is 8word-aligned address
.Lchkaligned8w:
	andi	t8,a1,0x1f
	beq	t8,zero,.Lchk16w
	subu	a2,a2,t8
	ld.b	$w0,0(a1)
	addiu	a1,a1,16
	addiu	a0,a0,16
	st.b	$w0,-16(a0)

# Now the src is 8word-aligned address
.Lchk16w:
	andi	t8,a2,0x3f	# any whole 64-byte chunks?
				# t8 is the byte count after 64-byte chunks
	beq	a2,t8,.Lchk4w	# if a2==t8, no 64-byte chunks
				# There will be at most 1 32-byte chunk after it
	subu	a3,a2,t8	# subtract from a2 the reminder
                                # Here a3 counts bytes in 16w chunks
	addu	a3,a1,a3	# Now a3 is the final src after 64-byte chunks

################################################################################
.Lloop16w:
	ld.b	$w0,0(a1)
	ld.b	$w1,16(a1)
	ld.b	$w2,32(a1)
	ld.b	$w3,48(a1)
	st.b	$w0,0(a0)
	st.b	$w1,16(a0)
	st.b	$w2,32(a0)
	st.b	$w3,48(a0)

	addiu	a1,a1,64	# adding 64 to src
	bne	a1,a3,.Lloop16w
	addiu	a0,a0,64	# adding 64 to dest
################################################################################

# Here we have src 4word-aligned but less than 64-bytes to go

.Lchk4w:
	andi	a2,t8,0xf
	beq	t8,a2,.Llast16
	subu	a3,t8,a2
	addu	a3,a1,a3	# Now a3 is the final src after 16-byte chunks

.Lloop4w:
	ld.b	$w0,0(a1)
	addiu	a1,a1,16
	addiu	a0,a0,16
	bne	a1,a3,.Lloop4w
	st.b	$w0,-16(a0)

# For the last (<16) bytes
.Llast16:
	blez	a2,.Lleave
	addu	a3,a1,a2	# a3 is the last src address
.Llast16loop:
	lb	v1,0(a1)
	addiu	a1,a1,1
	addiu	a0,a0,1
	bne	a1,a3,.Llast16loop
	sb	v1,-1(a0)

.Lleave:
	ld.b	$w0,16(sp)
	ld.b	$w1,32(sp)
	ld.b	$w2,48(sp)
	ld.b	$w3,64(sp)

	j	ra
	addiu	sp,sp,84

	.set	at
	.set	reorder

END(simd_memcpy)


/************************************************************************
 *  Implementation : Static functions
 ************************************************************************/
