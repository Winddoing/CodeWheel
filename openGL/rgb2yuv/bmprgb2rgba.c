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

typedef struct
{
    //unsigned short    bfType;
    unsigned long    bfSize;
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    unsigned long    bfOffBits;
} ClBitMapFileHeader;

typedef struct
{
    unsigned long  biSize;
    long   biWidth;
    long   biHeight;
    unsigned short   biPlanes;
    unsigned short   biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long   biXPelsPerMeter;
    long   biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
} ClBitMapInfoHeader;

typedef struct
{
    unsigned char rgbBlue; //该颜色的蓝色分量
    unsigned char rgbGreen; //该颜色的绿色分量
    unsigned char rgbRed; //该颜色的红色分量
    unsigned char rgbReserved; //保留值
} ClRgbQuad;

typedef struct
{
    int width;
    int height;
    int channels;
    unsigned char* imageData;
}ClImage;

ClImage* clLoadImage(char* path);
bool clSaveImage(char* path, ClImage* bmpImg);


ClImage* clLoadImage(char* path)
{
    ClImage* bmpImg;
    FILE* pFile;
    unsigned short fileType;
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
    if (!pFile)
    {
        free(bmpImg);
        return NULL;
    }

    fread(&fileType, sizeof(unsigned short), 1, pFile);
    if (fileType == 0x4D42) //string "BM"
    {
        //printf("bmp file! \n");

        fread(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);
        /*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
        printf("bmp文件头信息：\n");
        printf("文件大小：%d \n", bmpFileHeader.bfSize);
        printf("保留字：%d \n", bmpFileHeader.bfReserved1);
        printf("保留字：%d \n", bmpFileHeader.bfReserved2);
        printf("位图数据偏移字节数：%d \n", bmpFileHeader.bfOffBits);*/

        fread(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);
        /*printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");
        printf("bmp文件信息头\n");
        printf("结构体长度：%d \n", bmpInfoHeader.biSize);
        printf("位图宽度：%d \n", bmpInfoHeader.biWidth);
        printf("位图高度：%d \n", bmpInfoHeader.biHeight);
        printf("位图平面数：%d \n", bmpInfoHeader.biPlanes);
        printf("颜色位数：%d \n", bmpInfoHeader.biBitCount);
        printf("压缩方式：%d \n", bmpInfoHeader.biCompression);
        printf("实际位图数据占用的字节数：%d \n", bmpInfoHeader.biSizeImage);
        printf("X方向分辨率：%d \n", bmpInfoHeader.biXPelsPerMeter);
        printf("Y方向分辨率：%d \n", bmpInfoHeader.biYPelsPerMeter);
        printf("使用的颜色数：%d \n", bmpInfoHeader.biClrUsed);
        printf("重要颜色数：%d \n", bmpInfoHeader.biClrImportant);
        printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n");*/

        if (bmpInfoHeader.biBitCount == 8)
        {
            //printf("该文件有调色板，即该位图为非真彩色\n\n");
            channels = 1;
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;
            offset = (channels*width) % 4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }
            //bmpImg->mat = kzCreateMat(height, width, 1, 0);
            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->channels = 1;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
            step = channels*width;

            quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
            fread(quad, sizeof(ClRgbQuad), 256, pFile);
            free(quad);

            for (i = 0; i<height; i++)
            {
                for (j = 0; j<width; j++)
                {
                    fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    bmpImg->imageData[(height - 1 - i)*step + j] = pixVal;   //每次一个像素处理
                }
                if (offset != 0)
                {
                    for (j = 0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
        else if (bmpInfoHeader.biBitCount == 24)
        {
            //printf("该位图为位真彩色\n\n");
            channels = 3;
            width = bmpInfoHeader.biWidth;
            height = bmpInfoHeader.biHeight;

            bmpImg->width = width;
            bmpImg->height = height;
            bmpImg->channels = 3;
            bmpImg->imageData = (unsigned char*)malloc(sizeof(unsigned char)*width * 3 * height);
            step = channels*width;

            offset = (channels*width) % 4;
            if (offset != 0)
            {
                offset = 4 - offset;
            }

            fseek(pFile, bmpFileHeader.bfOffBits - sizeof(bmpInfoHeader) - sizeof(bmpFileHeader), SEEK_CUR);  //138-54?感觉应该没有138才对啊

            for (i = 0; i<height; i++)
            {
                for (j = 0; j<width; j++)
                {
                    for (k = 0; k<3; k++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                        bmpImg->imageData[(height - 1 - i)*step + j * 3 + k] = pixVal;  //
                    }
                    //kzSetMat(bmpImg->mat, height-1-i, j, kzScalar(pixVal[0], pixVal[1], pixVal[2]));
                }
                if (offset != 0)
                {
                    for (j = 0; j<offset; j++)
                    {
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);
                    }
                }
            }
        }
    }

    return bmpImg;
}

bool clSaveImage(char* path, ClImage* bmpImg)
{
    FILE *pFile;
    unsigned short fileType;
    ClBitMapFileHeader bmpFileHeader;
    ClBitMapInfoHeader bmpInfoHeader;
    int step;
    int offset;
    unsigned char pixVal = '\0';
    int i, j;
    ClRgbQuad* quad;

    pFile = fopen(path, "wb");
    if (!pFile)
    {
        return false;
    }

    fileType = 0x4D42;
    fwrite(&fileType, sizeof(unsigned short), 1, pFile);

    if (bmpImg->channels == 3)//24位，通道，彩图
    {
        step = bmpImg->channels*bmpImg->width;
        offset = step % 4;
        if (offset != 4)
        {
            step += 4 - offset;
        }

        bmpFileHeader.bfSize = bmpImg->height*step + 54;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54;
        fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 24;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 0;
        bmpInfoHeader.biClrImportant = 0;
        fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

        for (i = bmpImg->height - 1; i>-1; i--)
        {
            for (j = 0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3 + 1];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3 + 2];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            if (offset != 0)
            {
                for (j = 0; j<offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }
    else if (bmpImg->channels == 1)//8位，单通道，灰度图
    {
        step = bmpImg->width;
        offset = step % 4;
        if (offset != 4)
        {
            step += 4 - offset;
        }

        bmpFileHeader.bfSize = 54 + 256 * 4 + bmpImg->width;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = 54 + 256 * 4;
        fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

        bmpInfoHeader.biSize = 40;
        bmpInfoHeader.biWidth = bmpImg->width;
        bmpInfoHeader.biHeight = bmpImg->height;
        bmpInfoHeader.biPlanes = 1;
        bmpInfoHeader.biBitCount = 8;
        bmpInfoHeader.biCompression = 0;
        bmpInfoHeader.biSizeImage = bmpImg->height*step;
        bmpInfoHeader.biXPelsPerMeter = 0;
        bmpInfoHeader.biYPelsPerMeter = 0;
        bmpInfoHeader.biClrUsed = 256;
        bmpInfoHeader.biClrImportant = 256;
        fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

        quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
        for (i = 0; i<256; i++)
        {
            quad[i].rgbBlue = i;
            quad[i].rgbGreen = i;
            quad[i].rgbRed = i;
            quad[i].rgbReserved = 0;
        }
        fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
        free(quad);

        for (i = bmpImg->height - 1; i>-1; i--)
        {
            for (j = 0; j<bmpImg->width; j++)
            {
                pixVal = bmpImg->imageData[i*bmpImg->width + j];
                fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
            }
            if (offset != 0)
            {
                for (j = 0; j<offset; j++)
                {
                    pixVal = 0;
                    fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
                }
            }
        }
    }
    fclose(pFile);

    return true;

}

int rgb24_to_bmp(const char *rgb24path,const char *bmppath,int width,int height)
{
    //定义相应的bmp数据头结构体
    typedef struct
    {
        long imageSize;
        long blank;
        long startPosition;
    }BmpHead;

    typedef struct
    {
        long  Length;
        long  width;
        long  height;
        unsigned short  colorPlane;
        unsigned short  bitColor;
        long  zipFormat;
        long  realSize;
        long  xPels;
        long  yPels;
        long  colorUse;
        long  colorImportant;
    }InfoHead;

    int i=0,j=0;
    BmpHead m_BMPHeader={0};
    InfoHead  m_BMPInfoHeader={0};
    char bfType[2]={'B','M'};
    int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
    unsigned char *rgb24_buffer=NULL;
    FILE *fp_rgb24=NULL,*fp_bmp=NULL;

    if((fp_rgb24=fopen(rgb24path,"rb"))==NULL){
        printf("Error: Cannot open input RGB24 file.\n");
        return -1;
    }
    if((fp_bmp=fopen(bmppath,"wb"))==NULL){
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    rgb24_buffer=(unsigned char *)malloc(width*height*3);
    fread(rgb24_buffer,1,width*height*3,fp_rgb24);

    //设置相应的bmp头数据结构
    m_BMPHeader.imageSize=3*width*height+header_size;
    m_BMPHeader.startPosition=header_size;

    m_BMPInfoHeader.Length=sizeof(InfoHead);
    m_BMPInfoHeader.width=width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height=-height;
    m_BMPInfoHeader.colorPlane=1;
    m_BMPInfoHeader.bitColor=24;
    m_BMPInfoHeader.realSize=3*width*height;

    fwrite(bfType,1,sizeof(bfType),fp_bmp);
    fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
    fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for(j =0;j<height;j++){
        for(i=0;i<width;i++){
            char temp=rgb24_buffer[(j*width+i)*3+2];
            rgb24_buffer[(j*width+i)*3+2]=rgb24_buffer[(j*width+i)*3+0];
            rgb24_buffer[(j*width+i)*3+0]=temp;
        }
    }
    //写入相应的bmp数据值部分
    fwrite(rgb24_buffer,3*width*height,1,fp_bmp);
    fclose(fp_rgb24);
    fclose(fp_bmp);
    free(rgb24_buffer);
    printf("Finish generate %s!\n",bmppath);
    return 0;
}

int rgba32_to_bmp(const char *rgba32path,const char *bmppath,int width,int height)
{
    //定义相应的bmp数据头结构体
    typedef struct
    {
        long imageSize;
        long blank;
        long startPosition;
    }BmpHead;

    typedef struct
    {
        long  Length;
        long  width;
        long  height;
        unsigned short  colorPlane;
        unsigned short  bitColor;
        long  zipFormat;
        long  realSize;
        long  xPels;
        long  yPels;
        long  colorUse;
        long  colorImportant;
    }InfoHead;

    int i=0,j=0;
    BmpHead m_BMPHeader={0};
    InfoHead  m_BMPInfoHeader={0};
    char bfType[2]={'B','M'};
    int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
    unsigned char *rgba32_buffer=NULL;
    unsigned char *rgb24_buffer=NULL;
    FILE *fp_rgba32=NULL,*fp_bmp=NULL;

    if((fp_rgba32=fopen(rgba32path,"rb"))==NULL){
        printf("Error: Cannot open input RGB24 file.\n");
        return -1;
    }
    if((fp_bmp=fopen(bmppath,"wb"))==NULL){
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    rgba32_buffer=(unsigned char *)malloc(width*height*4);
    rgb24_buffer=(unsigned char *)malloc(width*height*3);
    fread(rgba32_buffer,1,width*height*4,fp_rgba32);
    for (int i=0; i<width*height; i++) {
        rgb24_buffer[i * 3 + 0] = rgba32_buffer[i*4+0];
        rgb24_buffer[i * 3 + 1] = rgba32_buffer[i*4+1];
        rgb24_buffer[i * 3 + 2] = rgba32_buffer[i*4+2];
    }

    //设置相应的bmp头数据结构
    m_BMPHeader.imageSize=3*width*height+header_size;
    m_BMPHeader.startPosition=header_size;

    m_BMPInfoHeader.Length=sizeof(InfoHead);
    m_BMPInfoHeader.width=width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height=-height;
    m_BMPInfoHeader.colorPlane=1;
    m_BMPInfoHeader.bitColor=24;
    m_BMPInfoHeader.realSize=3*width*height;

    fwrite(bfType,1,sizeof(bfType),fp_bmp);
    fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
    fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for(j =0;j<height;j++){
        for(i=0;i<width;i++){
            char temp=rgb24_buffer[(j*width+i)*3+2];
            rgb24_buffer[(j*width+i)*3+2]=rgb24_buffer[(j*width+i)*3+0];
            rgb24_buffer[(j*width+i)*3+0]=temp;
        }
    }
    //写入相应的bmp数据值部分
    fwrite(rgb24_buffer,3*width*height,1,fp_bmp);
    fclose(fp_rgba32);
    fclose(fp_bmp);
    free(rgb24_buffer);
    printf("Finish generate %s!\n",bmppath);
    return 0;
}

int main(int argc, char* argv[])
{
    //All picture's resolution is 1280x720
    //Gray Bar, from 16 to 235
    for (int a=87; a<500; a++) {
        char buff[128] = {0};
        char buff1[128] = {0};
        sprintf(buff,"rgba_%d.rgba",a);
        sprintf(buff1,"rgba_%d.bmp",a);
        rgba32_to_bmp(buff,buff1,3840,1920);
    }

    return 0;
}
/*
int main(int argc, char* argv[]) {


    FILE* FFF = fopen(argv[1],"r+");

    ClImage  bbb;
        bbb.width = 3840;
        bbb.height = 1920;
        bbb.channels = 3;
        bbb.imageData = new unsigned char[3840*1920*3];
        char rrr[4] = {0};

        for (size_t j = 0; j < 3840*1920; j++)
        {
            fread(rrr,4,1,FFF);
            bbb.imageData[j * 3 + 0] = rrr[0];
            bbb.imageData[j * 3 + 1] = rrr[1];
            bbb.imageData[j * 3 + 2] = rrr[2];
        }
        fclose(FFF);
        char buf[128] = {0};

                sprintf(buf,"%s.bmp",argv[1]);
                clSaveImage(buf,&bbb);
                getchar();
    return 0;
}*/
