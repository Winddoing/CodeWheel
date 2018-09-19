/*#############################################################
 *     File Name	: atom.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月19日 星期三 16时39分46秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h> 

#include "include/atom.h"

int g_iFlagAtom = 0;
#define WORK_SIZE 5000000
#define WORKER_COUNT 10
pthread_t g_tWorkerID[WORKER_COUNT];
int g_iSum = 0;
int lock = 0;

void * thr_worker(void *arg) 
{
	printf("WORKER THREAD %08X STARTUP\n", (unsigned int)pthread_self());
	int i=0;
	for (i=0; i<WORK_SIZE; ++i) {
		if (g_iFlagAtom) {
			ATOM_INC(g_iSum);
		} else {
			//ATOM_LOCK(lock);
			g_iSum ++;
			//ATOM_UNLOCK(lock);
		}
	}
	return NULL;
}

void * thr_management(void *arg) 
{
	printf("MANAGEMENT THREAD %08X STARTUP\n", (unsigned int)pthread_self());
	int i;
	for (i=0;i<WORKER_COUNT;++i) {
		pthread_join(g_tWorkerID[i], NULL);
	}
	printf("ALL WORKER THREADS FINISHED.\n");
	return NULL;
}

int main(int argc, const char* argv[]) 
{
	pthread_t tManagementID;
	int i=0; 
	struct timeval start, end;  

	gettimeofday(&start, NULL);
	pthread_create (&tManagementID, NULL, thr_management, NULL);

	for (i=0;i<WORKER_COUNT;++i) {
		pthread_create(&g_tWorkerID[i], NULL, thr_worker, NULL);
	}

	printf("CREATED %d WORKER THREADS\n", i);
	pthread_join(tManagementID, NULL);

	gettimeofday(&end, NULL);
	printf("THE SUM: %d\n", g_iSum);

	printf("Run time: %ldms\n", 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000);
	return 0;
}
