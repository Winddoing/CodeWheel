/*#############################################################
 *     File Name	: trans_sif.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时05分46秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>

char * readfile(char *);

char **readfile_1_svc(char ** w, CLIENT *clnt)
{
    static char *retcode = NULL;

    retcode = readfile(*(char**)w);

    return &retcode;
}
