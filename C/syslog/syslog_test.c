/*#############################################################
 *     File Name	: syslog_test.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月26日 星期一 10时18分19秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdarg.h>
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

enum cb_log_level {
        CB_LOG_EMERG = 0,
        CB_LOG_ALERT,
        CB_LOG_CRIT,
        CB_LOG_ERR,
        CB_LOG_WARNING,
        CB_LOG_NOTICE,
        CB_LOG_INFO,
        CB_LOG_DEBUG,
        CB_LOG_MAX,
};

static uint16_t syslog_level(enum cb_log_level level)
{
        switch (level) {
        case CB_LOG_EMERG:
                return LOG_EMERG;
        case CB_LOG_ALERT:
                return LOG_ALERT;
        case CB_LOG_CRIT:
                return LOG_CRIT;
        case CB_LOG_ERR:
                return LOG_ERR;
        case CB_LOG_WARNING:
                return LOG_WARNING;
        case CB_LOG_NOTICE:
                return LOG_NOTICE;
        case CB_LOG_INFO:
                return LOG_INFO;
        case CB_LOG_DEBUG:
                return LOG_DEBUG;
        default:
                return LOG_DEBUG;
        }
}


void cb_log_init(void)
{
        openlog("TEST", LOG_NDELAY, LOG_USER);
        //setlogmask(LOG_UPTO(LOG_NOTICE));
        setlogmask(LOG_UPTO(LOG_INFO));
}

void cb_log_deinit(void)
{
        closelog();
}

void cb_log(enum cb_log_level level, const char *fmt, ...)
{
        va_list ap;
        uint16_t l;

        l = syslog_level(level);
        va_start(ap, fmt);
        vsyslog(LOG_MAKEPRI(LOG_USER, l), fmt, ap);
        va_end(ap);
}


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
