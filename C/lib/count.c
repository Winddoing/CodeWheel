/*#############################################################
 *     File Name	: count.c
 *     Author		: wqshao
 *     Created Time	: 2018年10月08日 星期一 10时45分10秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include "count.h"

int get()
{
    return count;
}

void inc()
{
    pthread_mutex_lock(&mutex);
    count++;
    pthread_mutex_unlock(&mutex);
}
