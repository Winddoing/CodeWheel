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
	int int_a;
	int* int_a_p;
	char char_b;
	char* char_b_p;
	printf("\tSystem Data Width:\n");
	printf("sizeof int(%ld), int*(%ld), char(%ld), char*(%ld)\n",
			sizeof(int_a), sizeof(int_a_p), sizeof(char_b), sizeof(char_b_p));


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
	struct sc2_1 {
		char a;
		char b[0];
	};
	printf("sc1: sizeof-char*  = %ld\n", sizeof(struct sc1));
	printf("sc2: sizeof-char[] = %ld\n", sizeof(struct sc2));
	printf("sc2_1: packed sizeof-char[0] = %ld\n", sizeof(struct sc2_1));

	struct sc1 sc1_a;
	printf("\tsc1_a addr: %p\n", &sc1_a);
	printf("\tsc1_a.a addr: %p\n", &sc1_a.a);
	printf("\tsc1_a.b addr: %p\n", &sc1_a.b);

	struct sc2 sc2_a, sc2_b;
	printf("\tsc2_a addr: %p\n", &sc2_a);
	printf("\tsc2_a.a addr: %p\n", &sc2_a.a);
	printf("\tsc2_a.b addr: %p\n", &sc2_a.b);
	printf("\tsc2_b addr: %p\n", &sc2_b);

	struct __attribute__ ((packed)) sc3 {
		char a;
		char *b;
	};
	struct __attribute__ ((packed)) sc4 {
		char a;
		char b[];
	};
	printf("sc3: packed sizeof-char*  = %ld\n", sizeof(struct sc3));
	printf("sc4: packed sizeof-char[] = %ld\n", sizeof(struct sc4));

	struct sc3 sc3_a;
	printf("\tsc3_a addr: %p\n", &sc3_a);
	printf("\tsc3_a.a addr: %p\n", &sc3_a.a);
	printf("\tsc3_a.b addr: %p\n", &sc3_a.b);

	struct sc4 sc4_a, sc4_b;
	printf("\tsc4_a addr: %p\n", &sc4_a);
	printf("\tsc4_a.a addr: %p\n", &sc4_a.a);
	printf("\tsc4_a.b addr: %p\n", &sc4_a.b);
	printf("\tsc4_b addr: %p\n", &sc4_b);

	struct __attribute__ ((aligned(4))) sc5 {
		char a;
		char *b;
	};
	struct __attribute__ ((aligned(4))) sc6 {
		char a;
		char b[];
	};
	printf("sc5: aligned 4 sizeof-char*  = %ld\n", sizeof(struct sc5));
	printf("sc6: aligned 4 sizeof-char[] = %ld\n", sizeof(struct sc6));


	struct __attribute__ ((aligned(2))) sc7 {
		char a;
		char *b;
	};
	struct __attribute__ ((aligned(2))) sc8 {
		char a;
		char b[];
	};
	printf("sc7: aligned 2 sizeof-char*  = %ld\n", sizeof(struct sc7));
	printf("sc8: aligned 2 sizeof-char[] = %ld\n", sizeof(struct sc8));

	struct __attribute__ ((aligned(8))) sc9 {
		char a;
		char *b;
	};
	struct __attribute__ ((aligned(8))) sc10 {
		char a;
		char b[];
	};
	printf("sc9: aligned 8 sizeof-char*  = %ld\n", sizeof(struct sc9));
	printf("sc10: aligned 8 sizeof-char[] = %ld\n", sizeof(struct sc10));
	return 0;
}
