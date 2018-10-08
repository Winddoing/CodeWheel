/*#############################################################
 *     File Name	: main.c
 *     Author		: wqshao
 *     Created Time	: 2018年10月08日 星期一 10时47分29秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "libcount.h"

#define NUM 1000
/*#define LIBPATH "libcount.so"*/

void *ThreadRun(void *arg)
{
	int i = 0;

	void *handler = dlopen(LIBPATH, RTLD_LAZY);
	if (handler == NULL) {
		printf("ERROR:%s:dlopen\n", dlerror());
		return NULL;
	}

	void (*inc)() = (void (*)())dlsym(handler, "inc");
	if (inc == NULL) {
		printf("ERROR:%s:dlsym\n", dlerror());
		return NULL;
	}

	int (*get)() = (int (*)())dlsym(handler, "get");
	if (get == NULL) {
		printf("ERROR:%s:dlsym\n", dlerror());
		return NULL;
	}

	for (i = 0; i < NUM; i++) {
		inc();
		usleep(1000*1000);
		printf("INFO:PID(%d):%d\n", getpid(), get());
	}

	dlclose(handler);
}

int main()
{
	pthread_t tid;

	pthread_create(&tid, NULL, ThreadRun, NULL);
	printf("create Thread OK!!!\n");

	while (1);

	return 0;
}
