/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : offsetof.c
 *  Author       : wqshao
 *  Created Time : 2026-02-24 18:01:58
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h> //offsetof

struct aa {
	int a;
	int b;
	char c;
	int d;
};

int main(int argc, const char *argv[])
{
	
	printf("===> func: %s, line: %d, a offsetof=%ld\n", __func__, __LINE__, offsetof(struct aa, a));
	printf("===> func: %s, line: %d, b offsetof=%ld\n", __func__, __LINE__, offsetof(struct aa, b));
	printf("===> func: %s, line: %d, c offsetof=%ld\n", __func__, __LINE__, offsetof(struct aa, c));
	printf("===> func: %s, line: %d, d offsetof=%ld\n", __func__, __LINE__, offsetof(struct aa, d));
	return 0;
}
