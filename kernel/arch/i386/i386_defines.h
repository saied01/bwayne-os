#ifndef __I386_DEFINES_H__
#define __I386_DEFINES_H__

/* Misc */
/* -------------------------------------------------------------------------- */
// Y Filas
#define SIZE_N 40
#define ROWS SIZE_N

// X Columnas
#define SIZE_M 80
#define COLS SIZE_M

/* gdt indices */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 6

#define GDT_IDX_NULL_DESC 0
#define GDT_IDX_CODE_0 1
#define GDT_IDX_CODE_3 2
#define GDT_IDX_DATA_0 3
#define GDT_IDX_DATA_3 4
#define GDT_IDX_TSS 5

/* gdt offsets */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC (GDT_IDX_NULL_DESC << 3)

/* gdt segments for idt entries segment selector */
/* -------------------------------------------------------------------------- */
#define GDT_CODE_0_SEL ((GDT_IDX_CODE_0 << 3) | 0)
#define GDT_CODE_3_SEL ((GDT_IDX_CODE_3 << 3) | 3)
#define GDT_DATA_0_SEL ((GDT_IDX_DATA_0 << 3) | 0)
#define GDT_DATA_3_SEL ((GDT_IDX_DATA_3 << 3) | 3)

#define GDT_LIMIT_LOW(limit) (uint16_t)(((uint32_t)(limit)) & 0x0000FFFF)
#define GDT_LIMIT_HIGH(limit) (uint8_t)((((uint32_t)(limit)) >> 16) & 0x0F)

#define GDT_BASE_LOW(base) (uint16_t)(((uint32_t)(base)) & 0x0000FFFF)
#define GDT_BASE_MID(base) (uint8_t)((((uint32_t)(base)) >> 16) & 0xFF)
#define GDT_BASE_HIGH(base) (uint8_t)((((uint32_t)(base)) >> 24) & 0xFF)

#endif // !__I386_DEFINES_H__
