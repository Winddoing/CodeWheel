/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : log.h
 *  Author       : wqshao
 *  Created Time : 2022-08-24 14:49:04
 *  Description  :
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <syslog.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum log_level {
    MLOG_EMERG = 0,
    MLOG_ERR,
    MLOG_WARNING,
    MLOG_NOTICE,
    MLOG_INFO,
    MLOG_DEBUG,
    MLOG_MAX,
};
static int def_log_level = MLOG_DEBUG;

#ifndef LOG_TAG
#define LOG_TAG "TEST"
#endif

#define SYSLOG_DEF LOG_LOCAL3

#define MLOG_INIT() do { \
	openlog(LOG_TAG, LOG_PID | LOG_NDELAY, LOG_USER); \
	setlogmask(LOG_UPTO(LOG_DEBUG)); \
} while (0);

#define MLOG_DEINIT() do { \
	closelog(); \
} while (0);

#define MLOG(level, fmt, ...) do { \
    syslog(level, "[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__); \
} while (0);

#define MLOGE(fmt, ...) do { \
	if (def_log_level >= MLOG_ERR) \
		MLOG(LOG_ERR | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
} while(0);

#define MLOGW(fmt, ...) do { \
	if (def_log_level >= MLOG_WARNING) \
		MLOG(LOG_WARNING | SYSLOG_DEF, fmt, ##__VA_ARGS__);	\
} while(0);

#define MLOGI(fmt, ...) do { \
	if (def_log_level >= MLOG_INFO) \
		MLOG(LOG_INFO | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
} while(0);

#define MLOGD(fmt, ...) do { \
	if (def_log_level >= MLOG_DEBUG) \
		MLOG(LOG_DEBUG | SYSLOG_DEF, fmt, ##__VA_ARGS__); \
} while(0);

#ifdef __cplusplus
}
#endif

#endif//__LOG_H__
