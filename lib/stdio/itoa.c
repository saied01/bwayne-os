#include <stddef.h>
#include <stdint.h>

size_t itoa_dec(int value, char *buffer)
{
  unsigned int u;
  size_t i = 0;

  if (value == 0)
  {
    buffer[0] = '0';
    return 1;
  }

  int negative = value < 0;

  if (negative)
    u = (unsigned int)(-(value + 1)) + 1; // evita overflow en INT_MIN
  else
    u = (unsigned int)value;

  while (u > 0)
  {
    buffer[i++] = '0' + (u % 10);
    u /= 10;
  }

  if (negative)
    buffer[i++] = '-';

  // reverse string
  for (size_t j = 0; j < i / 2; j++)
  {
    char tmp = buffer[j];
    buffer[j] = buffer[i - 1 - j];
    buffer[i - 1 - j] = tmp;
  }

  return i;
}
