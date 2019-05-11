#ifndef __JPEG_DCT_H__
#define __JPEG_DCT_H__

#define JPEG_BLOCK_SIZE 8

void fastDCT_x86(unsigned char *image_data, short *dct_data);
void fastIDCTInit(short block[64], short dst[64]);
void fastIDCT(short data[JPEG_BLOCK_SIZE * JPEG_BLOCK_SIZE]);

#endif//__JPEG_DCT_H__
