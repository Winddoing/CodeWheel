/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : thread_name.c
 *  Author       : wqshao
 *  Created Time : 2025-01-23 15:19:23
 *  Description  :
 *  	ps -L -p 2432346
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/prctl.h> //prctl
#include <string.h>

/*
 * 线程名称的长度限制为16字节（包括终止符 \0），因此需要确保拼接后的名称不超过此限制。
 * 如果原始名称已经接近16字节，拼接后的名称可能会被截断。
 * prctl 是 Linux 特有的系统调用，在其他操作系统上可能不可用。
 * */
void set_thread_name_with_suffix(const char* suffix)
{
	char thread_name[16]; // 线程名称最多16字节（包括终止符）
	char new_name[16];    // 新名称缓冲区

	// 获取当前线程名称
	if (prctl(PR_GET_NAME, (unsigned long)thread_name, 0, 0, 0) == -1) {
		perror("prctl(PR_GET_NAME) failed");
		return;
	}

	// 打印原始线程名称
	printf("Original thread name: %s\n", thread_name);

	// 将原始名称和后缀拼接为新名称
	snprintf(new_name, sizeof(new_name), "%.*s-%s", (int)(sizeof(new_name) - strlen(suffix) - 2), thread_name, suffix);

	// 设置新线程名称
	if (prctl(PR_SET_NAME, (unsigned long)new_name, 0, 0, 0) == -1) {
		perror("prctl(PR_SET_NAME) failed");
		return;
	}

	// 打印新线程名称
	printf("New thread name: %s\n", new_name);
}

void* thread_function(void* arg)
{
	// 设置线程名称
	//pthread_setname_np(pthread_self(), "MyThread");

	set_thread_name_with_suffix("MyThread123456789");

	// 线程工作
	while (1) {
		printf("Thread is running...\n");
		sleep(1);
#if 0
		int *p = NULL;
		*p = 1;
#endif
	}
	return NULL;
}

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, thread_function, NULL);

	// 主线程工作
	while (1) {
		printf("Main thread is running...\n");
		sleep(1);
	}

	pthread_join(thread, NULL);
	return 0;
}
