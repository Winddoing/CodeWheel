/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : log.h
 *  Author       : wqshao
 *  Created Time : 2022-08-24 14:49:04
 *  Description  :
 */

#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

enum log_level {
    TST_LOG_EMERG = 0,
    TST_LOG_ALERT,
    TST_LOG_CRIT,
    TST_LOG_ERR,
    TST_LOG_WARNING,
    TST_LOG_NOTICE,
    TST_LOG_INFO,
    TST_LOG_DEBUG,
    TST_LOG_MAX,
};
static int def_log_level = TST_LOG_DEBUG;

#ifndef LOG_TAG
#define LOG_TAG "TEST"
#endif

#define TST_LOG_INIT() do {						\
	tst_log_init(LOG_TAG);						\
} while(0);

#define TST_LOG(level, fmt, ...) do {			\
    tst_log(level, "[%s:%d] " fmt,				\
           __func__, __LINE__, ##__VA_ARGS__);	\
} while (0);

#define MLOGE(fmt, ...) do {							\
	if (def_log_level >= TST_LOG_ERR)					\
		TST_LOG(TST_LOG_ERR, fmt, ##__VA_ARGS__);		\
} while(0);

#define MLOGW(fmt, ...) do {							\
	if (def_log_level >= TST_LOG_WARNING)				\
		TST_LOG(TST_LOG_WARNING, fmt, ##__VA_ARGS__);	\
} while(0);

#define MLOGI(fmt, ...) do {							\
	if (def_log_level >= TST_LOG_INFO)					\
		TST_LOG(TST_LOG_INFO, fmt, ##__VA_ARGS__);		\
} while(0);

#define MLOGD(fmt, ...) do {							\
	if (def_log_level >= TST_LOG_DEBUG)					\
		TST_LOG(TST_LOG_DEBUG, fmt, ##__VA_ARGS__);		\
} while(0);

void tst_log_init(char* ident);
void tst_log(enum log_level level, const char *fmt, ...);
void tst_log_fini();

#ifdef __cplusplus
}
#endif

#endif//__LOG_H__
