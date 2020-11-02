/*#############################################################
 *     File Name	: client.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时02分55秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "trans.h"
#include "common.h"

#define WSVERS MAKEWORD(0, 2)
#define RMACHINE "localhost"
CLIENT *handle;

char *readfile(char *);

int main(int argc, const char *argv[])
{
    char name[MAXNAME];
    char * buf;

    printf("Enter File Name: ");
    scanf("%s", name);
    handle = clnt_create(RMACHINE, FILETRANSPROG, FILETRANSVERS,"tcp");

    if (handle == 0) {
        printf("Could Not Connect To Remote Server./n");
        exit(1);
    }

    buf = readfile(name);
    printf("%s", buf);
    return 0;
}
