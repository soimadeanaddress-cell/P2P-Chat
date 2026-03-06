#include "../include/arena_pool.h"

//Credit to Lucas Ross for the code: https://github.com/upprsk

static inline void arena_block_init(arena_block* b, arena_block *next){
    b->next = next;

  b->buffer_end = ((UINT8 *)b) + BUFFER_SIZE;
  b->head = b->buffer;
}

static arena_block *arena_new_block(arena *a) {
  arena_block *blk = malloc(BUFFER_SIZE); //Should definitely be switched out later on.
  if (!blk) return NULL;

  arena_block_init(blk, a->blocks);
  a->blocks = blk;

  return blk;
}

static void *arena_block_alloc(arena_block *b, size_t size, size_t align) {
  UINT8 *head = (UINT8 *)ALIGN_TO((uintptr_t)b->head, align);
  if (head + size > b->buffer_end) {
    return NULL;
  }

  b->head = head + size;

  return head;
}

static arena_block *arena_get_block(arena *a) {
  if (a->blocks)
    return a->blocks;

  // allocate the first block in the list
  return arena_new_block(a);
}

static void *arena_alloc(arena *a, size_t size, size_t align) {
  arena_block *blk = arena_get_block(a);
  if (!blk)
    return NULL;

  void *buf = arena_block_alloc(blk, size, align);
  if (buf)
    return buf;

  blk = arena_new_block(a);
  return arena_block_alloc(blk, size, align);
}

static void arena_clear(arena *a) {
  arena_block *b = a->blocks;
  while (b) {
    arena_block *next = b->next;
    free(b);
    b = next;
  }

  a->blocks = NULL;
} 