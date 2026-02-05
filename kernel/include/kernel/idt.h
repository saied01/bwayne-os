#ifndef I386_IDT_H
#define I386_IDT_H

#include <stdint.h>

struct InterruptRegisters;

typedef struct str_idt_descriptor
{
  uint16_t idt_length;
  uint32_t idt_offset;
} __attribute__((__packed__)) idt_descriptor_t;

typedef struct str_idt_entry
{
  uint16_t offset_low;
  uint16_t segment_selector;
  uint8_t reserved; // Must be loaded to 0
  uint8_t attributes;
  uint16_t offset_high;
} __attribute__((__packed__)) idt_entry_t;

extern idt_entry_t idt[];
extern idt_descriptor_t IDT_DESC;

void idt_init(void);
extern void idt_flush(uint32_t);

void isr_handler(struct InterruptRegisters *regs);
void irq_handler(struct InterruptRegisters *regs);

struct InterruptRegisters
{
  uint32_t cr2;
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_num, err_code;
  uint32_t eip, csm, eflags, useresp, ss;
} __attribute__((__packed__));

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

extern void isr88();
extern void isr98();

#endif // !I386_IDT_H
