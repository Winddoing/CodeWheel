#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void (*oldProcess)(int);

void sigIntProcess(int signum)
{
	static int times;
	fprintf(stdout, "你第%d次按下<Ctrl + C>了%d。\n", 
					++times, signum);
	signal(SIGINT, oldProcess);
}

int main(int argc, char *argv[])
{
	oldProcess = signal(SIGINT, sigIntProcess);

	while(1)
	{
	
	}
	return EXIT_SUCCESS;
}

