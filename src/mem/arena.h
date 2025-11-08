#ifndef ARENA_H
#define ARENA_H

#include "base.h"

#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t   cap;
    size_t   curr;
    size_t   prev;
    uint8_t* buf;
} Arena;

GLOBAL Arena arena_create(size_t cap);
GLOBAL void  arena_destroy(Arena* arena);

GLOBAL void* arena_alloc(Arena* arena, size_t len);
GLOBAL void  arena_clear(Arena* arena);

typedef struct {
    Arena* arena;
    size_t old_prev;
    size_t old_curr;
} TempArena;

GLOBAL void* temp_arena_begin(TempArena* temp_arena, Arena* arena, size_t len);
GLOBAL void  temp_arena_end(TempArena* temp_arena);

#endif /* ARENA_H */
