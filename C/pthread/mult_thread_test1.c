/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : mult_thread_test1.c
 *  Author       : wqshao
 *  Created Time : 2022-08-23 09:33:25
 *  Description  :
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/syscall.h> //syscall

void* doSomeThing(void* arg)
{
    printf("From thread function: Thread ID: %ld, tid=%d\n", (long)pthread_self(), syscall(SYS_gettid));

    return NULL;
}

int main(int argc, char *argv[])
{
    size_t count = 10;
    pthread_t *tid;

    tid = malloc(count * sizeof *tid);

    for(size_t i = 0; i< count; i++) {
        int rc = pthread_create(&tid[i], NULL, &doSomeThing, NULL);
        if(rc) { /* failure */ }
    }

    for(size_t i = 0;i<count; i++) {
        pthread_join(tid[i], NULL);
    }

    free(tid);
    return 0;
}
