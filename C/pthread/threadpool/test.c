/*#############################################################
 *     File Name    : test.c
 *     Author       : wqshao
 *     Created Time : 2018年09月30日 星期日 11时54分04秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>     //sleep

#include "threadpool.h"

void *work(void *arg)
{
	char *p = (char *)arg;
	printf("threadpool callback fuction : %s.\n", p);
	sleep(1);
}

int main(void)
{
	struct threadpool *pool = threadpool_init(10, 20);

	threadpool_add_job(pool, work, "1");
	threadpool_add_job(pool, work, "2");
	threadpool_add_job(pool, work, "3");
#if 1
	threadpool_add_job(pool, work, "4");
	threadpool_add_job(pool, work, "5");
	threadpool_add_job(pool, work, "6");
	threadpool_add_job(pool, work, "7");
	threadpool_add_job(pool, work, "8");
	threadpool_add_job(pool, work, "9");
	threadpool_add_job(pool, work, "10");
	threadpool_add_job(pool, work, "11");
	threadpool_add_job(pool, work, "12");
	threadpool_add_job(pool, work, "13");
	threadpool_add_job(pool, work, "14");
	threadpool_add_job(pool, work, "15");
	threadpool_add_job(pool, work, "16");
	threadpool_add_job(pool, work, "17");
	threadpool_add_job(pool, work, "18");
	threadpool_add_job(pool, work, "19");
	threadpool_add_job(pool, work, "20");
	threadpool_add_job(pool, work, "21");
	threadpool_add_job(pool, work, "22");
	threadpool_add_job(pool, work, "23");
	threadpool_add_job(pool, work, "24");
	threadpool_add_job(pool, work, "25");
	threadpool_add_job(pool, work, "26");
	threadpool_add_job(pool, work, "27");
	threadpool_add_job(pool, work, "28");
	threadpool_add_job(pool, work, "29");
	threadpool_add_job(pool, work, "30");
	threadpool_add_job(pool, work, "31");
	threadpool_add_job(pool, work, "32");
	threadpool_add_job(pool, work, "33");
	threadpool_add_job(pool, work, "34");
	threadpool_add_job(pool, work, "35");
	threadpool_add_job(pool, work, "36");
	threadpool_add_job(pool, work, "37");
	threadpool_add_job(pool, work, "38");
	threadpool_add_job(pool, work, "39");
	threadpool_add_job(pool, work, "40");
#endif

	threadpool_destroy(pool);

	return 0;
}
