INTERNAL FILE* log_stream = NULL;

GLOBAL void log_set_stream(FILE* stream) {
        log_stream = stream;
}

GLOBAL FILE* log_get_stream(void) {
        return log_stream;
}

inline GLOBAL void log_printf(LogLevel level, const char* fmt, ...) {
        struct timeval tv;
        va_list        args;
        FILE*          stream  = NULL;
        struct tm*     tm_info = NULL;

        stream = log_get_stream();
        if ((level > LOG_LEVEL && level != LOG_LEVEL_FATAL) || stream == NULL) {
                return;
        }

        gettimeofday(&tv, NULL);

        tm_info = localtime(&tv.tv_sec);

        fprintf(stream,
                ANSI_COLOUR_WHITE "(%02d:%02d:%02d:%04d)" ANSI_COLOUR_CLEAR,
                tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
                (int) (tv.tv_usec / 1000));

        va_start(args, fmt);
        vfprintf(stream, fmt, args);
        va_end(args);
}
