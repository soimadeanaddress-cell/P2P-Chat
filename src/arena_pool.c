#include "../include/arena_pool.h"

//Credit to Lucas Ross for the code: https://github.com/upprsk

static inline void arena_block_init(struct arena_block* b, struct arena_block *next){
    b->next = next;

  b->buffer_end = ((UINT8 *)b) + BUFFER_SIZE;
  b->head = b->buffer;
}

static struct arena_block *arena_new_block(struct arena *a) {
  struct arena_block *blk = malloc(BUFFER_SIZE); //Should definitely be switched out later on.
  if (!blk) return NULL;

  arena_block_init(blk, a->blocks);
  a->blocks = blk;

  return blk;
}

static void *arena_block_alloc(struct arena_block *b, size_t size, size_t align) {
  UINT8 *head = (UINT8 *)ALIGN_TO((uintptr_t)b->head, align);
  if (head + size > b->buffer_end) {
    return NULL;
  }

  b->head = head + size;

  return head;
}

static struct arena_block *arena_get_block(struct arena *a) {
  if (a->blocks)
    return a->blocks;

  // allocate the first block in the list
  return arena_new_block(a);
}

static void *arena_alloc(struct arena *a, size_t size, size_t align) {
  struct arena_block *blk = arena_get_block(a);
  if (!blk)
    return NULL;

  void *buf = arena_block_alloc(blk, size, align);
  if (buf)
    return buf;

  blk = arena_new_block(a);
  return arena_block_alloc(blk, size, align);
}

static void arena_clear(struct arena *a) {
  struct arena_block *b = a->blocks;
  while (b) {
    struct arena_block *next = b->next;
    free(b);
    b = next;
  }

  a->blocks = NULL;
}