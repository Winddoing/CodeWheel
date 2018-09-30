/*#############################################################
 *     File Name	: dump_stack.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月30日 星期日 16时12分37秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>


#include "dumpstack.h"

void func_c()
{
	printf("xxxxxxxxxxxxxxxxxxxxxx\n");
	dump();
}

void func_b()
{
	func_c();
}

void func_a()
{
	func_b();
}

int main(int argc, const char *argv[])
{
	func_a();
	return 0;
}
