#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <linux/micro2440_leds.h>

#define LEDS_DEVICE  "/dev/led4s"

int main()
{
	int fd,led_control,led_state;
	struct leds_stat oneStat;
	char *data;   //定义一个指针用于指向QUERY_STRING存放的内容
	fd = open(LEDS_DEVICE, O_RDWR);  //打开led设备
	if(-1 == fd)
	{
		perror("open led device");
		exit(1);
	}
	printf("Content-type: text/html\n\n");
	printf("<html>\n");
	printf("<head><title>cgi led demo</title></head>\n");
	printf("<body>\n");
	printf("<p>led is setted successful! you can watch the led's change</p>\n");
	printf("<p><a herf=index.html><button>get back</button></a></p>\n");
	printf("</body>\n");

	data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 
	if(sscanf(data,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
	{  	//利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
		printf("<p>please input right"); 
		printf("</p>");
	} 
	printf("<p>led_control = %d,led_state =  %d</p>", led_control, led_state);
	if(led_control>3) 
	{ 
		printf("<p>Please input 0<=led_control<=3!"); 
		printf("</p>");
   	} 

	if(led_state>1) 
	{
	   	printf("<p>Please input 0<=led_state<=1!"); 
		printf("</p>"); 
	}
	oneStat.led_nr = led_control;
   	oneStat.led_st = led_state;
	if(-1 == ioctl(fd, LEDS_SET_ONE, &oneStat))
	{
		perror("ioctl Device:" LEDS_DEVICE);
		exit(0);
	}	
	//close(fd);
	printf("</html>\n");
	
	exit(0);
}
