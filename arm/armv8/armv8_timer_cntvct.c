/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : armv8_timer_cntvct.c
 *  Author       : wqshao
 *  Created Time : 2024-08-26 11:07:10
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t arm64_cntvct(void)
{
    uint64_t cnt;
    asm volatile("isb"); //Instruction Synchronization Barrier
    asm volatile("mrs %0, cntvct_el0" : "=r" (cnt));
    return cnt;
}

static inline uint64_t arm64_cntfreq(void)
{
    uint64_t freq;
    asm volatile("isb");
    asm volatile("mrs %0, cntfrq_el0" : "=r" (freq));
    return freq;
}

int main(int argc, const char *argv[])
{
	printf("===> func: %s, line: %d, cntvct=%ld, freq=%ld\n", __func__, __LINE__, arm64_cntvct(), arm64_cntfreq());

	{
		uint64_t us;
		us = arm64_cntvct() * 1000000U;
		us /= arm64_cntfreq();
		printf("===> func: %s, line: %d, time=%ldus\n", __func__, __LINE__, us);
	}
	
	return 0;
}
