/*#############################################################
 *     File Name	: main.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月08日 星期二 16时11分11秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "version.h"

#define LOG_TAG "xxx"
#include "log.h"

#ifdef USE_MYMATH
#include "my_math.h"
#endif

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stdout,"%s Version %d.%d.%d\n",
                argv[0],
                VERSION_MAJOR,
                VERSION_MINOR,
				VERSION_PATCH);
        fprintf(stdout, "Code Version: git-%s-%s\n", GIT_BRANCH_NAME, GIT_COMMIT_HASH);
        fprintf(stdout, "Build Time: %s, %s, (%s)\n", __DATE__, __TIME__, BUILD_TIMESTAMP);
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }
#if USE_MYMATH
    printf("===> func: %s, line: %d, file: %s\n", __func__, __LINE__, __FILE__);
#endif

    double inputValue = atof(argv[1]);
#ifdef USE_MYMATH
    printf("Now we use our own Math library. \n");
    double outputValue = cal_sqrt(inputValue);
#else
    printf("Now we use the standard library. \n");
    double outputValue = sqrt(inputValue);
#endif
    fprintf(stdout,"The square root of %g is %g\n",
            inputValue, outputValue);

	MLOG_INIT();
	MLOGI("Test info log\n");
	MLOGD("Test debug log\n");
	MLOGW("Test waring log\n");
	MLOGE("Test error log\n");

    //b = cal_sqrt(argc[1]);

    return 0;
}
