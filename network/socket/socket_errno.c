/*#############################################################
 *     File Name	: socket_errno.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月26日 星期三 11时05分34秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h> 

#define NUM 135

int main(void)
{
	int i;

	for (i = 0; i < NUM; i++) {
		printf("%d: %s\n", i, strerror(i));
	}

	perror("Over");

	return 0;
}
