/*#############################################################
 *     File Name	: trans_cif.c
 *     Author		: winddoing
 *     Created Time	: 2020年11月02日 星期一 11时05分20秒
 *     Description	:
 *############################################################*/

#include <rpc/rpc.h>
#include <stdio.h>

#include "trans.h"/* Client-side stub interface routines written by programmer */

extern CLIENT * handle;
static char **ret;

char *readfile(char *name)
{
   char **arg = &name;

   ret = readfile_1(arg, handle);

   return ret==NULL ? NULL : *ret;
}
