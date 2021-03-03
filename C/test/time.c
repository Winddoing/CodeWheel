/*#############################################################
 *     File Name	: time.c
 *     Author		: winddoing
 *     Created Time	: 2021年01月21日 星期四 13时52分23秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <time.h>

static char strtime[128] = {0};
static int64_t ONE_MIN  = 60;
static int64_t ONE_HOUR = 60*60;
static int64_t ONE_DAY  = 24*60*60;
static int64_t ONE_YEAR = 365*24*60*60;
static char* format_time(int64_t ms)
{
    int64_t year, day, hour, min, sec, left;
    year = day = hour = min = sec = left = 0;

    memset(strtime, 0, sizeof(strtime));

    left = ms / 1000;

    if(left >= ONE_YEAR) {
        year = left / ONE_YEAR;
        left = left % ONE_YEAR;
    }

    if(left >= ONE_DAY) {
        day = left / ONE_DAY;
        left = left % ONE_DAY;
    }

    if(left >= ONE_HOUR) {
        hour = left / ONE_HOUR;
        left = left % ONE_HOUR;
    }

    if(left >= ONE_MIN) {
        min = left / ONE_MIN;
        left = left % ONE_MIN;
    }

    sec = left;

    if(year) {
        sprintf(strtime, "%2ldY%3ldD", year, day);
    } else if(day) {
        sprintf(strtime, "%3ldD%2ldH", day, hour);
    } else if(hour) {
        sprintf(strtime, "%3ldH%2ldm", hour, min);
    } else if(min) {
        sprintf(strtime, "%3ldm%2lds", min, sec);
    } else if(sec) {
        sprintf(strtime, "%6lds", sec);
    }

    return strtime;
}

static int64_t getSystemBootMs() {
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC,&ts) == 0) {
        return ts.tv_sec * 1000LL + (ts.tv_nsec/1000000LL);
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    int i = 0;
    for (i = 0; i < 1000; i++) {
        printf("===> func: %s, line: %d, %s\n", __func__, __LINE__, format_time(getSystemBootMs()));
    }

    return 0;
}
