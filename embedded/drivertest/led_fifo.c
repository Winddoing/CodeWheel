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

#include <linux/micro2440_leds.h>

#define MYFIFO "/www/myfifo"
#define LEDS_DEVICE  "/dev/led4s"
#define MAX_BUFFER_SIZE PIPE_BUF

int main(int argc, char* argv[])
{
	int led_fd, fifo_fd, led_control,led_state, nread;
	struct leds_stat oneStat;
	char buff[MAX_BUFFER_SIZE];  //定义一个指针用于指向QUERY_STRING存放的内容

	led_fd = open(LEDS_DEVICE, O_RDWR);  //打开led设备
	if(-1 == led_fd)
	{
		perror("open led device");
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
	fifo_fd = open(MYFIFO, O_RDONLY);
	if(-1 == fifo_fd)
	{
		perror("open fifo file error\n");
		exit(1);
	}

	while(1)
	{
		//memset(buff, 0, sizeof(buff));
		if((nread = read(fifo_fd, buff, sizeof(buff))) < 0)
		{
			perror("Read data error");
			exit(1);
		}
		if(sscanf(buff,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
		{  	//利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
			printf("please input right \n"); 
			exit(1);
		} 
		if(led_control>3) 
		{ 
			printf("Please input 0<=led_control<=3!"); 
			exit(1);
		} 

		if(led_state>1) 
		{
			printf("Please input 0<=led_state<=1!"); 
			exit(1);
		}
		oneStat.led_nr = led_control;
		oneStat.led_st = led_state;
		ioctl(led_fd, LEDS_SET_ONE, &oneStat); 
	}

	close(led_fd);
	close(fifo_fd);

	return 0;
}
