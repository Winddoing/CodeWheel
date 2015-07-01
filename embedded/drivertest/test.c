#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LED "/dev/leds"

int main(int argc, char *argv[])
{
	int fs;
	ssize_t ret;
	int buf;

	fs = open(LED, O_RDWR);
	if(fs < 0)
	{
		fprintf(stdout, "open fail");
		return -1;
	}
	ret = read(fs, &buf, sizeof(buf));
	fprintf(stdout, "******%x*********\n", buf);
	if(ret < 0)
	{
		fprintf(stdout, "read fail");
		return -1;
	}
}
