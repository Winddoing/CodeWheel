/*#############################################################
 *     File Name	: char.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月08日 星期六 16时27分14秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *c1 = "a b c d";
	char c2[] = "a b c d";

	printf("c1: %s\n", c1);
	printf("c2: %s\n", c2);

	printf("c1-c1[0]=%c\n", c1[0]);
	printf("c2-c2[0]=%c\n", c2[0]);

	//c1[0] = 'x'; /* Segmentation fault (core dumped) */
	c2[0] = 'y';

	printf("c1-c1[0]=%c\n", c1[0]);
	printf("c2-c2[0]=%c\n", c2[0]);

	struct sc1 {
		char a;
		char *b;
	};
	struct sc2 {
		char a;
		char b[];
	};
	printf("sc1: sizeof-char*  = %ld\n", sizeof(struct sc1));
	printf("sc2: sizeof-char[] = %ld\n", sizeof(struct sc2));

	struct __attribute__ ((__packed__)) sc3 {
		char a;
		char *b;
	};
	struct __attribute__ ((__packed__)) sc4 {
		char a;
		char b[];
	};
	printf("sc3: packed sizeof-char*  = %ld\n", sizeof(struct sc3));
	printf("sc4: packed sizeof-char[] = %ld\n", sizeof(struct sc4));
	return 0;
}
