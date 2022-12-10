/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : log2.c
 *  Author       : wqshao
 *  Created Time : 2022-11-14 10:19:45
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <assert.h>

static const uint8_t tab_ceil_log2[] =
{
/*  0.. 7 */
  0, 0, 1, 2, 2, 3, 3, 3,
/*  8..15 */
  3, 4, 4, 4, 4, 4, 4, 4,
/* 16..23 */
  4, 5, 5, 5, 5, 5, 5, 5,
/* 24..31 */
  5, 5, 5, 5, 5, 5, 5, 5
};

/***************************************************************************/
int ceil_log2(uint16_t n)
{
  int v = 0;
  assert(n > 0);

  if(n < 32)
    return tab_ceil_log2[n];

  n--;

  // count the number of bit used to store the decremented n
  while(n != 0)
  {
    n >>= 1;
    ++v;
  }

  return v;
}

/***************************************************************************/
int floor_log2(uint16_t n)
{
  int s = -1;

  while(n != 0)
  {
    n >>= 1;
    ++s;
  }

  return s;
}

int main(int argc, const char *argv[])
{
	
	printf("===> func: %s, line: %d, ceil_log2[4]=%d\n", __func__, __LINE__, ceil_log2(4));
	printf("===> func: %s, line: %d, ceil_log2[5]=%d\n", __func__, __LINE__, ceil_log2(5));
	printf("===> func: %s, line: %d, ceil_log2[6]=%d\n", __func__, __LINE__, ceil_log2(6));

	printf("===> func: %s, line: %d, floor_log2[4]=%d\n", __func__, __LINE__, floor_log2(4));
	printf("===> func: %s, line: %d, floor_log2[5]=%d\n", __func__, __LINE__, floor_log2(5));
	printf("===> func: %s, line: %d, floor_log2[6]=%d\n", __func__, __LINE__, floor_log2(6));

	return 0;
}
