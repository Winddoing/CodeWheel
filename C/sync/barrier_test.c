/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : barrier_test.c
 *  Author       : wqshao
 *  Created Time : 2024-08-26 16:31:52
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
	int a = 0;
	int b = 0;


	asm("nop");
	a = 1;
	__sync_synchronize();
	b = 1;
	asm("nop");

	asm("nop");
	if (a == 11) {
		__sync_synchronize();
		b = 22;
	}
	asm("nop");

	asm("nop");
	asm volatile("" ::: "memory");
	asm("nop");

	return 0;
}
