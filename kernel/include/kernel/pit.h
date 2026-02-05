#ifndef KERNEL_PIT_H
#define KERNEL_PIT_H
#include <kernel/idt.h>

#define PIT_CHN0 0x40
#define PIT_CHN1 0x41
#define PIT_CHN2 0x42
#define PIT_CMD_CHN 0x43

void pit_init(void);
void on_irq0(struct InterruptRegisters *);

#endif // !KERNEL_PIT_H
