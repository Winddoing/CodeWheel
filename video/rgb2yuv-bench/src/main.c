#include <time.h>

#include "rgb2yuv.h"
#include <stdlib.h>
#include <stdio.h>

#define PIXELS_COUNT	(100 * 1000 * 1000)

static uint8_t pixels_tables[4 * PIXELS_COUNT];
static uint8_t pixels_tables64[4 * PIXELS_COUNT];
static uint8_t pixels_gstreamer[4 * PIXELS_COUNT];

int main(int argc, char *argv[])
{
	struct timespec now;
	clock_t start, finish;

	VideoConvert convert = {
			.width = PIXELS_COUNT,
			.cmatrix = {
					{  47,  157,  16,  4096 },
					{ -26,  -87, 112, 32768 },
					{ 112, -102, -10, 32768 },
			}
	};

	clock_gettime(CLOCK_REALTIME, &now);
	srand(now.tv_nsec);

	// prepare input buffers

	int i;
	for (i = 0; i < 4 * PIXELS_COUNT; i++)
	{
		uint8_t x = ((i % 4) * rand());
		pixels_tables[i] = x;
		pixels_tables64[i] = x;
		pixels_gstreamer[i] = x;
	}

	// run conversations

	start = clock();
	rgb2yuv_tables_init(&convert);
	rgb2yuv_tables(&convert, pixels_tables);
	finish = clock();
	printf("rgb2yuv_tables: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);

	start = clock();
	rgb2yuv_tables64_init(&convert);
	rgb2yuv_tables64(&convert, pixels_tables64);
	finish = clock();
	printf("rgb2yuv_tables64: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);

	start = clock();
	rgb2yuv_gstreamer(&convert, pixels_gstreamer);
	finish = clock();
	printf("rgb2yuv_gstreamer: %.3f sec\n", (float) (finish - start) / CLOCKS_PER_SEC);

	for (i = 0; i < PIXELS_COUNT * 4; i++)
	{
		if (pixels_gstreamer[i] != pixels_tables[i]
                || pixels_gstreamer[i] != pixels_tables64[i])
			printf("Difference at %d: %d %d %d\n", i, pixels_gstreamer[i], pixels_tables[i], pixels_tables64[i]);
	}

	return 0;
}
