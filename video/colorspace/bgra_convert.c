/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : bgra2yuv420p.c
 *  Author       : wqshao
 *  Created Time : 2023-01-19 16:35:20
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#define ALLOC_PIXEL_DATA(data, size)	\
	do {	\
		data = (uint8_t*)malloc(size);	\
		if (NULL == data) {	\
			printf("%s malloc failed. size=%d\n", __func__, size);	\
			return;	\
		}	\
		memset(data, 0, size);	\
	} while(0)

#define STORE_PIXEL_DATA(data, size, w, h, type) \
	do {	\
		char file[128];	\
		assert(data != NULL);	\
		sprintf(file, "%s_%dx%d.%s", __func__, w, h, type);	\
		printf("%s save: %s\n", __func__, file);	\
		FILE *fp = fopen(file, "wb+");	\
		if (fp == NULL) {	\
			printf("%s fp NULL.\n", __func__);	\
			return;	\
		}	\
		fwrite(data, 1, size, fp); \
		free(data);	\
		fclose(fp);	\
	} while(0)

static void RGBToYUV_v1(int R, int G, int B, int* Y, int* U, int* V)
{
	*Y = 0.299 * R + 0.587 * G + 0.114 * B;
	*U = -0.147 * R - 0.289 * G + 0.436 * B;
	*V = 0.615 * R - 0.515 * G - 0.100 * B;
}

static void YUVToRGB_v1(int Y, int U, int V, int* R, int* G, int* B)
{
	*R = Y + 1.14 * V;
	*G = Y - 0.39 * U - 0.58 * V;
	*B = Y + 2.03 * U;
}

static void RGBToYUV(int Red, int Green, int Blue, int* Y, int* U, int* V)

{
	*Y = ((Red << 6) + (Red << 3) + (Red << 2) + Red + (Green << 7) + (Green << 4) + (Green << 2) + (Green << 1) + (Blue << 4) + (Blue << 3) + (Blue << 2) + Blue) >> 8;
	*U = (-((Red << 5) + (Red << 2) + (Red << 1)) - ((Green << 6) + (Green << 3) + (Green << 1)) + ((Blue << 6) + (Blue << 5) + (Blue << 4))) >> 8;
	*V = ((Red << 7) + (Red << 4) + (Red << 3) + (Red << 2) + (Red << 1) - ((Green << 7) + (Green << 2)) - ((Blue << 4) + (Blue << 3) + (Blue << 1))) >> 8;
}

static void YUVToRGB(int Y, int U, int V, int* Red, int* Green, int* Blue)
{
	*Red = ((Y << 8) + ((V << 8) + (V << 5) + (V << 2))) >> 8;
	*Green = ((Y << 8) - ((U << 6) + (U << 5) + (U << 2)) - ((V << 7) + (V << 4) + (V << 2) + V)) >> 8;
	*Blue = ((Y << 8) + (U << 9) + (U << 3)) >> 8;
}

/*********************只不包含浮点运算********************/
//如果使用原始的转换出来的话，会出现偏色问题，下面在原始的基础上又做了微调，可以正常显示。
#define RGB2Y(r,g,b) \
    ((uint8_t)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16)
//	(((r << 6) + (r << 3) + (r << 2) + r + (g << 7) + (g << 4) + (g << 2) + (g << 1) + (b << 4) + (b << 3) + (b << 2) + b) >> 8)

#define RGB2U(r,g,b) \
    ((uint8_t)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128)
//	((-((r << 5) + (r << 2) + (r << 1)) - ((g << 6) + (g << 3) + (g << 1)) + ((b << 6) + (b << 5) + (b << 4))) >> 8)

#define RGB2V(r,g,b) \
    ((uint8_t)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128)
//	(((r << 7) + (r << 4) + (r << 3) + (r << 2) + (r << 1) - ((g << 7) + (g << 2)) - ((b << 4) + (b << 3) + (b << 1))) >> 8)

/**********************不包含浮点和乘法运算*****************/
#define RGB2Y_SHIFT(r,g,b) \
    ((uint8_t)((((r << 6) + (r << 3) + (r << 2) + r + (g << 7) + (g << 4) + (g << 2) + (g << 1) + (b << 4) + (b << 3) + (b << 2) + b) + 128) >> 8) + 16)

#define RGB2U_SHIFT(r,g,b) \
    ((uint8_t)(((-((r << 5) + (r << 2) + (r << 1)) - ((g << 6) + (g << 3) + (g << 1)) + ((b << 6) + (b << 5) + (b << 4))) + 128) >> 8) + 128)

#define RGB2V_SHIFT(r,g,b) \
    ((uint8_t)(((r << 7) + (r << 4) + (r << 3) + (r << 2) + (r << 1) - ((g << 7) + (g << 2)) - ((b << 4) + (b << 3) + (b << 1)) + 128) >> 8) + 128)

/*************************包含浮点和乘法运算，没有优化**************/

#define RGB2Y_FLOAT(r,g,b) ((int8_t)(( 0.299 * r) + ( 0.587 * g) + ( 0.114 * b)))
#define RGB2U_FLOAT(r,g,b) ((int8_t)((-0.14713 * r) + (-0.28886 * g) + ( 0.436 * b)))
#define RGB2V_FLOAT(r,g,b) ((int8_t)(( 0.615 * r) + (-0.51499 * g) + (-0.10001 * b)))

static void bgra2yuv420_multip(uint8_t *bgra_buf, uint8_t *yuv_buf, int width, int heigh)
{
	if (!bgra_buf || !yuv_buf || !width || !heigh) {
		printf("invalid param\n");
		return;
	}

	int bgra_bytes_num = 4;

	uint8_t *y = yuv_buf;
	uint8_t *u = yuv_buf + width * heigh;
	uint8_t *v = u + width * heigh / 4;

	for (int r = 0; r < heigh; r++) {
		for (int c = 0; c < width; c++) {
			int index = r * width + c;
			*y++ = RGB2Y(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);

			if ((r % 2 == 0) && (c % 2 == 0)) //偶数行，偶数列获取U
				*u++ = RGB2U(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);

			else if (c % 2 == 0) //奇数行，偶数列获取V
				*v++ = RGB2V(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);
		}
	}
}

static void bgra2yuv420_shift(uint8_t *bgra_buf, uint8_t *yuv_buf, int width, int heigh)
{
	if (!bgra_buf || !yuv_buf || !width || !heigh) {
		printf("invalid param\n");
		return;
	}

	int bgra_bytes_num = 4;

	uint8_t *y = yuv_buf;
	uint8_t *u = yuv_buf + width * heigh;
	uint8_t *v = u + width * heigh / 4;

	for (int r = 0; r < heigh; r++) {
		for (int c = 0; c < width; c++) {
			int index = r * width + c;
			*y++ = RGB2Y(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);

			if ((r % 2 == 0) && (c % 2 == 0)) //偶数行，偶数列获取U
				*u++ = RGB2U(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);

			else if (c % 2 == 0) //奇数行，偶数列获取V
				*v++ = RGB2V(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);
		}
	}
}


static void bgra2yuv420_float(uint8_t *bgra_buf, uint8_t *yuv_buf, int width, int heigh)
{
	if (!bgra_buf || !yuv_buf || !width || !heigh) {
		printf("invalid param\n");
		return;
	}

	int bgra_bytes_num = 4;

	uint8_t *y = yuv_buf;
	uint8_t *u = yuv_buf + width * heigh;
	uint8_t *v = u + width * heigh / 4;

	// BGRA中的A分量被丢弃不处理
	for (int r = 0; r < heigh; r++) {
		for (int c = 0; c < width; c++) {
			int index = r * width + c;
			*y++ = RGB2Y_FLOAT(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);

			if ((r % 2 == 0) && (c % 2 == 0)) //偶数行，偶数列获取U
				*u++ = RGB2U_FLOAT(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);
				//*u++ = 128;

			else if (c % 2 == 0) //奇数行，偶数列获取V
				*v++ = RGB2V_FLOAT(bgra_buf[index * bgra_bytes_num + 2], bgra_buf[index * bgra_bytes_num + 1], bgra_buf[index * bgra_bytes_num]);
				//*v++ = 128;
		}
	}
}

static void test_bgra2yuv420p(uint8_t *bgra_buf, uint32_t w, uint32_t h)
{
	int yuv420_size = w * h * 3 / 2;
	uint8_t *yuv420_buf = NULL;

	ALLOC_PIXEL_DATA(yuv420_buf, yuv420_size);

/*	bgra2yuv420_multip(bgra_buf, yuv420_buf, w, h); */
/*	bgra2yuv420_shift(bgra_buf, yuv420_buf, w, h); */
	bgra2yuv420_float(bgra_buf, yuv420_buf, w, h);

	STORE_PIXEL_DATA(yuv420_buf, yuv420_size, w, h, "yuv420");
}


int main(int argc, const char *argv[])
{
	int ret = 0;
	const uint8_t *bgra_file = NULL;
	int width = 0, heigh = 0;

	if (argc == 1) {
		printf("%s <width> <heigh> <bgra file>\n", argv[0]);
		return -1;
	}

	width     = atoi(argv[1]);
	heigh     = atoi(argv[2]);
	bgra_file = argv[3];

	printf("width=%d, heigh=%d, bgra file: %s\n", width, heigh, bgra_file);

	FILE *bgra_fp = fopen(bgra_file, "rb");
	if (NULL == bgra_fp) {
		printf("open file failed!\n");
		return -1;
	}

	struct stat statbuf;
	ret = stat(bgra_file, &statbuf);
	if (ret != 0) {
		printf("stat failed.\n");
		fclose(bgra_fp);
		return -1;
	}

	int size = statbuf.st_size;
	printf("%s file size = %d\n", bgra_file, size);

	uint8_t *bgra_buf = (uint8_t *)malloc(size);
	if (NULL == bgra_buf) {
		printf("malloc buffer failed.\n");
		fclose(bgra_fp);
		return -1;
	}

	if (fread(bgra_buf, 1, size, bgra_fp) != size) {
		printf("read error\n");
		free(bgra_buf);
		fclose(bgra_fp);
		return -1;
	}


	test_bgra2yuv420p(bgra_buf, width, heigh);


	free(bgra_buf);
	fclose(bgra_fp);

	return 0;
}
