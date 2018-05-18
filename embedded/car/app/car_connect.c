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

#define MAX_BUFFER_SIZE 20

int main()
{
	char* data;   //定义一个指针用于指向QUERY_STRING存放的内容
	char buff[MAX_BUFFER_SIZE];
	int fd, nwrite;

	printf("Content-type:text/html\n\n");
	data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 
	if(sscanf(data,"control=%s",buff)!=1)
	{   //利用sscnaf（）函数的特点将环境变量分别提取出l298_control和l298_state这两个值
		printf("please input right \n");
		exit(1);
	}

	fd = open(MYFIFO, O_WRONLY);
	if(-1 == fd)
	{
		printf("Open fifo file error\n");
		exit(1);
	}

	if((nwrite = write(fd, buff, sizeof(buff))) < 0)
	{
		printf("\nWrite data error\n");
		exit(1);
	}
	close(fd);

	return 0;
}
