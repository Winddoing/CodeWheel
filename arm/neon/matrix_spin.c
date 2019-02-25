/*#############################################################
 *     File Name	: matrix_spin.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月25日 星期一 17时01分59秒
 *     Description	:  矩阵旋转90度
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>


static uint8_t aa[4][4] = {
    {'A', 'B', 'C', 'D'},
    {'E', 'F', 'G', 'H'},
    {'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P'}
};

static void dump(uint8_t *a)
{
    int i = 0, j = 0;
    printf("%s: \n", __func__);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%c ", *(a + (i << 2) + j));
        }
        printf("\n");
    }
}

int main(int argc, const char *argv[])
{
    int i, j;
    uint8_t bb[4][4] = {0};

    dump(aa);
    for (i = 0; i < 4; i++) {
        for (j = 3; j >= 0; j--) {
            bb[i][3 - j] = aa[j][i];
        }
    }
    dump(bb);
    
    return 0;
}
