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

	fwrite(data, 1, size, fp);

	fclose(fp);
}

static void show_rgba()
{
	uint8_t *data = NULL;
	uint32_t size = PIC_WIDTH * PIC_HEIGHT * 4;

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
	uint32_t size = PIC_WIDTH * PIC_HEIGHT * 3 / 2;

	data = (uint8_t*)malloc(size);
	pixel_data = data;

	//memset(data, 0, size);	//绿色
	//memset(data, 255, size);	//粉色
	
#if 0
	//黑色：Y=0, U=128, V=128
	//Y
	memset(pixel_data, 0, PIC_WIDTH * PIC_HEIGHT);
	pixel_data += PIC_WIDTH * PIC_HEIGHT;
	//UV
	memset(pixel_data, 128, PIC_WIDTH * PIC_HEIGHT / 2);
#endif

	//白色：Y=255, U=128, V=128
	//Y
	memset(pixel_data, 255, PIC_WIDTH * PIC_HEIGHT);
	pixel_data += PIC_WIDTH * PIC_HEIGHT;
	//UV
	memset(pixel_data, 128, PIC_WIDTH * PIC_HEIGHT / 2);

	SAVE_PIXEL_DATA(data, size);

	free(data);
}

int main(int argc, const char *argv[])
{
	show_rgba();

	show_yuv_nv12();
	
	return 0;
}
