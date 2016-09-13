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

//1buffer指向存放数据的缓冲区，size是缓冲区的大小，in是写指针下标，out是读指针下标,在len和(fifo->size - fifo->in + fifo->out)之间取一个较小的值赋给len。注意，当(fifo->in == fifo->out+fifo->size)时，表示缓冲区已满，此时得到的较小值一定是0，后面实际写入的字节数也全为0。另一种边界情况是当len很大时（因为len是无符号的，负数对它来说也是一个很大的正数），这一句也能保证len取到一个较小的值，因为fifo->in总是大于等于fifo->out， 所以后面的那个表达式的值不会超过fifo->size的大小把上一步决定的要写入的字节数len“切开”，这里又使用了一个技巧。注意：实际分配 给fifo->buffer的字节数fifo->size，必须是2的幂，否则这里就会出错。既然fifo->size是2的幂，那么 (fifo->size-1)也就是一个后面几位全为1的数，也就能保证(fifo->in & (fifo->size - 1))总为不超过(fifo->size - 1)的那一部分，和(fifo->in)% (fifo->size - 1)的效果一样。

//这样后面的代码就不难理解了，它先向fifo->in到缓冲区末端这一块写数据，如果还没写完，在从缓冲区头开始写入剩下的，从而实现了循环缓冲。最后，把写指针后移len个字节，并返回len。

//从上面可以看出，fifo->in的值可以从0变化到超过fifo->size的数值，fifo->out也如此，但它们的差不会超过fifo->size


