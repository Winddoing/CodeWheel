/*#############################################################
 *     File Name	: memcpy_test.c
 *     Author		: winddoing
 *     Created Time	: 2021年07月28日 星期三 17时09分36秒
 *     Description	:
 *############################################################*/

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#define COUNTNUMBER 1000
#define SENDCORE 2

static void timespec_sub(struct timespec *t1, const struct timespec *t2) 
{
	assert(t1->tv_nsec >= 0);
	assert(t1->tv_nsec < 1000000000);
	assert(t2->tv_nsec >= 0);
	assert(t2->tv_nsec < 1000000000);
	t1->tv_sec -= t2->tv_sec;
	t1->tv_nsec -= t2->tv_nsec;
	if (t1->tv_nsec >= 1000000000) {
		t1->tv_sec++;
		t1->tv_nsec -= 1000000000;
	} else if (t1->tv_nsec < 0) {
		t1->tv_sec--;
		t1->tv_nsec += 1000000000;
	}
}

int main(void) 
{
	int rc;
	int i;
	int j;
	int k;
	char *src = NULL;
	char *dst = NULL;
	char *psrc = NULL;
	char *pdst = NULL;
	struct timespec ts_start, ts_end;
	int size = 65536;
	int temp=0;
	int count = 0;
	unsigned long long timec = 0;

	if (SENDCORE) {
		cpu_set_t cpu_set;
		CPU_ZERO(&cpu_set);
		CPU_SET(SENDCORE, &cpu_set);
		if (-1 == sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set)) {
			perror("send process band core error.");
		}
	}

	if (posix_memalign((void **) &src, 1024/*alignment*/, 0x60000000) != 0)
		//if(posix_memalign((void **)&src, 4096/*alignment*/, 0x60000000)!= 0)
	{
		printf("posix_memalign fail.\n");
		return -1;
	}

	if (posix_memalign((void **) &dst, 1024/*alignment*/, 0x60000000) != 0) {
		printf("posix_memalign fail.\n");
		free(src);
		return -1;
	}

	for (k = 0; k < 8; k++)
	{
		rc = clock_gettime(CLOCK_MONOTONIC, &ts_start);

		count=0;
		for (i = 0; i < COUNTNUMBER; i++)
		{
			temp=size/4096;
			psrc = src;
			pdst = dst;
#if 1
			memcpy(pdst, psrc, size);
#else
			for (j = 0; j < temp; j++)
			{
				memcpy(pdst, psrc, 4096);
				pdst = pdst + 4096;
				psrc = psrc + 4096;
				count++;
			}
#endif
		}

		rc = clock_gettime(CLOCK_MONOTONIC, &ts_end);

		printf("copy data by 4kb total count is %d\n", count/COUNTNUMBER);
		timespec_sub(&ts_end, &ts_start);
		/* display passed time, a bit less accurate but side-effects are accounted for */
		timec = ts_end.tv_sec * 1000000 + ts_end.tv_nsec / 1000;
		//printf("CLOCK_MONOTONIC reports %ld.%09ld seconds (total) for copy %d 1000 times\n", ts_end.tv_sec, ts_end.tv_nsec,size);
		printf("CLOCK_MONOTONIC reports %.2fus for copy %dtimes %dKB\n",
				(1.0 * timec) / COUNTNUMBER, COUNTNUMBER, size / 1024);
		size=size*2;
	}

	return EXIT_SUCCESS;
}
