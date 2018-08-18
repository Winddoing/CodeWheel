/*#############################################################
 *     File Name	: rand_valom.c
 *     Author		: wqshao
 *     Created Time	: 2018年08月18日 星期六 14时25分23秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <time.h> 

#include <sys/time.h>
#include <sys/types.h> 
#include <unistd.h>    


static void seedrand_val1()
{
	unsigned int rand_val = 0;

	srand((unsigned int)time(0));

	rand_val = rand();

	printf("%s: rand_val=%d\n", __func__, rand_val);
}

static void seedrand_val2()
{
	struct timeval tv;
	unsigned int rand_val = 0;

	gettimeofday(&tv, NULL);

	rand_val = tv.tv_sec^tv.tv_usec^getpid(); //秒(tv.tv_sec)和微秒(tv.tv_usec)和进程ID的位进行异或操作生成随机数

	printf("%s: rand_val=%d\n", __func__, rand_val);
}

int main()
{

	seedrand_val1();
	seedrand_val2();
	return 0;
}
