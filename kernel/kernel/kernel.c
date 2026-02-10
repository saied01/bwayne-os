#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/tty.h>
#include <keyboard/keyboard.h>

void kernel_main(void)
{
  terminal_initialize();

  gdt_init();
  idt_init();

  pic_init();
  pit_init();
  init_keyboard();

  for (;;)
  {
    asm volatile("hlt");
  }
}
