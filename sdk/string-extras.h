#ifndef _STRING_EXTRAS_H_
#define _STRING_EXTRAS_H_

#include <ctype.h>
#include "c_types.h"
#include "string.h"
#include "mem.h"
#include "osapi.h"

char* MOVE_TO_FLASH copyString(const char *string)
{
  int length = os_strlen(string) + 1;
  char *copy = (char *)os_zalloc(length);

  if (copy)
  {
    os_memcpy(copy, string, length);
  }

  return copy;
}

char* MOVE_TO_FLASH bufferToHex(unsigned char *buffer, char *output)
{
  int length = os_strlen((const char *)buffer);
  unsigned char *input = buffer;
  const char *hex = "0123456789ABCDEF";
  int i = 0;

  for (; i < length; i++)
  {
    *output++ = hex[(*input >> 4) & 0xF];
    *output++ = hex[(*input++) & 0xF];
  }

  *output = 0;
  return output;
}

int MOVE_TO_FLASH strcasecmp(const char *string1, const char *string2)
{
  int offset, ch;
  unsigned char a, b;

  offset = 0;
  ch = 0;

  while (*(string1 + offset) != '\0')
  {
    if (*(string2 + offset) == '\0')
      return (*(string1 + offset));

    a = (unsigned)*(string1 + offset);
    b = (unsigned)*(string2 + offset);
    ch = toupper(a) - toupper(b);

    if (ch != 0)
      return (ch);

    offset++;
  }

  return (ch);
}

#endif
