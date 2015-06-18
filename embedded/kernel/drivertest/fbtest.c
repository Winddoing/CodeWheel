#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define FB_DEVICE "/dev/fb0"
#define WIDTH 320
#define HEIGHT 240
#define COLOR 0xFFFF

int main(int argc, char *argv[])
{
	int fd, i;
	unsigned short *fbp;

	if(-1 == (fd = open(FB_DEVICE, O_RDWR)))
	{
		perror("Open Device: " FB_DEVICE);
		return errno;
	}
	fbp = (unsigned short *)mmap(NULL, WIDTH * HEIGHT * 
					sizeof(unsigned short),
					PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if((void *)-1 == fbp)
	{
		perror("Mmap Device: " FB_DEVICE);
		close(fd);
		return errno;
	}
	//draw top line
	memset(fbp, COLOR, WIDTH * sizeof(unsigned short));
	//draw bottom line
	memcpy(fbp + (HEIGHT - 1) * WIDTH, fbp,
					WIDTH * sizeof(unsigned short));
	for(i = 1; i < HEIGHT - 1; i++)
	{
		memset(fbp + i * WIDTH, COLOR, 2 * sizeof(unsigned short));
	}
	munmap(fbp, WIDTH * HEIGHT * sizeof(unsigned short));

	close(fd);
	return 0;
}
