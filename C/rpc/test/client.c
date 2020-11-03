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
    char *str;

    printf("Enter File Name: ");
    scanf("%s", name);
    handle = clnt_create(RMACHINE, FILETRANSPROG, FILETRANSVERS,"tcp");

    if (handle == 0) {
        printf("Could Not Connect To Remote Server./n");
        exit(1);
    }

    str = readfile(name);
    printf("%s\n", str);

    struct file_data *_file_data = NULL;
    _file_data = (struct file_data*)malloc(sizeof(struct file_data));
    if (!_file_data)
        printf("malloc file_data error\n");

    memset(_file_data->f_name, 0 , sizeof(char)*MAXNAME);
    _file_data->str = (char*)malloc(sizeof(char)*MAXLENGTH);


    check_file(name, str, _file_data);

    printf("file_data f_name:%s, str:%s\n", _file_data->f_name, _file_data->str);


    return 0;
}
