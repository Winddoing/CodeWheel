#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();

	if(pid < 0) {
		printf("error occurred!\n");
	} else if(pid == 0) {
		printf("I'm a ZOMBIE, ped=%d\n", getpid());
		exit(0);
	} else {
		printf("I'm father, ped=%d\n", getpid());
		system("ps -o pid,ppid,state,tty,command");
		sleep(3);
		wait(NULL);
		printf("father process is exiting.\n");
		system("ps -o pid,ppid,state,tty,command");
	}

	return 0;
}
