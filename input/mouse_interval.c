/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : mouse_interval.c
 *  Author       : wqshao
 *  Created Time : 2025-06-10 11:04:24
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
//#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>



int main() 
{
    struct input_event ev;
    long prev_time = 0, curr_time;

    int fd = open("/dev/input/mouse0", O_RDONLY);
    if (fd < 0) {
	    printf("open mouse0 failed. fd=%d, %s\n", fd, strerror(errno));
	    return -1;
    }

    while (read(fd, &ev, sizeof(ev)) > 0) {
        curr_time = ev.time.tv_sec * 1000000 + ev.time.tv_usec;
	printf("curr_time: %16ld\n", curr_time);
#if 0
        if (prev_time != 0) {
            printf("Event interval: %ld usec, %ld ms\n", curr_time - prev_time, (curr_time - prev_time) / 1000);
        }
        prev_time = curr_time;
#endif
    }

    close(fd);
    return 0;
}
