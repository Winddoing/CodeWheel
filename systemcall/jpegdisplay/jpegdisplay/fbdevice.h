#ifndef FBDEVICE_H
#define FBDEVICE_H

#include <stdint.h>
#include <sys/types.h>

#define DEF_DEVICENAME "/dev/fb"

#define ERRADDR ((void *)-1)

#define ERROR_FBSUCCESS	0
#define ERROR_FBOPEN	1
#define ERROR_FBIOCTL	2
#define ERROR_FBMMAP	3

typedef struct
{
	const char *deviceName;
	int fd;
	struct {
		uint16_t width;
		uint16_t height;
		uint16_t format;
	}args;
	size_t mapSize;
	void * mapAddr;
	int errNumber;
}FbInfo;

int initFbDevice(const char *deviceName, FbInfo *info);
void destoryFbDevice(FbInfo *info);
const char * fbErrorString(const FbInfo *info);
#endif // FBDEVICE_H

