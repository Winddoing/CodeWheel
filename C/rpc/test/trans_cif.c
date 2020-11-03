/*#############################################################
 *     File Name	: trans_cif.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时05分20秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>

#include "trans.h"/* Client-side stub interface routines written by programmer */
#include "common.h"

extern CLIENT * handle;
static char **ret;

char *readfile(char *name)
{
   char **arg = &name;

   ret = readfile_1(arg, handle);

   return ret==NULL ? NULL : *ret;
}

int check_file(char *file_name, char *str, struct file_data *f_data)
{
    int **ret;

    check_file_args args;

    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    args.file_name = file_name;
    args.str = str;
    args._file_data._file_data_val = f_data;

    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    ret = check_file_1(&args, handle);

    return ret==NULL?NULL:*ret;
}
