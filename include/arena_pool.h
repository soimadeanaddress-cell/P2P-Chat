#ifndef arena_h

#define arena_h

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#define UINT8 unsigned char
#define BUFFER_SIZE 1024 //1KiB

typedef struct{
    struct arena_block* next;
    UINT8 *head;
    UINT8 *buffer_end;
    UINT8 buffer[];
}arena_block;

typedef struct{
    arena_block* blocks;
}arena;

static void *arena_alloc(arena *a, size_t size, size_t align);

static void arena_clear(arena *a);

#endif