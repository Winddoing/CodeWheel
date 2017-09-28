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


/* #define TRACE_TIME */


#define DEFAULT_DEVICE "/dev/ttyS2"

#define PACKAGE_TEST_COUNT (1000) /* 100 */


//#define TX_USLEEP (1)
//#define TX_USLEEP (5*1000*1000)
//#define TX_USLEEP (3350)	/* 3Mbps */
//#define TX_USLEEP (1000)

/* OK */
/* #define TX_UNIT (64) */
/* #define RX_UNIT (64) */

/* #define TX_UNIT (32) */
/* #define RX_UNIT (32) */

/* Fail UART0-UART1, UART2 is OK. */
//#define TX_UNIT (8)
//#define RX_UNIT (64)

/* Fail UART0-UART1, UART2 is OK. */
/* #define TX_UNIT (64) */
/* #define RX_UNIT (8) */

/* Fail UART0-UART1, UART2 Fail. */
/* #define TX_UNIT (8) */
/* #define RX_UNIT (8) */

/* Fail UART0-UART1, UART2 Fail. */
#define TX_UNIT (8)
#define RX_UNIT TX_UNIT


/* kernel/include/uapi/asm-generic/termbits.h */
/* kernel/drivers/tty/serial/jz_uart.c */

//static unsigned int baud_rate = B115200;
//static unsigned int baud_rate = B230400;
//static unsigned int baud_rate = B460800;
//static unsigned int baud_rate = B500000;
//static unsigned int baud_rate = B576000;
//static unsigned int baud_rate = B921600;
//static unsigned int baud_rate = B1000000;
//static unsigned int baud_rate = B1152000;
//static unsigned int baud_rate = B1500000;
//static unsigned int baud_rate = B2000000;
//static unsigned int baud_rate = B2500000;
static unsigned int baud_rate = B3000000;
//static unsigned int baud_rate = B4000000;


struct package_header {
	unsigned int tag;
	unsigned int index;
};

//#define PACKAGE_DATA_SIZE (0)
#define PACKAGE_DATA_SIZE (1000)
#define PACKAGE_HEADER_SIZE (sizeof(struct package_header))
#define PACKAGE_SIZE (PACKAGE_DATA_SIZE+PACKAGE_HEADER_SIZE)

unsigned char ref_package_buf[PACKAGE_SIZE*2];



static int recv_should_stop = 0;

//static unsigned long loop_cnt = 0;
static unsigned long loop_bytes_cnt = 0;
static unsigned long send_package_cnt = 0;
static unsigned long send_bytes_cnt = 0;
static unsigned long recv_package_cnt = 0;
static unsigned long recv_package_no = 0;
static unsigned long recv_package_no_1 = 0;
static unsigned long recv_bytes_cnt = 0;
static unsigned long err_package_cnt = 0;

static unsigned long err_bytes_cnt = 0;


struct thread_data_struct {
	int serial_fd;
	char *file_name;
	int package_count;
};

struct baud_rate_struct {
	unsigned int rate;
	char * str;
};

struct baud_rate_struct baud_rate_array[] ={
#define RATE(R)					\
	{R, #R}
	RATE(B115200),
	RATE(B230400),
	RATE(B460800),
	RATE(B500000),
	RATE(B576000),
	RATE(B921600),
	RATE(B1000000),
	RATE(B1152000),
	RATE(B1500000),
	RATE(B2000000),
	RATE(B2500000),
	RATE(B3000000),
	RATE(B4000000),
#undef RATE
};

static char * baud_rate_to_string(unsigned int rate)
{
	int i;
	char * rate_str;

	rate_str = NULL;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

	struct baud_rate_struct * r;
	r= &baud_rate_array[0];
	for (i=0; i< ARRAY_SIZE(baud_rate_array); i++) {
		if (r->rate == rate) {
			rate_str = r->str;
			break;
		}
		r++;
	}

	return rate_str;
}


static int get_package_period_us(int baudrate)
{
	int us;

	if (baud_rate == B3000000)
		us = 3500;  		/* 3Mbps, period 3380 us */
	else if (baud_rate == B1500000)
		us = 3500*2;  		/* 1.5Mbps, period 7000 us */
	else if (baud_rate == B1000000)
		us = 10000+200;  	/* 1Mbps, period 10000 us */
	else
		us = 100000;  		/* Mbps, period 100*1000 us */

	return us;
}


int init_serial(char * uart_dev)
{
	struct termios options;
	//unsigned int baud_rate;

	int serial_fd = open(uart_dev, O_RDWR | O_NOCTTY /* | O_SYNC */);
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

	printf("set uart baud_rate= %x:%d, %s\n", baud_rate, baud_rate, baud_rate_to_string(baud_rate));
	cfsetospeed(&options, baud_rate);//设置波特率


	/*设置新属性，TCSANOW：所有改变立即生效*/
	tcflush(serial_fd, TCIFLUSH);//溢出数据可以接收，但不读
	tcsetattr(serial_fd, TCSANOW, &options);
	return serial_fd;
}

int init_ref_package_buf( int cnt)
{
	int iii;
	unsigned char *buf;
	struct package_header *h;

	h = (struct package_header *)&ref_package_buf[0];
	h->tag = 0x5a5a5a5a;
	h->index = 0xa3a2a1a0;

	buf = &ref_package_buf[PACKAGE_HEADER_SIZE];
	for (iii=0; iii< PACKAGE_DATA_SIZE; iii++) {
		*buf++ = iii&0xFF;
	}

#if 0
	for (iii=PACKAGE_HEADER_SIZE; iii< PACKAGE_SIZE; iii++) {
		ref_package_buf[iii] = iii&0xFF;
	}
#endif
	return 0;
}

#include <sys/time.h>
static inline int toUS(struct timeval * from, struct timeval * to)
{
	int ms;
	int sec;
	int usec;

	sec = to->tv_sec - from->tv_sec;
	usec = to->tv_usec - from->tv_usec;
	usec += sec*1000000;
	//ms = usec/1000;
	return usec;
}

static inline int toMS(struct timeval * from, struct timeval * to)
{
	int ms;
	ms = toUS(from, to)/1000;
	return ms;
}

#ifdef TRACE_TIME
#define DEFINE_TIMEVAL				\
	struct timeval __tv1, __tv2; int __us;	\
	gettimeofday(&__tv1, NULL);

#define PRINT_TIMEVAL(STR)			\
	gettimeofday(&__tv2, NULL);		\
	__us = toUS(&__tv1, &__tv2);		\
	printf(STR " in %d us\n", __us);	\
	gettimeofday(&__tv1, NULL)
#else
#define DEFINE_TIMEVAL				\
	do{} while(0)
#define PRINT_TIMEVAL(STR)			\
	do{} while(0)
#endif

static int check_received_data_with_ref_buffer(unsigned char * buf, unsigned char * ref_buf, int size)
{
	int iii;
	int err_cnt;
	err_cnt = 0;
	/* check received data */
	for (iii=0; iii< size; iii++) {
		unsigned char ref_val;
		unsigned char read_val;
		read_val = *buf++;
		ref_val = *ref_buf++;

		//printf("\tchecking error, index= %x [%02x:%02x]\n", iii, ref_val, read_val);
		if (read_val != ref_val) {
			err_cnt++;
			//printf("\tchecking error, index= %x [%02x:%02x]\n", iii, ref_val, read_val);
		}
	}

	return err_cnt;
}

static int check_package_header(unsigned char *read_buf, int size)
{
	int header;
	int header_max;

	header = 0;
	header_max = (size - PACKAGE_HEADER_SIZE);

	if (header_max<0) {
		return -1;
	}

	for(header=0; header<=header_max; header++)
	{
		unsigned int val;

		val = read_buf[3]<<24|read_buf[2]<<16|read_buf[1]<<8|read_buf[0];
		if ( val == 0x5a5a5a5a) {
			return header;
		}
		read_buf++;
	}

	return -1;
}


static int uart_read(int serial_fd, unsigned char *buf, int size)
{
	int read_len;
	int len;
	//printf("%s() enter, size= %d\n", __FUNCTION__, size);
	if ( size < 1 ) {
		return 0;
	}
	read_len = size;
	do {
		len = read(serial_fd, buf, read_len);
		//printf("read(serial_fd=%d) read_len= %d, return %d\n", serial_fd, read_len, len);
		if (len < 1) {
			printf("uart read error: request %d but len=%d\n", read_len, len);
			sleep(1);
		}
		read_len -= len;
		buf += len;
	} while(read_len);

	return size;
}

/*
  Add TRACE_TIME.
  If B3000000 baudrate,
  get_bufs() in 3390 us
  check_bufs() in 10 us
*/

static int uart_recv_package_impl(int serial_fd, int package_count)
{

	printf("%s()  serial_fd=%d\n", __FUNCTION__, serial_fd);
	//printf("%d %s()  serial_fd=%d\n", __LINE__, __FUNCTION__);

	err_package_cnt = 0;
	recv_package_cnt = 0;
	recv_bytes_cnt = 0;

	//for (recv_package_cnt=0; recv_package_cnt<package_count; recv_package_cnt++) {
	//for (recv_package_cnt=0; 1; recv_package_cnt++) {
	do {
		int iii;
		int read_len;
		unsigned char read_buf[PACKAGE_SIZE*2];
		unsigned char *buf;
		unsigned char *ref_buf;
		int match_header;

		DEFINE_TIMEVAL;

		buf = &read_buf[0];

		//memset(&buf[0], 0, RX_UNIT);
		for (iii=0; iii<PACKAGE_SIZE; iii++)
			buf[iii] = 0;

		/* receive PACKAGE_SIZE data */
		read_len = 0;
		match_header = 0;
		buf = &read_buf[0];

		do {
			int len;
			if ( match_header == 0) {
				//len = RX_UNIT;
				len = RX_UNIT<PACKAGE_SIZE?RX_UNIT:PACKAGE_SIZE;
			}
			else {
				len = PACKAGE_SIZE - read_len;
				if (len > RX_UNIT)
					len = RX_UNIT;
			}

			len = uart_read(serial_fd, buf, len);

			read_len += len;
			//printf("read_len=%d\n", read_len);
			/* check package header */
			if ( match_header == 0) {
				if (read_len >= PACKAGE_HEADER_SIZE) {
					int header = check_package_header(&read_buf[0], read_len);
					//printf("header=%d\n", header);
					if ( header >=0 ) {
						int move_size;
						err_bytes_cnt += header;

						if (header>0)
							printf("search package header= %d ,\t recv_pkg=%d, err_pkg=%d\n", header, recv_package_cnt, err_package_cnt);
						match_header = 1;
						/* memmov */
						move_size = read_len-header;
						memcpy(&read_buf[0], &read_buf[header], move_size);
						buf = &read_buf[0]+move_size; //???
						read_len = move_size;
						continue;
					}
					else if (read_len >= PACKAGE_SIZE+PACKAGE_HEADER_SIZE) {
						printf("%s()  memmov the last data to buffer head. read_len = %d\n", __FUNCTION__, read_len);
						/* memmov the last data to buffer head */
						memcpy(&read_buf[0], &read_buf[read_len-PACKAGE_HEADER_SIZE], PACKAGE_HEADER_SIZE);
						read_len = PACKAGE_HEADER_SIZE;
						buf = &read_buf[0]+read_len; //???
						err_package_cnt++;
						err_bytes_cnt += read_len;
						continue;
					}
				}
			}
			else if (read_len >= PACKAGE_SIZE) {
#if 0
				/* if found package header again, discard current package. */
				int header = check_package_header(&read_buf[0]+PACKAGE_HEADER_SIZE, read_len-PACKAGE_HEADER_SIZE);
				if ( header >=0 ) {
					printf("found package header again, discard current package");
					int move_size;
					match_header = 1;
					/* memmov */
					move_size = read_len-header;
					memcpy(&read_buf[0], &read_buf[0]+header, move_size);
					buf = &read_buf[0]+move_size; //???
					read_len = move_size;
					continue;
				}
#endif
				if (read_len > PACKAGE_SIZE) {
					printf("ERROR: read_len(%d) > PACKAGE_SIZE\n", read_len);
				}
				break;
			}

			buf += len;
		} while (1);

		PRINT_TIMEVAL("get_bufs");

		recv_bytes_cnt += read_len;

		{
			struct package_header *h;
			h = (struct package_header *)&read_buf[0];
			recv_package_no = h->index;
			if ( recv_package_no_1 < 1)
				recv_package_no_1 = recv_package_no;
		}

		/* check received data */
		buf = &read_buf[0] + PACKAGE_HEADER_SIZE;
		ref_buf = &ref_package_buf[0] + PACKAGE_HEADER_SIZE;
		int err_bytes;
		err_bytes = check_received_data_with_ref_buffer(buf, ref_buf, PACKAGE_DATA_SIZE);

		PRINT_TIMEVAL("check_bufs");

		if (err_bytes) {
			err_bytes_cnt += err_bytes;
			err_package_cnt++;
		}

		recv_package_cnt++;

		if (recv_package_cnt<10
		    || (recv_package_cnt<100 &&recv_package_cnt%10==0)
		    || (recv_package_cnt<1000 &&recv_package_cnt%100==0)
		    || (recv_package_cnt%1000==0)
			) {
			unsigned int recv_package_cnt_total = recv_package_cnt+err_package_cnt;
			float err_rate = (float)((float)err_package_cnt/((float)recv_package_cnt_total));
			float err_bytes_rate = (float)((float)err_bytes_cnt/((float)(recv_package_cnt_total*PACKAGE_SIZE)));
			/* printf("receive package: %d (err pkg: %d %.4f, err_bytes: %d, %.4f)\t|| (tx_no: %d - %d = %d)\n", */
			/*        recv_package_cnt, err_package_cnt, err_rate, err_bytes_cnt, err_bytes_rate, recv_package_no, recv_package_cnt_total, (recv_package_no-recv_package_cnt_total)); */
			printf("receive package: %d (err pkg: %d %.4f)\t|| (tx_no: (%d-%d)-%d = %d)\n",
			       recv_package_cnt, err_package_cnt, err_rate, recv_package_no, recv_package_no_1, recv_package_cnt_total, (recv_package_no+1-(recv_package_no_1) -recv_package_cnt_total));
		}

	} while (1);


err:
	printf("%d %s() recv_bytes_cnt=%d, err_bytes_cnt=%d \tfinish...\n", recv_package_cnt, __FUNCTION__, recv_bytes_cnt, err_bytes_cnt);

	/* free tread data */


	return 0;
}

int uart_recv_package(int serial_fd, int package_count)
{
	printf("%s()  serial_fd=%d\n", __FUNCTION__, serial_fd);

	return uart_recv_package_impl(serial_fd, package_count);

	return 0;
}

void *UartReceiveThread(void *priv_data)
{
	int cur;
	int serial_fd;
	struct thread_data_struct *thread_data;
	thread_data = (struct thread_data_struct*)priv_data;

	serial_fd = thread_data->serial_fd;
	int package_count = thread_data->package_count;

	printf("%s()  serial_fd=%d\n", __FUNCTION__, serial_fd);
	uart_recv_package_impl(serial_fd, package_count);

	return NULL;
}

int uart_recv_package_in_thread(int serial_fd, int package_count)
{
	pthread_t tid_thread_help;
	struct thread_data_struct *priv_data;

	priv_data = (struct thread_data_struct*)malloc(sizeof(struct thread_data_struct));
	priv_data->serial_fd = serial_fd;
	priv_data->package_count = package_count;

	pthread_create(&tid_thread_help,NULL, UartReceiveThread, (struct thread_data_struct*)priv_data);
	return 0;
}




int uart_send_package(int serial_fd, int package_count)
{

	send_bytes_cnt = 0;
	send_package_cnt = 0;

	///for (send_package_cnt=0; send_package_cnt<package_count; send_package_cnt++) {
	for (send_package_cnt=1; 1; send_package_cnt++) {
		int send_len;
		char * buf;
		struct timeval t_start, t_end;
		gettimeofday(&t_start, NULL);

		DEFINE_TIMEVAL;

		{		/* make package header */
			struct package_header *h;
			h = (struct package_header *)&ref_package_buf[0];
			h->tag = 0x5a5a5a5a;
			h->index = send_package_cnt;
		}

		buf = &ref_package_buf[0];
		send_len = PACKAGE_SIZE;

		while (send_len>0) {
			int len;

			len = send_len;
			if (len>TX_UNIT)
				len = TX_UNIT;

			len = write(serial_fd, buf, len);
			if (len < 1) {
				printf("write serial failed: total send_package_cnt=%d, send_len=%d, ret len=%d\n",
				       send_package_cnt, send_len, len);
				break;
			}
			buf += len;
			send_len -= len;
			send_bytes_cnt += len;
		}

		PRINT_TIMEVAL("send_buf()");
		/* extern int tcdrain (int __fd); */
		/* tcdrain(serial_fd); */
		/* PRINT_TIMEVAL("tcdrain()"); */


		if (send_package_cnt<10
		    || (send_package_cnt<100 &&send_package_cnt%10==0)
		    || (send_package_cnt<1000 &&send_package_cnt%100==0)
		    || (send_package_cnt%1000==0)
			) {
			printf("%s() send_package_cnt= %d\n",
			       __FUNCTION__, send_package_cnt);
		}

		/* Waiting receive port done... */
		//sleep(5);
		//usleep(5*1000*1000);
		//usleep(1*1000);
		//usleep(100);

		/* wait package tx end. Any better way to make a io sync? */
		gettimeofday(&t_end, NULL);
		int send_us = toUS(&t_start, &t_end);
		int period_us = get_package_period_us(0);

		if (send_us < period_us) {
			usleep(period_us-send_us);
		}
#ifdef TX_USLEEP
		usleep(TX_USLEEP);
#endif

	}

 	printf("%d %s() send_bytes_cnt= %d , finish...\n",
	       send_package_cnt, __FUNCTION__, send_bytes_cnt);

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
			loop_bytes_cnt +=len;

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

	printf("\tloop_bytes_cnt= %d\n", loop_bytes_cnt);

	printf("\trecv_package_cnt= %d\n", recv_package_cnt);

	/* save file */

	exit(0);
	return ;
}


int print_usage(char * name)
{
	printf("Usage:\n");
	printf("\t-dev,\tdevice like /dev/ttyS2\n");
	printf("\t-txrx, tx or rx\n");

	printf("\t\n");

	return 0;
}



int main(int argc, char **argv)
{
	int iii;
	int ret = 0;
	char uart_dev[100] = {0};
	int serial_fd;

	char * data_file;
	int data_fd;
	char * send_file_name = "file";
	char * recv_file_name = "file_recv";

	int package_cnt;
	int loopback;
	int receive_only;
	int send_only;

	package_cnt = PACKAGE_TEST_COUNT;

	strcpy(&uart_dev[0], DEFAULT_DEVICE);
	loopback = 0;
	receive_only = 0;
	send_only = 0;

	if (argc == 1) {
		print_usage(argv[0]);

		return 0;
	}

	iii = 1;
	while ( iii < argc ) {
		if (! strncmp(argv[iii], "-dev", strlen("-dev"))) {
			strcpy(&uart_dev[0], argv[iii+1]);
			iii +=2;
		}
		else if (! strncmp(argv[iii], "-txrx", strlen("-txrx"))) {
			if (! strcmp(argv[iii+1], "txrx")) {
				send_only = 0;
				receive_only = 0;
			}
			else if (! strcmp(argv[iii+1], "tx")) {
				send_only = 1;
			}
			else if (! strcmp(argv[iii+1], "rx")) {
				receive_only = 1;
			}
			else {
				send_only = 0;
				receive_only = 0;
			}
			iii +=2;
		}
		else {
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("UART: %s, send_only=%d, receive_only=%d\n",
	       &uart_dev[0], send_only, receive_only);

	//sighandler_t signal(int signum, sighandler_t handler);

	serial_fd = init_serial(&uart_dev[0]);

	if (serial_fd<0) {
		return -1;
	}

	printf("UART: TX_UNIT=%d, RX_UNIT=%d\n",
	       TX_UNIT, RX_UNIT);

	printf("PACKAGE_SIZE= %d, PACKAGE_HEADER_SIZE=%d\n",
	       PACKAGE_SIZE, PACKAGE_HEADER_SIZE);

	printf("get_package_period_us()= %d\n",  get_package_period_us(0));


	init_ref_package_buf(0);


	signal(SIGINT, &sighandler);


	if (!send_only) {
		ret = uart_recv_package_in_thread(serial_fd, package_cnt);
		//ret = uart_recv_package(serial_fd, package_cnt);
		if (receive_only) {
			while (1)
				sleep(3);
		}
	}


	/* send data */
	if (!receive_only) {
		ret = uart_send_package(serial_fd, package_cnt);
		if (ret) {
			goto failed;
		}
	}

	//printf("sleep1\n");	sleep(1);
	//printf("%s\n", buf);
failed:

	close(serial_fd);

	return 0;
}
