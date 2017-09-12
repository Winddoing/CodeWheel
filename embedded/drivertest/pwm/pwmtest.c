#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEV_NAME "/dev/pwm"

int main(int argc, char *argv[])
{
	unsigned int freq = 2000;
	unsigned int n = 0;
	int fd;
	char c;


	fd = open(DEV_NAME, O_RDWR);
	if(-1 == fd)
	{
		perror("Open");
		return EXIT_FAILURE;
	}
	while(1)
	{
		fprintf(stderr, "+/-/q :");
		c = getchar();
		switch(c){
			case '+':
				freq += 400;
				break;
			case '-':
				freq -= 400;
				break;
			case 'q':
				break;
		}
		if('q' == c)
			break;
		fprintf(stdout, "set freq to %dHz\n", freq);
		ioctl(fd, 1, &freq);
	}
	freq = 0;
	ioctl(fd, 1, &freq);

	close(fd);

	return EXIT_SUCCESS;
}

