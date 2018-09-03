/**
 * @file rgb2yuv_tables.c
 *
 * Modified version RGB2YUV from Novell-ch to use tables instead of multiply
 */

#include "rgb2yuv.h"

#define RGB2Y(R, G, B, C) ((t00[R] + t01[G] + t02[B] + C) >> 8)
#define RGB2U(R, G, B, C) ((t10[R] + t11[G] + t12[B] + C) >> 8)
#define RGB2V(R, G, B, C) ((t20[R] + t21[G] + t22[B] + C) >> 8)

static int t00[256], t01[256], t02[256];
static int t10[256], t11[256], t12[256];
static int t20[256], t21[256], t22[256];

void rgb2yuv_tables_init(VideoConvert *convert)
{
	int i;

	for (i = 0; i <= 255; i++)
	{
		t00[i] = i * convert->cmatrix[0][0];
		t01[i] = i * convert->cmatrix[0][1];
		t02[i] = i * convert->cmatrix[0][2];
		t10[i] = i * convert->cmatrix[1][0];
		t11[i] = i * convert->cmatrix[1][1];
		t12[i] = i * convert->cmatrix[1][2];
		t20[i] = i * convert->cmatrix[2][0];
		t21[i] = i * convert->cmatrix[2][1];
		t22[i] = i * convert->cmatrix[2][2];
	}
}

void rgb2yuv_tables(VideoConvert *convert, uint8_t *pixels)
{
	int t;
	int r, g, b;
	int c0 = convert->cmatrix[0][3];
	int c1 = convert->cmatrix[1][3];
	int c2 = convert->cmatrix[2][3];

	for (t = 0; t < convert->width * 4; t += 4)
	{
		r = pixels[t + 1];
		g = pixels[t + 2];
		b = pixels[t + 3];
		pixels[t + 1] = RGB2Y(r,g,b,c0);
		pixels[t + 2] = RGB2U(r,g,b,c1);
		pixels[t + 3] = RGB2V(r,g,b,c2);
	}
}
