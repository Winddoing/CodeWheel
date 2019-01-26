#ifndef __LOGGER_H__
#define __LOGGER_H__

/* flag definitions for logger_init() */
#define LOGGER_ROTATE_BY_SIZE       0x1
#define LOGGER_ROTATE_PER_HOUR      0x2
#define LOGGER_ROTATE_PER_DAY       0x4
#define LOGGER_ROTATE_DEFAULT       LOGGER_ROTATE_PER_DAY

#define LOGGER_ROTATE_FLAG_MASK     0x7

/* ------------------------------------------------------------------------- */

struct logger {
    struct logger_impl* handler;
};

int logger_init(struct logger*, const char* dirpath, const char* prefix,
                unsigned int flags, unsigned int max_megabytes);
void logger_destroy(struct logger*);

#ifdef NDEBUG
#define logger_debug(lp, fmt, ...)
#else
#define logger_debug(lp, fmt, ...)      __logger_debug(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#define logger_misc(lp, fmt, ...)       __logger_misc(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define logger_info(lp, fmt, ...)       __logger_info(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define logger_warning(lp, fmt, ...)    __logger_warning(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define logger_error(lp, fmt, ...)      __logger_error(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define logger_fatal(lp, fmt, ...)      __logger_fatal(lp, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/* ------------------------------------------------------------------------- */

#ifndef NDEBUG
void __logger_debug(struct logger*, const char* filename, int line,
                    const char* fmt, ...);
#endif

void __logger_misc(struct logger*, const char* filename, int line,
                   const char* fmt, ...);
void __logger_info(struct logger*, const char* filename, int line,
                   const char* fmt, ...);
void __logger_warning(struct logger*, const char* filename, int line,
                      const char* fmt, ...);
void __logger_error(struct logger*, const char* filename, int line,
                    const char* fmt, ...);
void __logger_fatal(struct logger*, const char* filename, int line,
                    const char* fmt, ...);

#endif
