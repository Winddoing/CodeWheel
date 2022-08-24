/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : test_demo1.c
 *  Author       : wqshao
 *  Created Time : 2022-08-24 16:12:51
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "test.h"

TEST(test_demo1_func1)
{
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
}

TEST(test_demo1_func2)
{
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	assert(1);
}

TEST(test_demo1_func3)
{
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	assert(0);
}
