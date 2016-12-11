#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>


int main(void)
{
	int count = 1;
	int pid;

	pid = fork( );

	printf("pid=%d\n", pid);
	if(pid < 0) {
		perror("fork error : ");
	} else if(pid == 0) {
		printf("This is son, his count is: %d (%p). and his pid is: %d\n", ++count, &count, getpid());
	} else {
		printf("This is father, his count is: %d (%p), his pid is: %d\n", count, &count, getpid());
	}

	return 0;
}
