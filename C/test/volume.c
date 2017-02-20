#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/soundcard.h>

#define DEVICE_NAME "/dev/audio"

int main(int argc, char *argv[])
{
	int sndFd;
	unsigned char vol[2] = {100, 100};

	if(-1 == (sndFd = open(DEVICE_NAME, O_RDWR)))
		goto Error;
	
	if(-1 == ioctl(sndFd,SOUND_MIXER_WRITE_VOLUME,
			(int *)vol))
		goto Error;
	close(sndFd);
	fprintf(stdout, "set volume to(100,100) success!\n");

	return EXIT_SUCCESS;
Error:
	perror("Error");
	close(sndFd);

	return EXIT_FAILURE;
}

