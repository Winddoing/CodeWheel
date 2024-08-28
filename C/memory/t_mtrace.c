/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : mtrace.c
 *  Author       : wqshao
 *  Created Time : 2024-05-21 17:06:07
 *  Description  :
 */

#include <mcheck.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int j;

	mtrace();

	for (j = 0; j < 2; j++)
		malloc(100);            /* Never freed--a memory leak */

	calloc(16, 16);             /* Never freed--a memory leak */
	exit(EXIT_SUCCESS);
}
