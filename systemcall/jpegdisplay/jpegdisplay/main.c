#include <stdlib.h>
#include <stdio.h>
#include <fbdevice.h>
#include <jpegdecoder.h>
#include <painter.h>

int main(int argc, char *argv[])
{
	FbInfo info;
	Pixmap map;

	initFbDevice(NULL, &info);
	jpegDecoder("a.jpg", info.args.format, &map);
	drawPixmap(&info, &map);


//void destroyDecoder(Pixmap *map);




	destoryFbDevice(&info);
	return EXIT_SUCCESS;
}

