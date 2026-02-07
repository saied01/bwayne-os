#include <stddef.h>
#include <string.h>

void *memset(void *ptr, int c, size_t n)
{
  unsigned char *p = (unsigned char *)ptr;

  for (size_t i = 0; i < n; i++)
    p[i] = (unsigned char)c;

  return ptr;
}
