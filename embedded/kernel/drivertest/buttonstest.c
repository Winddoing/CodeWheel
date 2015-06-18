#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>

#define BUTTONS_DEVICE "/dev/buttons"

int main(int argc, char *argv[])
{
	int fd, ret;
	unsigned char keyValue;
	fd_set rfds;

	if(-1 == (fd = open(BUTTONS_DEVICE, O_RDWR)))
	{
		perror("Open Device: " BUTTONS_DEVICE);

		fprintf(stdout, "xxxxxxxxxxxxxxxxxxx-------\n");
		return errno;
	}
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		ret = select(fd + 1, &rfds, NULL, NULL, NULL);
		if(ret < 0)
		{
			perror("Select Device: " BUTTONS_DEVICE);
			close(fd);
			return errno;
		}
		ret = read(fd, &keyValue, sizeof(keyValue));
		if(-1 == ret)
		{
			perror("Read Device: " BUTTONS_DEVICE);
			close(fd);
			return errno;
		}else if(ret != 0)
		{
			fprintf(stdout, "key '%c' is %s.\n",
					keyValue & 0x7F, ((keyValue & 0x80) ? 
							"Released" : "pressed"));
		}
		fprintf(stdout, "read next ---------------------\n");
	}
	close(fd);
	return 0;
}
