#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/tty.h>

void kernel_main(void)
{
  gdt_init();
  idt_init();
  pic_remap(0x20, 0x28);
  irq_clear_mask(1);
  // irq_clear_mask(0);

  terminal_initialize();
  terminal_writestring("gdt cargada piola \n");
  volatile int a = 1;
  volatile int b = 0;
  volatile int c = a / b;

  // for (;;)
  //   asm volatile("hlt");
}
