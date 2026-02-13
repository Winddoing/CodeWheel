/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : heaptrack_tst.c
 *  Author       : wqshao
 *  Created Time : 2025-11-21 16:47:29
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct aa {
	int aa;
	char c[1024];
};

int main(int argc, const char *argv[])
{
	struct aa *ap = NULL;

	ap = malloc(sizeof(struct aa));

#if 1
	void *vp = (void *)ap;
	struct aa *AP = (struct aa *)vp;
	free(AP);
#else
	free(ap);
#endif
	
	return 0;
}
