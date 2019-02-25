/*#############################################################
 *     File Name	: matrix_mul.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月25日 星期一 17时57分38秒
 *     Description	: 矩阵乘法 aa[i][j]*bb[i][j]
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if __aarch64__
#include <arm_neon.h>
#endif

static void dump(uint16_t *x)
{
    int i, j;

    printf("%s:\n", __func__);
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            printf("%3d ", *(x + (i << 2) + j));
        }
        printf("\n");
    }
}

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

    dump(aa);
    dump(bb);

    gettimeofday(&tv,NULL);                           
    start_us = tv.tv_sec + tv.tv_usec;    
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            cc[i][j] = aa[i][j] * bb[i][j];
        }
    }
    gettimeofday(&tv,NULL);                           
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] time %lld us\n", end_us - start_us);
    dump(cc);

#if __aarch64__
    gettimeofday(&tv,NULL);                           
    start_us = tv.tv_sec + tv.tv_usec;    
#if 1
    printf("aaaaaaaa\n");
    uint16x4_t _cc0;
    uint16x4_t _cc1;
    uint16x4_t _cc2;
    uint16x4_t _cc3;

    uint16x4_t _aa0 = vld1_u16(aa);
    uint16x4_t _aa1 = vld1_u16(aa + (1 << 2));
    uint16x4_t _aa2 = vld1_u16(aa + (2 << 2));
    uint16x4_t _aa3 = vld1_u16(aa + (3 << 2));

    uint16x4_t _bb0 = vld1_u16(bb);
    uint16x4_t _bb1 = vld1_u16(bb + (1 << 2));
    uint16x4_t _bb2 = vld1_u16(bb + (2 << 2));
    uint16x4_t _bb3 = vld1_u16(bb + (3 << 2));

    _cc0 = vmul_u16(_aa0, _bb0);
    _cc1 = vmul_u16(_aa1, _bb1);
    _cc2 = vmul_u16(_aa2, _bb2);
    _cc3 = vmul_u16(_aa3, _bb3);

    vst1_u16(cc, _cc0);
    vst1_u16(cc + (1 << 2), _cc0);
    vst1_u16(cc + (2 << 2), _cc0);
    vst1_u16(cc + (3 << 2), _cc0);
#else
    printf("bbbbbbbb\n");
    uint16x4_t _aa[4], _bb[4], _cc[4];

    for(i = 0; i < 4; i++) {
        _aa[i] = vld1_u16(aa + (i << 2));
        _bb[i] = vld1_u16(bb + (i << 2));
        _cc[i] = vmul_u16(_aa[i], _bb[i]);
        vst1_u16(cc + (1 << 2), _cc[i]);
    }
#endif

    gettimeofday(&tv,NULL);                           
    end_us = tv.tv_sec + tv.tv_usec;
    printf("aa[][]*bb[][] neon time %lld us\n", end_us - start_us);
    dump(cc);
#endif


    return 0;
}
