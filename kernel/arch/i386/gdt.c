#include "i386_defines.h"
#include <kernel/gdt.h>
#include <stdint.h>

gdt_descriptor_t GDT_DESC;
gdt_descriptor_t GDT_DESC_CPU;

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

  gdt_flush(&GDT_DESC);
}
