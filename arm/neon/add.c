/*#############################################################
 *     File Name	: add.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月25日 星期一 16时43分09秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#if __aarch64__
#include<arm_neon.h>
#endif

static void add_int_c(int* dst, int* src1, int* src2, int count)
{
    int i;
    for (i = 0; i < count; i++)
        dst[i] = src1[i] + src2[i];
}

static void dump(int *aa)
{
    int i = 0;

    printf("%s: ", __func__);
    for(i = 0; i < 10; i++) {
        printf("%d ", aa[i]);
    }
    printf("\n");
}

#if __aarch64__
static void add_int_neon(int* dst, int* src1, int* src2, int count)
{
  int i;
  for (i = 0; i < count; i += 4) {
      int32x4_t in1, in2, out;
      in1 = vld1q_s32(src1);
      src1 += 4;
      in2 = vld1q_s32(src2);
      src2 += 4;
      out = vaddq_s32(in1, in2);
      vst1q_s32(dst, out);
      dst += 4;
    }
}
#endif

int main(int argc, const char *argv[])
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int b[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int c[10] = {0};

    dump(c);
    add_int_c(c, a, b, 10);
    dump(c);

#if __aarch64__
    memset(c, 0, sizeof(int) * 10);
    dump(c);
    add_int_neon(c, a, b, 10);
    dump(c);
#endif



    return 0;
}
