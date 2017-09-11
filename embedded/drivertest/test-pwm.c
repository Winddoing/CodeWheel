#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PWM_TST_DEV_NAME "/dev/jz-pwm-tst"

int main(int argc, char **argv)
{
	int fd, ret;

	ret = open(PWM_TST_DEV_NAME, O_RDWR);
	if (ret < 0) {
		printf("Open %s fail...\n", PWM_TST_DEV_NAME);
		return -1;
	}

	while(1);
	return 0;
}

