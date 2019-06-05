/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2019年05月15日 星期三 09时36分09秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

void add_asm(int a, int b, int *c)
{
	__asm__ __volatile__(
			//"add %0, %1			\n\t"
			//"mov %1, (%2)		\n\t"
			"add %[a], %[b]		\n\t"
			"mov %[b], (%[c])	\n\t"
			: /*no output*/					//输出
			: [a] "r"(a), [b] "r"(b),		//输入
			  [c] "r"(c)
			: "memory"
			);
}

void add_c(int a, int b, int *c)
{
	asm("nop");
	*c = a + b;
	asm("nop");
}

int main(int argc, const char *argv[])
{
	int a = 3, b = 4;
	int c = 0;

	add_c(a, b, &c);
	printf("add_c c=%d\n", c);

	add_asm(a, b, &c);
	printf("add_asm c=%d\n", c);
	
	return 0;
}
