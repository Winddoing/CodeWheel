/*#############################################################
 *     File Name	: test_memcopy.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月18日 星期五 11时29分55秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> 

#define TEST_DATA_UNIT_KB 1*1024
#define TEST_DATA_UNIT_MB TEST_DATA_UNIT_KB*1204

int main(int argc, const char *argv[])
{
    unsigned char *src = NULL, *dst = NULL;
    struct timeval tv;
    long long start_ms = 0, end_ms = 0;
    int size = 0, cnt = 0;

    printf("%s, argc=%d, argv[1]=%s\n", __func__, argc, argv[1]);

    if (argc <= 1) {
        cnt = 10;;
    } else {
        cnt = atoi(argv[1]);
    }

    size = TEST_DATA_UNIT_MB * cnt;
    printf("%s, Test data size: %d MB\n", __func__, cnt);
    

    src = malloc(size * sizeof(*src));
    if (!src) {
        printf("src malloc error\n");
        return -1;
    }

    dst = malloc(size * sizeof(*dst));
    if (!dst) {
        printf("dst malloc error\n");
        free(src);
        return -1;
    }

    memset(src, rand(), size * sizeof(*src));

    gettimeofday(&tv,NULL);                           
    start_ms = tv.tv_sec*1000 + tv.tv_usec/1000;    

    memcpy(dst, src, size * sizeof(*src));

    gettimeofday(&tv,NULL);                           
    end_ms = tv.tv_sec*1000 + tv.tv_usec/1000;    

    printf("%s, Test memcpy: copy time=%lld ms, rate=%lld KB/ms\n", __func__,
            end_ms - start_ms, cnt * TEST_DATA_UNIT_KB / (end_ms - start_ms));

    free(src);
    free(dst);

    return 0;
}
