/*#############################################################
 *     File Name	: YUVtoRGB.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月23日 星期六 11时42分52秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

/* 验证 */

void YUV444TOYUV420(char *inbuf,  char *outbuf, int w, int h)
{
    char *srcY = NULL, *srcU = NULL, *srcV = NULL;
    char *desY = NULL, *desU = NULL, *desV = NULL;

    srcY = inbuf;           //Y
    srcU = srcY + w * h;    //U
    srcV = srcU + w * h;    //V

    desY = outbuf;
    desU = desY + w * h;
    desV = desU + w * h / 4;

    int half_width = w / 2;
    int half_height = h / 2;
    //Y
    memcpy(desY, srcY, w * h * sizeof(unsigned char));
    //UV
    for (int i = 0; i < half_height; i++) {
        for (int j = 0; j < half_width; j++) {
            *desU = *srcU;
            *desV = *srcV;
            desU++;
            desV++;
            srcU += 2;
            srcV += 2;
        }
        srcU = srcU + w;
        srcV = srcV + w;
    }
}



/******************************************************************************/

#define CLIP(color) (unsigned char)((color>0xFF)?0xff:((color<0)?0:color))

void YUV420toRGB (unsigned char *src, unsigned char *dst, int width, int height,
        int flipUV, int ColSpace)
{
    unsigned char *Y;
    unsigned char *V;
    unsigned char *U;
    int y1, y2, u, v;
    int v1=0, v2=0, u1=0, u2=0;
    unsigned char *pty1, *pty2;
    int i, j;
    unsigned char *RGB1, *RGB2;
    int r, g, b;

    //Initialization
    Y = src;
    V = Y + width * height;
    U = Y + width * height + width * height / 4;

    pty1 = Y;
    pty2 = pty1 + width;
    RGB1 = dst;
    RGB2 = RGB1 + 3 * width;
    for (j = 0; j < height; j += 2) {
        //printf ("process line %d\n",j);
        for (i = 0; i < width; i += 2) {
            if (flipUV) {
                u = (*V++) - 128;
                v = (*U++) - 128;
            } else {
                v = (*V++) - 128;
                u = (*U++) - 128;
            }
            switch (ColSpace) {
                // M$ color space
            case 0:
                {
                    v1 = ((v << 10) + (v << 9) + (v << 6) + (v << 5)) >> 10;	// 1.593
                    u1 = ((u << 8) + (u << 7) + (u << 4)) >> 10;	//         0.390
                    v2 = ((v << 9) + (v << 4)) >> 10;	//                0.515
                    u2 = ((u << 11) + (u << 4)) >> 10;	//               2.015
                }
                break;
                // PAL specific
            case 1:
                {
                    v1 = ((v << 10) + (v << 7) + (v << 4)) >> 10;	//      1.1406
                    u1 = ((u << 8) + (u << 7) + (u << 4) + (u << 3)) >> 10;	// 0.3984
                    v2 = ((v << 9) + (v << 6) + (v << 4) + (v << 1)) >> 10;	// 0.5800
                    u2 = ((u << 11) + (u << 5)) >> 10;	//              2.0312
                }
                break;
                // V4l2
            case 2:
                {
                    v1 = ((v << 10) + (v << 8) + (v << 7) + (v << 5)) >> 10;	//       1.406
                    u1 = ((u << 8) + (u << 6) + (u << 5)) >> 10;	//                0.343
                    v2 = ((v << 9) + (v << 7) + (v << 6) + (v << 5)) >> 10;	//        0.718
                    u2 = ((u << 10) + (u << 9) + (u << 8) + (u << 4) + (u << 3)) >> 10;	// 1.773
                }
                break;
            case 3:
                {
                    v1 = u1 = v2 = u2 = 0;
                }
                break;
            default:
                break;

            }
            //up-left

            y1 = (*pty1++);
            if (y1 > 0) {
                r = y1 + (v1);
                g = y1 - (u1) - (v2);
                b = y1 + (u2);

                r = CLIP (r);
                g = CLIP (g);
                b = CLIP (b);


            } else {
                r = g = b = 0;
            }
            *RGB1++ = r;
            *RGB1++ = g;
            *RGB1++ = b;

            //down-left


            y2 = (*pty2++);
            if (y2 > 0) {
                r = y2 + (v1);
                g = y2 - (u1) - (v2);
                b = y2 + (u2);

                r = CLIP (r);
                g = CLIP (g);
                b = CLIP (b);
            } else {
                r = b = g = 0;
            }
            *RGB2++ = r;
            *RGB2++ = g;
            *RGB2++ = b;

            //up-right
            y1 = (*pty1++);

            if (y1 > 0) {
                r = y1 + (v1);
                g = y1 - (u1) - (v2);
                b = y1 + (u2);

                r = CLIP (r);
                g = CLIP (g);
                b = CLIP (b);
            } else {
                r = g = b = 0;
            }

            *RGB1++ = r;
            *RGB1++ = g;
            *RGB1++ = b;

            //down-right
            y2 = (*pty2++);
            if (y2 > 0) {
                r = y2 + (v1);
                g = y2 - (u1) - (v2);
                b = y2 + (u2);

                r = CLIP (r);
                g = CLIP (g);
                b = CLIP (b);
            } else {
                r = b = g = 0;
            }

            *RGB2++ = r;
            *RGB2++ = g;
            *RGB2++ = b;
        }
        RGB1 += 3 * width;
        RGB2 += 3 * width;
        pty1 += width;
        pty2 += width;
    }

    //printf ("done YUV420 -> RGB \n");
}



#define CLIP(color) (unsigned char)(((color)>0xFF)?0xff:(((color)<0)?0:(color)))

/* translate RGB24 to YUV444 in input */
static void RGB_2_444 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, size;
    UINT8 R, G, B;
    INT32 Y, Cb, Cr;
    size = image_width * image_height;
    for (i = size; i > 0; i--) {
        B = input_ptr[0];
        G = input_ptr[1];
        R = input_ptr[2];

        //input_ptr -= 3;
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        Cb = CLIP (((-43 * R - 85 * G + 128 * B) >> 8) + 128);
        Cr = CLIP (((128 * R - 107 * G - 21 * B) >> 8) + 128);
        *input_ptr++ = (UINT8) Y;
        *input_ptr++ = (UINT8) Cb;
        *input_ptr++ = (UINT8) Cr;
    }
}


/* translate RGB24 to YUV422 in input */
static void RGB_2_422 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, size;
    UINT8 R, G, B, R1, G1, B1;
    INT32 Y, Yp, Cb, Cr;
    UINT8 * inbuf = input_ptr;
    size = image_width * image_height;
    for (i = size; i > 0; i--) {
        B = inbuf[0];
        G = inbuf[1];
        R = inbuf[2];
        B1 = inbuf[3];
        G1 = inbuf[4];
        R1 = inbuf[5];
        inbuf += 6;
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        Yp = CLIP ((77 * R1 + 150 * G1 + 29 * B1) >> 8);
        Cb = CLIP (((-43 * R - 85 * G + 128 * B) >> 8) + 128);
        Cr = CLIP (((128 * R - 107 * G - 21 * B) >> 8) + 128);
        *input_ptr++ = (UINT8) Y;
        *input_ptr++ = (UINT8) Cb;
        *input_ptr++ = (UINT8) Yp;
        *input_ptr++ = (UINT8) Cr;
    }
}


/* translate RGB24 to YUV420 in input */
static void RGB_2_420 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, j, size;
    UINT8 R, G, B, R1, G1, B1, Rd, Gd, Bd, Rd1, Gd1, Bd1;
    INT32 Y, Yd, Y11, Yd1, Cb, Cr;
    UINT8 * inbuf = input_ptr;
    UINT8 * inbuf1 = input_ptr + (image_width * 3);
    size = image_width * image_height >> 2;
    for (i = size, j = 0; i > 0; i--) {
        B = inbuf[0];
        G = inbuf[1];
        R = inbuf[2];
        B1 = inbuf[3];
        G1 = inbuf[4];
        R1 = inbuf[5];
        Bd = inbuf1[0];
        Gd = inbuf1[1];
        Rd = inbuf1[2];
        Bd1 = inbuf1[3];
        Gd1 = inbuf1[4];
        Rd1 = inbuf1[5];
        inbuf += 6;
        inbuf1 += 6;
        j++;
        if (j >= image_width / 2) {
            j = 0;
            inbuf += (image_width * 3);
            inbuf1 += (image_width * 3);
        }
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        Y11 = CLIP ((77 * R1 + 150 * G1 + 29 * B1) >> 8);
        Yd = CLIP ((77 * Rd + 150 * Gd + 29 * Bd) >> 8);
        Yd1 = CLIP ((77 * Rd1 + 150 * Gd1 + 29 * Bd1) >> 8);
        Cb = CLIP (((-43 * R - 85 * G + 128 * B) >> 8) + 128);
        Cr = CLIP (((128 * R - 107 * G - 21 * B) >> 8) + 128);
        *input_ptr++ = (UINT8) Y;
        *input_ptr++ = (UINT8) Y11;
        *input_ptr++ = (UINT8) Yd;
        *input_ptr++ = (UINT8) Yd1;
        *input_ptr++ = (UINT8) Cb;
        *input_ptr++ = (UINT8) Cr;
    }
}


/* translate RGB32 to YUV420 in input */
static void RGB32_2_420 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, j, size;
    UINT8 R, G, B, R1, G1, B1, Rd, Gd, Bd, Rd1, Gd1, Bd1;
    INT32 Y, Yd, Y11, Yd1, Cb, Cr;
    UINT8 * inbuf = input_ptr;
    UINT8 * inbuf1 = input_ptr + (image_width * 4);
    size = image_width * image_height >> 2;
    for (i = size, j = 0; i > 0; i--) {
        B = inbuf[0];
        G = inbuf[1];
        R = inbuf[2];
        B1 = inbuf[4];
        G1 = inbuf[5];
        R1 = inbuf[6];
        Bd = inbuf1[0];
        Gd = inbuf1[1];
        Rd = inbuf1[2];
        Bd1 = inbuf1[4];
        Gd1 = inbuf1[5];
        Rd1 = inbuf1[6];
        inbuf += 8;
        inbuf1 += 8;
        j++;
        if (j >= image_width / 2) {
            j = 0;
            inbuf += (image_width * 4);
            inbuf1 += (image_width * 4);
        }
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        Y11 = CLIP ((77 * R1 + 150 * G1 + 29 * B1) >> 8);
        Yd = CLIP ((77 * Rd + 150 * Gd + 29 * Bd) >> 8);
        Yd1 = CLIP ((77 * Rd1 + 150 * Gd1 + 29 * Bd1) >> 8);
        Cb = CLIP (((-43 * R - 85 * G + 128 * B) >> 8) + 128);
        Cr = CLIP (((128 * R - 107 * G - 21 * B) >> 8) + 128);
        *input_ptr++ = (UINT8) Y;
        *input_ptr++ = (UINT8) Y11;
        *input_ptr++ = (UINT8) Yd;
        *input_ptr++ = (UINT8) Yd1;
        *input_ptr++ = (UINT8) Cb;
        *input_ptr++ = (UINT8) Cr;
    }
}


/* translate RGB565 to YUV420 in input */
static void RGB565_2_420 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, j, size;
    UINT8 R, G, B, R1, G1, B1, Rd, Gd, Bd, Rd1, Gd1, Bd1;
    INT32 Y, Yd, Y11, Yd1, Cb, Cr;
    Myrgb16 * inbuf = (Myrgb16 *) input_ptr;
    Myrgb16 * inbuf1 = inbuf + (image_width);
    size = image_width * image_height >> 2;
    for (i = size, j = 0; i > 0; i--) {

        /*
           B = inbuf[0] & 0xf8;
           G = ((inbuf[0] & 0x07) << 5) | ((inbuf[1] & 0xe0) >> 3);
           R = (inbuf[1] & 0x1f) << 3;

           B1 = inbuf[2] & 0xf8;
           G1 = ((inbuf[2] & 0x07) << 5) | ((inbuf[3] & 0xe0) >> 3);
           R1 = (inbuf[3] & 0x1f) << 3;

           Bd = inbuf1[0] & 0xf8;
           Gd = ((inbuf1[0] & 0x07) << 5) | ((inbuf1[1] & 0xe0) >> 3);
           Rd = (inbuf1[1] & 0x1f) << 3;

           Bd1 = inbuf1[2] & 0xf8;
           Gd1 = ((inbuf1[2] & 0x07) << 5) | ((inbuf1[3] & 0xe0) >> 3);
           Rd1 = (inbuf1[3] & 0x1f) << 3;
           */
        B = inbuf[0].blue << 3;
        G = inbuf[0].green << 2;
        R = inbuf[0].red << 3;
        B1 = inbuf[1].blue << 3;
        G1 = inbuf[1].green << 2;
        R1 = inbuf[1].red << 3;
        Bd = inbuf1[0].blue << 3;
        Gd = inbuf1[0].green << 2;
        Rd = inbuf1[0].red << 3;
        Bd1 = inbuf1[1].blue << 3;
        Gd1 = inbuf[1].green << 2;
        Rd1 = inbuf[1].red << 3;
        inbuf += 2;
        inbuf1 += 2;
        j++;
        if (j >= image_width / 2) {
            j = 0;
            inbuf += (image_width);
            inbuf1 += (image_width);
        }
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        Y11 = CLIP ((77 * R1 + 150 * G1 + 29 * B1) >> 8);
        Yd = CLIP ((77 * Rd + 150 * Gd + 29 * Bd) >> 8);
        Yd1 = CLIP ((77 * Rd1 + 150 * Gd1 + 29 * Bd1) >> 8);
        Cb = CLIP (((-43 * R - 85 * G + 128 * B) >> 8) + 128);
        Cr = CLIP (((128 * R - 107 * G - 21 * B) >> 8) + 128);
        *input_ptr++ = (UINT8) Y;
        *input_ptr++ = (UINT8) Y11;
        *input_ptr++ = (UINT8) Yd;
        *input_ptr++ = (UINT8) Yd1;
        *input_ptr++ = (UINT8) Cb;
        *input_ptr++ = (UINT8) Cr;
    }
}

static void RGB_2_400 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, size;
    UINT8 R, G, B;
    INT32 Y;
    UINT8 * inbuf = input_ptr;
    size = image_width * image_height;
    for (i = size; i > 0; i--) {
        B = inbuf[0];
        G = inbuf[1];
        R = inbuf[2];
        inbuf += 3;
        Y = CLIP ((77 * R + 150 * G + 29 * B) >> 8);
        *input_ptr++ = (UINT8) Y;
    }
}


/* translate YUV444P to YUV444 in input */
static void YUV_2_444 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, size;
    UINT8 * Ytmp = NULL;
    UINT8 * Cbtmp = NULL;
    UINT8 * Crtmp = NULL;
    UINT8 * Buff = NULL;
    Buff =
        (UINT8 *) realloc ((UINT8 *) Buff,
                (size_t) (image_width * image_height * 3));
    if (Buff) {
        memcpy (Buff, input_ptr, image_width * image_height * 3);
        Ytmp = Buff;
        Cbtmp = Buff + image_width * image_height;
        Crtmp = Buff + (image_width * image_height << 1);
        size = image_width * image_height;
        for (i = size; i > 0; i--) {
            *input_ptr++ = (UINT8) * Ytmp++;
            *input_ptr++ = (UINT8) * Cbtmp++;
            *input_ptr++ = (UINT8) * Crtmp++;
        }
        free (Buff);
        Buff = NULL;
    }
}


/* translate YUV422P to YUV422 in input */
static void YUV_2_422 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 i, size;
    UINT8 * Ytmp = NULL;
    UINT8 * Cbtmp = NULL;
    UINT8 * Crtmp = NULL;
    UINT8 * Buff = NULL;
    Buff =
        (UINT8 *) realloc ((UINT8 *) Buff,
                (size_t) (image_width * image_height * 2));
    if (Buff) {
        memcpy (Buff, input_ptr, image_width * image_height * 2);
        Ytmp = Buff;
        Cbtmp = Buff + image_width * image_height;
        Crtmp = Cbtmp + (image_width * image_height >> 1);
        size = image_width * image_height;
        for (i = size; i > 0; i--) {
            *input_ptr++ = (UINT8) * Ytmp++;
            *input_ptr++ = (UINT8) * Cbtmp++;
            *input_ptr++ = (UINT8) * Ytmp++;
            *input_ptr++ = (UINT8) * Crtmp++;
        }
        free (Buff);
        Buff = NULL;
    }
}


/* translate YUV420P to YUV420 in input */
static void YUV_2_420 (UINT8 * input_ptr, UINT8 * output_ptr, UINT32 image_width,
        UINT32 image_height)
{
    UINT32 x, y, size;
    UINT8 * Ytmp = NULL;
    UINT8 * Y2tmp = NULL;
    UINT8 * Cbtmp = NULL;
    UINT8 * Crtmp = NULL;
    UINT8 * Buff = NULL;
    Buff =
        (UINT8 *) realloc ((UINT8 *) Buff,
                (size_t) ((image_width * image_height * 3) >> 1));
    if (Buff) {
        memcpy (Buff, input_ptr, (image_width * image_height * 3) >> 1);
        Ytmp = Buff;
        Y2tmp = Buff + image_width;
        Cbtmp = Buff + image_width * image_height;
        Crtmp = Cbtmp + (image_width * image_height >> 2);
        size = image_width * image_height >> 2;
        for (y = 0; y < image_height; y += 2) {
            for (x = 0; x < image_width; x += 2) {
                *input_ptr++ = (UINT8) * Ytmp++;
                *input_ptr++ = (UINT8) * Ytmp++;
                *input_ptr++ = (UINT8) * Y2tmp++;
                *input_ptr++ = (UINT8) * Y2tmp++;
                *input_ptr++ = (UINT8) * Cbtmp++;
                *input_ptr++ = (UINT8) * Crtmp++;
            }
            Ytmp += image_width;
            Y2tmp += image_width;
        }
        free (Buff);
        Buff = NULL;
    }
}

