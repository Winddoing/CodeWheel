#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"

#include "ipodimg.h"

#ifdef RELEASE_BUILD
#ifdef DBG_MSG
	#undef DBG_MSG
	#define DBG_MSG(_fmt, args...) 0
#endif
#endif

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

#define MAX_FILENAME_LEN	256
#define OS_FILE_SEPARATOR   "/\\"

typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef long		LONG;

#define SWAP16(n)		( ((n & 0xff00) >> 8) |  ((n & 0xff) << 8) )

#pragma pack(1)

typedef struct {
	/* BITMAPFILEHEADER*/
	BYTE	bfType[2];
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
	/* BITMAPINFOHEADER*/
	DWORD	BiSize;
	LONG	BiWidth;
	LONG	BiHeight;
	WORD	BiPlanes;
	WORD	BiBitCount;
	DWORD	BiCompression;
	DWORD	BiSizeImage;
	LONG	BiXpelsPerMeter;
	LONG	BiYpelsPerMeter;
	DWORD	BiClrUsed;
	DWORD	BiClrImportant;
} BMPHEAD;

typedef struct {
	BYTE B;
	BYTE G;
	BYTE R;
} TRGB, *PTRGB;

#pragma pack()

#define RGB2PIXEL565(r,g,b)	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

#define RGB2GRAY(r,g,b) 		((unsigned int) (0.299 * (r) + 0.587 * (g) + 0.114 * (b)) )

//#define GRAY2MONO2BPP(gray)	((gray & 0xC0) ? 3 : (gray & 0x30) ? 2 : (gray & 0x0C) ? 1 : (gray & 3) ? 0 : 0)
#define GRAY2MONO2BPP(gray)	((gray & 0xC0) >> 6)

#define CHECK_RANGE(in, min, max)	((in >= min) && (in <= max))

#define SET76BIT_BYTE(byteval, in)		( (( in & 3) << 6) | byteval ) 
#define SET54BIT_BYTE(byteval, in)		( (( in & 3) << 4) | byteval )
#define SET32BIT_BYTE(byteval, in)		( (( in & 3) << 2) | byteval )
#define SET10BIT_BYTE(byteval, in)		( (( in & 3) | byteval ) )

int Create24bppBmp(FILE *fpin, FILE *fpout, int width, int height);
int WriteBmpFile(char *infilename, char *outfilename);
void usage();
int PrintBmpHeader(char *name);

char * StripPath(char *buffer)
{
	char *p = buffer;
	while( 1 )
	{
		char *p1 = p;
		p1 = strpbrk( p, OS_FILE_SEPARATOR );
		if( p1 )
		{
			p = p1+1;
		}
		else
		{
			break;
		}
	}
	return(p);
}

void usage()
{
	printf("\nUsage: rgb2bmp -f <pure rgb file>\n");
	printf("-r Reverse rows \n");
	printf("-p <input pixel format> 2:RGB565 little endian, 3:RGB565 big endian \n");	
	printf("-i Invert color\n");
	printf("e.g.\n\t$ rgb2bmp -f mylogo.dat -r -p 2\n\n");
}

static int sReverseRowFlag = 0;
static int sDumpFlag = 0;
static int sPixelFormat = 2;  // 0x01:monochrome, 0x02:RGB565 little endian, 0x03:RGB565 big endian
static int sInvert2bpp = 0;
static int sPrintBmpHdr = 0;

typedef struct tagPixelStruct {
	int id;
	char *str;
} TPixelStruct, *PTPixelStruct;

#define MAX_PIXEL_FORMAT 		3

TPixelStruct sPixelFormatString[MAX_PIXEL_FORMAT] =
{
	{1, "Input format: monochrome 2bpp \n"},
	{2, "Input format: RGB565 little endian \n"},
	{3, "Input format: RGB565 big endian \n"},
};

int main(int argc, char *argv[])
{
	char name[MAX_FILENAME_LEN];
	char oname[MAX_FILENAME_LEN];
	int ch;
	char *p;
	int i;

	memset(name, 0, sizeof name);
	memset(oname, 0, sizeof oname);
	
	while ((ch = getopt(argc, argv, "df:ip:rz")) != -1) {
		switch (ch) {
			case 'd':
				sDumpFlag = 1;
				printf("option: Dumping, all other options ignored!\n");
				break;
			case 'f':
//				printf("Input file: (%s)\n", optarg);
				strcpy(name, optarg);

				strcpy(oname, optarg);				
				if (NULL != (p = strrchr(oname, '.'))) {
					*p = '\0';
				}
				strcat(oname, ".bmp");
				
				break;
			case 'i':
				sInvert2bpp = 1;
				break;
			case 'p':
				sPixelFormat = atoi(optarg);
				break;
			case 'r':
				sReverseRowFlag = 1;
				printf("option: Reverse rows\n");
				break;
			case 'z':
				sPrintBmpHdr = 1;
				break;
			case '?':
			default:
				usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (!name[0]) {
		usage();
		return -1;
	}

	if (sPrintBmpHdr) {
		PrintBmpHeader(name);
		return 0;
	}
	
	if (CHECK_RANGE(sPixelFormat, 1, 3)) { 
		for (i = 0; i < MAX_PIXEL_FORMAT; i++) {
			if (sPixelFormatString[i].id == sPixelFormat) {
				printf("%s \n", sPixelFormatString[i].str);
				break;
			}
		}
	}
	else {
		fprintf(stderr, "\nInvalid input pixel format specified!\n\n");
		usage();
	}
	
	if (WriteBmpFile(name, oname) < 0) {
		fprintf(stderr, "Error: WriteBmpFile\n");
	}

	return 0;
}

int PrintBmpHeader(char *name)
{
	BMPHEAD bmp;
	FILE *fp;

	if((fp = fopen(name, "rb")) == (FILE *)NULL) {
		fprintf(stderr, "Can't open file\n");
		return -1;
	}
		
	if ((fread(&bmp, 1, sizeof(BMPHEAD), fp)) != sizeof(BMPHEAD)) {
		fprintf(stderr, "Can't read file\n");
		return -1;
	}

	printf("\nBMP Header info for:%s\n", name);
	/* BITMAPFILEHEADER*/
	printf("bfType[2]:%c%c\n", bmp.bfType[0], bmp.bfType[1]);
	printf("bfSize:%d\n", (int)bmp.bfSize);
	printf("bfReserved1:%d\n", (int)bmp.bfReserved1);
	printf("bfReserved2:%d\n", (int)bmp.bfReserved2);
	printf("bfOffBits:%d\n", (int)bmp.bfOffBits);
	/* BITMAPINFOHEADER*/
	printf("BiSize:%d\n", (int)bmp.BiSize);
	printf("BiWidth:%d\n", (int)bmp.BiWidth);
	printf("BiHeight:%d\n", (int)bmp.BiHeight);
	printf("BiPlanes:%d\n", (int)bmp.BiPlanes);
	printf("BiBitCount:%d\n", (int)bmp.BiBitCount);
	printf("BiCompression:%d\n", (int)bmp.BiCompression);
	printf("BiSizeImage:%d\n", (int)bmp.BiSizeImage);
	printf("BiXpelsPerMeter:%d\n", (int)bmp.BiXpelsPerMeter);
	printf("BiYpelsPerMeter:%d\n", (int)bmp.BiYpelsPerMeter);
	printf("BiClrUsed:%d\n", (int)bmp.BiClrUsed);
	printf("BiClrImportant:%d\n\n", (int)bmp.BiClrImportant);

	fclose(fp);
	return 0;
}

int WriteBmpFile(char *infilename, char *outfilename)
{
	char namebuf[MAX_FILENAME_LEN];
	char *p;
	char *name;
	FILE *fp;
	FILE *fpout;

	strcpy(namebuf, infilename);
	if((p = strrchr(namebuf, '.')) != (char *)NULL)
		*p = '\0';

	name = StripPath(namebuf);
	
	if((fp = fopen(infilename, "rb")) == (FILE *)NULL) {
		fprintf(stderr, "Can't open file: %s\n", infilename);
		return 1;
	}
	
	if((fpout = fopen(outfilename, "w")) == (FILE *)NULL) {
		fclose(fpout);
		fprintf(stderr, "Could not open output file %s\n", outfilename);
		return 1;
	}

	if(Create24bppBmp(fp, fpout, 100, 100)) {
		fprintf(stderr, "Conversion failed: %s\n", infilename);
		fclose(fp);
		return 1;
	}
	
	printf("Output %s created!\n\n", outfilename);

	fclose(fpout);
	fclose(fp);

	return 0;
}

int LineSwap16(int cx, UCHAR *linebuffer)
{
	WORD *pos16;
	int i;

	pos16 = (WORD *) linebuffer;

	for (i = 0; i < cx; i++) {
		pos16[i] = SWAP16(pos16[i]);
	}

	return 0;
}

int Rgb565To888(int x, UCHAR *linebuffer, int linesize, UCHAR *line16buffer, int line16size, int bigEndian)
{
	TRGB *pos;
	WORD *pos16;
	WORD val;
	int i;

	pos = (TRGB *) linebuffer;
	pos16 = (WORD *) line16buffer;

	if (bigEndian) {
		for (i = 0; i < x; i++) {
			val = sInvert2bpp ? ~pos16[i] : pos16[i];
			pos[i].R = ((val >> 11) & 0x1F) << 3;
			pos[i].G = ((val >> 5) & 0x3F) << 2;
			pos[i].B = (val & 0x1F) << 3;
		}
	}
	else {
		for (i = 0; i < x; i++) {
			val = sInvert2bpp ? ~pos16[i] : pos16[i];
			val = SWAP16(val);
			pos[i].R = ((val >> 11) & 0x1F) << 3;
			pos[i].G = ((val >> 5) & 0x3F) << 2;
			pos[i].B = (val & 0x1F) << 3;
		}
	}	

	return 0;
}

int CopyLineBuffer(int row, UCHAR *destImage, UCHAR *srcLine, int lineLength)
{
	unsigned char *p_row_start;
	
	p_row_start = destImage + (row * lineLength);
	
	memcpy(p_row_start, srcLine, lineLength);
	
	return 0;
}

int Create24bppBmp(FILE *fpin, FILE *fpout, int width, int height)
{
	BMPHEAD bmp;
	UCHAR *line24buffer = NULL;
	UCHAR *imagebuffer = NULL, *linepos = NULL;
	int line24size = 0;
	int image_size, bitdepth;
	UCHAR *line16buffer = NULL;
	int line16size = 0, line16_x;
	int i;
	int row_num;

	// Fill BMP header
	memset(&bmp, 0, sizeof(BMPHEAD));

	bitdepth = 24;

	line24size = width * 3;
	line24size = (line24size+3) & ~3;
	line16_x = width * 2;
	line16size = width * 2;
	line16size = (line16size+3) & ~3;

	image_size = line24size * height;
	bmp.bfSize = sizeof(BMPHEAD) + image_size;
	
	bmp.bfType[0] = 'B';
	bmp.bfType[1] = 'M';
	bmp.BiWidth = width;
	bmp.BiHeight = height;
	bmp.BiPlanes = 1;
	bmp.BiBitCount = bitdepth;
	bmp.BiClrUsed = 1 << bmp.BiBitCount;
	bmp.bfOffBits = sizeof(BMPHEAD);
	bmp.BiSize = 40;
	bmp.BiSizeImage = image_size;

	// Write BMP header to file
	if (fwrite(&bmp, 1, sizeof(BMPHEAD), fpout) != sizeof(BMPHEAD)) {
		fprintf(stderr, "Error writing header");
	}

	// temp buffers
	imagebuffer = malloc(image_size);
	linepos = imagebuffer;
	line24buffer = malloc(line24size);
	line16buffer = malloc(line16size);

	for(i=0; i<height; i++) {
		memset(line16buffer, 0, line16size);
		memset(line24buffer, 0, line24size);
		// read sinle line from RGB565 data file
		if(fread(line16buffer, 1, line16_x, fpin) != (size_t)line16_x) {
			fprintf(stderr, "Error fread\n");
		}
		// copy from 16bpp RGB565 line to 24bpp RGB888 line
		Rgb565To888(width, line24buffer, line24size, line16buffer, line16size, (sPixelFormat == 3));

		// WIN BMP puts bitmaps reverse line order
		row_num = sReverseRowFlag ? (height - 1 - i) : (i);

		// Copy line to image buffer
		CopyLineBuffer( row_num, imagebuffer, line24buffer, line24size);			
	}

	// Dump image buffer to a file
	for(i=0; i<height; i++) {
		fwrite(imagebuffer + (i * line24size), 1, line24size, fpout);
	}

	free(line16buffer);
	free(line24buffer);
	free(imagebuffer);

	return 0;
}

