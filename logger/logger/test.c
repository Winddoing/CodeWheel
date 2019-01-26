/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月26日 星期六 15时15分56秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include "global_logger.h"

int main(int argc, const char *argv[])
{
    log_init(".", "test", LOGGER_ROTATE_PER_HOUR, 64);

    LOGI("Test info");
    LOGD("Test debug");
    LOGW("Test warn");
    LOGE("Test error");

    log_destroy();

    return 0;
}
