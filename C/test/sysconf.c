/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : sysconf.c
 *  Author       : wqshao
 *  Created Time : 2023-04-03 14:47:46
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>

#define ONE_MB (1024 * 1024)

int main()
{
    printf("The number of processors configured is :%ld\n",
        sysconf(_SC_NPROCESSORS_CONF));
    printf("The number of processors currently online (available) is :%ld\n",
        sysconf(_SC_NPROCESSORS_ONLN));
    printf ("The pagesize: %ld\n", sysconf(_SC_PAGESIZE));
    printf ("The number of pages: %ld\n", sysconf(_SC_PHYS_PAGES));
    printf ("The number of available pages: %ld\n", sysconf(_SC_AVPHYS_PAGES));
    printf ("The memory size: %lld MB\n",
        (long long)sysconf(_SC_PAGESIZE) * (long long)sysconf(_SC_PHYS_PAGES) / ONE_MB );
    printf ("The number of files max opened:: %ld\n", sysconf(_SC_OPEN_MAX));
    printf("The number of ticks per second: %ld\n", sysconf(_SC_CLK_TCK));
    printf ("The max length of host name: %ld\n", sysconf(_SC_HOST_NAME_MAX));
    printf ("The max number of simultaneous processes per user: %ld\n", sysconf(_SC_CHILD_MAX));

    return 0;
}
