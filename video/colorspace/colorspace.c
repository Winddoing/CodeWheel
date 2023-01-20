/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : colorspace.c
 *  Author       : wqshao
 *  Created Time : 2022-11-10 17:14:48
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PIC_WIDTH	720
#define PIC_HEIGHT	480

#define SAVE_PIXEL_DATA(data, size) \
	do {	\
		char file_name[32];	\
		sprintf(file_name, "%s_%dx%d.raw", __func__, PIC_WIDTH, PIC_HEIGHT); \
		save_file(file_name, data, size); \
	} while(0)

static void save_file(char* file_name, uint8_t *data, uint32_t size)
{
	FILE *fp = fopen(file_name, "wb+");

	printf("%s: %s\n", __func__, file_name);

	fwrite(data, 1, size, fp);

	fclose(fp);
}

static void show_rgba()
{
	uint8_t *data = NULL;
	uint32_t w = PIC_WIDTH;
	uint32_t h = PIC_HEIGHT;
	uint32_t size = w * h * 4;

	data = (uint8_t*)malloc(size);

#if 0
	memset(data, 0, size);		//黑色 rgb(0, 0, 0)
#else
	memset(data, 0xFF, size);	//白色 rgb(255, 255, 255)
#endif

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

static void show_yuv_nv12()
{
	uint8_t *data = NULL;
	uint8_t *pixel_data = NULL;
	uint32_t w = PIC_WIDTH;
	uint32_t h = PIC_HEIGHT;
	uint32_t size = w * h * 3 / 2;

	data = (uint8_t*)malloc(size);
	pixel_data = data;

	//memset(data, 0, size);	//绿色
	//memset(data, 255, size);	//粉色
	
#if 0
	//黑色：Y=0, U=128, V=128
	//Y
	memset(pixel_data, 0, w * h);
	pixel_data += w * h;
	//UV
	memset(pixel_data, 128, w * h / 2);
#endif

	//白色：Y=255, U=128, V=128
	//Y
	memset(pixel_data, 255, w * h);
	pixel_data += w * h;
	//UV
	memset(pixel_data, 128, w * h / 2);

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

static void show_yuv_420()
{
	uint8_t *data = NULL;
	uint8_t *pixel_data = NULL;
	uint32_t w = PIC_WIDTH;
	uint32_t h = PIC_HEIGHT;
	uint32_t size = w * h * 3 / 2;

	data = (uint8_t*)malloc(size);
	pixel_data = data;

	//memset(data, 0, size);	//绿色
	//memset(data, 255, size);	//粉色
	
#if 0
	//黑色：Y=0, U=128, V=128
	//Y
	memset(pixel_data, 0, w * h);
	pixel_data += w * h;
	//UV
	memset(pixel_data, 128, w * h / 2);

	//白色：Y=255, U=128, V=128
	//Y
	memset(pixel_data, 255, w * h);
	pixel_data += w * h;
	//UV
	memset(pixel_data, 128, w * h / 2);
#endif
	//Y
	memset(pixel_data, 124, w * h);
	pixel_data += w * h;
	//U
	memset(pixel_data, 100, w * h / 4);
	pixel_data += w * h / 4;
	//V
	memset(pixel_data, 212, w * h / 4);

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

static void show_yuv_422p()
{
	uint8_t *data = NULL;
	uint8_t *pixel_data_y = NULL;
	uint8_t *pixel_data_u = NULL;
	uint8_t *pixel_data_v = NULL;
	uint32_t w = PIC_WIDTH;
	uint32_t h = PIC_HEIGHT;
	uint32_t size = w * h * 2;

	data = (uint8_t*)malloc(size);

	pixel_data_y = data;
	pixel_data_u = data + w * h;
	pixel_data_v = data + w * h + w * h / 2;

	memset(data, 0, size);	//绿色
	//memset(data, 255, size);	//粉色
	
#if 0
	//黑色：Y=0, U=128, V=128
	//Y
	memset(pixel_data_y, 0, w * h);
	//U
	memset(pixel_data_u, 128, w * h / 2);
	//U
	memset(pixel_data_v, 128, w * h / 2);

#else
	//白色：Y=255, U=128, V=128
	//Y
	memset(pixel_data_y, 255, w * h);
	//U
	memset(pixel_data_u, 128, w * h / 2);
	//v
	memset(pixel_data_v, 128, w * h / 2);
#endif

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

static void show_yuv_444()
{
	uint8_t *data = NULL;
	uint8_t *pixel_data = NULL;
	uint32_t w = PIC_WIDTH;
	uint32_t h = PIC_HEIGHT;
	uint32_t size = w * h * 3;
	int pixel = 0;

	data = (uint8_t*)malloc(size);
	memset(data, 0, size);
	pixel_data = data;

	int step = 64;
	int w_step_cnt = w / step; 
	int h_step_cnt = h / step; 

	//printf("w_step_cnt=%d, h_step_cnt=%d, pixel_data=%p\n", w_step_cnt, h_step_cnt, pixel_data);

	//Y U V
	for (int cmp = 0; cmp < 3; cmp++) {
		uint8_t* pixel_data_cmp = pixel_data + w * h * cmp;
		for (int i = 0; i < h_step_cnt; i++) {
			uint8_t* pixel_h_data = pixel_data_cmp + w * i * step;

			for (int j = 0; j < w_step_cnt; j++) {
				uint8_t* pixel_w_data = pixel_h_data + j * step;

				//printf("draw block poc i,j=(%d, %d)\n", i, j);
				//分割块填充
				for (int pix_y = 0; pix_y < step; pix_y++) {
					//printf("draw block line-y=%d pixel_w_data=%p\n", pix_y, pixel_w_data);
					for (int pix_x = 0; pix_x < step; pix_x++) {
						pixel_w_data[pix_x] = pixel;
					}
					pixel_w_data += w;
				}

				pixel++;
				if (pixel > 255)
					pixel = 0;
			}
		}
	}

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

int main(int argc, const char *argv[])
{
	show_rgba();

	show_yuv_nv12();
	
	show_yuv_420();

	show_yuv_422p();

	show_yuv_444();

	return 0;
}
