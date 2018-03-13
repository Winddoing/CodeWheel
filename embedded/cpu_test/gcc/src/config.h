/* Configuration for GNU C-compiler for Sun Sparc.
   Copyright (C) 1988, 1993 Free Software Foundation, Inc.
   Contributed by Michael Tiemann (tiemann@cygnus.com).

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* #defines that need visibility everywhere.  */
#define FALSE 0
#define TRUE 1

#ifndef __STDC__
#define __STDC__ 1
#endif

#ifndef ONLY_INT_FIELDS
#define ONLY_INT_FIELDS 1
#endif

#ifndef NON_POSIX
#define POSIX
#endif


/* This describes the machine the compiler is hosted on.  */
#define HOST_BITS_PER_CHAR 8
#define HOST_BITS_PER_SHORT 16
#define HOST_BITS_PER_INT 32
#define HOST_BITS_PER_LONG 32
#define HOST_BITS_PER_LONGLONG 64

/* Doubles are stored in memory with the high order word first.  This
   matters when cross-compiling.  */
#define HOST_WORDS_BIG_ENDIAN 1

/* target machine dependencies.
   tm.h is a symbolic link to the actual target specific file.   */
#include "tm.h"

/* Arguments to use with `exit'.  */
#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33

/* If compiled with Sun CC, the use of alloca requires this #include.  */
/* Not needed by SUN - commented out for SPEC95			       */
/*#ifndef __GNUC__*/
/*#ifdef sun*/
/*#include "alloca.h"*/
/*#endif*/
/*#endif*/

#ifdef SPEC
/* Force all occurences of bzero, bcmp, and bcopy to their ANSI equivalents
   so that all vendors use the same calls.  */
#define index strchr
#define rindex strrchr
#define bcopy(a,b,n) memcpy((b),(a),(n))
#define bcmp(a,b,n) memcmp((a),(b),(n))
#define bzero(a,n) memset((a),0,(n))
#endif
