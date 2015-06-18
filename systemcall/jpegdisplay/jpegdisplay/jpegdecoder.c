#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>

#include <jpegdecoder.h>

typedef union{
	uint16_t rgb;
	struct{
		uint16_t b : 5;
		uint16_t g : 6;
		uint16_t r : 5;
	}colors;
}Color;

static void rgb24To565(PRgb565 dst, const PRgb24 src, size_t n)
{
	size_t i;
	Color *p = (Color *)dst;

	for(i = 0; i < n; i++)
	{
		p[i].colors.r = src[i][0] >> 3;
		p[i].colors.g = src[i][1] >> 2;
		p[i].colors.b = src[i][2] >> 3;
	}
}

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

	METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

int jpegDecoder(const char *fileName, Pixmap *map)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	PRgb565 pRgb565;
	PRgb24  pRgb24;

	if(NULL == fileName)
		return JPEGERR_OPEN;
	if(sampleFormat != RGB565 && sampleFormat != RGB24)
		return JPEGERR_INVFMT;
	map->fileName = fileName;
	map->sampleFormat = sampleFormat;
	if ((infile = fopen(fileName, "rb")) == NULL) {
		return JPEGERR_OPEN;
	}
	/* Step 1: allocate and initialize JPEG decompression object */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return JPEGERR_DECODER;
	}
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */
	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */
	jpeg_read_header(&cinfo, TRUE);

	/* Step 4: set parameters for decompression */
	/* Step 5: Start decompressor */
	jpeg_start_decompress(&cinfo);
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	/* Step 5.1: Set Pixmap struct */
	map->width = cinfo.output_width;
	map->height = cinfo.output_height;
	map->sampleBuffer = malloc(map->width * map->height *
			map->sampleFormat / 8);
	if(NULL == map->sampleBuffer)
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return JPEGERR_NOMEM;
	}
	pRgb565 = (PRgb565)map->sampleBuffer;
	pRgb24 = (PRgb24)map->sampleBuffer;

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		if(RGB24 == map->sampleFormat)
		{
			memcpy(pRgb24, buffer[0], row_stride);
			pRgb24 += map->width;
		}else
		{
			rgb24To565(pRgb565, (PRgb24)(buffer[0]), map->width);
			pRgb565 += map->width;
		}
	}

	/* Step 7: Finish decompression */
	(void) jpeg_finish_decompress(&cinfo);

	/* Step 8: Release JPEG decompression object */
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return JPEGERR_SUCCESS;
}

int main(int argc, char *argv[])
{
	Pixmap a;
	jpegDecoder("a.jpg", 16, &a);
	fprintf(stdout, "%d*%d %d", a.width, a.height, a.sampleFormat);

	return 0;
}

