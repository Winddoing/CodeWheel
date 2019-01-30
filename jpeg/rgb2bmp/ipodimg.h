
#ifndef __IPOD_IMGHDR__
#define __IPOD_IMGHDR__

typedef struct {
	int		width;		// image width in pixels
	int		height;		// image height in pixels
	int		rowsize;		// row size in bytes
	int		pixelformat; 	// 0x01:monochrome, 0x02:RGB565 little endian, 0x03:RGB565 big endian
	unsigned char *imagebits;	// image bits (dword right aligned)
} IMAGEHDR, *PIMAGEHDR;

#endif //__IPOD_IMGHDR__
