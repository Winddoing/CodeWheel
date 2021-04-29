/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月29日 星期四 15时58分39秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <signal.h>
#include <execinfo.h>

static void dump_stack()
{
    int j, nptrs;
#define SIZE 100
    void *buffer[SIZE];
    char **strings;

    nptrs = backtrace(buffer, SIZE);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("frame[%d]: %s\n", j, strings[j]);

    free(strings);
}

#if 0
void print_trace() {
    char pid_buf[30];
    sprintf(pid_buf,"%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)]=0;
    int child_pid = fork();
    if (!child_pid) {
        dup2(2,1); // redirect output to stderr
        fprintf(stdout,"stack trace for %s pid=%s",name_buf,pid_buf);
        execlp("gdb","gdb","--batch","-n","-ex","thread","-ex","bt", name_buf, pid_buf, NULL);
        abort(); /* If gdb failed to start */
    } else {
        waitpid(child_pid,NULL,0);
    }
}
#endif

static void sig_handler(int sig)
{
    if (sig == SIGINT) {
        printf("===> func: %s, line: %d, Ctrl+C\n", __func__, __LINE__);
    }

    if (sig == SIGTERM) {
        printf("===> func: %s, line: %d, killed\n", __func__, __LINE__);
        exit(0);
    }

    if (sig == SIGSEGV) {
        printf("===> func: %s, line: %d, SIGSEGV\n", __func__, __LINE__);
        dump_stack();
        //print_trace();
        exit(0);
    }
}


static void test_sig_process()
{
    struct sigaction sa;

    /* catch Ctrl+C */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;

    if(sigaction(SIGINT, &sa, NULL) == -1) {
        printf("sigaction for SIGINT failed!\n");
    }

    /* catch kill */
    if(sigaction(SIGTERM, &sa, NULL) == -1) {
        printf("sigaction for SIGTERM failed!\n");
    }

#if 1
    if(sigaction(SIGSEGV, &sa, NULL) == -1) {
        printf("sigaction for SIGSEGV failed!\n");
    }
#endif
}

static void func()
{
    int* a = NULL;

    *a = 10;
}

int main(int argc, const char *argv[])
{
    test_sig_process();

    func();

    return 0;
}
