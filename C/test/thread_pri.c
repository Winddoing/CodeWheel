/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : thread_pri.c
 *  Author       : wqshao
 *  Created Time : 2024-09-29 15:05:48
 *  Description  :
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func(void* arg) {
	// 线程的工作内容
	printf("Thread is running\n");
	sleep(2);
	printf("Thread is over\n");
	return NULL;
}

int main() {
	pthread_t thread_id;
	int policy;
	struct sched_param param;
	int priority;

	// 创建线程
	if (pthread_create(&thread_id, NULL, thread_func, NULL)) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	// 设置线程的调度策略为SCHED_RR（实时调度策略）
	//policy = SCHED_RR;
	policy = SCHED_FIFO;

	// 获取当前线程的调度参数
	if (pthread_getschedparam(thread_id, &policy, &param)) {
		perror("pthread_getschedparam");
		exit(EXIT_FAILURE);
	}

	printf("===> func: %s, line: %d, pri=%d\n", __func__, __LINE__, param.sched_priority);
	memset(&param, 0, sizeof(param));
	// 设置线程的优先级
	priority = 1; // 优先级范围通常是1-99，数值越大优先级越高
	param.sched_priority = priority;

	// 设置线程的调度策略和优先级
	if (pthread_setschedparam(thread_id, policy, &param)) {
		perror("pthread_setschedparam");
		exit(EXIT_FAILURE);
	}

	// 等待线程结束
	pthread_join(thread_id, NULL);

	return 0;
}
