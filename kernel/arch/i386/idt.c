#include "io.h"
#include <kernel/idt.h>

idt_entry_t idt[256] = {0}; // TODO: define dummy handler to initialice all entries

idt_descriptor_t IDT_DESC = {
    .idt_length = sizeof(idt) - 1,
    .idt_offset = (uint32_t)&idt};

/* given a 32 bit value, gives only the 16 less significant bits */
#define LOW_16_BITS(v) ((uint32_t)(v) & 0xFFFF)
/* given a 32 bit value, gives only the 16 most significant bits */
#define HIGH_16_BITS(v) ((uint32_t)(v) >> 16 & 0xFFFF)

#define IDT_ENTRY0(num)                        \
  idt[num] = (idt_entry_t){                    \
      .offset_high = HIGH_16_BITS(&_isr##num), \
      .offset_low = LOW_16_BITS(&_isr##num),   \
      .segment_selector = GDT_CODE_0_SEL,      \
      .reserved = 0,                           \
      .attributes = 0x8E}

#define IDT_ENTRY3(num)                        \
  idt[num] = (idt_entry_t){                    \
      .offset_high = HIGH_16_BITS(&_isr##num), \
      .offset_low = LOW_16_BITS(&_isr##num),   \
      .segment_selector = GDT_CODE_0_SEL,      \
      .reserved = 0,                           \
      .attributes = 0xEE}

void idt_init(void)
{
  outPortB(0x20, 0x11);
  outPortB(0xA0, 0x11);

  outPortB(0x21, 0x20);
  outPortB(0xA1, 0x28);

  outPortB(0x21, 0x04);
  outPortB(0xA1, 0x02);

  outPortB(0x21, 0x01);
  outPortB(0xA1, 0x01);

  outPortB(0x21, 0x0);
  outPortB(0xA1, 0x0);

  // Exceptions (0-31)
  for (int i = 0; i < 32; i++)
    IDT_ENTRY0(i);

  // keyboard and clock interrupts (IRQ: 32-47)
  IDT_ENTRY0(32);
  IDT_ENTRY0(33);

  // Syscalls 47-256
  IDT_ENTRY3(88);
  IDT_ENTRY3(98);

  idt_flush();
}
