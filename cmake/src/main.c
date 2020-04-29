/*#############################################################
 *     File Name	: main.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月08日 星期二 16时11分11秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include "Config.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stdout,"%s Version %d.%d\n",
                argv[0],
                TEST_VERSION_MAJOR,
                TEST_VERSION_MINOR);
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }

    double inputValue = atof(argv[1]);
#ifdef USE_MYMATH
    double outputValue = cal_sqrt(inputValue);
#else
    double outputValue = sqrt(inputValue);
#endif
    fprintf(stdout,"The square root of %g is %g\n",
            inputValue, outputValue);


    //b = cal_sqrt(argc[1]);

    return 0;
}
