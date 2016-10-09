#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	int fpid;

	fpid = fork();
	if (fpid < 0) {
		/* error occurred */
		fprintf(stderr,"Fork Failed!");
		exit(-1);
	} else if (fpid == 0) {
		/* child process */
		printf("This is Child Process!\n");
		printf("ppid: %d, pid: %d, fpid: %d\n", getppid(), getpid(), fpid);
	} else {
		/* parent process */
		printf("This is Parent Process!\n");

		printf("ppid: %d, pid: %d, fpid: %d\n", getppid(), getpid(), fpid);
		/* parent will wait for the child to complete*/
		wait(NULL);
		printf("Child Complete!\n");
	}

	return EXIT_SUCCESS;
}
