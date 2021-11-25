#ifndef STDIO_H
#define STDIO_H

#include "cdefs.h"
#include "stddef.h"

int mprintf(const char *fmt, ...) __printflike(1, 2);
int msnprintf(char *s, size_t n, const char *fmt, ...) __printflike(3, 4);
int msprintf(char *buf, const char *fmt, ...) __printflike(2, 3);

#ifdef STDARG_H
int vprintf(const char *fmt, va_list args);
int vsnprintf(char *s, size_t n, const char *fmt, va_list args);
#endif

#endif

