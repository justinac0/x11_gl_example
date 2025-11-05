#ifndef BASE_H
#define BASE_H

#ifdef NORMAL_BUILD
#define INTERNAL
#define GLOBAL
#define LOCAL
#else
#define INTERNAL static
#define GLOBAL   static
#define LOCAL    static
#endif

#define ARRAY_LEN(a) (sizeof(a) / sizeof(*a))

#endif /* BASE_H */
