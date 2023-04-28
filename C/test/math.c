/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : math.c
 *  Author       : wqshao
 *  Created Time : 2023-04-24 16:47:06
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __GNUC__

#define _CRT_SECURE_NO_WARNINGS
#define __AL_ALIGNED__(x) __attribute__((aligned(x)))
#define AL_INLINE inline
#define AL_API extern
#define AL_DEPRECATED(msg) __attribute__((deprecated(msg)))

#ifndef __cplusplus
#define static_assert _Static_assert
#endif

#else // _MSC_VER

#define __AL_ALIGNED__(x)
#define __attribute__(x)
#define AL_INLINE __inline
#define AL_API extern
#define AL_DEPRECATED(msg) __declspec(deprecated(msg))

#ifndef __cplusplus
#define static_assert(assertion, ...) _STATIC_ASSERT(assertion)
#endif

#endif


static AL_INLINE size_t BitsToBytes(size_t zBits)
{
  return (zBits + 7) / 8;
}

/***************************************************************************/
static AL_INLINE size_t BytesToBits(size_t zBytes)
{
  return zBytes * 8;
}

/***************************************************************************/
static AL_INLINE int Clip3(int iVal, int iMin, int iMax)
{
  return ((iVal) < (iMin)) ? (iMin) : ((iVal) > (iMax)) ? (iMax) : (iVal);
}

/***************************************************************************/
static AL_INLINE int Max(int iVal1, int iVal2)
{
  return ((iVal1) < (iVal2)) ? (iVal2) : (iVal1);
}

/***************************************************************************/
static AL_INLINE uint32_t UnsignedMax(uint32_t iVal1, uint32_t iVal2)
{
  return ((iVal1) < (iVal2)) ? (iVal2) : (iVal1);
}

/***************************************************************************/
static AL_INLINE size_t UnsignedMin(size_t iVal1, size_t iVal2)
{
  return ((iVal1) > (iVal2)) ? (iVal2) : (iVal1);
}

/***************************************************************************/
static AL_INLINE int Min(int iVal1, int iVal2)
{
  return ((iVal1) > (iVal2)) ? (iVal2) : (iVal1);
}

/***************************************************************************/
static AL_INLINE int Abs(int iVal)
{
  return ((iVal) > 0) ? (iVal) : (-(iVal));
}

/***************************************************************************/
static AL_INLINE int Sign(int iVal)
{
  return ((iVal) > 0) ? (1) : (((iVal) < 0) ? (-1) : (0));
}

/***************************************************************************/
// 设置iVal值为iRnd的整数倍
static AL_INLINE int RoundUp(int iVal, int iRnd)
{
  return iVal >= 0 ? ((iVal + iRnd - 1) / iRnd) * iRnd : (iVal / iRnd) * iRnd;
}

/***************************************************************************/
static AL_INLINE int RoundDown(int iVal, int iRnd)
{
  return iVal >= 0 ? (iVal / iRnd) * iRnd : ((iVal - iRnd + 1) / iRnd) * iRnd;
}

/***************************************************************************/
static AL_INLINE size_t UnsignedRoundUp(size_t zVal, size_t zRnd)
{
  return ((zVal + zRnd - 1) / zRnd) * zRnd;
}

/***************************************************************************/
static AL_INLINE size_t UnsignedRoundDown(size_t zVal, size_t zRnd)
{
  return (zVal / zRnd) * zRnd;
}


int main(int argc, const char *argv[])
{
	

	printf("===> func: %s, line: %d, RoundUp aa=%d\n", __func__, __LINE__, RoundUp(2048, 2));
	return 0;
}
