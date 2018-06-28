/*#############################################################
 *     File Name	: strtok.c
 *     Author		: wqshao
 *     Created Time	: 2018年06月26日 星期二 19时32分50秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int main(int argc, const char *argv[])
{
	char input[20] = "xxx abc www";
	char *delim = " ";
	char *p = NULL;

	printf("input: %s\n", input);

	p = strtok(input, delim);
	while(p != NULL) {
		printf("string: %s\n", p);
		p = strtok(NULL, delim);
	}

	return 0;
}
