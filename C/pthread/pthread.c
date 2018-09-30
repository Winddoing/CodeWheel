#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <unistd.h>

#include <pthread.h>

#define NUM_THREAD 3

static pthread_once_t once=PTHREAD_ONCE_INIT;
static pthread_barrier_t barrier;
static pthread_key_t log_file_key; //保存每个线程log文件的私有指针


int log_init()
{
	char filename[20];
	FILE* file;

	sprintf (filename, "thread-%d.log", (int)pthread_self());

	file = fopen(filename, "w");
	if (file == NULL) {
		printf("%s fopen fail, errno=%d\n", __func__, errno);
		return -1;
	}

	/* 将文件指针保存在log_file_key标识的TSD中。*/
	pthread_setspecific(log_file_key, file);

	return 0;
}

void log_write(const char* data)
{
	FILE* file = (FILE*)pthread_getspecific(log_file_key);          

	fprintf(file, "%s\n", data);                                     
}

void once_init_routine(void)
{
	printf("Go run ...\n");
}

void *print_hello(void *data)
{
	int index = (int)data;

	pthread_once(&once, once_init_routine); //只执行一次once_init_routine

	log_init();
	log_write("pthread start ...");

	sleep(index);

	pthread_barrier_wait(&barrier);
	printf("Hello World! thread index #%d, pthread_self=%ld\n", index, pthread_self());
}


void close_log(void* key)
{
	printf("%s key=%p\n", __func__, key);
    fclose((FILE*)key);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREAD];
	int ret, i;

	pthread_barrier_init(&barrier, NULL, NUM_THREAD);
	pthread_key_create(&log_file_key, close_log);

	for (i = 0; i < NUM_THREAD; i++) {
		ret = pthread_create(&threads[i], NULL, print_hello, (void*)i);
		if (ret) {
			printf("ERROR: pthread_create() return %d\n", ret);
			perror("print_hello\n");
			return -1;
		}
		printf("Create threads<%d>, thread ID=%ld\n", i, threads[i]);
	}

	for (i = 0; i < NUM_THREAD; i++) {
		pthread_join(threads[i], NULL); //等待所有线程处理完后，执行下面操作
	}

	pthread_barrier_destroy(&barrier);
	pthread_key_delete(log_file_key);

	printf("All thread over !!!\n");

	return 0;
}
