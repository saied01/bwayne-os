#include <io.h>
#include <kernel/pic.h>
#include <stdint.h>

void pic_init(void)
{
  pic_remap(0x20, 0x28);
  irq_clear_mask(0);
  irq_clear_mask(1);
}

void pic_remap(uint8_t offset1, uint8_t offset2)
{
  outport_b(PIC1, 0x11);
  outport_b(PIC2, 0x11);

  outport_b(PIC1_DATA, offset1);
  outport_b(PIC2_DATA, offset2);

  outport_b(PIC1_DATA, 0x04);
  outport_b(PIC2_DATA, 0x02);

  outport_b(PIC1_DATA, 0x01);
  outport_b(PIC2_DATA, 0x01);

  outport_b(PIC1_DATA, 0x0);
  outport_b(PIC2_DATA, 0x0);
}

void PIC_disable(void)
{
  outport_b(PIC1_DATA, 0xff);
  outport_b(PIC2_DATA, 0xff);
}

void pic_send_eoi(uint8_t irq)
{
  if (irq >= 8)
    outport_b(PIC2_COMMAND, PIC_EOI);

  outport_b(PIC1_COMMAND, PIC_EOI);
}

void irq_set_mask(uint8_t IRQline)
{
  uint16_t port;
  uint8_t value;

  if (IRQline < 8)
  {
    port = PIC1_DATA;
  }
  else
  {
    port = PIC2_DATA;
    IRQline -= 8;
  }
  value = inb(port) | (1 << IRQline);
  outport_b(port, value);
}

void irq_clear_mask(uint8_t IRQline)
{
  uint16_t port;
  uint8_t value;

  if (IRQline < 8)
  {
    port = PIC1_DATA;
  }
  else
  {
    port = PIC2_DATA;
    IRQline -= 8;
  }
  value = inb(port) & ~(1 << IRQline);
  outport_b(port, value);
}
