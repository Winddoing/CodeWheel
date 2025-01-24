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

void* thread_function(void* arg)
{
    // 设置线程名称
    pthread_setname_np(pthread_self(), "MyThread");

    // 线程工作
    while (1) {
        printf("Thread is running...\n");
        sleep(1);
	int *p = NULL;
	*p = 1;
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
