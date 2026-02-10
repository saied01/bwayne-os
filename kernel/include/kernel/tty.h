#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char);
void terminal_write(const char *, size_t);
void terminal_writestring(const char *);
void terminal_handle_scancode(uint8_t, uint8_t);
void update_shift(uint8_t);

#endif // !KERNEL_TTY_H
