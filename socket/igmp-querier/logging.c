/**
 * Copyright (c) 2013, Henrik Brix Andersen <henrik@brixandersen.dk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

#include "logging.h"

static int use_syslog = 0;

void
init_logger(int syslog)
{
    if (syslog) {
        use_syslog = syslog;
        openlog("igmpqd", LOG_PID, LOG_DAEMON);
    }
}

void
logger(log_level_t level, const char *fmt, ...)
{
    va_list args1, args2;
    int priority;
    FILE *stream;

    switch (level) {
    case LOG_LEVEL_INFO:
        priority = LOG_INFO;
        stream = stdout;
        break;

    case LOG_LEVEL_ERR:
        priority = LOG_ERR;
        stream = stderr;
        fprintf(stream, "Error: ");
        break;
    }

    va_start(args1, fmt);
    va_copy(args2, args1);

    vfprintf(stream, fmt, args1);
    fprintf(stream, "\n");
    if (use_syslog) {
        vsyslog(priority, fmt, args2);
    }

    va_end(args1);
    va_end(args2);
}
