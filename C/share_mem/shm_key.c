/*#############################################################
 *     File Name	: shm_key.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月13日 星期二 22时04分16秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/ipc.h>

int main( void )
{
    int i=0;
    for ( i = 1; i < 4; i++ )
        printf( "key = %x\n", ftok( "/tmp", i ) );


    printf("===> func: %s, line: %d, key=%x\n", __func__, __LINE__, ftok("abc_shm_mmm", 0));

    return 0;
}
