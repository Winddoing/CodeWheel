/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : bmprgb2rgba.c
 *  Author       : wqshao
 *  Created Time : 2022-09-26 11:17:00
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BF_TYPE(a, b) (((uint8_t)(a) << 8) | (uint8_t)(b))

#define BF_TYPE_MB     BF_TYPE('M', 'B')
#define BF_TYPE_BA     BF_TYPE('B', 'A')
#define BF_TYPE_CI     BF_TYPE('C', 'I')
#define BF_TYPE_CP     BF_TYPE('C', 'P')
#define BF_TYPE_IC     BF_TYPE('I', 'C')
#define BF_TYPE_PT     BF_TYPE('P', 'T')

//BMP图片的压缩方式
#define BI_RGB				(0)
#define BI_BITFIELDS        (3)

//BMP图片的像素位数
#define BMP_8_BIT			(8)
#define BMP_16_BIT			(16)
#define BMP_24_BIT			(24)
#define BMP_32_BIT			(32)

//16位BMP图片从文件头到图像数据的偏移量
#define BMP_16_BIT_BI_BITFIELDS_OFFSET	(66)
#define BMP_16_BIT_BI_RGB_OFFSET		(54)

//8位BMP图片调色板的大小
#define BMP_8_BIT_OFFSET				(1024)

//BMP图片的文件头，固定14字节
#define BMP_FILE_HEADER_LENGTH			(14)


typedef struct {
	uint16_t	bfType;			//该位图文件类型
	uint32_t	bfSize;			//该位图文件大小，单位字节
	uint16_t	bfReserved1;	//预留
	uint16_t	bfReserved2;	//预留
	uint32_t	bfOffBits;		//实际图像数据偏移量
} __attribute__ ((packed)) ClBitMapFileHeader;

typedef struct {
	uint32_t	biSize;          // 此结构体的大小 (14-17字节)
	uint32_t    biWidth;         // 图像的宽  (18-21字节)
	uint32_t    biHeight;        // 图像的高  (22-25字节)
	uint16_t	biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
	uint16_t	biBitCount;      // 一像素所占的位数，(28-29字节)当biBitCount=24时，该BMP图像就是24Bit真彩图，没有调色板项。
	uint32_t    biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
	uint32_t    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
	uint32_t	biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
	uint32_t	biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
	uint32_t    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数。 (46-49字节)
	uint32_t    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} __attribute__ ((packed)) ClBitMapInfoHeader;

typedef struct  {
	unsigned char rgbBlue;      //该颜色的蓝色分量
	unsigned char rgbGreen;     //该颜色的绿色分量
	unsigned char rgbRed;       //该颜色的红色分量
	unsigned char rgbReserved;  //保留值
} ClRgbQuad;

typedef struct {
	int width;
	int height;
	int channels;
	unsigned char* imageData;
} ClImage;


static void dump_file_header(ClBitMapFileHeader *fh)
{
	printf("============================================== \n");
	printf("bmp file header:\n");
	printf("         bfType : %d \n", fh->bfType);
	printf("         bfSize : %d \n", fh->bfSize);
	printf("    bfReserved1 : %d \n", fh->bfReserved1);
	printf("    bfReserved2 : %d \n", fh->bfReserved2);
	printf("      bfOffBits : %d \n", fh->bfOffBits);
}

static void dump_info_header(ClBitMapInfoHeader *ih)
{
	printf("============================================== \n");
	printf("bmp info header: \n");
	printf("         biSize : %d \n", ih->biSize);
	printf("        biWidth : %d \n", ih->biWidth);
	printf("       biHeight : %d \n", ih->biHeight);
	printf("       biPlanes : %d \n", ih->biPlanes);
	printf("     biBitCount : %d \n", ih->biBitCount);
	printf(" biClrImportant : %d \n", ih->biCompression);
	printf("    biSizeImage : %d \n", ih->biSizeImage);
	printf("biXPelsPerMeter : %d \n", ih->biXPelsPerMeter);
	printf("biYPelsPerMeter : %d \n", ih->biYPelsPerMeter);
	printf("      biClrUsed : %d \n", ih->biClrUsed);
	printf(" biClrImportant : %d \n", ih->biClrImportant);
	printf("============================================== \n");

}

static ClImage* clLoadImage(char* path)
{
	ClImage* bmpImg;
	FILE* pFile;
	uint16_t fileType;
	ClBitMapFileHeader bmpFileHeader;
	ClBitMapInfoHeader bmpInfoHeader;
	int channels = 1;
	int width = 0;
	int height = 0;
	int step = 0;
	int offset = 0;
	unsigned char pixVal;
	ClRgbQuad* quad;
	int i, j, k;

	bmpImg = (ClImage*)malloc(sizeof(ClImage));
	pFile = fopen(path, "rb+");

	if(!pFile) {
		free(bmpImg);
		return NULL;
	}

	fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
	dump_file_header(&bmpFileHeader);

	fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
	dump_info_header(&bmpInfoHeader);

	if (bmpFileHeader.bfType != BF_TYPE_MB) {
		printf("no bmp files\n");
		return NULL;
	}

	if(bmpInfoHeader.biBitCount == 8) {
		//printf("该文件有调色板，即该位图为非真彩色\n\n");
		channels = 1;
		width = bmpInfoHeader.biWidth;
		height = bmpInfoHeader.biHeight;
		offset = (channels * width) % 4;

		if(offset != 0) {
			offset = 4 - offset;
		}

		//bmpImg->mat = kzCreateMat(height, width, 1, 0);
		bmpImg->width = width;
		bmpImg->height = height;
		bmpImg->channels = 1;
		bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
		step = channels * width;

		quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
		fread(quad, sizeof(ClRgbQuad), 256, pFile);
		free(quad);

		for(i = 0; i < height; i++) {
			for(j = 0; j < width; j++) {
				fread(&pixVal, sizeof(unsigned char), 1, pFile);
				bmpImg->imageData[(height - 1 - i)*step + j] = pixVal;   //每次一个像素处理
			}

			if(offset != 0) {
				for(j = 0; j < offset; j++) {
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}

	} else if(bmpInfoHeader.biBitCount == 24) {
		//printf("该位图为位真彩色\n\n");
		channels = 3;
		width = bmpInfoHeader.biWidth;
		height = bmpInfoHeader.biHeight;

		bmpImg->width = width;
		bmpImg->height = height;
		bmpImg->channels = 3;
		bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char) * width * 3 * height);
		step = channels * width;

		offset = (channels * width) % 4;

		if(offset != 0) {
			offset = 4 - offset;
		}

		fseek(pFile, bmpFileHeader.bfOffBits - sizeof(bmpInfoHeader) - sizeof(bmpFileHeader), SEEK_CUR);  //138-54?感觉应该没有138才对啊

		for(i = 0; i < height; i++) {
			for(j = 0; j < width; j++) {
				for(k = 0; k < 3; k++) {
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
					bmpImg->imageData[(height - 1 - i)*step + j * 3 + k] = pixVal;  //
				}

				//kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));
			}

			if(offset != 0) {
				for(j = 0; j < offset; j++) {
					fread(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}

	return bmpImg;
}

static bool clSaveImage(char* path, ClImage* bmpImg)
{
	FILE *pFile;
	uint16_t fileType;
	ClBitMapFileHeader bmpFileHeader;
	ClBitMapInfoHeader bmpInfoHeader;
	int step;
	int offset;
	unsigned char pixVal = '\0';
	int i, j;
	ClRgbQuad* quad;

	pFile = fopen(path, "wb");

	if(!pFile) {
		return false;
	}

	fileType = 0x4D42;
	fwrite(&fileType, sizeof(uint16_t), 1, pFile);

	if(bmpImg->channels == 3) { //24位，通道，彩图
		step = bmpImg->channels * bmpImg->width;
		offset = step % 4;

		if(offset != 4) {
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = bmpImg->height * step + 54;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfOffBits = 54;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 24;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height * step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		for(i = bmpImg->height - 1; i > -1; i--) {
			for(j = 0; j < bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3 + 1];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i * bmpImg->width * 3 + j * 3 + 2];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}

			if(offset != 0) {
				for(j = 0; j < offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}

	} else if(bmpImg->channels == 1) { //8位，单通道，灰度图
		step = bmpImg->width;
		offset = step % 4;

		if(offset != 4) {
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = 54 + 256 * 4 + bmpImg->width;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfOffBits = 54 + 256 * 4;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 8;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height * step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 256;
		bmpInfoHeader.biClrImportant = 256;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);

		for(i = 0; i < 256; i++) {
			quad[i].rgbBlue = i;
			quad[i].rgbGreen = i;
			quad[i].rgbRed = i;
			quad[i].rgbReserved = 0;
		}

		fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
		free(quad);

		for(i = bmpImg->height - 1; i > -1; i--) {
			for(j = 0; j < bmpImg->width; j++) {
				pixVal = bmpImg->imageData[i * bmpImg->width + j];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}

			if(offset != 0) {
				for(j = 0; j < offset; j++) {
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}

	fclose(pFile);

	return true;

}

int rgb24_to_bmp(const char *rgb24path, const char *bmppath, int width, int height)
{
	int i = 0, j = 0;
	ClBitMapFileHeader m_BMPHeader = {0};
	ClBitMapInfoHeader  m_BMPInfoHeader = {0};
	int header_size = sizeof(ClBitMapFileHeader) + sizeof(ClBitMapInfoHeader);
	unsigned char *rgb24_buffer = NULL;
	FILE *fp_rgb24 = NULL, *fp_bmp = NULL;

	printf("header_size=%d\n", header_size);

	if((fp_rgb24 = fopen(rgb24path, "rb")) == NULL) {
		printf("Error: Cannot open input RGB24 file.\n");
		return -1;
	}

	if((fp_bmp = fopen(bmppath, "wb")) == NULL) {
		printf("Error: Cannot open output BMP file.\n");
		return -1;
	}

	rgb24_buffer = (unsigned char *)malloc(width * height * 3);
	fread(rgb24_buffer, 1, width * height * 3, fp_rgb24);

	//设置相应的bmp头数据结构
	//m_BMPHeader.bfType = BF_TYPE_MB;
	m_BMPHeader.bfType = 0x4D42;
	m_BMPHeader.bfSize = header_size + 3 * width * height;
	m_BMPHeader.bfOffBits = header_size;

	m_BMPInfoHeader.biSize = sizeof(ClBitMapInfoHeader);
	m_BMPInfoHeader.biWidth = width;
	//BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
	m_BMPInfoHeader.biHeight = -height;
	m_BMPInfoHeader.biPlanes = 1;
	m_BMPInfoHeader.biBitCount = 24;
	m_BMPInfoHeader.biSizeImage = 3 * width * height;

	fwrite(&m_BMPHeader, 1, sizeof(m_BMPHeader), fp_bmp);
	fwrite(&m_BMPInfoHeader, 1, sizeof(m_BMPInfoHeader), fp_bmp);

	//BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
	//It saves pixel data in Little Endian
	//So we change 'R' and 'B'
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			char temp = rgb24_buffer[(j * width + i) * 3 + 2];
			rgb24_buffer[(j * width + i) * 3 + 2] = rgb24_buffer[(j * width + i) * 3 + 0];
			rgb24_buffer[(j * width + i) * 3 + 0] = temp;
		}
	}

	//写入相应的bmp数据值部分
	fwrite(rgb24_buffer, 3 * width * height, 1, fp_bmp);
	fclose(fp_rgb24);
	fclose(fp_bmp);
	free(rgb24_buffer);
	printf("Finish generate %s!\n", bmppath);

	return 0;
}

int rgba32_to_bmp(const char *rgba32path, const char *bmppath, int width, int height)
{
	int i = 0, j = 0;
	ClBitMapFileHeader m_BMPHeader = {0};
	ClBitMapInfoHeader  m_BMPInfoHeader = {0};
	int header_size = sizeof(ClBitMapFileHeader) + sizeof(ClBitMapInfoHeader);
	unsigned char *rgba32_buffer = NULL;
	unsigned char *rgb24_buffer = NULL;
	FILE *fp_rgba32 = NULL, *fp_bmp = NULL;

	if((fp_rgba32 = fopen(rgba32path, "rb")) == NULL) {
		printf("Error: Cannot open input RGB24 file.\n");
		return -1;
	}

	if((fp_bmp = fopen(bmppath, "wb")) == NULL) {
		printf("Error: Cannot open output BMP file.\n");
		return -1;
	}

	rgba32_buffer = (unsigned char *)malloc(width * height * 4);
	rgb24_buffer = (unsigned char *)malloc(width * height * 3);

	fread(rgba32_buffer, 1, width * height * 4, fp_rgba32);

	for(int i = 0; i < width * height; i++) {
		//printf("R:0x%02x, G:0x%02x, B:0x%02x, A:0x%02x\n", rgba32_buffer[i * 4 + 0], rgba32_buffer[i * 4 + 1], rgba32_buffer[i * 4 + 2], rgba32_buffer[i * 4 + 3]);
		rgb24_buffer[i * 3 + 0] = rgba32_buffer[i * 4 + 0]; //R
		rgb24_buffer[i * 3 + 1] = rgba32_buffer[i * 4 + 1]; //G
		rgb24_buffer[i * 3 + 2] = rgba32_buffer[i * 4 + 2]; //B
	}

	printf("===> func: %s, line: %d, header_size=%d\n", __func__, __LINE__, header_size);
	printf("===> func: %s, line: %d, H size=%ld, I size=%ld\n", __func__, __LINE__, sizeof(m_BMPHeader), sizeof(m_BMPInfoHeader));
	//设置相应的bmp头数据结构
	m_BMPHeader.bfType            = BF_TYPE_MB;
	m_BMPHeader.bfSize            = header_size + width * height * 3;
	m_BMPHeader.bfReserved1       = 0;
	m_BMPHeader.bfReserved2       = 0;
	m_BMPHeader.bfOffBits         = header_size;

	m_BMPInfoHeader.biSize        = sizeof(ClBitMapInfoHeader);
	m_BMPInfoHeader.biWidth       = width;
	m_BMPInfoHeader.biHeight      = height;
	m_BMPInfoHeader.biPlanes      = 1;
	m_BMPInfoHeader.biBitCount    = BMP_24_BIT;
	m_BMPInfoHeader.biCompression = BI_RGB;
	m_BMPInfoHeader.biSizeImage   = width * height * 3;

	//设置相应的bmp头数据结构
	fwrite(&m_BMPHeader, 1, sizeof(m_BMPHeader), fp_bmp);
	fwrite(&m_BMPInfoHeader, 1, sizeof(m_BMPInfoHeader), fp_bmp);

	dump_file_header(&m_BMPHeader);
	dump_info_header(&m_BMPInfoHeader);

#if 1
	//bitmap format: BGR
	//BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
	//It saves pixel data in Little Endian
	//So we change 'R' and 'B'
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			char temp = rgb24_buffer[(j * width + i) * 3 + 2];
			rgb24_buffer[(j * width + i) * 3 + 2] = rgb24_buffer[(j * width + i) * 3 + 0];
			rgb24_buffer[(j * width + i) * 3 + 0] = temp;
		}
	}
#endif

	//写入相应的bmp数据值部分
	fwrite(rgb24_buffer, 3 * width * height, 1, fp_bmp);
	fclose(fp_rgba32);
	fclose(fp_bmp);
	free(rgb24_buffer);
	printf("%s, Finish generate %s!\n", __func__, bmppath);
	return 0;
}

int bmp_to_rgba(const char *bmppath, const char *rgba32path, int width, int height)
{
	int i = 0, j = 0;
	ClBitMapFileHeader m_BMPHeader = {0};
	ClBitMapInfoHeader  m_BMPInfoHeader = {0};
	int header_size = sizeof(ClBitMapFileHeader) + sizeof(ClBitMapInfoHeader);
	unsigned char *rgba32_buffer = NULL;
	unsigned char *rgb24_buffer = NULL;
	FILE *fp_rgba32 = NULL, *fp_bmp = NULL;

	if((fp_bmp = fopen(bmppath, "rb")) == NULL) {
		printf("Error: Cannot open output BMP file.\n");
		return -1;
	}
	if((fp_rgba32 = fopen(rgba32path, "wb")) == NULL) {
		printf("Error: Cannot open input RGB24 file.\n");
		return -1;
	}

	rgba32_buffer = (unsigned char *)malloc(width * height * 4);
	rgb24_buffer = (unsigned char *)malloc(width * height * 3);

	//read bmp image
#if 0
	//skip header size
	if (fseek(fp_bmp, header_size, SEEK_SET) == -1) {
		printf("bmp file fseek error\n");
		return -1;
	}
#else
	fread(&m_BMPHeader, sizeof(ClBitMapFileHeader), 1, fp_bmp);
	dump_file_header(&m_BMPHeader);

	fread(&m_BMPInfoHeader, sizeof(ClBitMapInfoHeader), 1, fp_bmp);
	dump_info_header(&m_BMPInfoHeader);
#endif
	fread(rgb24_buffer, 1, width * height * 3, fp_bmp);


#if 0
	//这样处理后，显示颜色正常，但是显示图像为垂直方向镜像
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			//printf("R:0x%02x, G:0x%02x, B:0x%02x\n", rgb24_buffer[(j * width + i) * 3 + 0], rgb24_buffer[(j * width + i) * 3 + 1], rgb24_buffer[(j * width + i) * 3 + 2]);
			rgba32_buffer[(j * width + i) * 4 + 2] = rgb24_buffer[(j * width + i) * 3 + 0];
			rgba32_buffer[(j * width + i) * 4 + 1] = rgb24_buffer[(j * width + i) * 3 + 1];
			rgba32_buffer[(j * width + i) * 4 + 0] = rgb24_buffer[(j * width + i) * 3 + 2];
			rgba32_buffer[(j * width + i) * 4 + 3] = 0;
		}
	}
#else
	////前后颠倒旋转
	//for(j = 0; j < height; j++) {
	//	for(i = 0; i < width; i++) {
	//		//printf("R:0x%02x, G:0x%02x, B:0x%02x\n", rgb24_buffer[(j * width + i) * 3 + 0], rgb24_buffer[(j * width + i) * 3 + 1], rgb24_buffer[(j * width + i) * 3 + 2]);
	//		rgba32_buffer[(j * width + i) * 4 + 2] = rgb24_buffer[((height - 1 - j) * width + (width - 1 - i)) * 3 + 0];
	//		rgba32_buffer[(j * width + i) * 4 + 1] = rgb24_buffer[((height - 1 - j) * width + (width - 1 - i)) * 3 + 1];
	//		rgba32_buffer[(j * width + i) * 4 + 0] = rgb24_buffer[((height - 1 - j) * width + (width - 1 - i)) * 3 + 2];
	//		rgba32_buffer[(j * width + i) * 4 + 3] = 0;
	//	}
	//}
	// 沿着水平方向上点颠倒
	for(j = 0; j < height; j++) {
		for(i = 0; i < width; i++) {
			//printf("R:0x%02x, G:0x%02x, B:0x%02x\n", rgb24_buffer[(j * width + i) * 3 + 0], rgb24_buffer[(j * width + i) * 3 + 1], rgb24_buffer[(j * width + i) * 3 + 2]);
			rgba32_buffer[(j * width + i) * 4 + 2] = rgb24_buffer[((height - 1 - j) * width + (width + i)) * 3 + 0];
			rgba32_buffer[(j * width + i) * 4 + 1] = rgb24_buffer[((height - 1 - j) * width + (width + i)) * 3 + 1];
			rgba32_buffer[(j * width + i) * 4 + 0] = rgb24_buffer[((height - 1 - j) * width + (width + i)) * 3 + 2];
			rgba32_buffer[(j * width + i) * 4 + 3] = 0;
		}
	}
#endif


	fwrite(rgba32_buffer, width * height * 4, 1, fp_rgba32);

	fclose(fp_rgba32);
	fclose(fp_bmp);
	free(rgb24_buffer);
	free(rgba32_buffer);

	printf("%s: Finish generate %s!\n", __func__, rgba32path);
	return 0;
}

int main(int argc, char* argv[])
{
	//clLoadImage("desktop.bmp");
#if 0
	printf("--> rgb24_to_bmp:\n");
	rgba32_to_bmp("tst_1920x1080.rgba", "tst_1920x1080.bmp", 1920, 1080);
#endif

	printf("--> bmp_to_rgba:\n");
	bmp_to_rgba("desktop.bmp", "desktop.rgba", 1920, 1080);

	return 0;
}
