/*#############################################################
 *     File Name	: aa.c
 *     Author		: winddoing
 *     Created Time	: 2020年12月29日 星期二 10时54分57秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
    char tst_name[32]  = {0};  

    FILE *fp = fopen("./test.conf", "r");
    if(!fp) {
        printf("can not open test.conf: %s\n", strerror(errno));
        exit(0);
    }

    char readbuf[512] = {0};
    char name[128]    = {0};
    char *delimiter = NULL;
    while(!feof(fp) && fgets(readbuf, sizeof(readbuf) - 1, fp)) {
        delimiter = strstr(readbuf, "=");

        printf("===> func: %s, line: %d, delimiter=%s\n", __func__, __LINE__, delimiter);
        if( delimiter ) {
        printf("===> func: %s, line: %d, delimiter=%s, readbuf=%s, ----%ld\n", __func__, __LINE__, delimiter, readbuf, delimiter - readbuf);
            strncpy(name, readbuf, delimiter - readbuf);
            printf("===> func: %s, line: %d, name=%s\n", __func__, __LINE__, name);

            if(readbuf[strlen(readbuf) - 1] == '\n') {
                readbuf[strlen(readbuf) - 1] = '\0';
            }
            if(readbuf[strlen(readbuf) - 1] == '\r') {
                readbuf[strlen(readbuf) - 1] = '\0';
            }

            if(strcmp(name, "tst_name") == 0) {
                strncpy(tst_name, delimiter + 1, strlen(delimiter) - 1);
            }
        }

        memset(readbuf, 0, sizeof(readbuf));
        memset(name, 0, sizeof(name));
    }

    fclose(fp);

    return 0;
}
