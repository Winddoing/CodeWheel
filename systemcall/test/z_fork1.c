#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();

	if (pid < 0) {
		perror("fork error:");
		exit(1);
	} else if (pid == 0) {
		printf("I am child process.I am exiting. pid=%d\n", getpid());
		exit(0);
	}

	printf("I am father process.I will sleep two seconds. pid=%d\n", getpid());
	sleep(2);
/*	wait();*/
	system("ps -o pid,ppid,state,tty,command");
	printf("father process is exiting.\n");

	return 0;
}
