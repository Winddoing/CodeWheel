#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define NUM_THREAD 3

void *print_hello(void *threadid)
{
	long tid;
	tid = (long)threadid;

	printf("Hello World! thread id #%ld\n", tid);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREAD];
	long i;
	int rc;

	for(i=0; i<NUM_THREAD; i++) {
		printf("Create thread <%ld>\n", i);
		rc = pthread_create(&threads[i], NULL, print_hello, (void*)i);
		if (rc) {
			printf("ERROR: pthread_create() return %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}
