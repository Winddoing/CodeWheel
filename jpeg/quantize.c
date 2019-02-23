/*#############################################################
 *     File Name    : quantize.c
 *     Author       : winddoing
 *     Created Time : 2019年02月19日 星期二 14时17分05秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 亮度分量量化表
static int quant_y[8][8] = {
    {16, 11, 10, 16, 24,  40,  51,  61},
    {12, 12, 14, 19, 26,  58,  60,  55},
    {14, 13, 16, 24, 40,  57,  69,  56},
    {14, 17, 22, 29, 51,  87,  80,  62},
    {18, 22, 37, 56, 68,  109, 103, 77},
    {24, 35, 55, 64, 81,  104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

// 色度分量量化表
static int quant_uv[8][8] = {
    {17, 18, 24, 47, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
};

// 生成带系数的量化表和反量化表
static void genReciprocalTable(int *pQuant, int dQF, int *pwQantTable, int *pwInvQuantTable)
{
    int row, col;
    unsigned char ucQuant;
    float Mquant = 0;
    double a = 0;

    for(row = 0; row < 8; row++) {
        for(col = 0; col < 8; col++) {
            ucQuant = *(pQuant + (row << 3) + col);

            if((dQF >= 1) && (dQF < 50))
                a = 50.0 / dQF;

            if((dQF >= 50) && (dQF <= 100))
                a = 2 - 2.0 * dQF / 100;

            if(a * ucQuant < 1)
                Mquant = 1;

            if((a * ucQuant >= 1) && (a * ucQuant <= 255))
                Mquant = round(a * ucQuant);

            if(a * ucQuant > 255)
                Mquant = 255;

            *(pwInvQuantTable + (row << 3) + col) = Mquant;
            *(pwQantTable + (row << 3) + col) = (1 << 16) / Mquant;
        }
    }
}

static void initialQuantizeTable(int quantize_type, int dQF, int *pwQantTable, int *pwInvQuantTable)
{
    if(quantize_type == 0) {
        genReciprocalTable((int *) quant_y, dQF, pwQantTable, pwInvQuantTable);

    } else {
        genReciprocalTable((int *) quant_uv, dQF, pwQantTable, pwInvQuantTable);
    }
}

static void dump_table(int *table)
{
    int i = 0;

    printf("===> %s:\n", __func__);
    for(i = 0; i < 64; i++) {
        printf("%5d ", table[i]);

        if((i + 1) % 8 == 0)
            printf("\n");
    }

}

int main(int argc, const char *argv[])
{
    //Y 分量DC 量化表
    int y_quantization_table[64];
    int y_in_quantization_table[64];
    //UV 分量DC 量化表
    int uv_quantization_table[64];
    int uv_in_quantization_table[64];

    initialQuantizeTable(0, 75, y_quantization_table, y_in_quantization_table);

    dump_table(y_quantization_table);
    dump_table(y_in_quantization_table);

    // round(quant_uv * 0.5)
    initialQuantizeTable(1, 75, uv_quantization_table, uv_in_quantization_table);

    dump_table(uv_quantization_table);
    dump_table(uv_in_quantization_table);

    return 0;
}
