/*#############################################################
 *     File Name	: trans_sif.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时05分46秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>

#include "trans.h"

char * readfile(char *);

char ** readfile_1_svc(char **w, struct svc_req *clnt)
{
    static char *retcode = NULL;

    retcode = readfile(*(char**)w);

    return &retcode;
}

int * check_file_1_svc(check_file_args *args, struct svc_req *clnt)
{
    int ret;

    ret = check_file(args->file_name, args->str, &args->_file_data._file_data_val);

    return &ret;
}
