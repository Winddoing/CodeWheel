/*#############################################################
 *     File Name	: mmx_add.c
 *     Author		: winddoing
 *     Created Time	: 2019年04月30日 星期二 14时58分08秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <xmmintrin.h>

void test_smid_add(uint8_t a[8], uint8_t b[8], uint8_t c[8])
{
	*(__m64*)c = _mm_add_pi8(*(__m64*)a, *(__m64*)b);
}

/*gcc -Wall -mmmx -o add  mmx_add.c*/
int main(int argc,char *argv[])
{ 
	uint8_t in1[] = {1, 2, 3, 4, 2, 4, 4, 1};
	uint8_t in2[] = {2, 3, 4, 5, 3, 1, 2, 5};
	uint8_t out[8] = {0};
	int i = 0;

	test_smid_add(in1, in2, out);

	for (i = 0; i < 8; i++) {
		printf("%d ", in1[i]);
	}

	printf("\n");
	for (i = 0; i < 8; i++) {
		printf("%d ", in2[i]);
	}
	printf("\n");

	for (i = 0; i < 8; i++) {
		printf("%d ", out[i]);
	}
	printf("\n");

	return(0);
}
