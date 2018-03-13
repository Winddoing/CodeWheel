#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/ptrace.h>
#include <string.h>
static int buf[256];
int main(int argc, char *argv[])
{
    pid_t child;
	struct pt_watch_regs watch;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		memset(buf,0,256 * 4);
		execl("/bin/ls", "ls", NULL);

    }
    else {
        wait(NULL);
		ptrace(PTRACE_GET_WATCH_REGS,child,&watch,0);
		printf("style:%d\n",watch.style);
		printf("num_valid:%d\n",watch.mips32.num_valid);
		for(int i = 0;i < 8;i++){
			printf("watchlo[%d]:%08x\n",i,watch.mips32.watchlo[i]);
			printf("watchhi[%d]:%04x\n",i,watch.mips32.watchhi[i]);
			printf("watch_mask[%d]:%04x\n",i,watch.mips32.watch_masks[i]);
		}
		watch.mips32.watchlo[0] = ((unsigned int)buf & (~0x7)) | 1;
		watch.mips32.watchhi[0] = 1;
		watch.mips32.watch_masks[0] = 0xffff;
		ptrace(PTRACE_SET_WATCH_REGS,child,&watch,0);

        ptrace(PTRACE_CONT, child, NULL, NULL);
    }
    return 0;
}
