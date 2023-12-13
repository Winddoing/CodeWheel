#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <linux/ioctl.h>
#include <linux/input.h>
#include <dirent.h>

#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))


void getState(char *file){
    int fd;
    int ret;
    uint8_t ledBitMask[(LED_MAX + 1) / 8];
    uint8_t evbit[EV_MAX/8 +1];

    fd = open(file, O_RDWR /*| O_CLOEXEC*/);
    if (fd < 0){
        printf("open file %s fail, err:%s\n", file, strerror(errno));
        fd = open(file, O_RDONLY);
        printf("opendir %s %s ret=%d\n", file, strerror(errno), ret);
        return;
    }
    
    memset(evbit, 0, sizeof(evbit));
    ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit);

    if(test_bit(EV_KEY, evbit) && test_bit(EV_REP, evbit) && test_bit(EV_LED, evbit)){
        memset(ledBitMask, 0, sizeof(ledBitMask));
        ret = ioctl(fd, EVIOCGLED(sizeof(ledBitMask)), ledBitMask);
    } else {
        ret = -1;
    }
    close(fd);

    if (ret < 0)
        return;
    
    printf("%s:\n", file);
    printf("\tNUML:%s\n", test_bit(LED_NUML, ledBitMask) ? "on" : "off");
    printf("\tCAPSL:%s\n", test_bit(LED_CAPSL, ledBitMask) ? "on" : "off");
    printf("\tSCROLL:%s\n", test_bit(LED_SCROLLL, ledBitMask) ? "on" : "off");
    return;
}

void scanDir(char *dir){
    char file[256];
    DIR *dfd;
    struct dirent *dp;

    dfd = opendir(dir);

    if (dfd == NULL){
        printf("can't open dir:%s error:%s\n", dir, strerror(errno));
        return;
    }

    while(dp = readdir(dfd)){
        if (strcmp(dp->d_name, ".") == 0  || strcmp(dp->d_name, "..") == 0){
            continue;
        }
        sprintf(file, "%s/%s", dir, dp->d_name);
        getState(file);
    }
    closedir(dfd);
    return;
}

void processPath(char *path){
    struct stat st;
    stat(path, &st);

    if ( (st.st_mode & S_IFMT) == S_IFDIR ){
        scanDir(path);
    }else{
        getState(path);
    }
    return;
}

int main(int argc, char *argv[]){
    char *path;
    int ret;
    if (argc < 2)
        path = "/dev/input";
    else
        path = argv[1];
    
    ret = access(path, F_OK | R_OK);
    if (ret < 0){
        printf("can't access path:%s, error:%s!\n", path, strerror(errno));
        return -1;
    }

    printf("get LED State from path: %s\n", path);
    
    processPath(path);

    return 0;
}