#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>

#define FALSE -1
#define TRUE 0

int UART_Open(char* port);
int UART_Close(int fd);
int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
int UART_Init(int fd, int speed,int flow_ctrlint ,int databits,int stopbits,char parity);
int UART_Recv(int fd, char *rcv_buf,int data_len, int timeout);
int UART_Send(int fd, char *send_buf,int data_len);

/*****************************************************************
 * 名称： UART0_Open
 * 功能： 打开串口并返回串口设备文件描述
 * 入口参数： fd :文件描述符 port :串口号(ttyS0,ttyS1,ttyS2)
 * 出口参数： 正确返回为1，错误返回为0
 *****************************************************************/
int UART_Open(char* port)
{
	int fd;
	fd = open( port, O_RDWR|O_NOCTTY   );//|O_NDELAY
	if (FALSE == fd){
		perror("Can't Open Serial Port");
		return(FALSE);
	}

	//判断串口的状态是否为阻塞状态
	if(fcntl(fd, F_SETFL, 0) < 0){
		printf("fcntl failed!\n");
		return(FALSE);
	} else {
		//    printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	}

	//测试是否为终端设备
	if(0 == isatty(STDIN_FILENO)){
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}

	return fd;
}

int  UART_Close(int fd)
{
	return close(fd);
}

/*******************************************************************
 * 名称： UART0_Set
 * 功能： 设置串口数据位，停止位和效验位
 * 入口参数： fd 串口文件描述符
 * speed 串口速度
 * flow_ctrl 数据流控制
 * databits 数据位 取值为 7 或者8
 * stopbits 停止位 取值为 1 或者2
 * parity 效验类型 取值为N,E,O,,S
 *出口参数： 正确返回为1，错误返回为0
 *******************************************************************/
int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
	int speed_arr[] = {
		B4000000, B576000, B115200,B38400, B19200, B9600, B4800, B2400, B1200, B300,
		B4000000, B576000, B115200,B38400, B19200, B9600, B4800, B2400, B1200, B300,
	};
	int name_arr[] = {
		4000000, 576000, 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
		4000000, 576000, 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300
	};
	struct termios options;
	int i;

	/*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数,还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
	*/
	if(tcgetattr( fd,&options) != 0){
		perror("SetupSerial 1");
		return(FALSE);
	}
	bzero(&options, sizeof(options));

	//设置串口输入波特率和输出波特率
	for(i= 0;i < sizeof(speed_arr) / sizeof(int);i++) {
		if (speed == name_arr[i]) {
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}

	//修改控制模式，保证程序不会占用串口
	options.c_cflag |= CLOCAL;
	//修改控制模式，使得能够从串口中读取输入数据
	options.c_cflag |= CREAD;
	//设置数据流控制
	switch(flow_ctrl){
	case 0 : //不使用流控制
		options.c_cflag &= ~CRTSCTS;
		break;
	case 1 : //使用硬件流控制
		options.c_cflag |= CRTSCTS;
		break;
	case 2 : //使用软件流控制
		options.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}
	//设置数据位
	options.c_cflag &= ~CSIZE; //屏蔽其他标志位
	switch (databits){
	case 5 :
		options.c_cflag |= CS5;
		break;
	case 6    :
		options.c_cflag |= CS6;
		break;
	case 7    :
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return (FALSE);
	}
	//设置校验位
	switch (parity) {
	case 'n':
	case 'N': //无奇偶校验位。
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O': //设置为奇校验
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E': //设置为偶校验
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's':
	case 'S': //设置为空格
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return (FALSE);
	}
	// 设置停止位
	switch (stopbits){
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return (FALSE);
	}
	//修改输出模式，原始数据输出
	options.c_oflag &= ~OPOST;
	//设置等待时间和最小接收字符
	options.c_cc[VTIME] = 0; /* 读取一个字符等待1*(1/10)s */
	options.c_cc[VMIN] = 0; /* 读取字符的最少个数为1 */

	//如果发生数据溢出，接收数据，但是不再读取
	tcflush(fd,TCIOFLUSH);

	//激活配置 (将修改后的termios数据设置到串口中）
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("com set error!/n");
		return (FALSE);
	}
	return (TRUE);
}


int UART_Init(int fd, int speed,int flow_ctrlint ,int databits,int stopbits,char parity)
{
	printf("jason: %s:%d\r\n", __FUNCTION__,__LINE__);
	//设置串口数据帧格式
	if (FALSE == UART_Set(fd,speed,flow_ctrlint,databits,stopbits,parity)) {
		return FALSE;
	} else {
		return TRUE;
	}
	write(fd, "FLUSH ERROR\n", sizeof("FLUSH ERROR\n"));
}



/*******************************************************************
 * 名称： UART0_Recv
 * 功能： 接收串口数据
 * 入口参数： fd :文件描述符
 * rcv_buf :接收串口中数据存入rcv_buf缓冲区中
 * data_len :一帧数据的长度
 * 出口参数： 正确返回为1，错误返回为0
 *******************************************************************/
int UART_Recv(int fd, char *rcv_buf,int data_len, int timeout)
{
	int len,fs_sel;
	fd_set fs_read;

	struct timeval time;

	FD_ZERO(&fs_read);
	FD_SET(fd,&fs_read);

	time.tv_sec = timeout;
	time.tv_usec = 0;

	//使用select实现串口的多路通信
	fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
	if(fs_sel){

		// printf("%s:%d  data_len=%d\n", __FUNCTION__,__LINE__, data_len);
		len = read(fd,rcv_buf,data_len);
		//  printf("%s:%d  len=%d\n", __FUNCTION__,__LINE__, len);
		return len;
	} else {
		return FALSE;
	}
}

/*******************************************************************
 * 名称： UART0_Send
 * 功能： 发送数据
 * 入口参数： fd :文件描述符
 * send_buf :存放串口发送数据
 * data_len :一帧数据的个数
 * 出口参数： 正确返回为1，错误返回为0
 *******************************************************************/
int UART_Send(int fd, char *send_buf,int data_len)
{
	int ret;

	ret = write(fd,send_buf,data_len);
	if (data_len == ret ){
		return ret;
	} else {
		tcflush(fd,TCOFLUSH);
		return FALSE;

	}

}

int main(int argc, char **argv)
{
/*	unsigned char send_str[] = "0123456789swq9876543210";*/
	unsigned char send_str[] = "5555555555555555555555";
	unsigned int data_len = sizeof(send_str) - 1;
	unsigned char recv_buf[data_len];
	int fd, i, ret, send_len, recv_len;

	if(argc != 5){
		printf("Usage: %s /dev/ttySn rate [0/1] file\n",argv[0]);
		printf("\t 0 --- send\r\n");
		printf("\t 1 --- recv\r\n");
		printf("\t send:  %s /dev/ttyS1 115200 0 s_file\r\n", argv[0]);
		printf("\t recv:  %s /dev/ttyS1 115200 1ile\r\n", argv[0]);
		return FALSE;
	}
	fd = UART_Open(argv[1]);
	if(FALSE == fd){
		printf("open error\n");
		exit(1);
	}

	ret = UART_Init(fd,atoi(argv[2]),0,8,1,'N');
/*	ret = UART_Init(fd,atoi(argv[2]),0,8,1,'O');*/
/*	ret = UART_Init(fd,atoi(argv[2]),0,8,1,'E');*/
	if (FALSE == fd){
		printf("Set Port Error\n");
		exit(1);
	}
	printf("Start Test [%s]\r\n", atoi(argv[3]) ? "recv" : "send");
	tcflush(fd, TCIOFLUSH);
	sleep(1);
	printf("device: %s, rate: %d\r\n", argv[1], atoi(argv[2]));
	tcflush(fd, TCIOFLUSH);
	sleep(1);

#define BUFFER_SIZE 256
/*#define BUFFER_SIZE 512*/
/*#define BUFFER_SIZE 640*/
/*#define BUFFER_SIZE 768 //error*/
/*#define BUFFER_SIZE 1024 //error*/
	char buffer[BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	int file_size = 0;
	if (atoi(argv[3]) == 0) {
		FILE *fp = fopen(argv[4], "r");
		if (fp == NULL) {
			printf("File:\t%s Not Found!\n", argv[4]);
		} else {
			bzero(buffer, BUFFER_SIZE);
			int file_block_length = 0;
			while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
/*				printf("send: file_block_length = %d, file_size = %d\n", file_block_length, file_size+=file_block_length);  */
				if (UART_Send(fd, buffer, file_block_length) < 0) {
					printf("Send File:\t%s send exit!\n", argv[4]);
					break;
				}
				bzero(buffer, sizeof(buffer));
			}
			fclose(fp);
			printf("File:\t%s Transfer Finished\r\n", argv[4]);
		}
	}
#if 0
		for(i = 0; i < 100; i++) {
			tcflush(fd, TCIOFLUSH);
			send_len = UART_Send(fd, send_str, data_len);
			if (send_len == data_len) {
				printf("No %d send %d data %s\r\n", i, send_len, send_str);
			} else {
				printf("ERROR: send fail... send_len=%d\r\n", send_len);
			}
			sleep(1);
		}
#endif
	else
	{
		FILE *fp_r = fopen(argv[4], "w");
		if (fp_r == NULL) {
			printf("File:\t%s Can Not Open To Write!\n", argv[4]);
			exit(1);
		}

		bzero(buffer, sizeof(buffer));
		int length = 0;
		int timeout = 100;
		while(length = UART_Recv(fd, buffer, BUFFER_SIZE, timeout)) {
/*			printf("recv: length = %d, file_size = %d\n", length, file_size+=length);  */
			if (length < 0) {
				printf("Recieve Data From Server %s exit!\n", argv[1]);
				break;
			}
			int write_length = fwrite(buffer, sizeof(char), length, fp_r);
			if (write_length < length) {
				printf("File:\t%s Write file failed!\n", argv[4]);
				break;
			}
			bzero(buffer, BUFFER_SIZE);
			timeout = 30;
		}
		printf("%s :Reception is complete\r\n", argv[0]);
		fclose(fp_r);
#if 0
		for(;;) {
			memset(recv_buf, 0, sizeof(recv_buf));
			tcflush(fd, TCIOFLUSH);
			recv_len = UART_Recv(fd, recv_buf, data_len, 10);
			if (recv_len > 0) {
				for(i = 0; i < recv_len; i++)
					printf("Receive data No %d is %c, ASCII[%x].\r\n", i, recv_buf[i], recv_buf[i]);

				printf("Total frame length is %d.\r\n", recv_len);
			} else {
				printf("Error: receive error.\r\n");
			}
			sleep(2);
		}
#endif
	}
	UART_Close(fd);
	return 0;

}


