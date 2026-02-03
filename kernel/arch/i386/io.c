#include "io.h"

void outport_b(uint16_t port, uint8_t val)
{
  asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}
