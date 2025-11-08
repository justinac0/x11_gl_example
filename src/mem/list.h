#ifndef LIST_H
#define LIST_H

#include "base.h"

#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t   type_size;
    size_t   len;
    size_t   cap;
    uint8_t* buf;
} List;

INTERNAL List  list_alloc_(size_t nmemb, size_t type_size);
INTERNAL void* list_at_(List* list, size_t index);
INTERNAL void* list_append_(List* list, void* data);
INTERNAL void  list_remove_(List* list, size_t index);

GLOBAL void list_free(List* list);
GLOBAL void list_reset(List* list);

typedef void (*ListPrintFunc)(void* data);
GLOBAL void list_print(List* list, ListPrintFunc print);

#define list_alloc(nmemb, type)       list_alloc_(nmemb, sizeof(type))
#define list_at(list, index, type)    (type*) list_at_(list, index)
#define list_append(list, data, type) (type*) list_append_(list, data)
#define list_remove(list, index)      list_remove_(list, index)

#endif /* LIST_H */
