#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();

	printf("pid=%d\n", pid);
	if(pid < 0) {
		printf("error occurred!\n");
	} else if(pid == 0) {
		sleep(6);
		printf("I'm a orphan, pid=%d, ppid=%d\n", getpid(), getppid());
		exit(0);
	} else {
/*		sleep(1);*/
		printf("Children Bye!, pid=%d, ppid=%d\n", getpid(), getppid());
	}

	return 0;
}
