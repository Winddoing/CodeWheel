/*#############################################################
 *     File Name	: bmp-tst.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月30日 星期三 15时34分05秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

/* 位图文件头主要是对位图文件的一些描述
 * 位图信息头主要是对位图图像方面信息的描述 */
/*************** 位图标准信息(54字节) ****************************/
/* 位图文件头 （位图头的字节数 = 位图文件字节数 - 位图图像数据字节数）*/

#pragma pack(1)
typedef struct BMP_FILE_HEADER
{
    unsigned short bType;       // 文件标识符
    unsigned int   bSize;       // 文件的大小
    unsigned short bReserved1;  // 保留值,必须设置为0
    unsigned short bReserved2;  // 保留值,必须设置为0
    unsigned int   bOffset;     // 文件头的最后到图像数据位开始的偏移量
} BMP_HEADER;    // 14 字节
#pragma pack()

/* 位图信息头 */
#pragma pack(1)
typedef struct BMP_FILE_INFO
{
    unsigned int   bInfoSize;      // 信息头的大小
    unsigned int   bWidth;         // 图像的宽度
    unsigned int   bHeight;        // 图像的高度
    unsigned short bPlanes;        // 图像的位面数
    unsigned short biBitCount;      // 每个像素的位数
    unsigned int   bCompression;   // 压缩类型
    unsigned int   bmpImageSize;   // 图像的大小,以字节为单位
    unsigned int   bXPelsPerMeter; // 水平分辨率
    unsigned int   bYPelsPerMeter; // 垂直分辨率
    unsigned int   bClrUsed;       // 使用的色彩数
    unsigned int   bClrImportant;  // 重要的颜色数
} BMP_INFO;      // 40 字节
#pragma pack()

/* 彩色表:调色板 */
typedef struct RGB_QUAD
{
    unsigned char rgbBlue;     // 蓝色强度
    unsigned char rgbGreen;    // 绿色强度
    unsigned char rgbRed;      // 红色强度
    unsigned char rgbReversed; // 保留值
} RGB;

void show_bmp_hear(BMP_HEADER *_header)
{
    printf("\n===>BMPFileHeader:\n");
    printf("文件标识符 = 0X%X\n", _header->bType);
    printf("BMP 文件大小 = %d 字节\n", _header->bSize);
    printf("保留值1 = %d \n", _header->bReserved1);
    printf("保留值2 = %d \n", _header->bReserved2);
    printf("文件头的最后到图像数据位开始的偏移量 = %d 字节\n", _header->bOffset);
}

void show_bmp_info(BMP_INFO *_info)
{
    printf("\n===>BMPInfo:\n");
    printf("信息头的大小 = %d 字节\n", _info->bInfoSize);
    printf("位图的高度 = %d \n", _info->bHeight);
    printf("位图的宽度 = %d \n", _info->bWidth);
    printf("图像的位面数(位面数是调色板的数量,默认为1个调色板) = %d \n", _info->bPlanes);
    printf("每个像素的位数 = %d 位\n", _info->biBitCount);
    printf("压缩类型 = %d \n", _info->bCompression);
    printf("图像的大小 = %d 字节\n", _info->bmpImageSize);
    printf("水平分辨率 = %d \n", _info->bXPelsPerMeter);
    printf("垂直分辨率 = %d \n", _info->bYPelsPerMeter);
    printf("使用的色彩数 = %d \n", _info->bClrUsed);
    printf("重要的色彩数 = %d \n", _info->bClrImportant);

    printf("\n压缩说明：有0（不压缩），1（RLE 8，8位RLE压缩），2（RLE 4，4位RLE压缩，3（Bitfields，位域存放）\n");
}

#if 0
void SaveBmp(const char *filename,unsigned char *rgbbuf,int width,int height)
{
    BMP_HEADER bfh;
    BMP_INFO bih;

    /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*3;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    FILE *file = fopen(filename, "wb");
    if (!file)
    {
            printf("Could not write file\n");
            return;
        }

    /*Write headers*/
    fwrite(&bfType,sizeof(bfType),1,file);
    fwrite(&bfh,sizeof(bfh),1, file);
    fwrite(&bih,sizeof(bih),1, file);

    fwrite(rgbbuf,width*height*3,1,file);
    fclose(file);
}
#endif

int main(int argc, const char *argv[])
{
    int i = 0;
    FILE *in_fp;
    unsigned char buffer[5];

    BMP_HEADER bmp_header;
    BMP_INFO bmp_info;

    printf("===> func: %s, line: %d, argc=%d\n", __func__, __LINE__, argc);

    for (i = 0; i < argc; i++)
        printf("argv[%d]=%s", i, argv[i]);
    printf("\n");

    if((in_fp = fopen(argv[1], "rb")) == NULL) {
        printf("Cann't open the file!\n");
        return 0;
    }

    printf("BMP_HEADER sizeof = %ld, BMP_INFO sizeof = %ld\n",
            sizeof(BMP_HEADER), sizeof(BMP_INFO));

    fseek(in_fp, 0, SEEK_SET);
    fread(&bmp_header, sizeof(BMP_HEADER), 1, in_fp);

    fread(&bmp_info, sizeof(bmp_info), 1, in_fp);

    int cur_fp = 0;
    cur_fp = ftell(in_fp);
    printf("Current file pointer %d (BMP file header size 54 Byte)\n", cur_fp);

    show_bmp_hear(&bmp_header);
    show_bmp_info(&bmp_info);

    fseek(in_fp, bmp_header.bOffset, SEEK_SET);

    fclose(in_fp);

    return 0;
}
