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
 * ���ƣ� UART0_Open
 * ���ܣ� �򿪴��ڲ����ش����豸�ļ�����
 * ��ڲ����� fd :�ļ������� port :���ں�(ttyS0,ttyS1,ttyS2)
 * ���ڲ����� ��ȷ����Ϊ1�����󷵻�Ϊ0
 *****************************************************************/
int UART_Open(char* port)
{
	int fd;
	fd = open( port, O_RDWR|O_NOCTTY   );//|O_NDELAY
	if (FALSE == fd){
		perror("Can't Open Serial Port");
		return(FALSE);
	}

	//�жϴ��ڵ�״̬�Ƿ�Ϊ����״̬
	if(fcntl(fd, F_SETFL, 0) < 0){
		printf("fcntl failed!\n");
		return(FALSE);
	} else {
		//    printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	}

	//�����Ƿ�Ϊ�ն��豸
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
 * ���ƣ� UART0_Set
 * ���ܣ� ���ô�������λ��ֹͣλ��Ч��λ
 * ��ڲ����� fd �����ļ�������
 * speed �����ٶ�
 * flow_ctrl ����������
 * databits ����λ ȡֵΪ 7 ����8
 * stopbits ֹͣλ ȡֵΪ 1 ����2
 * parity Ч������ ȡֵΪN,E,O,,S
 *���ڲ����� ��ȷ����Ϊ1�����󷵻�Ϊ0
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

	/*tcgetattr(fd,&options)�õ���fdָ��������ز������������Ǳ�����options,�ú���,�����Բ��������Ƿ���ȷ���ô����Ƿ���õȡ������óɹ�����������ֵΪ0��������ʧ�ܣ���������ֵΪ1.
	*/
	if(tcgetattr( fd,&options) != 0){
		perror("SetupSerial 1");
		return(FALSE);
	}
	bzero(&options, sizeof(options));

	//���ô������벨���ʺ����������
	for(i= 0;i < sizeof(speed_arr) / sizeof(int);i++) {
		if (speed == name_arr[i]) {
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}

	//�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���
	options.c_cflag |= CLOCAL;
	//�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������
	options.c_cflag |= CREAD;
	//��������������
	switch(flow_ctrl){
	case 0 : //��ʹ��������
		options.c_cflag &= ~CRTSCTS;
		break;
	case 1 : //ʹ��Ӳ��������
		options.c_cflag |= CRTSCTS;
		break;
	case 2 : //ʹ�����������
		options.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}
	//��������λ
	options.c_cflag &= ~CSIZE; //����������־λ
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
	//����У��λ
	switch (parity) {
	case 'n':
	case 'N': //����żУ��λ��
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O': //����Ϊ��У��
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E': //����ΪżУ��
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's':
	case 'S': //����Ϊ�ո�
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return (FALSE);
	}
	// ����ֹͣλ
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
	//�޸����ģʽ��ԭʼ�������
	options.c_oflag &= ~OPOST;
	//���õȴ�ʱ�����С�����ַ�
	options.c_cc[VTIME] = 0; /* ��ȡһ���ַ��ȴ�1*(1/10)s */
	options.c_cc[VMIN] = 0; /* ��ȡ�ַ������ٸ���Ϊ1 */

	//�����������������������ݣ����ǲ��ٶ�ȡ
	tcflush(fd,TCIOFLUSH);

	//�������� (���޸ĺ��termios�������õ������У�
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
	//���ô�������֡��ʽ
	if (FALSE == UART_Set(fd,speed,flow_ctrlint,databits,stopbits,parity)) {
		return FALSE;
	} else {
		return TRUE;
	}
	write(fd, "FLUSH ERROR\n", sizeof("FLUSH ERROR\n"));
}



/*******************************************************************
 * ���ƣ� UART0_Recv
 * ���ܣ� ���մ�������
 * ��ڲ����� fd :�ļ�������
 * rcv_buf :���մ��������ݴ���rcv_buf��������
 * data_len :һ֡���ݵĳ���
 * ���ڲ����� ��ȷ����Ϊ1�����󷵻�Ϊ0
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

	//ʹ��selectʵ�ִ��ڵĶ�·ͨ��
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
 * ���ƣ� UART0_Send
 * ���ܣ� ��������
 * ��ڲ����� fd :�ļ�������
 * send_buf :��Ŵ��ڷ�������
 * data_len :һ֡���ݵĸ���
 * ���ڲ����� ��ȷ����Ϊ1�����󷵻�Ϊ0
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


