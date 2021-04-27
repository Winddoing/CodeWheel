/*#############################################################
 *     File Name	: mult_thread_test.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月15日 星期四 11时48分52秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <errno.h>

void* _do_gpu_monitor(void* args)
{

    printf("===> func: %s, line: %d, file: %s\n", __func__, __LINE__, __FILE__);
    return NULL;
}


int main(int argc, const char *argv[])
{
    int i = 0;

    for (i = 0; i < 10; i++) {
        pthread_attr_t attr;                                                                                         
        pthread_t tid_gpu_monitor;                                              

        pthread_attr_init(&attr);                                               
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);            

        if(pthread_create(&tid_gpu_monitor, &attr, _do_gpu_monitor, NULL) != 0) {
            printf("create _do_gpu_monitor gpu_id[%d] thread fail, error %d\n", i + 1, strerror(errno));
        }                                                                       

        pthread_attr_destroy(&attr);                                            
    }

    return 0;
}
