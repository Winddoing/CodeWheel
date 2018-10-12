/*#############################################################
 *     File Name	: struct.c
 *     Author		: wqshao
 *     Created Time	: 2018年10月10日 星期三 16时55分10秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>


struct v {
	int a;
	int b;
};

void test(struct v *vp)
{
	int i = 0;

	for (i = 0; i < 3; i++) {
		vp[i].a = i;
		vp[i].b = i + 1;
	}

	for (i = 0; i < 3; i++) {
		printf("===> func: %s, line: %d, vp[%d].a=%d, vp[%d].b=%d\n",
				__func__, __LINE__, i, vp[i].a, i, vp[i].b);
	}
}

int main(int argc, const char *argv[])
{
	int i = 0;
	struct v *vp;

	vp = (struct v*)malloc(sizeof(struct v) * 3);
	if (!vp)
		return -1;

	test(vp);

	free(vp);

	struct v va[3];

	test(va);
	
	return 0;
}
