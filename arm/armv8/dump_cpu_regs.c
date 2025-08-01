/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : dump_cpu_regs.c
 *  Author       : wqshao
 *  Created Time : 2025-04-08 10:45:04
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline uint64_t read_cpuectlr_el1()
{
	uint64_t cnt;

	asm volatile("isb"); //Instruction Synchronization Barrier
	asm volatile("mrs %0, s3_1_c15_c2_1" : "=r" (cnt));

	return cnt;
}

static inline uint64_t read_mair_el1()
{
	uint64_t cnt;

	asm volatile("isb"); //Instruction Synchronization Barrier
	asm volatile("mrs %0, MAIR_EL1" : "=r" (cnt));

	return cnt;
}

static inline void dump_gtimer_regs()
{
	uint64_t freq = 0;
	uint64_t pcnt = 0, vcnt = 0;
	uint64_t off = 0;

	asm volatile("isb"); //Instruction Synchronization Barrier
	asm volatile("mrs %0, cntfrq_el0" : "=r" (freq));
	asm volatile("mrs %0, cntvct_el0" : "=r" (vcnt));
	//asm volatile("mrs %0, cntpct_el0" : "=r" (pcnt));
	//asm volatile("mrs %0, cntvoff_el2" : "=r" (off));

	printf("%s: freq=%ld, pcnt=%ld, vcnt=%ld, off=%ld\n", __func__, freq, pcnt, vcnt, off);
}

int main(int argc, const char *argv[])
{
	uint64_t val = 0;

	//val = read_cpuectlr_el1();  //cpuectlr_el1寄存器必须在EL1模式下读取, 用户空间EL0模式下读取会报非法指令
	//printf("CPUECTLR = 0x%016lx\n", val);

	//printf("MMU:");
	//val = read_mair_el1();
	//printf(" MAIR_EL1 = 0x%016lx\n", val);
	
	dump_gtimer_regs();

	return 0;
}
