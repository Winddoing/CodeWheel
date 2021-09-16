/*#############################################################
 *     File Name	: cnt.c
 *     Author		: winddoing
 *     Created Time	: 2021年05月10日 星期一 16时08分42秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void cnt(int datalen)
{
    static int len = 0;
    static int cnt = 1;
    char buf[64];
    static FILE *file = NULL;

    if (!file)
        file = fopen("./send_data.log", "w+");

    if (len != datalen) {
        if (len) {
            printf("data size: %d, cnt: %d\n", len, cnt);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "data size: %d bit, cnt: %d\n", len, cnt);

            fwrite(buf, 1, strlen(buf), file);
        }

        len = datalen;
        cnt = 1;
    } else {
        cnt++;
    }
}

int main(int argc, const char *argv[])
{
    cnt(2);
    cnt(2);
    cnt(2);
    cnt(2);
    cnt(5);
    cnt(5);
    cnt(5);
    cnt(5);
    cnt(5);
    cnt(5);
    cnt(7);
    cnt(7);
    cnt(7);
    cnt(7);
    cnt(4);
    cnt(4);
    cnt(9);
    cnt(1);
    cnt(1);
    cnt(1);
    cnt(0);

    return 0;
}
