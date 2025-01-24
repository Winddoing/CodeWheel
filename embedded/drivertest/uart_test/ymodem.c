/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : ymodem.c
 *  Author       : wqshao
 *  Created Time : 2025-01-06 09:56:39
 *  Description  :
 *   创建虚拟串口：
 *   	socat -d -d pty,raw,echo=0 pty,raw,echo=0
 *   	socat -d -d pty,raw,echo=0,link=/tmp/vpty1 pty,raw,echo=0,link=/tmp/vpty2
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define DEVICE "/dev/vpty1"


static int serial_fd = -1;

static int init_serial(void)
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

static int uart_send(int fd, char *data, int datalen)
{
	int len = 0;
	len = write(fd, data, datalen);//实际写入的长度
	if (len == datalen) {
		return len;
	} else {
		printf("===> func: %s, line: %d, data=%s, datalen=%d, len=%d, fd=%d\n", __func__, __LINE__, data, datalen, len, fd);
		tcflush(fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送
		 printf("send error: %s\n", strerror(errno));
		return -1;
	}
	tcdrain(fd);

	return len;
}

static int uart_recv(int fd, char *data, int datalen)
{
	int len=0;
	memset(data,0,datalen);
	len = read(fd, data, datalen);
	return len;
}

static int uart_getc_timeout(char *str, uint32_t us)
{
	uart_recv(serial_fd, str, 1);

	return 0;
}

static void uart_putc(char b)
{
	uart_send(serial_fd, &b, 1);
}

#define TIMEOUT (500*1000) //500ms

#define SOH     0x01    // 128byte data head
#define STX     0x02    // 1k byte data head
#define EOT     0x04    // 发送结束
#define ACK     0x06    // 应答，继续发送
#define NAK     0x15    // 非应答，表示需要重传
#define CAN     0x18    // Cancelled 取消发送
#define CRC16   0xFF


static void uart_clear_buff()
{
#if 0
	while (1) {
		uint8_t tmp;

		int ret = uart_getc_timeout(&tmp, TIMEOUT);  //timeout 1ms
		if (ret < 0)
			break;
	}
#endif
}

#if 0
static uint16_t calculate_crc(const uint8_t *data, uint32_t length)
{
    uint16_t crc = 0;
    for (int i = 0; i < length; ++i) {
        crc ^= data[i];
    }
    return crc;
}
#else
static uint16_t calculate_crc(const uint8_t *data, uint32_t length)
{
    uint16_t crc = 0;
    for (int i = 0; i < length; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}
#endif

static int ymodem_receive_data(uintptr_t addr)
{
	uint8_t fh = 0, bn[2], crc[2]; //frame head, blk num
	uint16_t blk_size = 128;
	uint8_t blk_seq_num = 0;
	uint8_t *buffer;
	int ret = 0;

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	uart_putc('\r');
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	while (1) {
		printf("===> func: %s, line: %d\n", __func__, __LINE__);
		uart_putc('C');

		ret = uart_getc_timeout(&fh, TIMEOUT);  //timeout 1ms
		if (ret != 0)
			continue;

		if (fh == SOH) {
			blk_size = 128;
			break;
		} else if (fh == STX) {
			blk_size = 1024;
			break;
		}
	}

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	// 起始帧
	uart_getc_timeout(&bn[0], TIMEOUT);
	uart_getc_timeout(&bn[1], TIMEOUT);

	uart_clear_buff();
	uart_putc(ACK);
	blk_seq_num++;

	uart_putc('C');
	// 数据帧
	while(1) {
		while(1) {
			ret = uart_getc_timeout(&fh, TIMEOUT);  //timeout 1ms
			if (ret != 0)
				continue;

			if (fh == SOH) {
				blk_size = 128;
				break;
			} else if (fh == STX) {
				blk_size = 1024;
				break;
			} else if (fh == EOT) {
				goto end;
			}
		}

		printf("===> func: %s, line: %d\n", __func__, __LINE__);
		uart_getc_timeout(&bn[0], TIMEOUT);
		uart_getc_timeout(&bn[1], TIMEOUT);

		if ((bn[0] ^ bn[1]) != (uint8_t)0xFF ||
				bn[0] != (uint8_t)(blk_seq_num & 0xFF)) {
			uart_clear_buff();
			uart_putc(NAK);
			printf("block num (%d) error!\n", blk_seq_num);
			continue;
		}

		buffer = (uint8_t*)addr;
		for (int i = 0; i < blk_size; i++) {
			ret = uart_getc_timeout(buffer + i, TIMEOUT);
			if (ret < 0) {
				printf("timeout.\n");
				break;
			}
		}

		uart_getc_timeout(&crc[0], TIMEOUT);
		uart_getc_timeout(&crc[1], TIMEOUT);


		uint16_t crc_received = (crc[0] << 8) | crc[1];
		uint16_t crc_calculated = calculate_crc(buffer, blk_size);
		if (crc_calculated != crc_received) {
			printf("===> func: %s, line: %d\n", __func__, __LINE__);
			goto end;
		}

		addr += blk_size;

		uart_putc(ACK);
		blk_seq_num = (blk_seq_num + 1) % 255;;
	}
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
end:
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	uart_putc(ACK);
	uart_putc(CAN);
	uart_putc(CAN);
	uart_putc(CAN);
	uart_putc(CAN);
#if 0
	uart_clear_buff();
	uart_putc('\n');
#endif

	return 0;
}

#define PRINT printf
#define HEXDUMP_LINE_LEN 16

static int chk_patt(const char *buf, unsigned int len, char chr)
{
	while (len--) {
		if (buf[len] != chr)
			return 0;
	}
	return 1;
}

static void pr_line(const char *buf, unsigned int len, unsigned int addr)
{
	int i;
	unsigned char chr;

	PRINT("0x%.8X: ", addr);

#if 0
	for (i = 0; i < HEXDUMP_LINE_LEN; i++) {
		chr = (unsigned char)buf[i];

		if (i < len)
			PRINT("%.2x ", chr);
		else
			PRINT("   ");
	}
#else
	for (i = 0; i < HEXDUMP_LINE_LEN; i+=4) {
		for (int j = 3; j >= 0; j--) {
			chr = (unsigned char)buf[i + j];
			PRINT("%.2x", chr);
		}
		PRINT(" ");
		if (i > len )
			PRINT("   ");
	}
#endif

	PRINT("|");
	for (i = 0; i < HEXDUMP_LINE_LEN; i++) {
		if (i < len) {
			if (buf[i] >= 0x20 && buf[i] <= 0x7E)
				PRINT("%c", buf[i]);
			else
				PRINT(".");
		} else {
			PRINT(" ");
		}
	}
	PRINT("|\n");
}

void hexdump(const char *buf, unsigned int length)
{
	unsigned int addr = 0, len;
	char chr, ommit = 0, same = 0;

	if (!length)
		return;

	for (; length; addr += len, buf += len, length -= len) {
		len = length < HEXDUMP_LINE_LEN ? length: HEXDUMP_LINE_LEN;

		if (same && chk_patt(buf, len, chr)) {
			if (ommit)
				continue;
			PRINT("0x%.8X: ", addr);
			PRINT("*\n");
			ommit = 1;
			continue;
		}
		
		pr_line(buf, len, addr);
		if (chk_patt(buf, len, buf[0])) {
			ommit = 0;
			same = 1;
			chr = buf[0];
		} else {
			same = ommit = 0;
		}
	}
	PRINT("0x%.8X:\n", addr);
}

#define  BUF_SZ (1024*1024)

int main(int argc, char **argv)
{
	int ret = 0;
	serial_fd = init_serial();

	char buf[BUF_SZ] = {0};

	ret = ymodem_receive_data((uintptr_t)buf);
	printf("===> func: %s, line: %d, ret=%d\n", __func__, __LINE__, ret);

	hexdump(buf, 1024);

	close(serial_fd);

	return 0;
}
