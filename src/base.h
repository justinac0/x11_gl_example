#ifndef BASE_H
#define BASE_H

#define INTERNAL static
#define GLOBAL   static
#define LOCAL    static

#define UNUSED(v) (void)(v)
#define ARRAY_LEN(a) (sizeof((a)) / sizeof((*a)))

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

#define ANSI_COLOUR_CLEAR        "\033[0m"
#define ANSI_COLOUR_BLACK        "\033[0;30m"
#define ANSI_COLOUR_RED          "\033[0;31m"
#define ANSI_COLOUR_GREEN        "\033[0;32m"
#define ANSI_COLOUR_YELLOW       "\033[0;33m"
#define ANSI_COLOUR_BLUE         "\033[0;34m"
#define ANSI_COLOUR_MAGENTA      "\033[0;35m"
#define ANSI_COLOUR_CYAN         "\033[0;36m"
#define ANSI_COLOUR_WHITE        "\033[0;37m"
#define ANSI_COLOUR_BRIGHT_BLACK "\033[1;30m"
#define ANSI_COLOUR_BRIGHT_RED   "\033[1;31m"
#define ANSI_COLOUR_BRIGHT_GREEN "\033[1;32m"
#define ANSI_COLOUR_BRIGHT_YELLOW "\033[1;33m"
#define ANSI_COLOUR_BRIGHT_BLUE  "\033[1;34m"
#define ANSI_COLOUR_BRIGHT_MAGENTA "\033[1;35m"
#define ANSI_COLOUR_BRIGHT_CYAN  "\033[1;36m"
#define ANSI_COLOUR_BRIGHT_WHITE "\033[1;37m"

#ifdef DEBUG
#define LOCATION_INFO ANSI_COLOUR_WHITE" [line="STR(__LINE__)":file='"__FILE__"']"ANSI_COLOUR_CLEAR
#else
#define LOCATION_INFO
#endif

#define TIME_MS (1)
#define TIME_SEC (TIME_MS*(1000))
#define TIME_MIN (TIME_SEC*(60))
#define TIME_HR (TIME_MIN*(60))

#define KB(v) ((1UL << 10) * (v))
#define MB(v) ((1UL << 20) * (v))
#define GB(v) ((1UL << 30) * (v))

#endif /* BASE_H */
