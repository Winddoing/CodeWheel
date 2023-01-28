/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : helper.h
 *  Author       : wqshao
 *  Created Time : 2022-12-14 15:50:21
 *  Description  :
 */

#ifndef __HELPER_H__
#define __HELPER_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof (a) / sizeof (a)[0])
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({							\
		const __typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
		(type *)( (char *)__mptr - offsetof(type,member) );})
#endif


#ifdef __cplusplus
}
#endif

#endif//__HELPER_H__
