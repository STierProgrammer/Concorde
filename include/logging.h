#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef enum { LOG_INFO, LOG_WARN, LOG_ERROR } log_level_t;

static inline void server_log(log_level_t level, const char *fmt, ...) {
    const char *prefix = (level == LOG_INFO) ? "[INFO]" :
                         (level == LOG_WARN) ? "[WARN]" : "[ERROR]";
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "%s ", prefix);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}

#define server_info(...) server_log(LOG_INFO, __VA_ARGS__)
#define server_warn(...) server_log(LOG_WARN, __VA_ARGS__)
#define server_error(...) server_log(LOG_ERROR, __VA_ARGS__)

#endif