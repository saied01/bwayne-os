#include <kernel/tty.h>

void kernel_main(void)
{
  terminal_initialize();
  terminal_writestring("Hi,\n my  \n I can put new lines. \n ASCII art: \n xd \n\n\n\n\n\n\n\n\n\n\n diome \n\n\n\n\n\n\n supaaaaaaaaaaaaaa \n\n\n supuesta linea \n \n");
}
