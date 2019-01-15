/*#############################################################
 *     File Name	: namespace.c
 *     Author		: wqshao
 *     Created Time	: 2018年03月27日 星期二 14时59分56秒
 *     Description	:
 *############################################################*/

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mount.h>
#include <string.h>
#include <errno.h>

#define STACK_SIZE (1024*1024)
static char child_stack[STACK_SIZE];

char *const child_args[] = {
    "/bin/bash",
    NULL
};

int child_main(void *args)
{
    printf("in child process %d\n", getpid());

    sethostname("NewNameSpace-wqshao", 20); //设置主机名

    //system("mount -t proc proc /proc"); //重新mount proc文件系统到 /proc下

    if (mount("proc", "rootfs/proc", "proc", 0, NULL) !=0 ) {
        perror("proc");
    }

#if 0
    if (mount("sysfs", "rootfs/sys", "sysfs", 0, NULL)!=0) {
        perror("sys");
    }

    if (mount("udev", "rootfs/dev", "devtmpfs", 0, NULL)!=0) {
        perror("dev");
    }
#endif

    if (mount("devpts", "rootfs/dev/pts", "devpts", 0, NULL)!=0) {
        perror("dev/pts");
    }

    if (mount("none", "rootfs/tmp", "tmpfs", 0, NULL)!=0) {
        perror("tmp");
    }

    if (mount("shm", "rootfs/dev/shm", "tmpfs", 0, NULL)!=0) {
        perror("dev/shm");
    }

    /* 挂载主机home目录*/
    if (mount("/home/user", "rootfs/mnt", "none", MS_BIND, NULL)!=0) {
        perror("mnt");
    }

    /* chroot 隔离目录 */
    if ( chdir("rootfs") != 0 || chroot("./") != 0 ){
        perror("chdir/chroot");
    }
    execv(child_args[0], child_args);

    perror("execv:");
    printf("Something's wrong!\n");

    return 1;
}

int main()
{
    printf("process start: %d\n", getpid());
    int child_pid = clone(child_main, child_stack + STACK_SIZE,
            CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD, NULL);
    printf("process clone child_pid=%d\n", child_pid);
    if (child_pid != -1) {
        waitpid(child_pid, NULL, 0);
    } else {
        printf("clone failed, %s\n", strerror(errno));
    }
    printf("end \n");

    return 0;
}
