#include <kernel/tty.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

// Only works for 32 bit ix-86 targets
#if !defined(__i386__)
#error "You need to compile with a ix86-elf compiler"
#endif

size_t strlen(const char *str)
{
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

void kernel_main(void)
{
  terminal_initialize();
  terminal_writestring("Hi,\n my  \n I can put new lines. \n ASCII art: \n xd \n\n\n\n\n\n\n\n\n\n\n diome \n\n\n\n\n\n\n supaaaaaaaaaaaaaa \n\n\n supuesta linea \n \n");
}
