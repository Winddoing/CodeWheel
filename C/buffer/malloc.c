/*#############################################################
 *     File Name	: malloc.c
 *     Author		: wqshao
 *     Created Time	: 2018年05月22日 星期二 09时57分54秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <sys/time.h>

int main()
{
	struct timeval start, end;
	int *p1, *p2;
	int i = 0;
	unsigned int malloc_s = 0, memset_s = 0;

	p2 = malloc(2048);

#define TCOUNT 100000
	for (i = 0; i < TCOUNT; i++) {
		gettimeofday(&start, NULL);
		p1 = malloc(2048);
		free(p1);
		gettimeofday(&end, NULL);

		malloc_s += end.tv_usec - start.tv_usec;

		memset(&start, 0, sizeof(start));
		memset(&end, 0, sizeof(end));

		gettimeofday(&start, NULL);
		memset(p2, 0, 2048);
		gettimeofday(&end, NULL);

		memset_s += end.tv_usec - start.tv_usec;
	}
	printf("===> func: %s, line: %d, malloc time s:%d, v:%d\n", __func__, __LINE__, malloc_s, malloc_s / TCOUNT);
	printf("===> func: %s, line: %d, memset time s:%d, v:%d\n", __func__, __LINE__, malloc_s, memset_s / TCOUNT);


	return 0;
}

