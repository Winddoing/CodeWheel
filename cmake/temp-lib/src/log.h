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
    S_LOG_EMERG = 0,
    S_LOG_ALERT,
    S_LOG_CRIT,
    S_LOG_ERR,
    S_LOG_WARNING,
    S_LOG_NOTICE,
    S_LOG_INFO,
    S_LOG_DEBUG,
    S_LOG_MAX,
};

extern int def_log_level;

#ifndef LOG_TAG
#define LOG_TAG "Enc"
#endif

#define LOG_INIT() do {						\
	slog_init(LOG_TAG);						\
} while(0);

#define LOG_FINI() do {						\
	slog_fini();								\
} while(0);


#define LOG(level, fmt, ...) do {			\
    slog(level, "[%s:%d] " fmt,				\
           __func__, __LINE__, ##__VA_ARGS__);	\
} while (0);

#define MLOGE(fmt, ...) do {							\
	if (def_log_level >= S_LOG_ERR)					\
		LOG(S_LOG_ERR, fmt, ##__VA_ARGS__);		\
} while(0);

#define MLOGW(fmt, ...) do {							\
	if (def_log_level >= S_LOG_WARNING)				\
		LOG(S_LOG_WARNING, fmt, ##__VA_ARGS__);	\
} while(0);

#define MLOGI(fmt, ...) do {							\
	if (def_log_level >= S_LOG_INFO)					\
		LOG(S_LOG_INFO, fmt, ##__VA_ARGS__);		\
} while(0);

#define MLOGD(fmt, ...) do {							\
	if (def_log_level >= S_LOG_DEBUG)					\
		LOG(S_LOG_DEBUG, fmt, ##__VA_ARGS__);		\
} while(0);

void slog_init(char* ident);
void slog(enum log_level level, const char *fmt, ...);
void slog_fini();

#ifdef __cplusplus
}
#endif

#endif//__LOG_H__
