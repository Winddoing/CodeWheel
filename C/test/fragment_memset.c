/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : fragment_memset.c
 *  Author       : wqshao
 *  Created Time : 2025-09-25 10:31:51
 *  Description  : 测试内存碎片化
 *  2025-09-26：Teq下测试，平均每秒400MB
 *  	loop test: [8], total time: 1171 ms, total size: 400 MB, speed=342.02 MB/s
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h> //malloc_trim
#include <sys/time.h>

#define MAX_NUM (200*1024)  //实际测试100M左右

#define CACHE_LINE_SIZE 64

// 使dcache无效并清理
void dcache_invalidate_clean(void *addr, size_t size)
{
	if (addr == NULL || size == 0) return;

	uintptr_t start = (uintptr_t)addr;
	uintptr_t end = start + size;
	uint32_t cache_line_sz = CACHE_LINE_SIZE;

	cache_line_sz = sysconf(_SC_LEVEL1_DCACHE_LINESIZE); //64Byte

	// 对齐到缓存行
	start &= ~(cache_line_sz - 1);

#if defined(__aarch64__)
	// ARM64处理
#if 0 //以下指令只能在el1下运行，也就是内核态，用户态为el0
	for (uintptr_t ptr = start; ptr < end; ptr += cache_line_sz) {
		__asm__ __volatile__("dc civac, %0" :: "r"(ptr));
	}
#endif
	__asm__ __volatile__("dsb sy" ::: "memory");
#else
	// 其他架构 - 使用编译屏障
	__asm__ __volatile__("" ::: "memory");
#endif
}

static uint64_t get_time_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

int main(int argc, const char *argv[])
{
	char *ptrs[MAX_NUM] = {NULL};
	int sz[MAX_NUM] = {0};
	int loop = 0;
	uint64_t total_time_ms = 0;
	uint64_t total_test_size = 0;

	while (1) {
		loop++;
		srand(get_time_ms());
		uint64_t t1 = get_time_ms();
		for (int i = 0; i < MAX_NUM; i++) {
			sz[i] = (rand() % 1024) + 1;   // 1byte ~ 1kB
			int size = sz[i] * sizeof(char);
			ptrs[i] = malloc(size);
			dcache_invalidate_clean(ptrs, size);
			memset(ptrs[i], rand(), size);
			dcache_invalidate_clean(ptrs, size);
			//madvise(ptrs[i], size, MADV_DONTNEED);  //mmap
			if (i % 3 == 0) {
				int idx = (rand() % 1024) + 1;
				if (ptrs[idx] != NULL) {
					free(ptrs[idx]);
					malloc_trim(0);
					ptrs[idx] = NULL;
				}
			}
		}
		uint64_t t2 = get_time_ms();
		uint64_t test_sz = 0;
		for (int i = 0; i < MAX_NUM; i++) {
			if (ptrs[i] != NULL) {
				free(ptrs[i]);
				ptrs[i] = NULL;
			}
			test_sz += sz[i];
		}
		malloc_trim(0);
		//printf("-->loop test: [%03d], time: %ld ms, size: %ld MB\n", loop, t2 - t1, test_sz / 1024 / 1024);
		total_time_ms += t2 - t1;
		total_test_size += test_sz;
		if (total_time_ms >= 1000) {
			printf("<%06d> loop test: [%03d], total time: %ld ms, total size: %ld MB, speed=%.2lf MB/s\n",
					getpid(),
					loop, total_time_ms, total_test_size / 1024 / 1024,
					(total_test_size * 1.0 * 1000 / (total_time_ms * 1024 * 1024)));
			total_time_ms = 0;
			total_test_size = 0;
		}
	}

	printf("===> func: %s, line: %d, while sleep.\n", __func__, __LINE__);
	while(1) {
		sleep(60);
	}

	return 0;
}
