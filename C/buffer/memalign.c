#include <stdio.h>

int main()
{
	int i;
	void *buf;

	buf = memalign(4, 64); //align size invalid

	for(i=0; i<8; i++)
		printf("buf%d addr: 0x%08x\n", i, buf++);

	return 0;
}
