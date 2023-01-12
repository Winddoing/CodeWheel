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
#include <sys/epoll.h>
#include <assert.h>
#include <unistd.h>

struct event {
	int event_fd;
	int epoll_fd;
};

static int create_event(struct event *ev)
{
	assert(ev != NULL);

	ev->event_fd = eventfd(0, EFD_CLOEXEC | EFD_SEMAPHORE);
	if (ev->event_fd == -1){
		printf("eventfd create error:%s\n", strerror(errno));
		return -1;
	}
	ev->epoll_fd = epoll_create(1);
	if (ev->epoll_fd == -1) {
		printf("epolldf create error: %s\n", strerror(errno));
		return -1;
	}

	struct epoll_event eventItem;
	memset(&eventItem, 0, sizeof(struct epoll_event));
	eventItem.events  = EPOLLIN;
	eventItem.data.fd = ev->event_fd;
	if(epoll_ctl(ev->epoll_fd, EPOLL_CTL_ADD, ev->event_fd, &eventItem) == -1) {
		printf("epoll_ctl error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

static int delete_event(struct event *ev)
{
	assert(ev != NULL);

	close(ev->event_fd);
	ev->event_fd = -1;

	close(ev->epoll_fd);
	ev->epoll_fd = -1;

	return 0;
}

/**
 * @brief  wait_event等待事件被触发, 同时可以设置超时时间，单位ms
 *
 * @param ev  event结构体指针
 * @param timeout_ms	等待超时时间，如果为0将死等事件被触发
 *
 * @returns   
 */
static int wait_event(struct event *ev, int timeout_ms)
{
	uint64_t v;
	ssize_t ret;

	assert(ev != NULL);

	struct epoll_event eventItem;
	ret = epoll_wait(ev->epoll_fd, &eventItem, 1, timeout_ms);
	if (ret == 0) {
		printf("epoll wait timeout: %d ms, errno=%d\n", timeout_ms, errno);
		return -1;
	}

	ret = read(eventItem.data.fd, &v, sizeof(uint64_t));
	if (ret != sizeof(uint64_t)) {
		printf("eventfd read error, ret=%ld, errno=%d(%s)\n", ret, errno, strerror(errno));
		return ret;
	}

	return 0;
}

static int set_event(struct event *ev)
{
    uint64_t v = 1;
    ssize_t ret;

	assert(ev != NULL);

    ret = write(ev->event_fd, &v, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
		printf("eventfd write error, ret=%ld, errno=%d(%s)\n", ret, errno, strerror(errno));
		return -1;
	}

	return 0;
}

int main(int argc, const char *argv[])
{

	struct event ev;

	create_event(&ev);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	//set_event(&ev);
	set_event(&ev);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	wait_event(&ev, 100);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	delete_event(&ev);

	return 0;
}
