#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/pic.h>
#include <stdint.h>

struct InterruptRegisters;

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
  uint8_t irq = regs->int_num - 32;
  if (irq < 16)
  {
    void (*handler)(struct InterruptRegisters *);
    handler = irq_routines[irq];

    if (handler)
      handler(regs);

    pic_send_eoi(irq);
  }
}
