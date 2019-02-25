/*#############################################################
 *     File Name	: cpu_cache.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月25日 星期一 11时32分44秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

void get_cache_size()
{
    FILE *fp = NULL;
    char buf[10];

    fp = fopen("/sys/devices/system/cpu/cpu1/cache/index0/size", "r");
    if(fp != NULL ) {
        fgets(buf, 10, (FILE*)fp);
        printf("Cache size %s\n", buf);
    }

    fclose(fp);
}

int main(int argc, const char *argv[])
{
    get_cache_size();

    long l1_dcache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    long l1_icache_line_size = sysconf(_SC_LEVEL1_ICACHE_LINESIZE);
    long l2_cache_line_size = sysconf(_SC_LEVEL2_CACHE_LINESIZE);
    long l3_cache_line_size = sysconf(_SC_LEVEL3_CACHE_LINESIZE);

    printf("L1 DCache Line Size is %ld bytes.\n", l1_dcache_line_size);
    printf("L1 ICache Line Size is %ld bytes.\n", l1_icache_line_size);
    printf("L2 Cache Line Size is %ld bytes.\n", l2_cache_line_size);
    printf("L3 Cache Line Size is %ld bytes.\n", l3_cache_line_size);

    return 0;
}
