#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>

#define VALID_PROCSTATE 0xa84b62fc

struct procState {
    char procname[32];
    FILE * fp;
    int pid;
    unsigned int utimeold;
    unsigned int utimenew;
    unsigned int stimeold;
    unsigned int stimenew;
    unsigned int allold;
    unsigned int allnew;
    int valid;
};

#define MAX_THREADS_NUM 100
             
struct procState lanProc[MAX_THREADS_NUM];
int procTotal = 0;
char targetProcName[64] = "";
int targetProcPid = 0;

char taskName[64] = "";
FILE *taskfp;
FILE *statfp;

char *version = "1.1.0";
char *procdevice = "/proc/stat";

struct dirent *taskdirent;
DIR * taskdir;

struct stats {
    unsigned int user;
    unsigned int nice;
    unsigned int system;
    unsigned int idle;
    unsigned int total;
};

void readprocessstat(void);

int getdata(struct stats *st)
{
    unsigned char   buf[80];
    int i;

    for(i = 0; i < procTotal; i++) {
        if((lanProc[i].fp = fopen(lanProc[i].procname, "r")) == NULL) {
            fprintf(stderr, "ERROR: failed, errno=%d\n", errno);
            exit(-1);
        }

        fscanf(lanProc[i].fp, "%d %s %s %s %s %s %s %s %s %s %s %s %s %lu %lu", &lanProc[i].pid,
               &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0], &buf[0],
               &lanProc[i].utimenew, &lanProc[i].stimenew);
        fclose(lanProc[i].fp);
    }

    if((statfp = fopen(procdevice, "r")) == NULL) {
        fprintf(stderr, "ERROR: failed to open %s, errno=%d\n",
                procdevice, errno);
        exit(0);
    }

    fscanf(statfp, "%s %d %d %d %d", &buf[0], &st->user, &st->nice, &st->system, &st->idle);
    fclose(statfp);

    st->total = st->user + st->nice + st->system + st->idle;

    for(i = 0; i < procTotal; i++) {
        lanProc[i].allnew = lanProc[i].utimenew + lanProc[i].stimenew;
    }

    return(0);
}

void usage(FILE *fp, int rc)
{
    fprintf(fp, "Usage: ./thread_top [-h?v] [-p <pid>] [-c <count>] [-s seconds]\n\n");
    fprintf(fp, "        -h?            this help\n");
    fprintf(fp, "        -v             print version info\n");
    fprintf(fp, "        -p <pid>       pid of target proccess\n");
    fprintf(fp, "        -c count       repeat count times\n");
    fprintf(fp, "        -s seconds     seconds between output\n");
    exit(rc);
}

int main(int argc, char *argv[])
{
    struct stats st, stold;
    unsigned int curtotal;
    int c = 0;
    int cnt = 1;
    int loop = 1;
    int delay = 1;
    int busy = 0;
    int max_busy = 0;
    int i = 0;

    if(argc < 2)
        usage(stdout, 0);

    while((c = getopt(argc, argv, "h?vc:s:p:")) > 0) {
        switch(c) {
        case 'v':
            printf("%s: version %s\n", argv[0], version);
            exit(0);

        case 'c':
            loop = 0;
            cnt = atoi(optarg) + 1;
            break;

        case 's':
            delay = atoi(optarg);
            break;

        case 'h':
        case '?':
            usage(stdout, 0);
            break;

        case 'p':

            //struct dirent {
            //   ino_t          d_ino;       /* inode number */
            //   off_t          d_off;       /* offset to the next dirent */
            //   unsigned short d_reclen;    /* length of this record */
            //   unsigned char  d_type;      /* type of file */
            //   char           d_name[256]; /* filename */
            //};

            sprintf(targetProcName, "%s", optarg);

            sprintf(taskName, "/proc/%s/task", optarg);

            if((taskdir = opendir(taskName)) == NULL) {
                fprintf(stderr, "ERROR: failed to open %s, errno=%d\n", taskName, errno);
                exit(-1);
            }

            targetProcPid = atol(targetProcName);

            for(i = 0; i < MAX_THREADS_NUM; i++) {
                if((taskdirent = readdir(taskdir)) == NULL) {
                    break;
                }

                lanProc[procTotal].pid = atoi(taskdirent->d_name); // /proc/%s/task 下的文件名即为线程tid号

                if(lanProc[procTotal].pid < targetProcPid) { //一般不会小于父进程id
                    continue;
                }

                sprintf(lanProc[procTotal].procname, "/proc/%s/task/%s/stat", optarg, taskdirent->d_name);
                lanProc[procTotal].valid = VALID_PROCSTATE;
                procTotal++;
            }

            break;

        default:
            fprintf(stderr, "ERROR: unkown option '%c'\n", c);
            usage(stderr, 1);
            break;
        }
    }

    getdata(&st);

    for(c = 0; (loop || (c < cnt)); c++) {
        sleep(delay);

        stold = st;
        getdata(&st);

        curtotal = st.total - stold.total;
        busy = ((st.system + st.user + st.nice) - (stold.system + stold.user + stold.nice)) * 100 / curtotal;

        if(max_busy < busy)
            max_busy = busy;

        if(c) //第一次不打印
            printf("busy %3d%%:max=%3d%%(system %3d%%, user %3d%%, nice %3d%%, idle %3d%%)\n",
                   busy, max_busy,
                   (st.system - stold.system) * 100 / curtotal,
                   (st.user - stold.user) * 100 / curtotal,
                   (st.nice - stold.nice) * 100 / curtotal,
                   (st.idle - stold.idle) * 100 / curtotal);

        for(i = 0; i < procTotal; i++) {
            if(lanProc[i].valid != VALID_PROCSTATE)
                continue;

            if(c) //第一次不打印
                //printf("pthread:%3d | alltime:%6lu %3d%% | usrtime:%6lu %3d%% | systime:%6lu %3d%% |\n",
                printf("pthread:%3d | usrtime:%6lu %3d%% | systime:%6lu %3d%% |\n",
                       lanProc[i].pid,
                       //lanProc[i].allnew - lanProc[i].allold,
                       //(lanProc[i].allnew - lanProc[i].allold) * 100 / curtotal,
                       lanProc[i].utimenew - lanProc[i].utimeold,
                       (lanProc[i].utimenew - lanProc[i].utimeold) * 100 / curtotal,
                       lanProc[i].stimenew - lanProc[i].stimeold,
                       (lanProc[i].stimenew - lanProc[i].stimeold) * 100 / curtotal);

            lanProc[i].allold = lanProc[i].allnew;
            lanProc[i].utimeold = lanProc[i].utimenew;
            lanProc[i].stimeold = lanProc[i].stimenew;
        }

        printf("\n");
    }

    exit(0);
}
