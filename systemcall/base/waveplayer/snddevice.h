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
	int  error;
}SndPlayer;

int openSndDevice(const char *devName, SndPlayer *player);
int setSndDevice(SndPlayer *player);
int writeSndDevice(SndPlayer *player);
int closeSndDevice(SndPlayer *player);
const char *sndErrorString(const SndPlayer *player);

#define SNDERR_SUCCESS	0
#define SNDERR_OPENDEV	11
#define SNDERR_INVAARG	12
#define SNDERR_WRITEDA	13


#endif //SNDDEVICE_H

