#ifndef LOG_H
#define LOG_H

#include "base.h"
#include <stdio.h>

// By default, log everything
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

typedef enum {
    LOG_LEVEL_FATAL   = 0x00,
    LOG_LEVEL_ERROR   = 0x10,
    LOG_LEVEL_WARNING = 0x20,
    LOG_LEVEL_INFO    = 0x30,
    LOG_LEVEL_DEBUG   = 0x40,
} LogLevel;

GLOBAL void  log_set_stream(FILE* stream);
GLOBAL FILE* log_get_stream(void);
GLOBAL void  log_printf(LogLevel level, const char* fmt, ...);

// NOTE: sleep is platform specifig
// TODO(justin): write helper funcs for time
#include <unistd.h>
#define LOG_FATAL(fmt, ...)                                                                                \
    {                                                                                                      \
        log_printf(LOG_LEVEL_FATAL, LOCATION_INFO ANSI_COLOUR_RED " [fatal] " ANSI_COLOUR_CLEAR fmt, "\n", \
                   ##__VA_ARGS__);                                                                         \
        if (log_get_stream() != NULL) {                                                                    \
            fflush(log_get_stream());                                                                      \
            fclose(log_get_stream());                                                                      \
        }                                                                                                  \
        for (;;) { sleep(1); /* pause program execution */ }                                               \
    }

#define LOG_ERROR(fmt, ...) \
    log_printf(LOG_LEVEL_ERROR, LOCATION_INFO ANSI_COLOUR_RED " [error] " ANSI_COLOUR_CLEAR fmt "\n", ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...)                                                                                \
    log_printf(LOG_LEVEL_WARNING, LOCATION_INFO ANSI_COLOUR_YELLOW " [warning] " ANSI_COLOUR_CLEAR fmt "\n", \
               ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    log_printf(LOG_LEVEL_INFO, LOCATION_INFO ANSI_COLOUR_CYAN " [info] " ANSI_COLOUR_CLEAR fmt "\n", ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
    log_printf(LOG_LEVEL_DEBUG, LOCATION_INFO ANSI_COLOUR_MAGENTA " [debug] " ANSI_COLOUR_CLEAR fmt "\n", ##__VA_ARGS__)

#endif /* LOG_H */
