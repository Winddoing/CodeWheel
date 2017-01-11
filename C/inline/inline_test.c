#include <stdio.h>


inline unsigned int test1(unsigned int a, unsigned int b)
{
	unsigned int ret;

	ret = a + b;

	return ret;
}


static inline unsigned int test2(unsigned int a, unsigned int b)
{
	unsigned int ret;

	ret = a * b;

	return ret;
}

int main(int argc, char const* argv[])
{

	unsigned a, b;

	a = test1(3, 6);

	b = test2(2, 5);

/*	printf("a=%d, b=%d\n", a, b);*/

	return 0;
}
