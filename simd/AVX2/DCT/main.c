/*#############################################################
 *     File Name	: main.c
 *     Author		: winddoing
 *     Created Time	: 2019年04月30日 星期二 14时32分54秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "jpeg_DCT.h"

extern void jsimd_fdct_islow_avx2(uint8_t *data); 

static void dump_uint8(uint8_t *data)
{
	int i = 0;

	for (i = 0; i < 64; i++) {
		printf("%5d ", data[i]);
		if (((i+1) % 8) == 0)
			printf("\n");
	}
	printf("\n");
}

static void dump_int16(int16_t *data)
{
	int i = 0;

	for (i = 0; i < 64; i++) {
		printf("%5d ", data[i]);
		if (((i+1) % 8) == 0)
			printf("\n");
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	/* 测试整体编译通过为正常 */
	int i =0;
	uint8_t data[64] = {0};
	int16_t out[64] = {0};
	int16_t dct_dst[64] = {0};

	for (i = 0; i < 64; i++) {
		data[i] = i;
	}
	dump_uint8(data);

	fastDCT_x86(data, out);

	//jsimd_fdct_islow_avx2(data);
	//_jsimd_fdct_islow_avx2(data);
	//dump_uint8(data);

	dump_int16(out);

	fastIDCTInit(out, dct_dst);  
	fastIDCT(dct_dst); 

	dump_int16(dct_dst);
	
	return 0;
}
