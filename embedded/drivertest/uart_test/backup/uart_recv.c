#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define DEVICE "/dev/ttyGS0"

init_serial(void)
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
	cfsetospeed(&options, B115200);//设置波特率
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

int main(int argc, char **argv)
{
	int ret = 0;
	char buf[100] = {0};
	int serial_fd = init_serial();

	ret = uart_recv(serial_fd, buf, 100);
	printf("%s\n", buf);

	close(serial_fd);
	return 0;
}
