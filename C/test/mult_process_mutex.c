/*#############################################################
 *     File Name	: mult_process_mutex.c
 *     Author		: winddoing
 *     Created Time	: 2021年06月28日 星期一 16时14分13秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct _FOO
{
    int nCount;
    int nData;
}FOO,*PFOO;

int main(int argc,char *argv[])
{
    FOO *ptr;
    pid_t pid;

    pthread_mutexattr_t mutexattr;
    pthread_mutex_t mutex;

    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr,PTHREAD_PROCESS_SHARED);	//设置为进程共享

    pthread_mutex_init(&mutex,&mutexattr);

    //匿名内存映射，让父子进程都操作ptr指向的内存区，如果不使用共享内存，则父子进程的ptr指向的是各自的内存空间
    ptr = (PFOO)mmap(NULL,sizeof(FOO),PROT_READ | PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);	
    ptr->nCount = 1;
    ptr->nData = 2;
    printf("%d,%d\n",ptr->nCount,ptr->nData);

    if( (pid = fork()) < 0) {
        printf("fork error\n");
        return -1;
    } else if( 0 == pid) {
        //子进程
        for(int i = 0;i<3;i++) {
            pthread_mutex_lock(&mutex);
            ptr->nCount++;
            printf("child ++ === %d\n",ptr->nCount);
            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
    } else {
        //父进程 
        for(int i = 0;i<3;i++) {
            pthread_mutex_lock(&mutex);
            ptr->nCount += 2;
            printf("parent +2 === %d\n",ptr->nCount);
            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
    }

    waitpid(pid,NULL,0);
    munmap(NULL,sizeof(FOO));
    return 0;
}
