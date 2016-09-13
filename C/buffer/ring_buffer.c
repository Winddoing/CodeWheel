#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFSIZE 1024 * 1024
#define min(x, y) ((x) < (y) ? (x) : (y))

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct cycle_buffer {
	unsigned char *buf;
	unsigned int size;
	unsigned int in;
	unsigned int out;
	pthread_mutex_t lock;
};

static struct cycle_buffer *fifo = NULL;

static int init_cycle_buffer(void)
{
	int size = BUFFSIZE, ret;

	ret = size & (size - 1); //size = 2^x 
	if (ret)
		return ret;

	fifo = (struct cycle_buffer *) malloc(sizeof(struct cycle_buffer));
	if (!fifo)
		return -1;

	memset(fifo, 0, sizeof(struct cycle_buffer));

	fifo->size = size;
	fifo->in = fifo->out = 0;

	pthread_mutex_init(&fifo->lock, NULL);

	fifo->buf = (unsigned char *) malloc(size);
	if (!fifo->buf)
		free(fifo);
	else
		memset(fifo->buf, 0, size);

	return 0;
}


unsigned int fifo_get(unsigned char *buf, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->in - fifo->out);
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));

	memcpy(buf, fifo->buf + (fifo->out & (fifo->size - 1)), l);
	memcpy(buf + l, fifo->buf, len - l);

	fifo->out += len;

	return len;
}



unsigned int fifo_put(unsigned char *buf, unsigned int len)
{
	unsigned int l;

	len = min(len, fifo->size - fifo->in + fifo->out);
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));

	memcpy(fifo->buf + (fifo->in & (fifo->size - 1)), buf, l);
	memcpy(fifo->buf, buf + l, len - l);

	fifo->in += len;

	return len;
}



static void * thread_read(void *arg)
{
	char buf[1024];
	unsigned int n;

	pthread_detach(pthread_self());

	for (;;) {
		memset(buf, 0, sizeof(buf));
		pthread_mutex_lock(&fifo->lock);
		n = fifo_get(buf, sizeof(buf));
		pthread_mutex_unlock(&fifo->lock);
		write(STDOUT_FILENO, buf, n);
	}

	printf("after thread_read : %s\n",buf);

	return NULL;
}

static void * thread_write(void *arg)
{
	unsigned char buf[] = "hello world";

	pthread_detach(pthread_self());

	for (;;) {
		pthread_mutex_lock(&fifo->lock);
		fifo_put(buf, strlen(buf));
		pthread_mutex_unlock(&fifo->lock);
	}

	return NULL;
}

int main(void)
{
	int ret;
	pthread_t wtid, rtid;

	ret = init_cycle_buffer();
	if (ret == -1)
		return ret;

	pthread_create(&wtid, NULL, thread_write, NULL);
	pthread_create(&rtid, NULL, thread_read, NULL);

	pthread_exit(NULL);

	return 0;
}


