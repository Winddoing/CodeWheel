/*#############################################################
 *     File Name    : dct1.c
 *     Author       : winddoing
 *     Created Time : 2019年01月31日 星期四 16时35分38秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_X   (8)
#define MAX_Y   (8)

typedef unsigned int   UINT32;
typedef int      INT32;
typedef unsigned short UINT16;
typedef short    INT16;
typedef unsigned char  UINT8;
typedef char     INT8;

INT16 org[MAX_X * MAX_Y] = {
    -172, -172, -172, -173, -175, -170, -158, -131,
    -171, -172, -173, -173, -170, -159, -137, -117,
    -172, -172, -171, -166, -154, -136, -117, -103,
    -172, -170, -164, -152, -133, -115, -98, -94,
    -170, -165, -153, -136, -113, -96, -87, -96,
    -160, -150, -139, -122, -103, -93, -91, -104,
    -145, -134, -125, -115, -107, -102, -104, -114,
    -130, -119, -113, -111, -112, -114, -118, -125,
};

double result[MAX_X * MAX_Y];
double idct_result[MAX_X * MAX_Y];

static void DCT(INT16 * data)
{
    UINT16 i;
    INT32 x0, x1, x2, x3, x4, x5, x6, x7, x8;

    /* All values are shifted left by 10
       and rounded off to nearest integer */
    static const UINT16 c1 = 1420; /* cos PI/16 * root(2)  */
    static const UINT16 c2 = 1338; /* cos PI/8 * root(2)   */
    static const UINT16 c3 = 1204; /* cos 3PI/16 * root(2) */
    static const UINT16 c5 = 805;  /* cos 5PI/16 * root(2) */
    static const UINT16 c6 = 554;  /* cos 3PI/8 * root(2)  */
    static const UINT16 c7 = 283;  /* cos 7PI/16 * root(2) */
    static const UINT16 s1 = 3;
    static const UINT16 s2 = 10;
    static const UINT16 s3 = 13;

    for(i = 8; i > 0; i--) {
        x8 = data[0] + data[7];
        x0 = data[0] - data[7];
        x7 = data[1] + data[6];
        x1 = data[1] - data[6];
        x6 = data[2] + data[5];
        x2 = data[2] - data[5];
        x5 = data[3] + data[4];
        x3 = data[3] - data[4];
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        data[0] = (INT16)(x4 + x5);
        data[4] = (INT16)(x4 - x5);
        data[2] = (INT16)((x8 * c2 + x7 * c6) >> s2);
        data[6] = (INT16)((x8 * c6 - x7 * c2) >> s2);
        data[7] = (INT16)((x0 * c7 - x1 * c5 + x2 * c3 - x3 * c1) >> s2);
        data[5] = (INT16)((x0 * c5 - x1 * c1 + x2 * c7 + x3 * c3) >> s2);
        data[3] = (INT16)((x0 * c3 - x1 * c7 - x2 * c1 - x3 * c5) >> s2);
        data[1] = (INT16)((x0 * c1 + x1 * c3 + x2 * c5 + x3 * c7) >> s2);
        data += 8;
    }

    data -= 64;

    for(i = 8; i > 0; i--) {
        x8 = data[0] + data[56];
        x0 = data[0] - data[56];
        x7 = data[8] + data[48];
        x1 = data[8] - data[48];
        x6 = data[16] + data[40];
        x2 = data[16] - data[40];
        x5 = data[24] + data[32];
        x3 = data[24] - data[32];
        x4 = x8 + x5;
        x8 -= x5;
        x5 = x7 + x6;
        x7 -= x6;
        data[0] = (INT16)((x4 + x5) >> s1);
        data[32] = (INT16)((x4 - x5) >> s1);
        data[16] = (INT16)((x8 * c2 + x7 * c6) >> s3);
        data[48] = (INT16)((x8 * c6 - x7 * c2) >> s3);
        data[56] = (INT16)((x0 * c7 - x1 * c5 + x2 * c3 - x3 * c1) >> s3);
        data[40] = (INT16)((x0 * c5 - x1 * c1 + x2 * c7 + x3 * c3) >> s3);
        data[24] = (INT16)((x0 * c3 - x1 * c7 - x2 * c1 - x3 * c5) >> s3);
        data[8] = (INT16)((x0 * c1 + x1 * c3 + x2 * c5 + x3 * c7) >> s3);
        data++;
    }
}

static double dct(int h, int v)
{
    int x = 0, y = 0;
    double value = 0;

    for(y = 0; y < MAX_Y; y++) {
        for(x = 0; x < MAX_X; x++) {
            double kc = cos((M_PI * v * ((2.0 * y) + 1.0)) / 16.0) * 
                cos((M_PI * h * ((2.0 * x) + 1.0)) / 16.0);
            value += org[(y * 8) + x] *  kc;
        }
    }

    if((h == 1) && (v == 0)) {
        //printf("\n h1 v0 %lf\n", value);
    }

    if(h == 0) {
        value *= 0.5 / sqrt(2.0);

    } else {
        value *= 0.5;
    }

    if(v == 0) {
        value *= 0.5 / sqrt(2.0);

    } else {
        value *= 0.5;
    }

    return value;
}

static double idct(int x, int y)
{
    int h = 0, v = 0;
    double value = 0;
    double c_u = 0;
    double c_v = 0;

    for(h = 0; h < MAX_Y; h++) {
        for(v = 0; v < MAX_X; v++) {
            if(h == 0) {
                c_u = 0.5 / sqrt(2.0);

            } else {
                c_u = 0.5;
            }

            if(v == 0) {
                c_v = 0.5 / sqrt(2.0);

            } else {
                c_v = 0.5;
            }

            value += c_v * c_u * result[(v * MAX_X) + h] * 
                cos((M_PI * v * ((2.0 * y) + 1.0)) / 16.0) * 
                cos((M_PI * h  * ((2.0 * x) + 1.0)) / 16.0);
        }
    }

    return value;
}

static void show(INT16 *org)
{
    int x, y;

    for(y = 0; y < MAX_Y; y++) {
        for(x = 0; x < MAX_X; x++) {
            printf("%d ", org[(y * MAX_X) + x]);
        }
        printf("\n");
    }
    printf("\n");

}

static void DCT_test(INT16 *org)
{
    INT16 *src = (INT16*)calloc(1, sizeof(INT16) * MAX_X * MAX_Y);

    memcpy(src, org, sizeof(INT16) * MAX_X * MAX_Y);

    DCT(src);

    show(src);
}

int main(void)
{
    int x, y, h, v;

    memset(idct_result, 0x0, sizeof(idct_result));

    printf("orginal\n");
    show(org);

    printf("dct result [double]\n");

    for(v = 0; v < MAX_Y; v++) {
        for(h = 0; h < MAX_X; h++) {
            result[(v * MAX_X) + h] = dct(h, v);
            printf("%lf ", result[(v * MAX_X) + h]);
        }

        printf("\n");
    }

    printf("\n");


    printf("idct result\n");
    for(y = 0; y < MAX_Y; y++) {
        for(x = 0; x < MAX_X; x++) {
            idct_result[(y * MAX_X) + x] = idct(x, y);
            printf("%lf ", idct_result[(y * MAX_X) + x]);
        }
        printf("\n");
    }
    printf("\n");


    printf("Test DCT [INT16]:\n");
    DCT_test(org);

    return 0;
}
