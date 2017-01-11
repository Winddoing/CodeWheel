#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void (*oldProcess)(int);

void sigIntProcess(int signum)
{
	static int times;
	fprintf(stdout, "你第%d次按下<Ctrl + C> sig=%d oldProcess=%p。\n",
					++times, signum, oldProcess);
	signal(signum, oldProcess);
}

int main(int argc, char *argv[])
{
	int *a;
/*	oldProcess = signal(SIGINT, sigIntProcess);*/

	oldProcess = signal(11, sigIntProcess);
	*a = 1;

	while(1)
	{

	}
	return EXIT_SUCCESS;
}

