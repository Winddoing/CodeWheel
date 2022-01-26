/*#############################################################
 *     File Name	: section_struct_demo.c
 *     Author		: winddoing
 *     Created Time	: 2022年01月26日 星期三 14时45分58秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern const struct test __start_test_section, __stop_test_section;

struct test {
    const char *name;
    void (*run)(void);
} __attribute__ ((aligned (16)));

#define TEST(name)      \
    static void name(void);     \
                                \
    const struct test test##name        \
        __attribute__ ((used, section ("test_section"))) = {    \
        #name, name  };                      \
                            \
    static void name(void)



TEST(aaa)
{

    printf("aaa===> func: %s, line: %d\n", __func__, __LINE__);
}


TEST(bbb)
{

    printf("bbb===> func: %s, line: %d\n", __func__, __LINE__);
}


int main(int argc, const char *argv[])
{
    const struct test *t;

    for (t = &__start_test_section; t < &__stop_test_section; t++) {

        t->run();
    }
    
    return 0;
}
