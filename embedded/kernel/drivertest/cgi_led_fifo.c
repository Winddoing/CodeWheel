#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <limits.h>
#include <string.h>

#define MYFIFO "/www/myfifo"
#define MAX_BUFFER_SIZE PIPE_BUF

int main()
{
	char* data;   //定义一个指针用于指向QUERY_STRING存放的内容
	char buff[MAX_BUFFER_SIZE];
	int fd, nwrite;
	int led_control, led_state;
	
	printf("Content-type: text/html\n\n");
	printf("<html>\n");
	printf("<head><title>cgi led demo</title></head>\n");
	printf("<body>\n");
	printf("<p>led is setted successful! you can watch the led's change</p>\n");
	printf("<p><a herf=index.html><button>get back</button></a></p>\n");
	printf("</body>\n");

	data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 
	strcpy(buff, data);
	fd = open(MYFIFO, O_WRONLY);
	if(-1 == fd)
	{
		printf("Open fifo file error\n");
		exit(1);
	}
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx<p>please input right%s\n", buff); 
	if((nwrite = write(fd, buff, sizeof(buff))) < 0)
	{
		printf("\nWrite data error\n");
		exit(1);
	}
	printf("---%d-----\n", nwrite);
	if(sscanf(buff,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
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
	close(fd);
	printf("</html>\n");
	
	return 0;
}
