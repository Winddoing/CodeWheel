/**
 * @file rgb2yuv_wiki.c
 *
 * RGB2YUV from gstreamer
 */

#include "rgb2yuv.h"

#define SCALE    (8)
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

void rgb2yuv_gstreamer(VideoConvert *convert, uint8_t *pixels)
{
	int i;
	int r, g, b;
	int y, u, v;
	uint8_t *p = pixels;

	for (i = 0; i < convert->width; i++)
	{
		r = p[i * 4 + 1];
		g = p[i * 4 + 2];
		b = p[i * 4 + 3];

		y = (convert->cmatrix[0][0] * r + convert->cmatrix[0][1] * g
				+ convert->cmatrix[0][2] * b + convert->cmatrix[0][3]) >> SCALE;
		u = (convert->cmatrix[1][0] * r + convert->cmatrix[1][1] * g
				+ convert->cmatrix[1][2] * b + convert->cmatrix[1][3]) >> SCALE;
		v = (convert->cmatrix[2][0] * r + convert->cmatrix[2][1] * g
				+ convert->cmatrix[2][2] * b + convert->cmatrix[2][3]) >> SCALE;

		p[i * 4 + 1] = CLAMP(y, 0, 255);
		p[i * 4 + 2] = CLAMP(u, 0, 255);
		p[i * 4 + 3] = CLAMP(v, 0, 255);
	}
}
