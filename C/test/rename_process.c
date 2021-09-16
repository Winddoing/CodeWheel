/*#############################################################
 *     File Name	: rename_process.c
 *     Author		: winddoing
 *     Created Time	: 2021年07月27日 星期二 18时09分42秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>


static void dump_arg(int argc, const char *argv[])
{
    int i = 0;

    printf("pid: %d, argc=%d\n", getpid(), argc);
    for (i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

}

int main(int argc, const char *argv[])
{
    unsigned int i = 0;
    unsigned int pid1, pid2;

    dump_arg(argc, argv);

    int argv0size = strlen(argv[0]);
    printf("===> func: %s, line: %d, argv0size=%d\n", __func__, __LINE__, argv0size);

    int argv1size;
    if (argv[1]) {
        argv1size = strlen(argv[1]);
        printf("===> func: %s, line: %d, argv1size=%d\n", __func__, __LINE__, argv1size);
    }

    if(0 == fork()){
        if (argv[1]) {
            char child_pname[argv1size];
            memset(child_pname, 0, strlen(child_pname));
            sprintf(child_pname, "a%s", "th.1");
            //rename_proc_title(argc, (char **)argv, child_pname);
            strncpy(argv[1], child_pname, argv1size);
        }
        dump_arg(argc, argv);
        for(;;){
            for(i=0;i<100000;i++);
            pid1= getpid();
        }
        printf("This is the child process,process id is %d",pid1);
    }else{
        dump_arg(argc, argv);
        for(;;){
            for(i=0;i<100000;i++);
            pid1= getpid();
        }
        printf("This is the child process,process id is %d",pid1);
    }
    return 0;
}

