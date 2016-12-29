
volatile unsigned char a[1204];

int func()
{
	int i;

	asm __volatile__("ssnop\t\n");
	for(i = 0; i < 1024; i++)
		a[i] = i;
	asm __volatile__("ssnop\t\n");

	return 0;
}
