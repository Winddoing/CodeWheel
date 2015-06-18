#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <global.h>

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	int fifoFd = -1;
	ssize_t ret;

	fifoFd = open(FIFO_NAME, O_RDONLY);
	if(-1 == fifoFd)
		goto Error;
	bzero(buffer, sizeof(buffer));
	while(1)
	{
		ret = read(fifoFd, buffer, BUFFER_SIZE);
		if(-1 == ret)
			goto Error;
		if(0 == ret)
				break;
		if(0 == strncasecmp(QUIT_CMD, buffer, strlen(QUIT_CMD)))
			break;
		fprintf(stdout, "%s\n", buffer);
	}
	close(fifoFd);
	fprintf(stdout, "Bye-Bye !\n");
	return EXIT_SUCCESS;
Error:
	perror("Writefifo");
	if(-1 == fifoFd)
		return EXIT_FAILURE;
	close(fifoFd);
	return EXIT_FAILURE;
}






