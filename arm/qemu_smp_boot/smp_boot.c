/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : smp_boot.c
 *  Author       : wqshao
 *  Created Time : 2026-03-13 11:21:59
 *  Description  :
 */

#include <stdint.h>

#define barrier() __asm__ __volatile__("": : :"memory")

void boot(void)
{
	asm("nop");
	asm("nop");
	register uint64_t *addr = (uint64_t*)0xfff000d8;

	barrier();
	register uint64_t idx = 0;
	asm("mrs %0, mpidr_el1" : "=r" (idx));
	idx &= 0x3;

	barrier();
	addr += (idx << 3);
	do {
		asm("wfe");
	} while(*addr != 0);

	barrier();
	register void (*func)(void) = (void (*)(void))*addr;
	func();

	asm("nop");
	asm("nop");
}
