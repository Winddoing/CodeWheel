#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>
#include <sys/types.h>

typedef struct{
	char 		riffFlag[4];
	uint32_t 	fileLength;
	char		waveFlag[4];
	char		fmtFlag[4];
	char		unused[4];
	uint16_t	fmtType;
	uint16_t	channels;
	uint16_t	rate;
	uint32_t	bps;
	uint16_t	blockSize;
	uint16_t	bits;
	char		dataFlag[4];
	uint32_t	voiceLength;
}WaveHead;

typedef struct {
	const char *fileName;
	int fd;
	size_t fileLength;
	WaveHead *head;
	char * data;
}WaveInfo;

int playWave(const WaveInfo *info);
int getWaveFile(const char *fileName, WaveInfo *info);
void freeWaveFile(WaveInfo *info);

#endif // WAVE_H

