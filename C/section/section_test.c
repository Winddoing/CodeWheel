/*#############################################################
 *     File Name	: section_test.c
 *     Author		: winddoing
 *     Created Time	: 2022年01月26日 星期三 14时17分01秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>

typedef void (*init_call)(void);

/*
 * These two variables are defined in link script.
*/

extern init_call _init_start;
extern init_call _init_end;

#define _init __attribute__((unused, section(".myinit")))
#define DECLARE_INIT(func) init_call _fn_##func _init = func

static void A_init(void)

{
    write(1, "A_init\n", sizeof("A_init\n"));
}
DECLARE_INIT(A_init);

static void B_init(void)
{
    printf("B_init\n");
}
DECLARE_INIT(B_init);

static void C_init(void)
{
    printf("C_init\n");
}
DECLARE_INIT(C_init);

/*

 * DECLARE_INIT like below:
 *  static init_call _fn_A_init __attribute__((unused, section(".myinit"))) = A_init;
 *  static init_call _fn_C_init __attribute__((unused, section(".myinit"))) = C_init;
 *  static init_call _fn_B_init __attribute__((unused, section(".myinit"))) = B_init;
*/
void do_initcalls(void)
{
    init_call *init_ptr = &_init_start;
    for (; init_ptr < &_init_end; init_ptr++) {
        printf("init address: %p\n", init_ptr);
        (*init_ptr)();
    }
}

int main(void)
{
    do_initcalls();
    return 0;
}
