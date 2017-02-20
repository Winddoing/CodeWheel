/**********************************************************
    > File Name: off.c
    > Author: wqshao
    > Created Time: 2017年02月20日 星期一 17时13分46秒
 *********************************************************/

/* linux-2.6.38.8/include/linux/compiler-gcc4.h */
#define __compiler_offsetof(a,b) __builtin_offsetof(a,b)

/* linux-2.6.38.8/include/linux/stddef.h */
#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#include <stdio.h>

struct test_struct {
    int num;
    char ch;
    float fl;
};

int main(void)
{
	printf("sizeof(struct test_struct) = %d\n", sizeof(struct test_struct));
    printf("offsetof(struct test_struct, num) = %d\n",
			offsetof(struct test_struct, num));

    printf("offsetof(struct test_struct,  ch) = %d\n",
			offsetof(struct test_struct, ch));

    printf("offsetof(struct test_struct,  fl) = %d\n",
			offsetof(struct test_struct, fl));

    return 0;
}
