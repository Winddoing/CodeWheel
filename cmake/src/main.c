/*#############################################################
 *     File Name	: main.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月08日 星期二 16时11分11秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include "my_math.h"

int main(int argc, char** argv)
{
    double a = 49.0; 
    double b = 0.0;

    printf("input a:%f\n",a);
    b = cal_sqrt(a);
    printf("sqrt result:%f\n",b);

    return 0;
}
