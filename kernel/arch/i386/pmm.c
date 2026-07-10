#include "pmm.h"
#include <stddef.h>
#include <stdint.h>

static uint32_t bitmap[BITMAP_SIZE];

// calculate frame from address:
// physical address ->
// frame = address / PAGE_SIZE ->
// entry = frame / 32
// bit   = frame % 32
static size_t addr_to_frame(size_t addr)
{
  return addr / PAGE_SIZE;
}

static char test_bit(size_t frame)
{
  size_t entry = frame / BITS_PER_ROW;
  size_t bit_index = frame % BITS_PER_ROW;

  return (bitmap[entry] >> bit_index) & 1;
}
static void set_bit(size_t frame);
static void clear_bit(size_t frame);

void *alloc_page();
void *free_page();
