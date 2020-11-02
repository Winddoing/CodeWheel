#include <stdio.h>
#include <stdlib.h>

#define MAXNAME 20
#define MAXLENGTH 1024

char *readfile(char *name)
{
	FILE *file = fopen(name, "r");
	char * buf = (char *)malloc(sizeof(char) * MAXLENGTH);

	if(file == NULL) {
		printf("File Cann't Be Open!");
		return 0;
	}

	printf("The File Content is:\n");

	while(fgets(buf, MAXLENGTH - 1, file) != NULL) {
		return buf;
	}

	return NULL;
}

int main(int argc, const char *argv[])
{
	char name[MAXNAME];

	printf("Enter File Name: ");
	scanf("%s", name);
	printf("%s", readfile(name));

    return 0;
}

