#include <kernel/gdt.h>
#include <kernel/tty.h>

void kernel_main(void)
{
  gdt_init();

  terminal_initialize();
  terminal_writestring("gdt cargada piola \n");

  for (;;)
    asm volatile("hlt");
}
