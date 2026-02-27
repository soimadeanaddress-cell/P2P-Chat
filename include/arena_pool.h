#ifndef arena_h

#define arena_h

#include <stdlib.h>
#define UINT8 unsigned char
#define BUFFER_SIZE 1024 //1KiB

struct arena_block{
    struct arena_block* next;
    UINT8 *head;
    UINT8 *buffer_end;
    UINT8 buffer[];
};

struct arena{
    struct arena_block* blocks;
};

static void *arena_alloc(struct arena *a, size_t size, size_t align);

static void arena_clear(struct arena *a);

#endif