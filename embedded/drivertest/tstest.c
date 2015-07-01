#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#define DEV_NAME "/dev/touchscreen-1wire"

int main(int argc, char *argv[])
{
	int fd;
	int ret;
	fd_set rfds;
	uint32_t key;

	fd = open(DEV_NAME, O_RDONLY);
	if(-1 == fd) goto Error;

	while(1)
	{
		FD_ZERO(&rfds);//清空监听句柄
		FD_SET(fd, &rfds); //使监听句柄与文件描述符进行关联
		//只对读进行监听，而且不考虑超时
		if(-1 == select(fd + 1, &rfds, NULL, NULL, NULL))
			goto Error;
		ret = read(fd, &key, sizeof(key));
		if(-1 == ret) goto Error;
		if(ret == 0) continue;
		fprintf(stdout, "%s : (%03d, %03d)\n", 
						(key & 0x80000000 ? "DOWN" : " UP "),
						(key >> 16) & 0x7FFF, key & 0xFFFF);
	}
	close(fd);
	return EXIT_SUCCESS;
Error:
	perror("Backlight Error");
	if(-1 != fd) close(fd);
	return errno;
}

