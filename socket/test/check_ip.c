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
    char* whitelist_address_seg = "192.168.2.0";
    //char* whitelist_address_seg = "0.0.0.0";
    char* whitelist_netmark     = "255.255.255.0";

    struct in_addr nseg_addr, nseg_netmark, check_addr;

    inet_pton(AF_INET, whitelist_address_seg, (void *)&nseg_addr);
    inet_pton(AF_INET, whitelist_netmark, (void *)&nseg_netmark);
    inet_pton(AF_INET, check_ip, (void *)&check_addr);

    
    printf("===> func: %s, line: %d\n, nseg_ip:%x, nseg_mark:%x, ip:%x\n", __func__, __LINE__, nseg_addr.s_addr, nseg_netmark.s_addr, check_addr.s_addr);

    
    printf("===> func: %s, line: %d, ----:%x\n", __func__, __LINE__, nseg_netmark.s_addr & check_addr.s_addr);
    printf("===> func: %s, line: %d, ----:%x\n", __func__, __LINE__, nseg_netmark.s_addr & nseg_addr.s_addr);
    printf("===> func: %s, line: %d, --|--:%x\n", __func__, __LINE__, (nseg_netmark.s_addr & check_addr.s_addr) | nseg_addr.s_addr);
    printf("===> func: %s, line: %d, --^--:%x\n", __func__, __LINE__, nseg_netmark.s_addr & check_addr.s_addr ^ nseg_addr.s_addr);

    printf("===> func: %s, line: %d, nseg_addr.s_addr:%x\n", __func__, __LINE__, nseg_addr.s_addr);

    if (!(nseg_addr.s_addr & nseg_netmark.s_addr) ||
            (nseg_netmark.s_addr & check_addr.s_addr) ==  nseg_addr.s_addr) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, const char *argv[])
{
    int ret = 0;

    ret = is_ip_valid("192.168.1.2");
    printf("===> func: %s, line: %d, ret=%d\n", __func__, __LINE__, ret);
    
    ret = is_ip_valid("192.168.2.2");
    printf("===> func: %s, line: %d, ret=%d\n", __func__, __LINE__, ret);

    return 0;
}
