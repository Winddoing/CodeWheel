#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include <linux/micro2440_l298.h>

#define MYFIFO "/www/myfifo"
#define L298_DEVICE  "/dev/mycar"
#define MAX_BUFFER_SIZE 20

int main(int argc, char* argv[])
{
	int l298_fd, fifo_fd, nread;
	char buff[MAX_BUFFER_SIZE];  //定义一个指针用于指向QUERY_STRING存放的内容

	l298_fd = open(L298_DEVICE, O_RDWR);  //打开l298设备
	if(-1 == l298_fd)
	{
		perror("open l298 device");
		exit(1);
	}

	if(-1 == access(MYFIFO, F_OK))
	{
		if((mkfifo(MYFIFO, 0666) < 0) && (errno != EEXIST))
		{
			printf("Connot create fifo file\n");
			exit(1);
		}
	}

	while(1)
	{
		if(-1 == (fifo_fd = open(MYFIFO, O_RDONLY)))
		{
			perror("open fifo file error\n");
			exit(1);
		}
		if((nread = read(fifo_fd, buff, sizeof(buff))) < 0)
		{
			perror("Read data error");
			exit(1);
		}
		//fprintf(stderr, "---------%s-", buff);

		if(!strcmp(buff, "UP")) 
		{ 
			if(-1 == ioctl(l298_fd, CAR_GO_UP))
			{   
				perror("Ioctl Device: " L298_DEVICE);
				close(l298_fd);
				return errno;
			}   
		} 
		if(!strcmp(buff, "DOWN")) 
		{ 
			if(-1 == ioctl(l298_fd, CAR_GO_DOWN))
			{   
				perror("Ioctl Device: " L298_DEVICE);
				close(l298_fd);
				return errno;
			}   
		} 
		if(!strcmp(buff, "LEFT")) 
		{ 
			if(-1 == ioctl(l298_fd, CAR_GO_LEFT))
			{   
				perror("Ioctl Device: " L298_DEVICE);
				close(l298_fd);
				return errno;
			}   
		} 
		if(!strcmp(buff, "RIGHT")) 
		{ 
			if(-1 == ioctl(l298_fd, CAR_GO_RIGHT))
			{   
				perror("Ioctl Device: " L298_DEVICE);
				close(l298_fd);
				return errno;
			}   
		} 
		if(!strcmp(buff, "STOP")) 
		{ 
			if(-1 == ioctl(l298_fd, CAR_GO_STOP))
			{   
				perror("Ioctl Device: " L298_DEVICE);
				close(l298_fd);
				return errno;
			}   
		}
		memset(buff, 0, sizeof(buff));
		close(fifo_fd);
	}
	close(l298_fd);

	return 0;
}
