#include <stdio.h>
#include <stdlib.h>

int main(int argv, char* argc[])
{
	int n = 0;

	while(1)
	{
		sleep(1);
		fprintf(stderr, "\r%02d", n++);
	}

	return EXIT_SUCCESS;
}
