#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<error.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ioctl.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>




#define LED1 1
#define LED2 3

#define LED_ON 1
#define LED_OFF 0

struct pollfd gecfd[1];


void gec_sighandler(int signo)
{
	printf("gec_sighandler!\n");

}




int main(void)
{
	
	int fd, len, result;
	char key;
	unsigned long ret;

	fd = open("/dev/char_dev",O_RDWR);
	if(fd < 0){
		perror("open failed.");
		return -1;
	}

#if 0
	gecfd[0].fd = fd;
	gecfd[0].events = POLLIN | POLLERR;

	while(1)
	{
		ret  =  poll(gecfd, 1, 5000);
	   if(ret < 0)
	   	{
			perror("poll\n");
	   	}
	   else if(ret ==0 )
	   	{
		   	printf("timeout!\n");
	   	}
		else if(ret & POLLIN)
		{
			printf("coming!\n");
			read(fd,&key,1);
			printf("key = %d \n",key);
			
		}

		ioctl(fd,LED1,LED_ON);
		//........
		
	}

#endif

#if 0

       fd_set rfds;
       struct timeval tv;
       int retval;

	while(1)
		{
       /* Watch stdin (fd 0) to see when it has input. */
       FD_ZERO(&rfds);
       FD_SET(fd, &rfds);

       /* Wait up to five seconds. */
       tv.tv_sec = 5;
       tv.tv_usec = 0;

       retval = select(fd+1, &rfds, NULL, NULL, &tv);
       /* Don¡¯t rely on the value of tv now! */

       if (retval == -1)
           perror("select()");
       else if (retval)
       	{
           printf("Data is available now.\n");
			read(fd,&key,1);
			printf("key = %d \n",key);		   
       	}
           /* FD_ISSET(0, &rfds) will be true. */
       else
           printf("No data within five seconds.\n");
		}
#endif


#if 1
	signal(SIGIO, gec_sighandler);
	fcntl(fd,F_SETOWN,getpid());
	fcntl(fd,F_SETFL, O_ASYNC | fcntl(fd,F_GETFL));

	while(1);
#endif
	
#if 0
	while(1)
	{
		ioctl(fd,LED1,LED_OFF);
		ioctl(fd,LED2,LED_OFF);

		sleep(1);
		
		ioctl(fd,LED1,LED_ON);
		ioctl(fd,LED2,LED_ON);
		
		sleep(1);
	}
#endif

while(1)
{
	read(fd,&key,1);
    printf("key = %d \n",key);
}

	close(fd);

	return 0;
}
