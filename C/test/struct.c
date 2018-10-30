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

struct av_ {
	struct v *vap[3];
};

static struct av_ *av;

static int test(struct v **vp)
{
	int i = 0, i_do = 0;
	int ret = 0;

	for (i = 0; i < 3; i++) {
		vp[i] = (struct v*)malloc(sizeof(struct v));
		if (!vp[i]) {
			i_do = i;
			ret = -1;
			goto err;
		}
		vp[i]->a = i;
		vp[i]->b = i + 1;
	}
	i_do = i;
	printf("===> func: %s, line: %d, i_do=%d\n", __func__, __LINE__, i_do);

	for (i = 0; i < 3; i++) {
		if (vp[i]) {
			printf("===> func: %s, line: %d, vp[%d].a=%d, vp[%d].b=%d\n",
					__func__, __LINE__, i, vp[i]->a, i, vp[i]->b);
		}
	}

err:
	for (i = 0; i < i_do; i++) {
		if (vp[i])
			free(vp[i]);
	}

	return ret;
}

int main(int argc, const char *argv[])
{
	int ret = 0;

	av = (struct av_*)malloc(sizeof(struct av_));
	if (!av) {
		return -1;
	}
	ret = test(av->vap);

	free(av);

	return ret;
}
