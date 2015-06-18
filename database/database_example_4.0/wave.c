#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <wave.h>

#include <linux/soundcard.h>

#define SOUND_DEVICE "/dev/audio"

static size_t getFileLength(int fd)
{
	struct stat st;

	fstat(fd, &st);

	return st.st_size;
}

int playWave(const WaveInfo *info)
{
	int fd;
	int t;

	fd = open(SOUND_DEVICE, O_WRONLY);
	if(-1 == fd) return -1;
	
	if(-1 == ioctl(fd, SNDCTL_DSP_RESET))
		goto setSndDeviceError;
	t = info->head->rate;
	if(-1 == ioctl(fd, SNDCTL_DSP_SPEED, &t))
		goto setSndDeviceError;
	t = info->head->bits;
	if(-1 == ioctl(fd, SNDCTL_DSP_SETFMT, &t))
		goto setSndDeviceError;
	t = info->head->channels;
	if(-1 == ioctl(fd, SNDCTL_DSP_CHANNELS, &t))
		goto setSndDeviceError;
	if(-1 == write(fd, info->data, info->fileLength))
		goto setSndDeviceError;
	close(fd);
	return 0;
setSndDeviceError:
	perror("Play");
	close(fd);
	return -1;
}
int getWaveFile(const char *fileName, WaveInfo *info)
{
	info->fd = open(fileName, O_RDONLY);
	if(-1 == info->fd)
		return -1;
	info->fileLength = getFileLength(info->fd);
	info->head = (WaveHead *)mmap(NULL, info->fileLength, 
					PROT_READ, MAP_PRIVATE, info->fd, 0);
	if((void *)-1 == info->head)
	{
		close(info->fd);
		return -1;
	}
	info->fileName = fileName;
	info->data = (char *)(info->head + 1);
	return 0;
}

void freeWaveFile(WaveInfo *info)
{
	if(-1 == info->fd) return;
	if((WaveHead *) -1 != info->head)
		munmap(info->head, info->fileLength);
	close(info->fd);
	info->fd = -1;
	info->head = (WaveHead *)-1;
}

