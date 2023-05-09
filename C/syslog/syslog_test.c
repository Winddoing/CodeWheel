/*#############################################################
 *     File Name	: syslog_test.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月26日 星期一 10时18分19秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <syslog.h>

typedef enum {
    kNoneLevel,
    kErrorLevel,
    kWarningLevel,
    kInformationLevel,
    kDebugLevel
} DebugLog;

#define DEFAULT_LOG_LEVEL  kInformationLevel

#define SYSLOG_DEF LOG_LOCAL3

#define LOG_TAG	"Tst"

#define MLOG(level, fmt, ...) \
	do { \
		syslog(level, "<%s> [%s:%d] " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__); \
} while(0);

#define MLOGD(fmt, ...) \
    do { \
        if(DEFAULT_LOG_LEVEL >= kDebugLevel) { \
            MLOG(LOG_DEBUG | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
        } \
    } while(0);

#define MLOGI(fmt, ...) \
    do { \
        if(DEFAULT_LOG_LEVEL >= kInformationLevel) { \
            MLOG(LOG_INFO | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
        } \
    } while(0);

#define MLOGW(fmt, ...) \
    do { \
        if(DEFAULT_LOG_LEVEL >= kWarningLevel) { \
            MLOG(LOG_WARNING | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
        } \
    } while(0);

#define MLOGE(fmt, ...) \
    do { \
        if(DEFAULT_LOG_LEVEL >= kErrorLevel) { \
            MLOG(LOG_ERR | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
        } \
    } while(0);


int main(int argc, const char *argv[])
{
    int aa = 11;

    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    MLOGI("Test MLOGI, int=%d\n", aa);
    MLOGW("Test MLOGW, %d, %s\n", aa, "test");
    MLOGE("Test MLOGE, ssss\n");

    return 0;
}
