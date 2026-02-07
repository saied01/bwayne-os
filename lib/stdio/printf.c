#include "itoa.h"
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool print(const char *data, size_t length)
{
  const unsigned char *bytes = (const unsigned char *)data;

  for (size_t i = 0; i < length; i++)
  {
    if (putchar(bytes[i]) == EOF)
      return false;
  }

  return true;
}

int printf(const char *restrict format, ...)
{
  va_list parameters;
  va_start(parameters, format);

  int written = 0;

  while (*format != '\0')
  {
    size_t maxrem = INT_MAX - written;
    if (format[0] != '%')
    {
      size_t amount = 0;
      while (format[amount] && format[amount] != '%')
        amount++;

      if (amount > maxrem)
      {
        // TODO: set errno to OVERFLOW
        return -1;
      }

      if (!print(format, amount))
        return -1;

      format += amount;
      written += amount;
      continue;
    }
    format++;

    if (*format == '%')
    {
      if (!maxrem)
      {
        // todo: set errno to overflow
        return -1;
      }
      if (!print(format, 1))
        return -1;
      format++;
      written++;
      continue;
    }
    else if (*format == 'c')
    {
      char c = (char)va_arg(parameters, int);
      if (!maxrem)
      {
        // todo: set errno to overflow
        return -1;
      }
      if (!print(&c, sizeof(c)))
        return -1;
      format++;
      written++;
      continue;
    }
    else if (*format == 's')
    {
      const char *s = va_arg(parameters, const char *);
      size_t len = strlen(s);
      if (maxrem < len)
      {
        // todo: set errno to overflow
        return -1;
      }
      if (!print(s, len))
        return -1;
      format++;
      written += len;
      continue;
    }
    else if (*format == 'd')
    {
      int d = va_arg(parameters, int);
      char buf[12];
      size_t len = itoa_dec(d, buf);
      if (len > maxrem)
      {
        // todo: set errno to overflow
        return -1;
      }
      if (!print(buf, len))
        return -1;
      format++;
      written += len;
      continue;
    }
    else
    {
      return -1;
    }
  }

  va_end(parameters);
  return written;
}
