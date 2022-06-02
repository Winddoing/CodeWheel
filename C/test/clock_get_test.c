#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>

/*
 * clock()函数的精确度是10毫秒(ms)
 * times()函数的精确度是10毫秒(ms)
 * gettimofday()函数的精确度是微秒(μs)
 * clock_gettime()函数的计量单位为十亿分之一，也就是纳秒(ns)
 */

#define WAIT for(i = 0; i < 298765432; i++);
#define MILLION    1000000

int main (int argc, char *argv[])
{
	int i;
	long ttt;
	clock_t s,e;
	struct tms aaa;

	s=clock();
	WAIT;
	e=clock();
	printf("clock time : %.12f\n",(e-s)/(double)CLOCKS_PER_SEC);


	long tps = sysconf(_SC_CLK_TCK);
	s=times(&aaa);
	WAIT;
	e=times(&aaa);
	printf("times time : %.12f\n",(e-s)/(double)tps);


	struct timeval tvs, tve;
	gettimeofday(&tvs, NULL);
	WAIT;
	gettimeofday(&tve, NULL);
	double span = tve.tv_sec-tvs.tv_sec + (tve.tv_usec-tvs.tv_usec)/1000000.0;
	printf("gettimeofday time: %.12f\n",span);


	struct timespec tpstart;
	struct timespec tpend;

	clock_gettime(CLOCK_REALTIME, &tpstart);
	WAIT;
	clock_gettime(CLOCK_REALTIME, &tpend);
	double timedif = (tpend.tv_sec-tpstart.tv_sec)+(tpend.tv_nsec-tpstart.tv_nsec)/1000000000.0;
	printf("clock_gettime time: %.12f\n", timedif);

	return 0;
}


