#include <stdio.h>
#include <stdlib.h>

struct st
{
	int a;
	char b;
	float c;
	char d;
	double e;	
};

int main(int argc, char *argv[])
{
	fprintf(stdout, "sizeof=%d\n", sizeof(struct st));	

	return EXIT_SUCCESS;
}

