/*#############################################################
 *     File Name    : a.c
 *     Author       : winddoing
 *     Created Time : 2019年02月13日 星期三 10时17分09秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <arm_neon.h>

int main(int argc, const char *argv[])
{
    int i;
    uint8x8_t v;        // define v as a vector with 8 lanes of 8-bit data
    unsigned char A[8]; // allocate memory for eight 8-bit data

    asm("nop");
    asm("nop");
    v = vcreate_u8(0x0102030405060708); // create a vector that contains the values
    // 1,2,3,4,5,6,7,8
    asm("nop");
    asm("nop");

    vst1_u8(A, v); // store the vector to memory, in this case, to array A

    for(i = 0; i < 8; i++)
        printf("%d ", A[i]);

    printf("\n");

    return 0;
}
