#include "i386_defines.h"
#include <kernel/gdt.h>
#include <stdint.h>
#include <string.h>

gdt_descriptor_t GDT_DESC;
gdt_descriptor_t GDT_DESC_CPU;
tss_entry_t tss_entry;

extern void gdt_flush(gdt_descriptor_t *);

gdt_entry_t gdt[GDT_COUNT] =
    {
        [GDT_IDX_NULL_DESC] = {
            .Limit0 = 0x0000,
            .Base0 = 0x0000,
            .Base1 = 0x00,
            .AccessByte = 0x00,
            .Flags = 0x00,
            .Base2 = 0x00,
        },

        [GDT_IDX_CODE_0] = {
            .Limit0 = 0xFFFF,
            .Base0 = 0x0000,
            .Base1 = 0x00,
            .AccessByte = 0x9A,
            .Flags = 0xCF,
            .Base2 = 0x00,
        },

        [GDT_IDX_CODE_3] = {
            .Limit0 = 0xFFFF,
            .Base0 = 0x0000,
            .Base1 = 0x00,
            .AccessByte = 0xFA,
            .Flags = 0xCF,
            .Base2 = 0x00,
        },

        [GDT_IDX_DATA_0] = {
            .Limit0 = 0xFFFF,
            .Base0 = 0x0000,
            .Base1 = 0x00,
            .AccessByte = 0x92,
            .Flags = 0xCF,
            .Base2 = 0x00,
        },

        [GDT_IDX_DATA_3] = {
            .Limit0 = 0xFFFF,
            .Base0 = 0x0000,
            .Base1 = 0x00,
            .AccessByte = 0x92,
            .Flags = 0xF2,
            .Base2 = 0x00,
        },
};

void gdt_init(void)
{
  GDT_DESC.limit = sizeof(gdt) - 1;
  GDT_DESC.base = (uint32_t)(uintptr_t)gdt;

  write_tss(5, 0x10, 0x0);

  gdt_flush(&GDT_DESC);
  tss_flush();
}

void write_tss(uint32_t num, uint16_t ss0, uint32_t esp0)
{
  uint32_t base = (uint32_t)&tss_entry;
  uint32_t limit = base + sizeof(tss_entry);

  set_gdt_entry(num, base, limit, 0xE9, 0x00);
  memset(&tss_entry, 0, sizeof(tss_entry));

  tss_entry.ss0 = ss0;
  tss_entry.esp0 = esp0;
  tss_entry.cs = 0x08 | 0x3;
  tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void set_gdt_entry(uint32_t idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
  gdt[idx].Base0 = GDT_BASE_LOW(base);
  gdt[idx].Base1 = GDT_BASE_MID(base);
  gdt[idx].Base2 = GDT_BASE_HIGH(base);

  gdt[idx].Limit0 = GDT_LIMIT_HIGH(limit);
  gdt[idx].Flags = GDT_LIMIT_LOW(limit);
  gdt[idx].Flags |= (flags & 0xF0);

  gdt[idx].AccessByte = access;
}
