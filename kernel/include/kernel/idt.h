#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>
#define INTERRUPT_GATE_TYPE 0 // TODO Completar

typedef struct str_idt_descriptor
{
  uint16_t idt_length;
  uint32_t idt_addr;
} __attribute__((__packed__)) idt_descriptor_t;

typedef struct str_idt_entry
{
  // TODO Completar
} __attribute__((__packed__)) idt_entry_t;

extern idt_entry_t idt[];
extern idt_descriptor_t IDT_DESC;

void idt_init(void);

#endif // !__IDT_H__
