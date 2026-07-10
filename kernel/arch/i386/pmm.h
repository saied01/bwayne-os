// will every task have own address space?
// yes -> add extra parameter to mapping/unmapping functions containing pointer to root page table
#ifndef PMM_H
#define PMM_H

#include <stddef.h>
#include <stdint.h>
#define PAGE_SIZE 4096 // 4kb
#define BITS_PER_ROW 32
#define MAX_MEMORY (1 * 1024 * 1024)
#define NUM_PAGES (MAX_MEMORY / PAGE_SIZE)
#define BITMAP_SIZE NUM_PAGES / 32 // with 32 bytes we can map 1MB of memory
// then to get a specific page address we do
// bit_number = (row * BITS_PER_ROW) + column
// address = bit_number * PAGE_SIZE
// bit_number represents the bit in the bitmap

void *alloc_page();
void *free_page();

#endif // !PMM_H
