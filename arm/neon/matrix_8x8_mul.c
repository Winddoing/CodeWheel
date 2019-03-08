/*#############################################################
 *     File Name    : matrix_mul.c
 *     Author       : winddoing
 *     Created Time : 2019年02月25日 星期一 17时57分38秒
 *     Description  : 矩阵乘法 aa[i][j]*bb[i][j]
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

#if __aarch64__
#include <arm_neon.h>
#endif

static void dump(uint16_t **x)
{
    int i, j;
    uint16_t *xx = (uint16_t *)x;

    printf("%s:\n", __func__);

    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            printf("%3d ", *(xx + (i << 3) + j));
        }

        printf("\n");
    }
}

static void matrix_mul_c(uint16_t aa[][8], uint16_t bb[][8], uint16_t cc[][8])
{
    int i = 0, j = 0;

    printf("===> func: %s, line: %d\n", __func__, __LINE__);

    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            cc[i][j] = aa[i][j] * bb[i][j];
        }
    }

}

#if __aarch64__
static void matrix_mul_neon(uint16_t **aa, uint16_t **bb, uint16_t **cc)
{
    printf("===> func: %s, line: %d\n", __func__, __LINE__);
#if 1
    uint16_t (*a)[8] = (uint16_t (*)[8])aa;
    uint16_t (*b)[8] = (uint16_t (*)[8])bb;
    uint16_t (*c)[8] = (uint16_t (*)[8])cc;

    printf("aaaaaaaa\n");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    uint16x8_t _aa0 = vld1q_u16((uint16_t*)a[0]);
    uint16x8_t _aa1 = vld1q_u16((uint16_t*)a[1]);
    uint16x8_t _aa2 = vld1q_u16((uint16_t*)a[2]);
    uint16x8_t _aa3 = vld1q_u16((uint16_t*)a[3]);
    uint16x8_t _aa4 = vld1q_u16((uint16_t*)a[4]);
    uint16x8_t _aa5 = vld1q_u16((uint16_t*)a[5]);
    uint16x8_t _aa6 = vld1q_u16((uint16_t*)a[6]);
    uint16x8_t _aa7 = vld1q_u16((uint16_t*)a[7]);

    uint16x8_t _bb0 = vld1q_u16((uint16_t*)b[0]);
    uint16x8_t _bb1 = vld1q_u16((uint16_t*)b[1]);
    uint16x8_t _bb2 = vld1q_u16((uint16_t*)b[2]);
    uint16x8_t _bb3 = vld1q_u16((uint16_t*)b[3]);
    uint16x8_t _bb4 = vld1q_u16((uint16_t*)b[4]);
    uint16x8_t _bb5 = vld1q_u16((uint16_t*)b[5]);
    uint16x8_t _bb6 = vld1q_u16((uint16_t*)b[6]);
    uint16x8_t _bb7 = vld1q_u16((uint16_t*)b[7]);

    uint16x8_t _cc0 = vmulq_u16(_aa0, _bb0);
    uint16x8_t _cc1 = vmulq_u16(_aa1, _bb1);
    uint16x8_t _cc2 = vmulq_u16(_aa2, _bb2);
    uint16x8_t _cc3 = vmulq_u16(_aa3, _bb3);
    uint16x8_t _cc4 = vmulq_u16(_aa4, _bb4);
    uint16x8_t _cc5 = vmulq_u16(_aa5, _bb5);
    uint16x8_t _cc6 = vmulq_u16(_aa6, _bb6);
    uint16x8_t _cc7 = vmulq_u16(_aa7, _bb7);

    vst1q_u16((uint16_t*)c[0], _cc0);
    vst1q_u16((uint16_t*)c[1], _cc1);
    vst1q_u16((uint16_t*)c[2], _cc2);
    vst1q_u16((uint16_t*)c[3], _cc3);
    vst1q_u16((uint16_t*)c[4], _cc4);
    vst1q_u16((uint16_t*)c[5], _cc5);
    vst1q_u16((uint16_t*)c[6], _cc6);
    vst1q_u16((uint16_t*)c[7], _cc7);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
#else
    printf("bbbbbbbb\n");
    int i = 0;
    uint16x8_t _aa[8], _bb[8], _cc[8];
    uint16_t *a = (uint16_t*)aa;
    uint16_t *b = (uint16_t*)bb;
    uint16_t *c = (uint16_t*)cc;

    for(i = 0; i < 8; i++) {
        _aa[i] = vld1q_u16(a + (i << 2));
        _bb[i] = vld1q_u16(b + (i << 2));
        _cc[i] = vmulq_u16(_aa[i], _bb[i]);
        vst1q_u16(c + (i << 2), _cc[i]);
    }

#endif
}

static void matrix_mul_asm(uint16_t **aa, uint16_t **bb, uint16_t **cc)
{
    printf("===> func: %s, line: %d\n", __func__, __LINE__);

    uint16_t *a = (uint16_t*)aa;
    uint16_t *b = (uint16_t*)bb;
    uint16_t *c = (uint16_t*)cc;

#if 0
    asm volatile(
        "ldr q0, [%0, #0]           \n\t"
        "ldr q1, [%0, #16]           \n\t"
        "ldr q2, [%0, #32]          \n\t"
        "ldr q3, [%0, #48]          \n\t"
        "ldr q4, [%0, #64]          \n\t"
        "ldr q5, [%0, #80]          \n\t"
        "ldr q6, [%0, #96]          \n\t"
        "ldr q7, [%0, #112]          \n\t"

        "ldr q8,  [%1, #0]           \n\t"
        "ldr q9,  [%1, #16]           \n\t"
        "ldr q10, [%1, #32]          \n\t"
        "ldr q11, [%1, #48]          \n\t"
        "ldr q12, [%1, #64]          \n\t"
        "ldr q13, [%1, #80]          \n\t"
        "ldr q14, [%1, #96]          \n\t"
        "ldr q15, [%1, #112]          \n\t"

        "mul v0.8h, v0.8h, v8.8h    \n\t"
        "mul v1.8h, v1.8h, v9.8h    \n\t"
        "mul v2.8h, v2.8h, v10.8h    \n\t"
        "mul v3.8h, v3.8h, v11.8h    \n\t"
        "mul v4.8h, v4.8h, v12.8h    \n\t"
        "mul v5.8h, v5.8h, v13.8h    \n\t"
        "mul v6.8h, v6.8h, v14.8h    \n\t"
        "mul v7.8h, v7.8h, v15.8h    \n\t"

        "str q0, [%2, #0]            \n\t"
        "str q1, [%2, #16]            \n\t"
        "str q2, [%2, #32]           \n\t"
        "str q3, [%2, #48]           \n\t"
        "str q4, [%2, #64]           \n\t"
        "str q5, [%2, #80]           \n\t"
        "str q6, [%2, #96]           \n\t"
        "str q7, [%2, #112]           \n\t"

        : "+r"(a),   //%0
          "+r"(b),   //%1
          "+r"(c)    //%2
        :
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
        "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
    );
#else
    // test, OK
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm volatile(
        "ld4 {v0.8h, v1.8h, v2.8h, v3.8h}, [%0]     \n\t"
        "ld4 {v8.8h, v9.8h, v10.8h, v11.8h}, [%1]   \n\t"

        "mul v0.8h, v0.8h, v8.8h                    \n\t"
        "mul v1.8h, v1.8h, v9.8h                    \n\t"
        "mul v2.8h, v2.8h, v10.8h                   \n\t"
        "mul v3.8h, v3.8h, v11.8h                   \n\t"

        "st4 {v0.8h, v1.8h, v2.8h, v3.8h}, [%2]     \n\t"


        "add x1, %0, #64                            \n\t"
        "add x2, %1, #64                            \n\t"
        "add x3, %2, #64                            \n\t"


        "ld4 {v4.8h, v5.8h, v6.8h, v7.8h}, [x1]     \n\t"
        "ld4 {v12.8h, v13.8h, v14.8h, v15.8h}, [x2] \n\t"

        "mul v4.8h, v4.8h, v12.8h                   \n\t"
        "mul v5.8h, v5.8h, v13.8h                   \n\t"
        "mul v6.8h, v6.8h, v14.8h                   \n\t"
        "mul v7.8h, v7.8h, v15.8h                   \n\t"

        "st4 {v4.8h, v5.8h, v6.8h, v7.8h}, [x3]     \n\t"

        : "+r"(a),   //%0
          "+r"(b),   //%1
          "+r"(c)    //%2
        :
        : "cc", "memory", "x1", "x2", "x3", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
            "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
    );
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm ("nop");
#endif
}
#endif


int main(int argc, const char *argv[])
{
    uint16_t aa[8][8] = {
        { 1,  2,  3,  4,  3,  5,  6, 1 },
        { 5,  6,  7,  8, 11,  3,  4, 12},
        { 3,  6,  8,  1,  9,  2,  6, 2 },
        { 2,  6,  7,  1, 12, 22,  7, 1 },
        { 4,  2,  1, 11,  6,  9,  4, 10},
        { 1,  2,  3,  4,  5,  6,  7, 8 },
        {11, 22, 33, 44, 55, 66, 77, 88},
        { 8,  7,  6,  5,  4,  3,  2, 1 }
    };

    uint16_t bb[8][8] = {
        { 1,  3,  5,  7, 11, 22, 33, 44},
        { 2,  4,  6,  8,  4,  5,  1, 6 },
        { 2,  5,  7,  9, 12,  8,  3, 9 },
        { 5,  2,  7,  1, 10,  9,  2, 4 },
        {11, 22, 33, 44, 55, 66, 77, 88},
        { 3,  6,  9,  4,  8,  1, 11, 5 },
        { 1,  2,  3,  4,  5,  6,  7, 8 },
        { 8,  7,  6,  5,  4,  3,  2, 1 }
    };

    uint16_t cc[8][8] = {0};
    int i, j;
    struct timeval tv;
    long long start_us = 0, end_us = 0;

    dump((uint16_t **)aa);
    dump((uint16_t **)bb);
    dump((uint16_t **)cc);

    /* ******** C **********/
    gettimeofday(&tv, NULL);
    start_us = tv.tv_sec + tv.tv_usec;

    matrix_mul_c(aa, bb, cc);

    gettimeofday(&tv, NULL);
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] C time %lld us\n", end_us - start_us);
    dump((uint16_t **)cc);

#if __aarch64__
    /* ******** NEON **********/
    memset(cc, 0, sizeof(uint16_t) * 8 * 8);
    gettimeofday(&tv, NULL);
    start_us = tv.tv_sec + tv.tv_usec;

    matrix_mul_neon((uint16_t **)aa, (uint16_t **)bb, (uint16_t **)cc);

    gettimeofday(&tv, NULL);
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] neon time %lld us\n", end_us - start_us);
    dump((uint16_t **)cc);

    /* ******** asm **********/
    memset(cc, 0, sizeof(uint16_t) * 8 * 8);
    gettimeofday(&tv, NULL);
    start_us = tv.tv_sec + tv.tv_usec;

    matrix_mul_asm((uint16_t **)aa, (uint16_t **)bb, (uint16_t **)cc);

    gettimeofday(&tv, NULL);
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] asm time %lld us\n", end_us - start_us);
    dump((uint16_t **)cc);
#endif

    return 0;
}
