#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/tty.h>

void kernel_main(void)
{
  terminal_initialize();
  terminal_writestring("gdt cargada piola \n");
  gdt_init();
  idt_init();
  pic_remap(0x20, 0x28);
  irq_clear_mask(0);
  pit_init();

  // irq_clear_mask(1);

  for (;;)
  {
    asm volatile("hlt");
  }
}
