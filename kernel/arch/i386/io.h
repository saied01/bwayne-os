#ifndef I386_IO_H
#define I386_IO_H

#include <stdint.h>

void outport_b(uint16_t, uint8_t);
uint8_t inb(uint16_t);

#endif // !I386_IO_H
