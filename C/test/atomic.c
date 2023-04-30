/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : atomic.c
 *  Author       : wqshao
 *  Created Time : 2022-10-30 15:16:29
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
	int a = 1;
	int ret = 0;

	ret = __sync_fetch_and_add(&a, 1);
	printf("===> func: %s, line: %d, a=%d, ret=%d\n", __func__, __LINE__, a, ret);
	ret = __sync_fetch_and_sub(&a, 1);
	printf("===> func: %s, line: %d, a=%d, ret=%d\n", __func__, __LINE__, a, ret);

	a = 1;
	ret = __sync_add_and_fetch(&a, 1);
	printf("===> func: %s, line: %d, a=%d, ret=%d\n", __func__, __LINE__, a, ret);
	ret = __sync_sub_and_fetch(&a, 1);
	printf("===> func: %s, line: %d, a=%d, ret=%d\n", __func__, __LINE__, a, ret);
	
	return 0;
}
