/*
 * Copyright (c) 2012-2017 Roberto E. Vargas Caballero
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Portions copyright (c) 2018-2019, ARM Limited and Contributors.
 * All rights reserved.
 */

#ifndef STDDEF_H
#define STDDEF_H

#ifndef SIZET_
typedef unsigned long size_t;
#define SIZET_
#endif

#ifndef SSIZET_
typedef long ssize_t;
#define SSIZET_
#endif

#ifndef _PTRDIFF_T
typedef long ptrdiff_t;
#define _PTRDIFF_T
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define offsetof(st, m) __builtin_offsetof(st, m)

#endif /* STDDEF_H */
