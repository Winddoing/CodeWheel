#include <stdio.h>  
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <termios.h>  
#include <stdlib.h>  


int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);  
int open_port(int fd, int comport);  

int main(void)  
{  
	int fd;  
	int nread , nwrite;  
	int i;  
	char buff[8];  
	fd_set rd;  

	if((fd = open_port(fd,1)) < 0)  
	{  
		perror("open_port error");  
		return ;  
	}  

	if((i = set_opt(fd,115200,8,'N',1)) < 0)  
	{  
		perror("set_opt error");  
		return ;  
	}  

	FD_ZERO(&rd);  
	FD_SET(fd,&rd);  
	while(FD_ISSET(fd,&rd))  
	{  
		if(select(fd+1,&rd,NULL,NULL,NULL) < 0){  
			perror("select");  
		}  
		else{  
			while((nread = read(fd,buff,8)) > 0)  
			{  
				printf("nread=%d,%s/n",nread,buff);  
			}  
		}  
	}  
	close(fd);  
	return ;  
}  
