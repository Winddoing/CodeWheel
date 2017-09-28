#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> 		/* kernel/include/uapi/asm-generic/termbits.h */
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>



#define DEVICE "/dev/ttyS2"

//#define TX_SLEEP (100)

#define TX_UNIT (32)
#define RX_UNIT (64)

#define BUF_MAX (256)

unsigned char ref_buf[BUF_MAX];

static int recv_should_stop = 0;

static unsigned int loop_cnt = 0;
static unsigned int send_cnt = 0;
static unsigned int recv_cnt = 0;
static unsigned int err_cnt = 0;


int init_serial(void)
{
	struct termios options;

	int serial_fd = open(DEVICE, O_RDWR | O_NOCTTY);
	if (serial_fd < 0) {
		perror("open");
		return -1;
	}

	tcgetattr(serial_fd, &options);
	options.c_cflag &= ~(ICANON | ECHOE |ISIG);
	options.c_iflag &= ~(INLCR|ICRNL);
	options.c_iflag &= ~(IXON);
	options.c_cflag &= ~OPOST;
	options.c_cflag |= (CLOCAL | CREAD);//设置控制模式状态，本地连接，接收使能
	options.c_cflag &= ~CSIZE;//字符长度，设置数据位之前一定要屏掉这个位
	options.c_cflag &= ~CRTSCTS;//无硬件流控
	options.c_cflag |= CS8;//8位数据长度
	options.c_cflag &= ~CSTOPB;//1位停止位
	options.c_iflag |= IGNPAR;//无奇偶检验位
	options.c_oflag = 0; //输出模式
	options.c_lflag = 0; //不激活终端模式

	/* kernel/include/uapi/asm-generic/termbits.h */
	/* kernel/drivers/tty/serial/jz_uart.c */

	cfsetospeed(&options, B115200);//设置波特率
	//cfsetospeed(&options, B230400);//设置波特率
	//cfsetospeed(&options, B460800);//设置波特率
	//cfsetospeed(&options, B500000);//设置波特率
	//cfsetospeed(&options, B576000);//设置波特率
	//cfsetospeed(&options, B921600);//设置波特率
	//cfsetospeed(&options, B1000000);//设置波特率
	//cfsetospeed(&options, B1152000);//设置波特率
	//cfsetospeed(&options, B1500000);//设置波特率
	//cfsetospeed(&options, B2000000);//设置波特率
	//cfsetospeed(&options, B2500000);//设置波特率
	//cfsetospeed(&options, B3000000);//设置波特率
	//cfsetospeed(&options, B4000000);//设置波特率
	/*设置新属性，TCSANOW：所有改变立即生效*/
	tcflush(serial_fd, TCIFLUSH);//溢出数据可以接收，但不读
	tcsetattr(serial_fd, TCSANOW, &options);
	return serial_fd;
}

int uart_send(int fd, char *data, int datalen)
{
	int len = 0;
	len = write(fd, data, datalen);//实际写入的长度
	if (len == datalen) {
		return len;
	} else {
		tcflush(fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("send error\n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		return -1;
	}

	return len;
}

int uart_recv(int fd, char *data, int datalen)
{
	int len=0;
	memset(data,0,datalen);
	len = read(fd, data, datalen);
	return len;
}

struct thread_data_struct {
	int serial_fd;
	char *file_name;
};


void *UartReceiveThread(void *priv_data)
{
	//int recv_cnt;
	int recv_fd;
	int serial_fd;
	char *file_name;
	struct thread_data_struct *thread_data;
	thread_data = (struct thread_data_struct*)priv_data;

	recv_cnt = 0;
	serial_fd = thread_data->serial_fd;
	file_name = thread_data->file_name;

	/* int fd2 = open("/tmp/test.svg", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH); */
	recv_fd = open(file_name, O_CREAT|O_TRUNC|O_WRONLY,  S_IWUSR |S_IRUSR | S_IRGRP | S_IROTH);
	//recv_fd = creat(file_name, O_TRUNC|O_WRONLY);

	printf("%s() recv_file: (%s), fd=%d\n", __FUNCTION__,  file_name, recv_fd);

	if (recv_fd<0) {
		printf("%s() open(%s) failed, recv_fd=%d\n", __FUNCTION__, file_name, recv_fd);
		return NULL;
	}

	do {
		int len;
		int read_len;
		char buf[128];

		//if (recv_should_stop || send_cnt == recv_cnt) {
		if (recv_should_stop) {
			printf("%s() main thread stop\n", __FUNCTION__);
			break;
		}

		//printf("recv_cnt=%d\n", recv_cnt);

		read_len = read(serial_fd, buf, RX_UNIT);
		//printf("read(serial_fd=%d) = %d\n", serial_fd, read_len);

		if (read_len < 1) {
			printf("uart read error: %d\n", read_len);
			break;
		}
		recv_cnt += read_len;

		len = write(recv_fd, buf, read_len);
		if (len <= 0) {
			printf("save to (%s) failed: %d\n", file_name, len);
			break;
		}

	} while (1);



	/* save receive file */
	close(recv_fd);
	printf("%s() recv_cnt=%d\n", __FUNCTION__, recv_cnt);


	/* free tread data */



	return NULL;
}

#if CONFIG_RECEIVE_ONLY

int uart_recv_to_file(int serial_fd, char *recv_file_name)
{
	pthread_t tid_thread_help;
	struct thread_data_struct *priv_data;

	priv_data = (struct thread_data_struct*)malloc(sizeof(struct thread_data_struct));
	priv_data->serial_fd = serial_fd;
	priv_data->file_name = recv_file_name;

	UartReceiveThread(priv_data);
	return 0;
}

#else
int uart_recv_to_file(int serial_fd, char *recv_file_name)
{
	pthread_t tid_thread_help;
	struct thread_data_struct *priv_data;

	priv_data = (struct thread_data_struct*)malloc(sizeof(struct thread_data_struct));
	priv_data->serial_fd = serial_fd;
	priv_data->file_name = recv_file_name;

	pthread_create(&tid_thread_help,NULL, UartReceiveThread, (struct thread_data_struct*)priv_data);
	return 0;
}
#endif

int uart_send_from_file(int serial_fd, char *send_file_name)
{
	int send_fd;
	int buflen, len;
	char buf[1024];

	send_cnt = 0;
	buflen = TX_UNIT;

	send_fd = open(send_file_name, O_RDWR);

	printf("open(%s) send file: %d\n", send_file_name, send_fd);

	if (send_fd<0) {
		printf("open(%s) send file failed\n", send_file_name);
		return -1;
	}


	do {
		int read_len;
		read_len = read(send_fd, buf, buflen);

		//printf("read_len=%d\n", read_len);

		if (read_len < 1) {
			//printf("read file finish. read_len=%d\n", read_len);
			break;
		}

		while (read_len) {
			len = write(serial_fd, buf, read_len);
			if (len < 0) {
				printf("write serial failed: %d\t", len);
				printf("send_cnt=%d\n", send_cnt);
				break;
			}

			//tcflush(serial_fd, TCIFLUSH);//溢出数据可以接收，但不读

			read_len -= len;
			send_cnt += len;
		}
		//printf("send_cnt=%d\n", send_cnt);

		//usleep(1000);

	} while (1);

	//recv_should_stop = 1;
	printf("%s() send_cnt=%d\n", __FUNCTION__, send_cnt);


	close(send_fd);

	return 0;
}

int init_ref_buf( int cnt)
{
	int iii;

	for (iii=0; iii< BUF_MAX; iii++) {
		ref_buf[iii] = iii&0xFF;
	}

	return 0;
}


void *UartReceiveThread256(void *priv_data)
{
	//int recv_cnt;
	int cur;
	int serial_fd;
	struct thread_data_struct *thread_data;
	thread_data = (struct thread_data_struct*)priv_data;

	recv_cnt = 0;
	serial_fd = thread_data->serial_fd;

	printf("%s()  serial_fd=%d\n", __FUNCTION__, serial_fd);

	cur = 0;
	do {
		int read_len;
		unsigned char buf[BUF_MAX];
		static int next_time = 1024;

		//memset(&buf[0], 0, RX_UNIT);
		for (read_len=0; read_len<BUF_MAX; read_len++)
			buf[read_len];

		read_len = read(serial_fd, &buf[0], RX_UNIT);
		//printf("read(serial_fd=%d) = %d\n", serial_fd, read_len);

		if (read_len < 1) {
			printf("uart read error: %d\n", read_len);
			printf("%s() recv_cnt=%d, err_cnt=%d\n", __FUNCTION__, recv_cnt, err_cnt);
		}

		recv_cnt += read_len;
		if (recv_cnt >= next_time) {
			printf("%s() recv_cnt=%d, err_cnt=%d\n", __FUNCTION__, recv_cnt, err_cnt);
			next_time += 1024;
		}

		if (recv_cnt/1024 >= 100) {
			printf("%s() finish... recv_cnt=%d, err_cnt=%d\n", __FUNCTION__, recv_cnt, err_cnt);
			return NULL;
		}

		/* check data received */
		do {
			int iii, cmp_len;
			cmp_len = BUF_MAX-cur;
			if (read_len < cmp_len) {
				cmp_len = read_len;
			}

			for (iii=0; iii< cmp_len; iii++) {
				if (buf[iii] != ref_buf[cur+iii]) {
					err_cnt++;
					float err_rate;
					err_rate = (float)((float)err_cnt/(float)recv_cnt);
					printf("\tchecking error, %f = (err=%d/total=%d), ref[%02x]::[%02x], cur=%02x, read=%d,%d\n",
					       err_rate, err_cnt, recv_cnt, ref_buf[cur+iii], buf[iii], cur+iii, read_len, iii);
					if (err_cnt>100) {
						//goto err;
					}
				}
			}

			read_len -= cmp_len;
			cur += cmp_len;
			if (cur>=BUF_MAX) {
				//printf("cur loop back: %d\n", cur);
				cur = 0;
			}

		} while (read_len);

	} while (1);


err:

	printf("%s() recv_cnt=%d\n", __FUNCTION__, recv_cnt);

	/* free tread data */



	return NULL;
}

//#if CONFIG_RECEIVE_ONLY
#if 1
int uart_recv_256(int serial_fd, char *recv_file_name)
{
	pthread_t tid_thread_help;
	struct thread_data_struct *priv_data;

	priv_data = (struct thread_data_struct*)malloc(sizeof(struct thread_data_struct));
	priv_data->serial_fd = serial_fd;
	priv_data->file_name = recv_file_name;

	UartReceiveThread256(priv_data);
	return -1;
}

#else
int uart_recv_to_file(int serial_fd, char *recv_file_name)
{
	pthread_t tid_thread_help;
	struct thread_data_struct *priv_data;

	priv_data = (struct thread_data_struct*)malloc(sizeof(struct thread_data_struct));
	priv_data->serial_fd = serial_fd;
	priv_data->file_name = recv_file_name;

	pthread_create(&tid_thread_help,NULL, UartReceiveThread256, (struct thread_data_struct*)priv_data);
	return 0;
}
#endif




int uart_send_256(int serial_fd, char *send_file_name)
{
	int send_fd;
	int send_len, cur;
	static int next_time = 1024;

	cur = 0;
	send_cnt = 0;

	do {
		char * buf;
		if (cur >= BUF_MAX) {
			cur = 0;
		}
		buf = &ref_buf[cur];

		send_len = BUF_MAX-cur;
		if (send_len>TX_UNIT) {
			send_len = TX_UNIT;
		}

		cur += send_len;
		send_cnt += send_len;
#ifdef TX_SLEEP
		usleep(TX_SLEEP); /* sleep 1ms */
#endif
		while (send_len>0) {
			int len;
			len = write(serial_fd, buf, send_len);
			if (len < 1) {
				printf("write serial failed: total send_cnt=%d, send_len=%d, ret len=%d\n",
				       send_cnt, send_len, len);
				//break;
			}
			if (send_len != len || send_len != TX_UNIT) {
				printf("TX_UNIT write serial failed: total send_cnt=%d, send_len=%d, ret len=%d\n",
				       send_cnt, send_len, len);
				//break;
			}
			buf += len;
			send_len -= len;
		}

		if (send_cnt >= next_time) {
			next_time += 1024;
			printf("%s() send_cnt=%d\n", __FUNCTION__, send_cnt);
			usleep(300*1000);
		}

		if (send_cnt/1024 >= 100) {
			printf("%s() finish... send_cnt=%d\n", __FUNCTION__, send_cnt);
			return 0;
		}

	} while (1);

	//recv_should_stop = 1;
	printf("%s() send_cnt=%d\n", __FUNCTION__, send_cnt);


	return 0;
}


int do_loopback(int serial_fd)
{

	while (1) {
		int len;
		char buf[16];
		len = read(serial_fd, buf, 1);
		if (len <= 0) {
			break;
		}
		else {
			loop_cnt +=len;

			len = write(serial_fd, buf, 1);
			if (len < 0) {
				printf("write serial failed: %d\n", len);
				break;
			}
		}
	}

	printf("%s(), exit error...\n", __FUNCTION__);

	return 0;
}

void sighandler(int signum)
{
	printf("%s(), signum=%d\n", __FUNCTION__, signum);

	printf("\tloop_cnt= %d\n", loop_cnt);

	printf("recv_cnt=%d\n", recv_cnt);

	/* save file */

	exit(1);
	return ;
}


int print_usage(char * name)
{
	printf("Usage:\n");
	printf("\t\n");
	printf("\t\n");

	printf("\t\n");

	return 0;
}



int main(int argc, char **argv)
{
	int ret = 0;
	char buf[100] = {0};
	int serial_fd;

	char * uart_dev;
	char * data_file;
	int data_fd;
	char * send_file_name = "file";
	char * recv_file_name = "file_recv";

	int loopback;
	int receive;
	int send;

	loopback = 0;
	receive = 0;
	send = 1;

	if (argc == 1) {
		print_usage(argv[0]);
	}

	if (argc > 1) {
		receive = 1;
	}

	//sighandler_t signal(int signum, sighandler_t handler);

	signal(SIGINT, &sighandler);

	init_ref_buf(0);

	serial_fd = init_serial();


	if (receive) {
		ret = uart_recv_256( serial_fd, NULL);
		if (1 || ret) {
			goto failed;
		}
	}

	if (send) {
		ret = uart_send_256(serial_fd, send_file_name);
		if (ret) {
			goto failed;
		}
	}

	printf("sleep1\n");	sleep(1);

	//printf("%s\n", buf);
failed:

	close(serial_fd);

	return 0;
}
