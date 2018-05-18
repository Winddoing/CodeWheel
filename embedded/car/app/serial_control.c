#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>  
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>  
#include <errno.h>
#include <string.h>

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);  
int open_port(int fd, int comport);  
char Speaker(char CMD,char par,char * pString);
//char *change(char *s);
//void url_to_gb_or_utf(char *get_url, char *return_gb_or_utf);

#define MAX_BUFFER_SIZE 126
#define MYFIFO "/www/sport"
/*
   struct dataFamt{
   char pre1;
   char pre2;
   int length;
   char type1;
   char type2;
   char *data;
   int ecc;
   };
   void send(struct dataFamt* dft, char *text)
   {
   dft->pre1 = "FD";
   }
 */
int i, fd, fifo_fd, nread;  
int main(int argc, char* argv[])
{
	int tag = 1, typeNumber = 0;
	char type[30], text[100];
	char new_text[100];
	char buff[MAX_BUFFER_SIZE];  //定义一个指针用于指向QUERY_STRING存放的内容
	//struct dataFamt dataFt;

	if((fd = open_port(fd,2)) < 0)  
	{  
		perror("open_port error");  
		exit(1);
	}  

	if((i = set_opt(fd,9600,8,'N',1)) < 0)  
	{  
		perror("set_opt error");  
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
		fifo_fd = open(MYFIFO, O_RDONLY);
		if(-1 == fifo_fd)
		{
			perror("open fifo file error\n");
			exit(1);
		}
		bzero(buff, sizeof(buff));
		if((nread = read(fifo_fd, buff, sizeof(buff))) < 0)
		{
			perror("Read data error");
			exit(1);
		}
		fprintf(stdout, "%s\n", buff);
		sscanf(buff,"type=%[^&]&text=%s", type, text);
		/*
		if(sscanf(buff,"type=%[^&]&text=%[^&]", type, text)!=2)
		{   //利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
			fprintf(stdout, "----------------------%s\n", buff);
			printf("please input right \n"); 
			exit(1);
		}*/
		//fprintf(stdout, "type==%s\n\ntext==%s\n\n", type, text);	
		if(!strcmp(type, "text")) 
		{ 
			//	fprintf(stdout, "-------------\n");	
			//new_text = change(text);
			url_to_gb_or_utf(text, new_text);
			typeNumber = strlen(text);
			fprintf(stdout, "----%d---------\n", typeNumber);	
			//				printf("text=%s\n", new_text); 
			write(fd,new_text,typeNumber);  
			//		Speaker(1,1,new_text);
		}	 

		if(!strcmp(type, "steering")) 
		{
			//				printf("steering=%s\n", text); 
			typeNumber = strlen(text);
			fprintf(stdout, "xxxxxxxxxxxxxxxx%d\n", typeNumber);
			write(fd,text,typeNumber);  
			//Speaker(2,0,text);
		}
		memset(buff, 0, nread);//使用会使控制数据出错
		fprintf(stdout, "fifo===%d\n", buff);
		close(fifo_fd);
	}
	close(fd);

	return 0;
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
	//char *dev[]={"/dev/ttySAC0","/dev/ttySAC1","/dev/ttySAC2"};  
	//long vdisable;  
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


/*将十六进制数转为十进制*/
int hex_to_decade(char * s)
{
	char *digits="0123456789ABCDEF";

	/*判断大小写，小写的话转为大写，达到统一*/
	if (islower (s[0]))
		s[0]=toupper(s[0]);
	if (islower (s[1]))
		s[1]=toupper(s[1]);

	return 16*(strchr(digits,s[0])-strchr(digits,'0'))+(strchr(digits,s[1])-strchr(digits,'0'));
}

void url_to_gb_or_utf(char *get_url, char *return_gb_or_utf)
{
	int url_position;/*用来保存get_url的位置*/
	int return_position;/*用来保存解码后的字符串的位置*/
	int url_len;/*用来保存get_url的长度*/
	char tmp[3];/*保存%后面的十六进制字符*/
	url_len = strlen(get_url);
	return_position = 0;

	fprintf(stdout, "length=%d\n", url_len);
	for ( url_position = 0; url_position < url_len; )
	{
		/*如果是%将它后面的十六进制字符考到数组里*/
		if ( get_url[url_position] == '%' ){
			tmp[0] = get_url[url_position+1];/*第一个十六进制字符*/
			tmp[1] = get_url[url_position+2];/*第二个*/
			tmp[2] = '\0';

			url_position+= 3; /*使url_position跳到的下一个%*/
			/*将十六进制数转为十进制后考入要返回的数组里*/

			return_gb_or_utf[return_position] = hex_to_decade(tmp);
			fprintf(stdout, "------%d\n", return_gb_or_utf[return_position] );
		}
		/*如果不是特殊字符，如英文，数字那么直接返回*/
		else{
			return_gb_or_utf[return_position] = get_url[url_position];
			url_position++;
		}


		return_position++;
	}

	return_gb_or_utf[return_position] = 0;

}

char Speaker(char CMD,char par,char * pString)
{  
	char headOfFrame[5];  
	char length;   //定义字符串长度  
	char ecc = 0;     //定义校验字节  
	int i = 0;   
	if (pString == NULL) //空字符串   
		return 1;        
	headOfFrame[0]=0XFD;   //构造帧头FD  
	headOfFrame[1]=0X00;   //构造数据区长度的高字节 
	length = strlen(pString);  //需要发送文本的长度  
	headOfFrame[2]=length+3;//构造数据区长度的低字节  
	headOfFrame[3]=CMD; //构造命令字：合成播放命令  
	headOfFrame[4]=par; //构造命令参数：编码格式为GB2312   
	for(i=0;i<5;i++)        //依次发送构造好的5个帧头字节  
	{    
		ecc=ecc^(headOfFrame[i]); //对发送的字节进行异或校验   
		//	USART_SendByte(USART2,headOfFrame[i]);   
	}
	write(fd,headOfFrame,5);  
	for(i=0;i<length;i++)       //依次发送待合成的文本数据   
	{
		ecc=ecc^(*pString);   
		//	USART_SendByte(USART2,*pString);   
		pString ++;   
	}
	//	USART_SendByte(USART2,ecc);    //最后发送校验和
	write(fd,pString,length);

	//write(fd,ecc,8);  
	return 0; //成功返回0 
}   //语音读整数 

/*
   char *change(char *s)
   {
   char *new_name = malloc(25);
   char z[3] = {0};
//char a;
char *p;
//char *m;
int i = 0;
int k;
//char c;
p = s;
while(*p)
{
if(*p == '%')
{
z[0] = *(p+1);
z[1] = *(p+2);
sscanf(z,"%x",&k);
fprintf(stdout, "------%s\n", z);
new_name[i++] = k;
p = p+3;
}
else if(*p == '+')
{
new_name[i++] = ' ';
p++;
}
else
{
new_name[i++] = *p;
p++;
}
}
new_name[i++] = '\0';
fprintf(stdout, "--------%d\n", new_name);
return new_name;
}*/
