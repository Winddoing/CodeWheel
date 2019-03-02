/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月26日 星期二 11时17分34秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

int x_add_1(int x)
{
    int ret = 0;

    asm volatile("add %0, %1, 1" : "=r" (ret) : "r" (x));

    return ret;
}

int add(int a, int b)
{
    int ret = 0;

    asm volatile(
            "add %0, %1, %2"
            
            : "=r" (ret)
            : "r" (a), "r" (b)
            );

    return ret;
}

/*
 * ://输出
 *
 * ://输入
 *
 * ://用到的寄存器
 *
 * +r 表示读写，=r 只可写
 * */

int main(int argc, const char *argv[])
{
    int x = 3;

    printf("Test Add %d+1=%d\n", x, x_add_1(x));

    printf("Test Add 2+3=%d\n", add(2, 3));
    
    return 0;
}
