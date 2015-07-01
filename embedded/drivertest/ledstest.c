#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <linux/micro2440_leds.h>

//#define LEDS_DEVICE "/dev/leds"
#define LEDS_DEVICE "/dev/led4s"

void enterKeyInput(const char * prompt)
{
	char c;
	
	fprintf(stdout, "%s, press 'Enter' key to continue !", prompt);
	while((c = getchar()) != '\n');
}

int main(int argc, char *argv[])
{
	int fd;
	unsigned int stat;
	struct leds_stat oneStat;
	char buf[1024];

	if(-1 == (fd = open(LEDS_DEVICE, O_RDWR)))
	{
		perror("Open Device: " LEDS_DEVICE);
		return errno;
	}
	stat = 0xAA;
	sprintf(buf, "--- test write stat to 0x%X.", stat);
	enterKeyInput(buf);
	if(-1 == write(fd, &stat, sizeof(stat)))
	{
		perror("Write Device: " LEDS_DEVICE);
		close(fd);
		return errno;
	}
	enterKeyInput("--- test read. ");
	if(-1 == read(fd, &stat, sizeof(stat)))
	{
		perror("Read Device: " LEDS_DEVICE);
		close(fd);
		return errno;
	}
	fprintf(stdout, "stat is 0x%x\n", stat);
	enterKeyInput("--- test ALL_CHA by ioctl.");
	if(-1 == ioctl(fd, LEDS_ALL_CHA))
	{
		perror("Ioctl Device: " LEDS_DEVICE);
		close(fd);
		return errno;
	}
	enterKeyInput("--- test SET_ONE by ioctl.");
	oneStat.led_nr = 3;
	oneStat.led_st = 1;
	if(-1 == ioctl(fd, LEDS_SET_ONE, &oneStat))
	{
		perror("Ioctl Device: " LEDS_DEVICE);
		close(fd);
		return errno;
	}
	enterKeyInput("--- test GET_ONE by ioctl.");
	oneStat.led_nr = 1;
	if(-1 == ioctl(fd, LEDS_GET_ONE, &oneStat))
	{
		perror("Ioctl Device: " LEDS_DEVICE);
		close(fd);
		return errno;
	}
	fprintf(stdout, "stat.led_nr = %d, stat.led_st = %d\n",
					oneStat.led_nr, oneStat.led_st);

	close(fd);

	return 0;
}
