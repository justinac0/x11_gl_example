#include "mem/list.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

INTERNAL List list_alloc_(size_t nmemb, size_t type_size) {
    List list;
    list.cap       = nmemb;
    list.len       = 0;
    list.type_size = type_size;
    list.buf       = malloc(nmemb * type_size);
    assert(list.buf);
    return list;
}

INTERNAL void* list_at_(List* list, size_t index) {
    assert(list != NULL);

    if (index >= list->len) {
        return NULL;
    }

    size_t offset = index * list->type_size;
    return (void*) ((uint8_t*) list->buf + offset);
}

INTERNAL void* list_append_(List* list, void* data) {
    assert(list != NULL);
    assert(data != NULL);

    if (list->len >= list->cap) {
        return NULL;
    }

    size_t offset = list->len * list->type_size;
    void*  ptr    = list->buf + offset;
    memcpy(ptr, data, list->type_size);
    list->len++;

    return ptr;
}

INTERNAL void list_remove_(List* list, size_t index) {
    assert(list != NULL);

    if (index >= list->len) {
        return;
    }

    void* dest = list_at_(list, index);
    assert(dest);

    void* src = list_at_(list, list->len - 1);
    assert(src);

    memcpy(dest, src, list->type_size);
    list->len--;
}

GLOBAL void list_free(List* list) {
    free(list->buf);
}

GLOBAL void list_reset(List* list) {
    list->len = 0;
}

GLOBAL void list_print(List* list, ListPrintFunc print) {
    for (size_t i = 0; i < list->len; i++) {
        void* data = list_at_(list, i);
        if (data) {
            print(data);
        }
    }
}
