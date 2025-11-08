#include "arena.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

INTERNAL bool is_power_of_two(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

INTERNAL uintptr_t align_forward(uintptr_t ptr, size_t align) {
    uintptr_t p, a, modulo;

    assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t) align;

    modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

GLOBAL Arena arena_create(size_t cap) {
    Arena arena;

    arena.buf = malloc(cap);
    assert(arena.buf);

    arena.prev = 0;
    arena.curr = 0;
    arena.cap  = cap;

    return arena;
}

GLOBAL void arena_destroy(Arena* arena) {
    assert(arena);

    free(arena->buf);
}

INTERNAL void* arena_alloc_align(Arena* arena, size_t len, size_t align) {
    assert(arena);

    uintptr_t curr_ptr = (uintptr_t) arena->buf + (uintptr_t) arena->curr;

    uintptr_t offset = align_forward(curr_ptr, align);
    offset -= (uintptr_t) arena->buf;

    if (offset + len <= arena->cap) {
        void* ptr   = &arena->buf[offset];
        arena->prev = offset;
        arena->curr = offset + len;
        memset(ptr, 0, len);

        return ptr;
    }

    return NULL;
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif

GLOBAL void* arena_alloc(Arena* arena, size_t len) {
    return arena_alloc_align(arena, len, DEFAULT_ALIGNMENT);
}

GLOBAL void arena_clear(Arena* arena) {
    assert(arena);

    arena->prev = 0;
    arena->curr = 0;
}

GLOBAL void* temp_arena_begin(TempArena* temp_arena, Arena* arena, size_t len) {
    assert(temp_arena);
    assert(arena);

    temp_arena->arena    = arena;
    temp_arena->old_prev = arena->prev;
    temp_arena->old_curr = arena->curr;

    return arena_alloc_align(arena, len, DEFAULT_ALIGNMENT);
}

GLOBAL void temp_arena_end(TempArena* temp_arena) {
    assert(temp_arena);

    temp_arena->arena->curr = temp_arena->old_curr;
    temp_arena->arena->prev = temp_arena->old_prev;
    temp_arena->arena       = NULL;
    temp_arena->old_prev    = 0;
    temp_arena->old_curr    = 0;
}
