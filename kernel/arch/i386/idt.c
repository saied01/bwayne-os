#include "i386_defines.h"
#include "io.h"
#include <kernel/idt.h>
#include <kernel/tty.h>

struct InterruptRegisters;

idt_entry_t idt[256] = {0}; // TODO: define dummy handler to initialize all entries

idt_descriptor_t IDT_DESC = {
    .idt_length = sizeof(idt) - 1,
    .idt_offset = (uint32_t)&idt};

/* given a 32 bit value, gives only the 16 less significant bits */
#define LOW_16_BITS(v) ((uint32_t)(v) & 0xFFFF)
/* given a 32 bit value, gives only the 16 most significant bits */
#define HIGH_16_BITS(v) ((uint32_t)(v) >> 16 & 0xFFFF)

#define IDT_ENTRY0(num)                       \
  idt[num] = (idt_entry_t){                   \
      .offset_high = HIGH_16_BITS(&isr##num), \
      .offset_low = LOW_16_BITS(&isr##num),   \
      .segment_selector = GDT_CODE_0_SEL,     \
      .reserved = 0,                          \
      .attributes = 0x8E}

#define IRQ_ENTRY(num1, num2)                  \
  idt[num2] = (idt_entry_t){                   \
      .offset_high = HIGH_16_BITS(&irq##num1), \
      .offset_low = LOW_16_BITS(&irq##num1),   \
      .segment_selector = GDT_CODE_0_SEL,      \
      .reserved = 0,                           \
      .attributes = 0x8E}

#define IDT_ENTRY3(num)                       \
  idt[num] = (idt_entry_t){                   \
      .offset_high = HIGH_16_BITS(&isr##num), \
      .offset_low = LOW_16_BITS(&isr##num),   \
      .segment_selector = GDT_CODE_0_SEL,     \
      .reserved = 0,                          \
      .attributes = 0xEE}

void idt_init(void)
{
  outport_b(0x20, 0x11);
  outport_b(0xA0, 0x11);

  outport_b(0x21, 0x20);
  outport_b(0xA1, 0x28);

  outport_b(0x21, 0x04);
  outport_b(0xA1, 0x02);

  outport_b(0x21, 0x01);
  outport_b(0xA1, 0x01);

  outport_b(0x21, 0x0);
  outport_b(0xA1, 0x0);

  // Exceptions (0-31)
  IDT_ENTRY0(0);
  IDT_ENTRY0(1);
  IDT_ENTRY0(2);
  IDT_ENTRY0(3);
  IDT_ENTRY0(4);
  IDT_ENTRY0(5);
  IDT_ENTRY0(6);
  IDT_ENTRY0(7);
  IDT_ENTRY0(8);
  IDT_ENTRY0(9);
  IDT_ENTRY0(10);
  IDT_ENTRY0(11);
  IDT_ENTRY0(12);
  IDT_ENTRY0(13);
  IDT_ENTRY0(14);
  IDT_ENTRY0(15);
  IDT_ENTRY0(16);
  IDT_ENTRY0(17);
  IDT_ENTRY0(18);
  IDT_ENTRY0(19);
  IDT_ENTRY0(20);

  // IRQ: 32-47 (keyboard and clock interrupts: 32 and 33)
  IRQ_ENTRY(0, 32);
  IRQ_ENTRY(1, 33);
  IRQ_ENTRY(2, 34);
  IRQ_ENTRY(3, 35);
  IRQ_ENTRY(4, 36);
  IRQ_ENTRY(5, 37);
  IRQ_ENTRY(6, 38);
  IRQ_ENTRY(7, 39);
  IRQ_ENTRY(8, 40);
  IRQ_ENTRY(9, 41);
  IRQ_ENTRY(10, 42);
  IRQ_ENTRY(11, 43);
  IRQ_ENTRY(12, 44);
  IRQ_ENTRY(13, 45);
  IRQ_ENTRY(14, 46);
  IRQ_ENTRY(15, 47);

  // Syscalls 47-256
  IDT_ENTRY3(88);
  IDT_ENTRY3(98);

  idt_flush((uint32_t)&IDT_DESC);
}

const char *exception_messages[] = {"Divide Error #DE [0]",
                                    "Debug #DB [1]",
                                    "NMI Interrupt [2]",
                                    "Breakpoint #BP [3]",
                                    "Overflow #OF [4]",
                                    "BOUND Range Exceeded #BR [5]",
                                    "Invalid Opcode #UD [6]",
                                    "Device Not Available #NM [7]",
                                    "Double Fault (#DF) [8]",
                                    "Coprocessor Segment Overrun [9]",
                                    "Invalid TSS (#TS) [10]",
                                    "Segment Not Present (#NP) [11]",
                                    "Stack Fault (#SS) [12]",
                                    "General Protection (#GP) [13]",
                                    "Page-Fault (#PF) [14]",
                                    "RESERVED [15]",
                                    "Floating-Point Error (#MF) [16]",
                                    "Alignment Check (#AC) [17]",
                                    "Machine-Check (#MC) [18]",
                                    "SIMD Floating-Point (#XM) [19]",
                                    "Virtualization (#VE) [20]",
                                    "Control Protection (#CP) [21]"};

void isr_handler(struct InterruptRegisters *regs)
{
  if (regs->int_num < 32)
  {
    terminal_writestring(exception_messages[regs->int_num]);
    terminal_writestring("\n");
    terminal_writestring("exception, system halted \n");
    for (;;)
      ;
  }
}

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs))
{
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
  irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters *regs)
{
  void (*handler)(struct InterruptRegisters *regs);
  handler = irq_routines[regs->int_num - 32];

  if (handler)
  {
    handler(regs);
  }

  if (regs->int_num >= 40)
  {
    outport_b(0xA0, 0x20);
  }

  outport_b(0x20, 0x20);
}
