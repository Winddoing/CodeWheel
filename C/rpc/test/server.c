/*#############################################################
 *     File Name	: server.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时03分27秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>

#include "trans.h"
#include "common.h"

char * readfile(char * name)
{
    FILE *file = fopen(name, "r");
    char * buf = (char *)malloc(sizeof(char)*MAXLENGTH);
    if (file == NULL) {
        printf("File Cann't Be Open!");
        return 0;
    }
    printf("The File Content is : \n");
    while (fgets(buf, MAXLENGTH-1, file) != NULL) {
        return buf;
    }

    return NULL;
}
