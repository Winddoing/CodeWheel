/* ext.c : NKCH
 * Made this file to define the various things which are not present in the
 * library for SH3.
 */

#include <time.h>
char *__progname; 			/* For m88ksim */
#ifndef NKSUN
char sys_errlist[80]; 			/* For ijpeg */
#endif

#if 0
int					/* For m88ksim */
signal(int sig_name, int *func())
{
   /* I've commented this as this is called and only spoils the results file.
    * Under normal circumstances, i don't think (infact i hope) that the
    * signal won't be raised.
    */

   /* printf("NK Signal called. Bypassing the call\n"); */
}
#endif
/*
char *getcwd()
{
   printf("NK Getcwd called. Bypassing the call\n");
   return((char *)0);
}
*/
/* For the following i've also modified the simulator to support this call. 
 * Look for
 * the comment NKCH in the source code of the simulator to find out the 
 * changes.
 */

#ifndef NKSUN
#define	SYS_times	43
#define	SYS_time	13
clock_t
times(tms) 
struct tms *tms;
{
  return __trap34 (SYS_times, tms);
}

time_t time(tm)
time_t *tm;
{
  return __trap34 (SYS_time, tm);
}

#endif

int _gettimeofday()
{
   printf("_gettimeofday called. Bypassing the call\n");
   return 0;
}

int _times_r()
{
   printf("_times_r called. Bypassing the call\n");
   return 0;
}

/* The following were added just to make perl also work */
#if 1
int dup()
{
	printf("dup called. Bypassing the call\n");
	return 0;
}
#ifndef NKSUN
int sys_nerr()
{
	printf("sys_nerr called. Bypassing the call\n");
	return 0;
}
#endif
int chmod()
{
	printf("chmod called. Bypassing the call\n");
	return 0;
}
int execvp()
{
	printf("execvp called. Bypassing the call\n");
	return 0;
}
int execl()
{
	printf("execl called. Bypassing the call\n");
	return 0;
}
int utime()
{
	printf("utime called. Bypassing the call\n");
	return 0;
}
int chdir()
{
	printf("chdir called. Bypassing the call\n");
	return 0;
}
int execv()
{
	printf("execv called. Bypassing the call\n");
	return 0;
}

#endif

/* Changes made to compile spec92 gcc */
#if 1
int access()
{
	printf("access called. Bypassing the call\n");
	return 0;
}

int pipe()
{
	printf("pipe called. Bypassing the call\n");
	return 0;
}
#endif
