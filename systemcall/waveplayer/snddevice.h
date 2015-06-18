#ifndef SNDDEVICE_H
#define SNDDEVICE_H

#include <stdint.h>

#define DEF_DEVICE_NAME "/dev/audio"

typedef struct{
	 const char *devName;
	int fd;
	struct{
		int rate;
		int bits;
		int channels;
	}args;
	uint8_t *buffer;
	uint32_t length;
}SndPlayer;

int openSndDevice(const char *devName, SndPlayer *player);
int setSndDevice(const SndPlayer *player);
int writeSndDevice(const SndPlayer *player);
int closeSndDevice(SndPlayer *player);

#define SNDERR_SUCCESS	0
#define SNDERR_OPENDEV	11
#define SNDERR_INVAARG	12
#define SNDERR_WRITEDA	13
#define SNDERR_SPECREE  14


#endif //SNDDEVICE_H

