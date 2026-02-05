#include "io.h"

void outport_b(uint16_t port, uint8_t val)
{
  asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

uint8_t inb(uint16_t port)
{
  uint8_t ret;
  asm volatile("inb %w1, %b0"
               : "=a"(ret)
               : "Nd"(port)
               : "memory");
  return ret;
}
