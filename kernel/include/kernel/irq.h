#ifndef IRQ_H
#define IRQ_H

#include <kernel/idt.h>
#include <kernel/pic.h>
#include <stdint.h>

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *));
void irq_uninstall_handler(int);
void irq_handler(struct InterruptRegisters *);

#endif // !IRQ_H
