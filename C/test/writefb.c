#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FB_DEVICE "/dev/fb"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define ERRORARG(s) {fprintf(stderr,\
			   	"Invalid color: %s\n",\
				(s)); return EXIT_FAILURE;}

typedef union{
	uint16_t rgb;
	struct{
		uint16_t b : 5;
		uint16_t g : 6;
		uint16_t r : 5;
	}colors;
}Color;

int main(int argc, char *argv[])
{
	int fd;
	uint8_t r, g, b;
	Color color;
	char *endptr;
	int i;
	
	if(argc != 4)
	{
		fprintf(stderr, 
			"Usage:%s <r> <g> <b>\n",
			argv[0]);
		return EXIT_FAILURE;
	}
	r = strtol(argv[1], &endptr, 10);
	if(*endptr != '\0') ERRORARG(argv[1]);
	g = strtol(argv[2], &endptr, 10);
	if(*endptr != '\0') ERRORARG(argv[2]);
	b = strtol(argv[3], &endptr, 10);
	if(*endptr != '\0') ERRORARG(argv[3]);

	color.colors.r = r >> 3;
	color.colors.g = g >> 2;
	color.colors.b = b >> 3;
	
	fd = open(FB_DEVICE, O_WRONLY);
	if(-1 == fd) goto Error;
	for(i = 0; i < SCREEN_WIDTH 
				* SCREEN_HEIGHT; i++)
	{
		if(-1 == write(fd, &color, 
				sizeof(color)))
			goto Error;
	}
	close(fd);

	return EXIT_SUCCESS;
Error:
	perror("WriteFb");
	close(fd);
	return EXIT_FAILURE;
}

