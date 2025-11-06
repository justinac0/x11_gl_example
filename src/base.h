#ifndef BASE_H
#define BASE_H

#define INTERNAL static
#define GLOBAL   static
#define LOCAL    static

#define UNUSED(a) (void)(a)

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

#endif /* BASE_H */
