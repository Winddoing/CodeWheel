/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : raii_malloc_free.c
 *  Author       : wqshao
 *  Created Time : 2025-12-25 10:15:35
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef __maybe_unused
# define __maybe_unused         __attribute__((unused))
#endif

#define __cleanup(func) __maybe_unused __attribute__((__cleanup__(func)))

#define __free(_name)	__cleanup(__free_##_name)

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

static void __free_my_free(char **ptr)
{
	printf("%s\n", __func__);
	if (*ptr)
		my_free(*ptr);
}

void test_malloc_free()
{
	printf("===> func: %s, line: %d, entry.\n", __func__, __LINE__);
	char *buf __free(my_free) = my_malloc(8);
	printf("===> func: %s, line: %d, exit.\n", __func__, __LINE__);
}

int main(int argc, const char *argv[])
{
	printf("===> func: %s, line: %d, entry.\n", __func__, __LINE__);
	test_malloc_free();
	
	printf("===> func: %s, line: %d, exit.\n", __func__, __LINE__);
	return 0;
}
