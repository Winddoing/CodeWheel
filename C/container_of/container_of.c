/* linux-2.6.38.8/include/linux/compiler-gcc4.h */
#define __compiler_offsetof(a,b) __builtin_offsetof(a,b)

/* linux-2.6.38.8/include/linux/stddef.h */
#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/* linux-2.6.38.8/include/linux/kernel.h *
 * container_of - cast a member of a structure out to the containing structure
 * @ptr: the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({	    \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#include <stdio.h>

struct test_struct {
    int num;
    char ch;
    float fl;
};

int main(void)
{
    struct test_struct init_test_struct = { 99, 'C', 59.12 };

    char *char_ptr = &init_test_struct.ch;

    struct test_struct *test_struct = container_of(char_ptr, struct test_struct, ch);

    printf(" test_struct->num = %d\n test_struct->ch = %c\n test_struct->fl = %f\n",
	    test_struct->num, test_struct->ch, test_struct->fl);

    return 0;
}

