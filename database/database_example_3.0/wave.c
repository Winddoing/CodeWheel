#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <wave.h>

#define WAVE_FLAG "wave"

static size_t getFileLength(int fd)
{
	struct stat st;

	fstat(fd, &st);

	return st.st_size;
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

