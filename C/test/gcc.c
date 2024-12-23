/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : gcc.c
 *  Author       : wqshao
 *  Created Time : 2024-10-29 10:21:21
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
#ifdef __GNUC__
	printf("__GNUC__ = %d\n",__GNUC__);
#endif
#ifdef __GNUC_MINOR__
	printf("__GNUC_MINOR__ = %d\n",__GNUC_MINOR__);
#endif
#ifdef __GNUC_PATCHLEVEL__
	printf("__GNUC_PATCHLEVEL__ = %d\n",__GNUC_PATCHLEVEL__);
#endif


	/* Test for GCC > 3.2.0 */
#if __GNUC__ > 3 || \
	(__GNUC__ == 3 && (__GNUC_MINOR__ > 2 || \
			   (__GNUC_MINOR__ == 2 && \
			    __GNUC_PATCHLEVEL__ > 0)))
	printf("gcc > 3.2.0\n");
#endif


	return 0;
}

