#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

// defines gdt descriptor
// packed 48 bit structure
typedef struct str_gdt_descriptor
{
  uint16_t limit;
  uint32_t base;
} __attribute__((__packed__)) gdt_descriptor_t;

// type → read/write/exec/conforming
// s → 1 = code/data
// dpl → ring
// p → present
// l → 64-bit (0 on i386)
// db → 0 = 16-bit, 1 = 32-bit
// g → granulariry (4KiB)

typedef struct str_gdt_entry
{
  uint16_t Limit0;
  uint16_t Base0;
  uint8_t Base1;
  uint8_t AccessByte;
  uint8_t Flags;
  uint8_t Base2;
} __attribute__((packed)) gdt_entry_t;

typedef struct str_tss_entry
{
  uint32_t prev_tss;
  uint32_t esp0;
  uint32_t ss0;
  uint32_t esp1;
  uint32_t ss1;
  uint32_t esp2;
  uint32_t ss2;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;

  uint32_t es;
  uint32_t cs;
  uint32_t ss;
  uint32_t ds;
  uint32_t fs;
  uint32_t gs;
  uint32_t ldtr;
  uint32_t trap;
  uint32_t iomap_base;
} __attribute__((packed)) tss_entry_t;

void init_gdt(void);
void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0);
void set_gdt_entry(uint32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void gdt_init(void);
void tss_flush(void);

extern gdt_entry_t gdt[];
extern gdt_descriptor_t GDT_DESC;
extern gdt_descriptor_t GDT_DESC_CPU;

#endif // !__GDT_H__
