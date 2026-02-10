#include <io.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/pit.h>
#include <kernel/tty.h>
#include <stdint.h>

uint64_t ticks;
const uint32_t freq = 100;
struct InterruptRegisters *regs;

void pit_init(void)
{
  ticks = 0;
  irq_install_handler(0, &on_irq0);

  // PIT oscillator runs at (roughly) 1.1931816666 MHz
  uint32_t divisor = 1193180 / freq;

  outport_b(PIT_CMD_CHN, 0x36);
  outport_b(PIT_CHN0, (uint8_t)(divisor & 0xFF));
  outport_b(PIT_CHN0, (uint8_t)(divisor >> 8) & 0xFF);
}

void on_irq0(struct InterruptRegisters *regs)
{
  (void)regs;
  ticks += 1;
}
