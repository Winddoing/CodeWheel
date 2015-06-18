#ifndef PAINTER_H
#define PAINTER_H

#include <fbdevice.h>
#include <jpegdecoder.h>

typedef struct{
	uint16_t width;
	uint16_t height;
}Size;
typedef struct{
	int x;
	int y;
}Point;
typedef struct{
	Point start;
	Size  size;
}Rect;

void drawPixmap(const FbInfo *info, const Pixmap *map);


#endif // PAINTER_H

