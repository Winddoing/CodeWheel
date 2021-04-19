/*#############################################################
 *     File Name	: check_ip.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月19日 星期一 21时44分12秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <arpa/inet.h>

static int is_ip_valid(char* check_ip)
{
    char* net_segment_ip = "192.168.2.0";
    char* net_segment_mark = "255.255.255.0";

    struct in_addr nseg_ip_addr, nseg_mark_addr, check_ip_addr;

    inet_pton(AF_INET, net_segment_ip, (void *)&nseg_ip_addr);
    inet_pton(AF_INET, net_segment_mark, (void *)&nseg_mark_addr);
    inet_pton(AF_INET, check_ip, (void *)&check_ip_addr);

    
    printf("===> func: %s, line: %d\n, nseg_ip:%x, nseg_mark:%x, ip:%x\n", __func__, __LINE__, nseg_ip_addr.s_addr, nseg_mark_addr.s_addr, check_ip_addr.s_addr);

    
    printf("===> func: %s, line: %d, ----:%x\n", __func__, __LINE__, nseg_mark_addr.s_addr & check_ip_addr.s_addr);
    printf("===> func: %s, line: %d, --|--:%x\n", __func__, __LINE__, (nseg_mark_addr.s_addr & check_ip_addr.s_addr) | nseg_ip_addr.s_addr);
    printf("===> func: %s, line: %d, --^--:%x\n", __func__, __LINE__, nseg_mark_addr.s_addr & check_ip_addr.s_addr ^ nseg_ip_addr.s_addr);

    if ((nseg_mark_addr.s_addr & check_ip_addr.s_addr) ==  nseg_ip_addr.s_addr)
        return 1;
    else
        return 0;
}

int main(int argc, const char *argv[])
{
    int ret = 0;

    ret = is_ip_valid("192.168.1.2");
    printf("===> func: %s, line: %d, ret=%d\n", __func__, __LINE__, ret);
    
    return 0;
}
