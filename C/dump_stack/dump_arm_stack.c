/*#############################################################
 *     File Name	: dumpstack.c
 *     Author		: winddoing
 *     Created Time	: 2019年02月22日 星期五 11时38分34秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <signal.h>
#include <dlfcn.h>

/* ARM platform */

/* define MACROS for test packed frames */
int vxdsIsString(void *p)
{
    uint8_t *str = p;

    for(int i = 0; i < 256; i++) {
        if((str[i] == 0) && (i != 0))
            break;

        if((str[i] == '\t') || (str[i] == '\r') || (str[i] == '\n'))
            continue;

        if((str[i] < 0x20) || (str[i] >= 127))
            return 0;
    }

    return 1;
}

__attribute__ ((section(".text.unlikely")))
void dump_stack(int sig)
{
    char reason[128];
    int status;
    void **fp = NULL;
    void *next;
    void **top;
    int cnt = 0;
    void *fault;
    void *badpc;
    void *esr;
    uint64_t *regs;

    __asm__( "mov %0, x29\n" : "=r"(fp));

    top   = fp[0];
    fault = top[-548];
    badpc = top[-515];
    esr   = top[-445];

    if (sig == SIGSEGV)
        snprintf(reason, 127, "Segment fault @%p, pc = %p, esr = %p", fault, badpc, esr);
    else if (sig == SIGINT || sig == SIGTERM)
        snprintf(reason, 127, "User break");
    else if (sig == SIGBUS || sig == SIGPIPE)
        snprintf(reason, 127, "System break");
    else
        snprintf(reason, 127, "Unknown error");

    printf("Signal come, reason = %s\n", reason);
    regs = (uint64_t*)&top[-547];
    printf("Caller:   \t%016lx\n", regs[30]);
    printf("Stack end:\t%016lx\n", regs[32]);
    for (int i = 0; i < 30; i++) {
        printf("\tx%-2d: %016lx", i, regs[i]);
        if (i%2 == 1)
            printf("\n");
    }

#if 0
    void **cur = fp;
    void **end = fp[0];
    do {
        if (((intptr_t)*cur & ~0xfffffful) == ((intptr_t)dump_stack & ~0xfffffful))
            printf("#%-2d, addr = %p, p = %p, code  offset: %lx\n", cur - fp, cur, *cur, *cur - (void*)dump_stack);
        else if (((intptr_t)*cur & ~0xffffful) == ((intptr_t)fp & ~0xffffful))
            printf("#%-2d, addr = %p, p = %p, stack offset: %lx\n", cur - fp, cur, *cur, *cur - (void*)fp);
        else if (vxdsIsString(cur))
            printf("#%-2d, addr = %p, p = %p, string ' %s '\n", cur - fp, cur, *cur, cur);
        else
            printf("#%-2d, addr = %p, p = %p\n", cur - fp, cur, *cur);
    } while(cur++ < end);
#endif

    printf("%-12p fp=%-12p %s+ %lx\n", badpc, NULL, NULL, badpc - (void*)dump_stack);
    while((next = fp[0]) != NULL) {
        //printf("Frame pointer = %p\n", next);
        void * pcur = fp[1];

        if (cnt > 0)
            printf("%-12p fp=%-12p %s+ %lx\n", pcur, fp, NULL, pcur - (void*)dump_stack);

        fp = fp[0];
        cnt++;
    }

    //printf("Backstrace (%d deep)\n", cnt);
    exit(1);
}

__attribute__ ((constructor))
void dumpstack_init()
{
    signal(SIGTERM, dump_stack);
    signal(SIGINT,  dump_stack);
    signal(SIGSEGV, dump_stack);
    signal(SIGBUS,  dump_stack);
    signal(SIGPIPE, dump_stack);
}

void test()
{
    int *p;

    *p = 1111; //SIGSEGV
}


int main(int argc, const char *argv[])
{
    dumpstack_init();
    
    test();
    
    return 0;
}
