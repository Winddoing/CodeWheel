#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#if 0
int main()
{
	int i;
	printf(">>");
	fflush(stdout);

	for(i = 1; i <= 100; i++) {
		if(i < 11) {
			printf(">");
			fflush(stdout);
		}

		printf("\b\b\b%d%%",i);
		fflush(stdout);

		usleep(10000);
	}
	printf("\n");
}
#endif

int main()
{
	int i = 0;
	char bar[101];
	const char* lable="|/-\\";

	while(i <= 100) {
		bar[i]='\0';
		printf("[%-100s] [%c] [%d%]\r", bar, lable[i%4], i);
		fflush(stdout);
		bar[i]='#';
		i++;
		usleep(10000);
	}
	printf("\n");
	return 0;
}

