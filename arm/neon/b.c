/*#############################################################
 *     File Name	: b.c
 *     Author		: winddoing
 *     Created Time	: 2019年03月01日 星期五 15时12分21秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

float SumSquareError_C(const float* src_a, const float* src_b, int count)
{
    float sse = 0u;
    int i;
    for (i = 0; i < count; ++i) {
        float diff = src_a[i] - src_b[i];
        sse += (float)(diff * diff);
    }
    return sse;
}

float SumSquareError_NEON1(const float* src_a, const float* src_b, int count)
{
    float sse;
    asm volatile (
            // Clear q8, q9, q10, q11
            "veor    q8, q8, q8                      \n"
            "veor    q9, q9, q9                      \n"
            "veor    q10, q10, q10                   \n"
            "veor    q11, q11, q11                   \n"
          "1:                                        \n"

/*            "vld1.32     {q0, q1}, [%[src_a]]!       \n"*/
/*            "vld1.32     {q2, q3}, [%[src_a]]!       \n"*/
/*            "vld1.32     {q12, q13}, [%[src_b]]!     \n"*/
/*            "vld1.32     {q14, q15}, [%[src_b]]!     \n"*/
            "vld1.32     {q0, q1}, [%0]!       \n"
            "vld1.32     {q2, q3}, [%0]]!       \n"
            "vld1.32     {q12, q13}, [%1]]!     \n"
            "vld1.32     {q14, q15}, [%1]!     \n"

/*            "subs %[count], %[count], #16            \n"*/
            "subs %2, %2, #16            \n"

            // q0, q1, q2, q3 are the destination of vsub.
            // they are also the source of vmla.

            "vsub.f32 q0, q0, q12                    \n"
            "vmla.f32   q8, q0, q0                   \n"
            "vsub.f32   q1, q1, q13                  \n"
            "vmla.f32   q9, q1, q1                   \n"
            "vsub.f32   q2, q2, q14                  \n"
            "vmla.f32   q10, q2, q2                  \n"
            "vsub.f32   q3, q3, q15                  \n"
            "vmla.f32   q11, q3, q3                  \n"
            "bgt        1b                           \n"
            "vadd.f32   q8, q8, q9                   \n"
            "vadd.f32   q10, q10, q11                \n"
            "vadd.f32   q11, q8, q10                 \n"
            "vpadd.f32  d2, d22, d23                 \n"
            "vpadd.f32  d0, d2, d2                   \n"
            "vmov.32    %3, d0[0]                    \n"

            : "=r" (sse)
            : "r"(src_a), "r"(src_b), "r"(count)
            : "cc", "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13","q14", "q15"
                );
    return sse;
}

int main(int argc, const char *argv[])
{
    float aa[4] = {1.2, 1.0, 3.2, 4};
    float bb[4] = {3.3, 2.1, 2.2, 1.4};
    float cc = 0u;

    cc = SumSquareError_C(aa, bb, 4);
    printf("===> func: %s, line: %d, cc=%f\n", __func__, __LINE__, cc);

    cc = SumSquareError_NEON1(aa, bb, 4);
    printf("===> func: %s, line: %d, cc=%f\n", __func__, __LINE__, cc);

    return 0;
}
