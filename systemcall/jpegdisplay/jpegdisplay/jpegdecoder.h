#ifndef JPEGDECODER_H
#define JPEGDECODER_H

#include <stdint.h>

#define JPEGERR_SUCCESS	0
#define JPEGERR_OPEN	1
#define JPEGERR_DECODER	2
#define JPEGERR_NOMEM	3
#define JPEGERR_INVFMT	4

typedef uint16_t * PRgb565;
typedef uint8_t (*PRgb24)[3];

typedef struct{
	const char* fileName;
#define RGB565	16
#define RGB24	24
	int sampleFormat;
	uint16_t width;
	uint16_t height;
	void *sampleBuffer;
}Pixmap;

int jpegDecoder(const char *fileName, int sampleFormat, Pixmap *map);
void destroyDecoder(Pixmap *map);

#endif // JPEGDECODER_H

