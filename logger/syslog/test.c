/*#############################################################
 *     File Name	: test.c
 *     Author		: winddoing
 *     Created Time	: 2019年01月26日 星期六 12时37分22秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <syslog.h>

#define TEST_TAG "Test Log"

int main(int argc, const char *argv[])
{
    /* 打开syslog */
    // LOG_CONS 同时写入到控制台/dev/console
    // LOG_PID 在每条消息中加上调用者的进程ID
    // LOG_LOCAL2 指定输出log，local2.* /var/log/TestLog
    openlog(TEST_TAG, LOG_CONS | LOG_PID, LOG_LOCAL2);

    /* 过滤syslog */
    // 打印info，warn，debug信息
    setlogmask(LOG_MASK(LOG_INFO) | LOG_MASK(LOG_WARNING)
            | LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_ERR));

    syslog(LOG_INFO, "Test %s","info");

    syslog(LOG_WARNING, "Test %s","warn");

    syslog(LOG_DEBUG, "Test %s", "debug");

    syslog(LOG_ERR, "Test %s", "error");

    closelog();

    return 0;
}
