#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>

// #define DUMP(func, call) printf("%s: func = %p, called by = %p\n", __FUNCTION__, func, call)
#define DUMP(func, call) printf("%p\n", call_site)
static int call_level = 0;
static void *last_fn = NULL;

void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void *this_func, void *call_site)
{
    //DUMP(this_func, call_site);
    int i;
    Dl_info di;

    for(i = 0; i < call_level; i++) 
        printf("   ");

    if(last_fn != this_func) 
        ++call_level;

    if(dladdr(this_func, &di)) {
        printf("%s\t\t(%s)\ttid:%lu -->\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname, pthread_self());
        // printf("%s\t\t(%s)\ttid:%lx %lu\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname, pthread_self(), pthread_self());
        // printf("%s\t\t(%s)\ttid:%x\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname, (int) (0xFFFFFF & pthread_self()));
    }

    last_fn = this_func;
}

void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void *this_func, void *call_site)
{
    // DUMP(this_func, call_site);

    --call_level;
#if 0
    Dl_info di;
    int i;

    for(i = 0; i < call_level; i++) printf("   ");

    if(dladdr(this_func, &di)) {
        printf("%s\t\t(%s)\ttid:%lu <--\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname, pthread_self());
    }

#endif
}

int do_multi(int a, int b)
{
    return a * b;
}

int do_add(int a, int b)
{
    return a + b;
}

int do_calc(int a, int b)
{
    do_multi(a, b);
}

void func_c(void) {}
void func_b(void)
{
    func_c();
}
void func_a(void)
{
    func_b();
    func_c();
}
void *thread_func_a(void *para)
{
    func_a();
}
void *thread_func_b(void *para)
{
    func_a();
}

int main()
{
    int a = 4, b = 5;

    do_calc(a, b);

    pthread_t pta, ptb;

    pthread_create(&pta, NULL, thread_func_a, (void *) NULL);
    do_add(a, b);
    pthread_create(&ptb, NULL, thread_func_b, (void *) NULL);
    pthread_join(pta, NULL);
    pthread_join(ptb, NULL);
    return 0;
}
