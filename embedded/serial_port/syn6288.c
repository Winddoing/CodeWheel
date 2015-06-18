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
int Speaker(int fd, char * pString);

int main(void)  
{  
	int fd;  
	int nwrite;  
	int i;  
	char buff[]="hello\n";  

	if((fd = open_port(fd,2)) < 0)  
	{  
		perror("open_port error");  
		return ;  
	}  

	if((i = set_opt(fd,9600,8,'N',1)) < 0)  
	{  
		perror("set_opt error");  
		return ;  
	}  

	printf("fd=%d\n",fd);  
//	nwrite = write(fd,buff,8);  
	if(Speaker(fd, buff) != 0)
	{
		perror("speaker error");
		return;
	}
	printf("nwrite=%d\n",nwrite);  
	close(fd);  
	return ;  
}  
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)  
{  
	struct termios newtio;  
	struct termios oldtio;  

	if(tcgetattr(fd,&oldtio) != 0)  
	{  
		perror("SetupSerial 2");  
		return -1;  
	}  

	bzero(&newtio,sizeof(newtio));  
	newtio.c_cflag |= CLOCAL |CREAD;  
	newtio.c_cflag &= ~CSIZE;  

	switch(nBits)  
	{  
		case 7:  
			newtio.c_cflag |= CS7;  
			break;  
		case 8:  
			newtio.c_cflag |= CS8;  
			break;      
	}  

	switch(nEvent)  
	{  
		case 'O':  
			newtio.c_cflag |= PARENB;  
			newtio.c_cflag |= PARODD;  
			newtio.c_iflag |= (INPCK | ISTRIP);  
			break;  
		case 'E':  
			newtio.c_iflag |= (INPCK |ISTRIP);  
			newtio.c_cflag |= PARENB;  
			newtio.c_cflag &= ~PARODD;  
			break;  
		case 'N':  
			newtio.c_cflag &= ~PARENB;  
			break;  
	}  

	switch(nSpeed)  
	{  
		case 2400:  
			cfsetispeed(&newtio,B2400);  
			cfsetospeed(&newtio,B2400);  
			break;  
		case 4800:  
			cfsetispeed(&newtio,B4800);  
			cfsetospeed(&newtio,B4800);  
			break;  
		case 9600:  
			cfsetispeed(&newtio,B9600);  
			cfsetospeed(&newtio,B9600);  
			break;  
		case 115200:  
			cfsetispeed(&newtio,B115200);  
			cfsetospeed(&newtio,B115200);  
			break;  
		case 460800:  
			cfsetispeed(&newtio,B460800);  
			cfsetospeed(&newtio,B460800);  
			break;            
		default:  
			cfsetispeed(&newtio,B9600);  
			cfsetospeed(&newtio,B9600);  
			break;  
	}  

	if(nStop == 1){  
		newtio.c_cflag &= ~CSTOPB;  
	}  
	else if(nStop ==2){  
		newtio.c_cflag |= CSTOPB;  
	}  
	newtio.c_cc[VTIME] = 0;  
	newtio.c_cc[VMIN] = 0;  

	tcflush(fd,TCIFLUSH);  
	if((tcsetattr(fd,TCSANOW,&newtio)) != 0)  
	{  
		perror("com set error");  
		return -1;  
	}  
	printf("set done!\n");  
	return 0;  
}  

int open_port(int fd,int comport)  
{  
	char *dev[]={"/dev/ttySAC0","/dev/ttySAC1","/dev/ttySAC2"};  
	long vdisable;  
	if(comport == 1)  
	{  
		fd = open("/dev/ttySAC0",O_RDWR|O_NOCTTY|O_NDELAY);  
		if(fd == -1){  
			perror("Can't Open Serial Port");  
			return -1;  
		}  
	}  

	else if(comport == 2)  
	{  
		fd = open("/dev/ttySAC1",O_RDWR|O_NOCTTY|O_NDELAY);  
		if(fd == -1){  
			perror("Can't Open Serial Port");  
			return -1;  
		}  
	}  

	else if(comport == 3)  
	{  
		fd = open("/dev/ttySAC2",O_RDWR|O_NOCTTY|O_NDELAY);  
		if(fd == -1){  
			perror("Can't Open Serial Port");  
			return -1;  
		}  
	}  

	if(fcntl(fd,F_SETFL,0) < 0){  
		printf("fcntl failed\n");  
	}  
	else{  
		printf("fcntl=%d\n",fcntl(fd,F_SETFL,0));  
	}  

	if(isatty(STDIN_FILENO) == 0){  
		printf("standard input is not a terminal device\n");  
	}  
	else{  
		printf("isatty sucess!\n");  
	}  

	printf("fd-open=%d\n",fd);  
	return fd;  
}  


//语音播报程序  
int Speaker(int fd, char * pString)
{  
	char headOfFrame[5];  
	char length;   //定义字符串长度  
	char ecc[] = 0;     //定义校验字节  
	int i = 0;   
	if (pString == NULL) //空字符串   
		return 1;    /*****************发送过程**********************/     
	headOfFrame[0]=0XFD;   //构造帧头FD  
	headOfFrame[1]=0X00;   //构造数据区长度的高字节 
	length = strlen(pString);  //需要发送文本的长度  
	headOfFrame[2]=length+3;//构造数据区长度的低字节  
	headOfFrame[3]=0X01; //构造命令字：合成播放命令  
	headOfFrame[4]=0X00; //构造命令参数：编码格式为GB2312   
	for(i=0;i<5;i++)        //依次发送构造好的5个帧头字节  
	{    
		ecc=ecc^(headOfFrame[i]); //对发送的字节进行异或校验   
	//	write(fd,headOfFrame[i],8);  
		//USART_SendByte(headOfFrame[i]);   
	}
	write(fd,headOfFrame,8);  
	for(i=0;i<length;i++)       //依次发送待合成的文本数据   
	{
		ecc=ecc^(*pString);   
		//write(fd,*pString,8);  
		//USART_SendByte(*pString);   
		pString ++;   
	}
	write(fd,pString,8);  
	write(fd,ecc,8);  
	//USART_SendByte(ecc);    //最后发送校验和
	return 0; //成功返回0 
}   //语音读整数 
