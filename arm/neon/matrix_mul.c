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

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            printf("%3d ", *(xx + (i << 2) + j));
        }

        printf("\n");
    }
}

static void matrix_mul_c(uint16_t aa[][4], uint16_t bb[][4], uint16_t cc[][4])
{
    int i = 0, j = 0;

    printf("===> func: %s, line: %d\n", __func__, __LINE__);

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            cc[i][j] = aa[i][j] * bb[i][j];
        }
    }

}

#if __aarch64__
static void matrix_mul_neon(uint16_t **aa, uint16_t **bb, uint16_t **cc)
{
    printf("===> func: %s, line: %d\n", __func__, __LINE__);
#if 1
    uint16_t (*a)[4] = (uint16_t (*)[4])aa;
    uint16_t (*b)[4] = (uint16_t (*)[4])bb;
    uint16_t (*c)[4] = (uint16_t (*)[4])cc;

    printf("aaaaaaaa\n");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    uint16x4_t _cc0;
    uint16x4_t _cc1;
    uint16x4_t _cc2;
    uint16x4_t _cc3;

    uint16x4_t _aa0 = vld1_u16((uint16_t*)a[0]);
    uint16x4_t _aa1 = vld1_u16((uint16_t*)a[1]);
    uint16x4_t _aa2 = vld1_u16((uint16_t*)a[2]);
    uint16x4_t _aa3 = vld1_u16((uint16_t*)a[3]);

    uint16x4_t _bb0 = vld1_u16((uint16_t*)b[0]);
    uint16x4_t _bb1 = vld1_u16((uint16_t*)b[1]);
    uint16x4_t _bb2 = vld1_u16((uint16_t*)b[2]);
    uint16x4_t _bb3 = vld1_u16((uint16_t*)b[3]);

    _cc0 = vmul_u16(_aa0, _bb0);
    _cc1 = vmul_u16(_aa1, _bb1);
    _cc2 = vmul_u16(_aa2, _bb2);
    _cc3 = vmul_u16(_aa3, _bb3);

    vst1_u16((uint16_t*)c[0], _cc0);
    vst1_u16((uint16_t*)c[1], _cc1);
    vst1_u16((uint16_t*)c[2], _cc2);
    vst1_u16((uint16_t*)c[3], _cc3);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
#else
    printf("bbbbbbbb\n");
    int i = 0;
    uint16x4_t _aa[4], _bb[4], _cc[4];
    uint16_t *a = (uint16_t*)aa;
    uint16_t *b = (uint16_t*)bb;
    uint16_t *c = (uint16_t*)cc;

    for(i = 0; i < 4; i++) {
        _aa[i] = vld1_u16(a + (i << 2));
        _bb[i] = vld1_u16(b + (i << 2));
        _cc[i] = vmul_u16(_aa[i], _bb[i]);
        vst1_u16(c + (i << 2), _cc[i]);
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
    asm(
        "ldr d3, [%0, #0]           \n\t"
        "ldr d2, [%0, #8]           \n\t"
        "ldr d1, [%0, #16]          \n\t"
        "ldr d0, [%0, #24]          \n\t"

        "ldr d7, [%1, #0]           \n\t"
        "ldr d6, [%1, #8]           \n\t"
        "ldr d5, [%1, #16]          \n\t"
        "ldr d4, [%1, #24]          \n\t"

        "mul v3.4h, v3.4h, v7.4h    \n\t"
        "mul v2.4h, v2.4h, v6.4h    \n\t"
        "mul v1.4h, v1.4h, v5.4h    \n\t"
        "mul v0.4h, v0.4h, v4.4h    \n\t"

        //"add v3.4h, v3.4h, v7.4h    \n\t"
        //"add v2.4h, v2.4h, v6.4h    \n\t"
        //"add v1.4h, v1.4h, v5.4h    \n\t"
        //"add v0.4h, v0.4h, v4.4h    \n\t"

        "str d3, [%2,#0]            \n\t"
        "str d2, [%2,#8]            \n\t"
        "str d1, [%2,#16]           \n\t"
        "str d0, [%2,#24]           \n\t"

        : "+r"(a),   //%0
          "+r"(b),   //%1
          "+r"(c)    //%2
        :
        : "cc", "memory", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7"
    );
#else
    // test, OK
    asm(
        "ld4 {v0.4h, v1.4h, v2.4h, v3.4h}, [%0] \n"
        "ld4 {v4.4h, v5.4h, v6.4h, v7.4h}, [%1] \n"

        "mul v3.4h, v3.4h, v7.4h    \n\t"
        "mul v2.4h, v2.4h, v6.4h    \n\t"
        "mul v1.4h, v1.4h, v5.4h    \n\t"
        "mul v0.4h, v0.4h, v4.4h    \n\t"

        "st4 {v0.4h, v1.4h, v2.4h, v3.4h}, [%2] \n"

        : "+r"(a),   //%0
          "+r"(b),   //%1
          "+r"(c)    //%2
        :
        : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7"
    );
#endif
}
#endif

int main(int argc, const char *argv[])
{
    uint16_t aa[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {3, 6, 8, 1},
        {2, 6, 7, 1}
    };

    uint16_t bb[4][4] = {
        {1, 3, 5, 7},
        {2, 4, 6, 8},
        {2, 5, 7, 9},
        {5, 2, 7, 1}
    };

    uint16_t cc[4][4] = {0};
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
    memset(cc, 0, sizeof(uint16_t) * 4 * 4);
    gettimeofday(&tv, NULL);
    start_us = tv.tv_sec + tv.tv_usec;

    matrix_mul_neon((uint16_t **)aa, (uint16_t **)bb, (uint16_t **)cc);

    gettimeofday(&tv, NULL);
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] neon time %lld us\n", end_us - start_us);
    dump((uint16_t **)cc);

    /* ******** asm **********/
    memset(cc, 0, sizeof(uint16_t) * 4 * 4);
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
