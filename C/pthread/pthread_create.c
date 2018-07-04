/*#############################################################
 *     File Name	: pthread_create.c
 *     Author		: wqshao
 *     Created Time	: Wed 27 Jun 2018 12:14:35 AM DST
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>



/* print process and thread IDs */
void printids(const char *s)
{
	pid_t pid, ppid;
	pthread_t tid;

	pid= getpid();
	ppid = getppid();
	tid = pthread_self();

	printf("%16s pid %5u ppid %5u tid %16u (0x%x)\n",
			s, (unsigned int)pid, (unsigned int)ppid,
			(unsigned int)tid, (unsigned int)tid);
}

/* thread process */
void *thread_func(void *arg)
{
	printids("new thread: ");

	printf("%s: return 0x123\n", __func__);
	return (void *)0x123;
}

/* main func */
int main()
{
	int err;
	void *tret; /* thread return value */
	pthread_t ntid;

	err = pthread_create(&ntid, NULL, thread_func, NULL);
	if (err != 0)
		perror("can't create thread");

	err = pthread_join(ntid, &tret);
	if (err != 0)
		perror("can't join thread");

	printids("main thread: ");
	printf("thread exit code: %p\n", tret);

	return 0;
}
