/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : cleanup.c
 *  Author       : wqshao
 *  Created Time : 2025-12-25 10:37:58
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define __cleanup(x) __attribute__((__cleanup__(x)))

#if 0
#define printf(fmt, ...) do {} while(0)
#endif

#define USE_CLEANUP

static void* my_malloc(int size)
{
	printf("%s: size=%d\n", __func__, size);
	return malloc(size);
}

static void my_free(void *ptr)
{
	printf("%s\n", __func__);
	free(ptr);
}

static void __free(char **ptr)
{
	printf("%s\n", __func__);
	if (*ptr) {
		my_free(*ptr);
		*ptr = NULL;
	}
}

void test_malloc_free()
{
	printf("===> func: %s, line: %d, entry.\n", __func__, __LINE__);
#ifdef USE_CLEANUP
	__cleanup(__free) char *buf = my_malloc(8);
#else
	char *buf = my_malloc(8);
	my_free(buf);
#endif
	printf("===> func: %s, line: %d, exit.\n", __func__, __LINE__);
}

int main(int argc, const char *argv[])
{
	printf("===> func: %s, line: %d, entry.\n", __func__, __LINE__);
	test_malloc_free();
	
	printf("===> func: %s, line: %d, exit.\n", __func__, __LINE__);
	return 0;
}
