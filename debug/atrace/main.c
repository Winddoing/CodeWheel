/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : main.c
 *  Author       : wqshao
 *  Created Time : 2023-09-12 15:23:27
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "trace.h"


void aa()
{
	atrace_begin_body(__func__);
	
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	atrace_end_body();

}

int main(int argc, const char *argv[])
{

	aa();


	while(1)
		sleep(1);

	
	return 0;
}
