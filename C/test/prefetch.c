/*#############################################################
 *     File Name	: prefetch.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月25日 星期一 10时01分40秒
 *     Description	: 矩阵乘法
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#if __aarch64__
  #define nop asm("nop")
#else
  #define nop 
#endif

int main(int argc, const char *argv[])
{
    unsigned long i, j, k;
    int N = 700;
    int res[N][N], mul1[N][N], mul2[N][N];
    clock_t start, end;
    long time1 = 0, time2 = 0;
    
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            mul1[i][j] = (i + 1) * j;
            mul2[i][j] = i * j;
        }
    }

    start = clock();
    /* mul2的地址空间不是连续的
     * 初始化时mul2[0][x], 一行一行赋值，地址连续
     * 执行读取mul2[x][0], 一列一列读取，地址不连续*/
    nop;nop;nop;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                // 行 x 列
                res[i][j] += mul1[i][k] * mul2[k][j];
            }
        }
    }
    nop;nop;nop;
    end = clock();
    time1 = end - start;
    printf("Run Time1 %f s\n", (double)time1 / CLOCKS_PER_SEC);

    int tmp[N][N];
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            mul1[i][j] = (i + 1) * j;
            mul2[i][j] = i * j;
        }
    }

    start = clock();
    // 矩阵转换，列变换（列变行）
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            tmp[i][j] = mul2[j][i];
        }
    }
    /* CPU读取连续的tmp地址时，使用指令预取（硬件）*/
    nop;nop;nop;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                res[i][j] += mul1[i][k] * tmp[j][k];
            }
        }
    }
    nop;nop;nop;
    end = clock();
    time2 = end - start;
    printf("Run Time2 %f s\n", (double)time2 / CLOCKS_PER_SEC);

    printf("Time2 and Time1 upgrade %f %\n", (double)(time1 - time2) / time1 * 100);

    return 0;
}
