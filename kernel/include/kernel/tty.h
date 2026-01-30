#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char);
void terminal_write(const char *, size_t);
void terminal_writestring(const char *);

#endif // !TTY_H
