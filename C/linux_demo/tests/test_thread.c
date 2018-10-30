#include <stdio.h>
#include <unistd.h>
#include <timer.h>

#include <pthread.h>

//子线程,回调函数,并发执行
static void *task_routine(void *arg)
{
	int cnt = 0;
	while(1)
	{
		printf("[%s:%d]: thread id : %lu\n",__func__,__LINE__,pthread_self());
		sleep(1);
		if(cnt > 10)
		{
			//退出while循环.
			break;
		}
		cnt++;
	}
	//本线程退出
	printf("[%s:%d]: thread id : %lu exit...\n",__func__,__LINE__,pthread_self());
	return NULL;
}


int main(int argc, char *argv[])
{
	pthread_t thread;
	int ret = -1;
	printf("[%s:%d]: thread id : %lu\n",__func__,__LINE__,pthread_self());
	//创建子线程task_routine,此刻开始,子线程与主线程并发执行.
	ret = pthread_create(&thread,NULL,task_routine,NULL);
	if(ret < 0)
	{
		printf("[%s:%d]: pthread_create fail!\n",__func__,__LINE__);
		return -1;
	}
	
	//等待子线程task_routine退出.
	pthread_join(thread,NULL);

	//休息5秒
	printf("[%s:%d]: thread id : %lu sleep 5s ...\n",__func__,__LINE__,pthread_self());
	sleep(5);
	//主线程退出
	printf("[%s:%d]: thread id : %lu exit...\n",__func__,__LINE__,pthread_self());
 	return 0;
}

