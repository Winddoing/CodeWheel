/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : eventfd.c
 *  Author       : wqshao
 *  Created Time : 2022-07-14 14:00:19
 *  Description  :
 *    数据同步，写一次才能读一次，如果多次进行读将被阻塞住
 *    可用于多进程或线程之间的流程同步
 *
 *    gcc eventfd.c && ./a.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <errno.h>
#include <string.h>  //strerror
#include <sys/eventfd.h>
#include <unistd.h>


static int open_event()
{
	int eventFd = eventfd(0, EFD_CLOEXEC | EFD_SEMAPHORE);
	if (eventFd == -1){
		printf("eventfd create error:%s\n", strerror(errno));
		return -1;
	}
	return eventFd;
}

static void close_event(int fd)
{
	close(fd);
}

static void read_event(int fd)
{
    uint64_t v;
    ssize_t n;
    n = read(fd, &v, sizeof(uint64_t));
    if (n != sizeof(uint64_t))
        printf("eventfd read error\n");
}

static void write_event(int fd)
{
    uint64_t v = 1;
    ssize_t n;
    n = write(fd, &v, sizeof(uint64_t));
    if (n != sizeof(uint64_t))
        printf("eventfd write error\n");
}


int main(int argc, const char *argv[])
{
	int fd = 0;

	fd = open_event();
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	write_event(fd);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	read_event(fd);
	//read_event(fd);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	close_event(fd);

	return 0;
}
