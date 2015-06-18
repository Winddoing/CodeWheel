#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_NAME "/dev/fb"

int main(int argc, char *argv[])
{
	int i, j, fd;
	uint8_t r = 0x00;
	uint8_t g = 0xFF;
	uint8_t b = 0x00;
	uint16_t rgb = 0;

	rgb = ((r & 0xF8) << 8) | 
			((g & 0xFC) << 3) | 
			(b >> 3);
	fd = open(DEV_NAME, O_WRONLY);
	for(i = 0; i < 1024; i++ )
	{
		for(j = 0; j < 768; j++)
		{
			if(-1 == write(fd, &rgb, sizeof(rgb)))
				goto Error;
		}
	}
	close(fd);

	return EXIT_SUCCESS;
Error:
	perror("DF");
	close(fd);
	return EXIT_SUCCESS;
}

