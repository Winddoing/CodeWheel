/*#############################################################
 *     File Name    : zigzag.c
 *     Author       : winddoing
 *     Created Time : 2019年02月18日 星期一 14时02分53秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _node {
    int value;
    int index;
} Node;

//冒泡排序
int BubbleSort(Node *pnData, int nLen)
{
    bool isOk = false;

    for(int i = 0; i < nLen - 1 && !isOk; ++i) {
        isOk = true;

        for(int j = nLen - 1; j > i; --j) {
            if(pnData[j].index < pnData[j - 1].index) {
                int nTemp = pnData[j].index;
                int nData = pnData[j].value;
                pnData[j].index = pnData[j - 1].index;
                pnData[j].value = pnData[j - 1].value;
                pnData[j - 1].index = nTemp;
                pnData[j - 1].value = nData;
                isOk = false;
            }
        }
    }

    return 1;
}

int scan[64] = {
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};


int main(int argc, const char *argv[])
{
    int arr[64] = {0};

    for(int i = 0; i < 64; i++) {
        arr[i] = rand() % 100;
    }

    printf("==>Original array:\n");

    for(int j = 0; j < 64; j++) {
        printf("%5d", arr[j]);

        if((j + 1) % 8 == 0)
            printf("\n");
    }


    Node tmp[64];

    for(int k = 0; k < 64; k++) {
        tmp[k].value = arr[k];
        tmp[k].index = scan[k];
    }

    BubbleSort(tmp, 64);

    printf("==>ZigZag array:\n");

    for(int m = 0; m < 64; m++) {
        printf("%5d", tmp[m].value);

        if((m + 1) % 8 == 0)
            printf("\n");
    }

    return 0;
}

