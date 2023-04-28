/*#############################################################
 *     File Name	: strtok.c
 *     Author		: wqshao
 *     Created Time	: 2018年06月26日 星期二 19时32分50秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>


static void test1()
{
	int dim[3][2];
	char input[] = "120x21 32x64 120x21";
	char *token;

	token = strtok(input, " ");
	for (int i = 0; i < 3; i++) {
		printf("token=%s\n", token);
		sscanf(token, "%dx%d", &dim[i][0], &dim[i][1]);
		token = strtok(NULL, " ");
	}

	for (int i = 0; i < 3; i++) {
		printf("[%d, %d]\n", dim[i][0], dim[i][1]);
	}
}

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

	test1();

	return 0;
}
