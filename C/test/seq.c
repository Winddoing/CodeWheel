/*#############################################################
 *     File Name	: seq.c
 *     Author		: winddoing
 *     Created Time	: 2021年06月26日 星期六 11时12分53秒
 *     Description	:
 *############################################################*/

//https://blog.csdn.net/hi_software/article/details/51768026

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, const char *argv[])
{
    int i = 0;
    uint16_t seq = 1;
    uint16_t prev_seq = 0;

    for (i = 0; i < 70000; i++) {
        int16_t diff = (int16_t)(seq - prev_seq);
        if (diff == 1) {
            printf("===> func: %s, line: %d, i=%d, seq=%d, prev_seq=%d, diff=%d\n", __func__, __LINE__, i, seq, prev_seq, diff);
        } else {
            printf("xxxxxxxxxxxx===> func: %s, line: %d, i=%d, seq=%d, prev_seq=%d, diff=%d\n", __func__, __LINE__, i, seq, prev_seq, diff);
        }
        prev_seq = seq;
        seq++;
    }
    return 0;
}

