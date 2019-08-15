/*#############################################################
 *     File Name    : aaa.c
 *     Author       : winddoing
 *     Created Time : 2019年02月27日 星期三 11时23分46秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define  DC_VAL_SIZE 12
#define  AC_VAL_SIZE 162

struct DCVar {
    int EHUFCO_DC[12]; //code
    int EHUFSI_DC[12]; //size
    int VALPTR_DC[16];
    int MINCODE_DC[16];
    int MAXCODE_DC[16];
};


struct ACVar {
    int EHUFCO_AC[251];
    int EHUFSI_AC[251];
    int VALPTR_AC[16];
    int MINCODE_AC[16];
    int MAXCODE_AC[16];
};


/*
 * This is the Huffman table specification for the example DC luminance table
 * given in Annex K of the JPEG standard.
 *
 * bits_dc is the list of the number of codewords for each codeword length,
 *   starting at length 1 and going to length 16.
 *
 * huffval_dc is the list of categories, which is the alphabet of symbols
 *   for the Huffman coder
 */

static int bits_y_dc[16] =
{0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};

static int huffval_y_dc[12] =
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static int bits_chrom_dc[16] =
{0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
static int huffval_chrom_dc[12] =
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

/*
 * This is the Huffman table specification for the example AC luminance table
 * given in Annex K of the JPEG standard.
 *
 * bits_ac is the list of the number of codewords for each codeword length,
 *   starting at length 1 and going to length 16.
 *
 * huffval_ac is the list of (run,category) pairs that forms the alphabet
 *   of symbols for the Huffman coder.  The most-significant 4 bits
 *   correspond to the run, the least-significant 4 bits correspond to
 *   the category, so that the symbol is formed as (run*16 + category)
 */
static int bits_y_ac[16] =
{0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d};
static int huffval_y_ac[162] = {
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
    0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
    0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
    0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
    0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

static int bits_chrom_ac[16] =
{0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77};
static int huffval_chrom_ac[162] = {
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
    0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
    0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
    0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
    0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
    0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};


static int Generate_size_table(int *BITS, int *HUFFSIZE)
{
    int I, J, K;
    K = 0;
    I = 0;
    J = 1;
    int LASTK;

    while(I < 16) {
        while(J <= BITS[I]) {
            HUFFSIZE[K] = I + 1;
            K++;
            J++;
        }

        I++;
        J = 1;

    }

    HUFFSIZE[K] = 0;
    LASTK = K;
    printf("===> func: %s, line: %d, LASTK=%d\n", __func__, __LINE__, LASTK);
    return (LASTK);

}


static int Generate_code_table(int *HUFFSIZE, int *HUFFCODE)
{
    int K, CODE, SI;
    K = 0;
    CODE = 0;
    SI = HUFFSIZE[0];


Loop1:
    HUFFCODE[K] = CODE;

    CODE++;
    K++;

    if(HUFFSIZE[K] == SI)
        goto Loop1;

    if(HUFFSIZE[K] == 0)
        return (0);

    else {
Loop2:
        CODE = CODE << 1;
        SI++;
    }

    if(HUFFSIZE[K] == SI)
        goto Loop1;

    else
        goto Loop2;
}

static void Order_codes(int *HUFFVAL, int *HUFFCODE, int *HUFFSIZE, int *EHUFCO, int *EHUFSI, int LASTK)
{
    int K, I;
    K = 0;

    while(K < LASTK) {
        I = HUFFVAL[K];
        EHUFCO[I] = HUFFCODE[K];
        EHUFSI[I] = HUFFSIZE[K];

        K++;
    }

    return;
}

static char* itoa(int value, char* result, int base)
{
	// check that the base if valid
	if(base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while(value);

	// Apply negative sign
	if(tmp_value < 0) *ptr++ = '-';

	*ptr-- = '\0';

	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

static void dump(int *table, int n)
{
    int i = 0;

    printf("===> %s: {\n", __func__);

    for(i = 0; i < n; i++) {
        printf("%5d ", table[i]);

        if((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n}\n");
}

static void dump_to_2(int *table, int n)
{
    int i = 0;

    printf("===> %s: {\n", __func__);

    for(i = 0; i < n; i++) {
        char S2[20];                   
        itoa(table[i], S2, 2);

        printf("%5s ", S2);

        if((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("\n}\n");
}

static void InitHuffmanEncode(int bits_dc[16], int bits_ac[16], int huffval_dc[12],
                              int huffval_ac[162], struct DCVar *dc_var, struct ACVar *ac_var)
{
    int HUFFSIZE_DC[DC_VAL_SIZE + 1];
    int HUFFCODE_DC[DC_VAL_SIZE + 1];
    int HUFFSIZE_AC[AC_VAL_SIZE + 1];
    int HUFFCODE_AC[AC_VAL_SIZE + 1];

    int LASTK_DC = Generate_size_table(bits_dc, HUFFSIZE_DC);
    //printf("===> func: %s, line: %d\n", __func__, __LINE__);
    //dump(bits_dc, 16);
    //dump(HUFFSIZE_DC, DC_VAL_SIZE + 1);
    int LASTK_AC = Generate_size_table(bits_ac, HUFFSIZE_AC);
    //printf("===> func: %s, line: %d\n", __func__, __LINE__);
    //dump(bits_ac, 16);
    //dump(HUFFSIZE_AC, AC_VAL_SIZE + 1);

    Generate_code_table(HUFFSIZE_DC, HUFFCODE_DC);
    Generate_code_table(HUFFSIZE_AC, HUFFCODE_AC);

    Order_codes(huffval_dc, HUFFCODE_DC, HUFFSIZE_DC, dc_var->EHUFCO_DC, dc_var->EHUFSI_DC, LASTK_DC);
    Order_codes(huffval_ac, HUFFCODE_AC, HUFFSIZE_AC, ac_var->EHUFCO_AC, ac_var->EHUFSI_AC, LASTK_AC);
}

static void Decoder_tables(int *BITS, int *HUFFCODE, int *VALPTR, int *MINCODE, int *MAXCODE)
{
    int I, J;
    I = 0;
    J = 0;

    for(I = 0; I < 16; I++) {

        if(BITS[I] == 0)
            MAXCODE[I] = (-1);

        else {
            VALPTR[I] = J;
            MINCODE[I] = HUFFCODE[J];
            J = J + BITS[I] - 1;
            MAXCODE[I] = HUFFCODE[J];
            J++;
        }
    }
}

static void InitHuffmanDecode(int bits_dc[16], int bits_ac[16], int huffval_dc[12],
                              int huffval_ac[162], struct DCVar *dc_var, struct ACVar *ac_var)
{
    int HUFFSIZE_DC[DC_VAL_SIZE + 1];
    int HUFFCODE_DC[DC_VAL_SIZE + 1];
    int HUFFSIZE_AC[AC_VAL_SIZE + 1];
    int HUFFCODE_AC[AC_VAL_SIZE + 1];

    int LASTK_DC = Generate_size_table(bits_dc, HUFFSIZE_DC);
    int LASTK_AC = Generate_size_table(bits_ac, HUFFSIZE_AC);

    Generate_code_table(HUFFSIZE_DC, HUFFCODE_DC);
    Generate_code_table(HUFFSIZE_AC, HUFFCODE_AC);

    Order_codes(huffval_dc, HUFFCODE_DC, HUFFSIZE_DC, dc_var->EHUFCO_DC, dc_var->EHUFSI_DC, LASTK_DC);
    Order_codes(huffval_ac, HUFFCODE_AC, HUFFSIZE_AC, ac_var->EHUFCO_AC, ac_var->EHUFSI_AC, LASTK_AC);

    Decoder_tables(bits_dc, HUFFCODE_DC, dc_var->VALPTR_DC, dc_var->MINCODE_DC, dc_var->MAXCODE_DC);
    Decoder_tables(bits_ac, HUFFCODE_AC, ac_var->VALPTR_AC, ac_var->MINCODE_AC, ac_var->MAXCODE_AC);
}

int main(int argc, const char *argv[])
{
    struct DCVar dc_var_y;
    struct ACVar ac_var_y;
    struct DCVar dc_var_uv;
    struct ACVar ac_var_uv;

    memset(&dc_var_y,  0, sizeof(struct DCVar));
    memset(&ac_var_y,  0, sizeof(struct ACVar));
    memset(&dc_var_uv, 0, sizeof(struct DCVar));
    memset(&ac_var_uv, 0, sizeof(struct ACVar));

    InitHuffmanEncode(bits_y_dc, bits_y_ac, huffval_y_dc, huffval_y_ac, &dc_var_y, &ac_var_y);

    InitHuffmanEncode(bits_chrom_dc, bits_chrom_ac, huffval_chrom_dc, huffval_chrom_ac, &dc_var_uv, &ac_var_uv);

    /*
     * 参考： http://www.cnblogs.com/dxs959229640/p/3853790.html
     * */
#if 1
    printf("亮度DC系数:\n");
    printf("===> size, EHUFSI_DC:\n");
    dump(dc_var_y.EHUFSI_DC, 12);
    dump_to_2(dc_var_y.EHUFSI_DC, 12);

    printf("===> code, EHUFCO_DC:\n");
    dump(dc_var_y.EHUFCO_DC, 12);
    dump_to_2(dc_var_y.EHUFCO_DC, 12);

    printf("色差DC系数:\n");
    printf("===> size, EHUFSI_DC:\n");
    dump(dc_var_uv.EHUFSI_DC, 12);
    dump_to_2(dc_var_uv.EHUFSI_DC, 12);

    printf("===> code, EHUFCO_DC:\n");
    dump(dc_var_uv.EHUFCO_DC, 12);
    dump_to_2(dc_var_uv.EHUFCO_DC, 12);

    printf("===> code, MINCODE_DC:\n");
    dump(dc_var_uv.MINCODE_DC, 16);
    dump_to_2(dc_var_uv.MINCODE_DC, 16);

    printf("===> code, MAXCODE_DC:\n");
    dump(dc_var_uv.MAXCODE_DC, 16);
    dump_to_2(dc_var_uv.MAXCODE_DC, 16);
#endif

#if 1
    printf("亮度AC系数:\n");
    printf("===> size, EHUFSI_AC:\n");
    dump(ac_var_y.EHUFSI_AC, 251);

    printf("===> code, EHUFCO_AC:\n");
    dump(ac_var_y.EHUFCO_AC, 251);
#endif

    return 0;
}
