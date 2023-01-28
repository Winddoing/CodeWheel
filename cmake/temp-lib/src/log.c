/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : log.c
 *  Author       : wqshao
 *  Created Time : 2022-08-24 14:13:26
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <syslog.h>
#include <stdarg.h> //va_start

#include "log.h"
#include "helper.h"

int def_log_level = S_LOG_INFO;

static uint16_t syslog_level(enum log_level level)
{
    switch (level) {
    case S_LOG_EMERG:
        return LOG_EMERG;
    case S_LOG_ALERT:
        return LOG_ALERT;
    case S_LOG_CRIT:
        return LOG_CRIT;
    case S_LOG_ERR:
        return LOG_ERR;
    case S_LOG_WARNING:
        return LOG_WARNING;
    case S_LOG_NOTICE:
        return LOG_NOTICE;
    case S_LOG_INFO:
        return LOG_INFO;
    case S_LOG_DEBUG:
        return LOG_DEBUG;
    default:
        return LOG_DEBUG;
    }
}

void slog_debug()
{
	def_log_level = S_LOG_DEBUG;
}

void slog_init(char* ident)
{
	openlog(ident, LOG_PID | LOG_NDELAY, LOG_USER);
	setlogmask(LOG_UPTO(LOG_DEBUG));
}

void slog(enum log_level level, const char *fmt, ...)
{
	va_list ap;
	uint16_t l;

	l = syslog_level(level);

	va_start(ap, fmt);
#if 1
	UNUSED(l);
	vprintf(fmt, ap);
#else
	vsyslog(LOG_MAKEPRI(LOG_USER, l), fmt, ap);
#endif
	va_end(ap);
}

void slog_fini()
{
	closelog();
}
