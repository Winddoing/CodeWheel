#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
	int count = 1;
	int pid;
	int status;
	int *a;

	pid = fork( );
	printf("pid=%d\n", pid);

	if(pid < 0) {
		perror("fork error : ");
	} else if(pid == 0) {
		printf("This is son, his count is: %d (%p). and his pid is: %d\n", 
				++count, &count, getpid());
		*a = 3;
		sleep(10);
		_exit(0);
	} else {
		pid = wait(&status);

		printf("This is father, his count is: %d (%p), his pid is: %d, son exit status: %d[%08x]\n", 
				count, &count, getpid(), status, status);
	}

	return 0;
}
