#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>


int main(void)
{
	int count = 1;
	int child;

	child = vfork();

	if(child < 0) {
		perror("vfork error : ");
	} else if(child == 0) {
		printf("This is son, his count is: %d (%p). and his pid is: %d\n", ++count, &count, getpid());
		sleep(1);
		_exit(0);
	} else {
		printf("This is father, his count is: %d (%p), his pid is: %d\n", count, &count, getpid());
	}

	return 0;
}
