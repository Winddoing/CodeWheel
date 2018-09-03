/**
 * @file rgb2yuv_tables64.c
 *
 * Modified version RGB2YUV from Novell-ch to use tables instead of multiply
 * and all components in one 64-bit number
 */

#include "rgb2yuv.h"

static int64_t t_r3[256], t_g3[256], t_b3[256];

void rgb2yuv_tables64_init(VideoConvert *convert)
{
	int i, j;
	int k_r[] = { convert->cmatrix[0][0], convert->cmatrix[1][0], convert->cmatrix[2][0] };
	int k_g[] = { convert->cmatrix[0][1], convert->cmatrix[1][1], convert->cmatrix[2][1] };
	int k_b[] = { convert->cmatrix[0][2], convert->cmatrix[1][2], convert->cmatrix[2][2] };

	for (i = 0; i <= 255; i++)
	{
		int64_t r = 0, g = 0, b = 0;
		for (j = 0; j <= 2; j++)
		{
			r = (r << 16) + k_r[j] * i;
			g = (g << 16) + k_g[j] * i;
			b = (b << 16) + k_b[j] * i;
		}
		t_r3[i] = r;
		t_g3[i] = g;
		t_b3[i] = b;
	}
}

void rgb2yuv_tables64(VideoConvert *convert, uint8_t *pixels)
{
	int t;
	uint8_t r, g, b;
	int64_t c = ((int64_t) convert->cmatrix[0][3] << 32)
			+ ((int64_t) convert->cmatrix[1][3] << 16)
			+ ((int64_t) convert->cmatrix[2][3] << 0);

	for (t = 0; t < convert->width * 4; t += 4)
	{
		r = pixels[t + 1];
		g = pixels[t + 2];
		b = pixels[t + 3];

		int64_t x3 = t_r3[r] + t_g3[g] + t_b3[b] + c;

		pixels[t + 1] = x3 >> 40;
		pixels[t + 2] = x3 >> 24;
		pixels[t + 3] = x3 >> 8;
	}
}
