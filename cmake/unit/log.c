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

static uint16_t syslog_level(enum log_level level)
{
    switch (level) {
    case TST_LOG_EMERG:
        return LOG_EMERG;
    case TST_LOG_ALERT:
        return LOG_ALERT;
    case TST_LOG_CRIT:
        return LOG_CRIT;
    case TST_LOG_ERR:
        return LOG_ERR;
    case TST_LOG_WARNING:
        return LOG_WARNING;
    case TST_LOG_NOTICE:
        return LOG_NOTICE;
    case TST_LOG_INFO:
        return LOG_INFO;
    case TST_LOG_DEBUG:
        return LOG_DEBUG;
    default:
        return LOG_DEBUG;
    }
}

void tst_log_init(char* ident)
{
	openlog(ident, LOG_NDELAY, LOG_USER);
	setlogmask(LOG_UPTO(LOG_NOTICE));
}

void tst_log(enum log_level level, const char *fmt, ...)
{
	va_list ap;
	uint16_t l;

	l = syslog_level(level);

	va_start(ap, fmt);
#if 0
	vprintf(fmt, ap);
#else
	vsyslog(LOG_MAKEPRI(LOG_USER, l), fmt, ap);
#endif
	va_end(ap);
}

void tst_log_fini()
{
	closelog();
}
