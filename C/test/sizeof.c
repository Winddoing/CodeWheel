#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
	fprintf(stdout, "sizeof=%ld\n", sizeof(struct st));

	static const uint8_t EOSNal[] =
	{
		0, 0, 1, 0x0B, 0
	}; // simulate end_of_stream
	int iSize = sizeof EOSNal;


	printf("===> func: %s, line: %d, iSize=%d, %ld\n", __func__, __LINE__, iSize, sizeof(uint8_t));


	return EXIT_SUCCESS;
}

