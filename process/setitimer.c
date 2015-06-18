#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>

static uint32_t time;
static uint8_t  color;

void sigAlrmProcess(int signum)
{
	time++;
	fprintf(stderr, "\r[%02dm%02d:%02d[00m",
			color + 30, time / 60, time % 60);
}
void sigVtAlrmProcess(int signum)
{
	color++;
	color %= 10;
}
int main(int argc, char *argv[])
{
	struct itimerval rVal, vVal;

	//用于设置系统定时期的间隔时间结构
	//启动时间1us,间隔时间1s
	rVal.it_interval.tv_sec = 1;
	rVal.it_interval.tv_usec = 0;
	rVal.it_value.tv_sec = 0;
	rVal.it_value.tv_usec = 1;

	//用于设置虚拟定时期的间隔时间结构
	//启动时间1s,间隔时间2s
	vVal.it_interval.tv_sec = 2;
	vVal.it_interval.tv_usec = 0;
	vVal.it_value.tv_sec = 1;
	vVal.it_value.tv_usec = 0;

	//设置信号处理函数
	signal(SIGALRM, sigAlrmProcess);
	signal(SIGVTALRM, sigVtAlrmProcess);

	//设置两个定时器
	setitimer(ITIMER_REAL, &rVal, NULL);
	setitimer(ITIMER_VIRTUAL, &vVal, NULL);

	while(1)
	{
	//	system("./waveplayer xp.wav");
	}
	return EXIT_SUCCESS;
}

