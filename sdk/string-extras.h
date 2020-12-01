#ifndef _STRING_EXTRAS_H_
#define _STRING_EXTRAS_H_

#include <ctype.h>

#include "c_types.h"
#include "mem.h"
#include "osapi.h"

char *ICACHE_FLASH_ATTR copyString(const char *string);
char *ICACHE_FLASH_ATTR bufferToHex(unsigned char *buffer, char *output);

// LOCAL char *ICACHE_FLASH_ATTR strcpy(char *dst, char *src) {
//   while (*dst++ = *src++)
//     ;
//   return dst - 1;
// }

void zerofill(unsigned char *stream, int amount);
int strcasecmp(const char *s1, const char *s2);

class String
{
public:
  int length = 0;

  String();

  void append(String string);
  void append(const char *chars);
  void append(const uint8_t *chars);
  void append(uint8_t *chars);
  void append(uint8_t *chars, int charsLength);
  uint8_t *getBuffer();

private:
  int bufferLength = 1024;
  uint8_t *buffer;
};

////
char *ICACHE_FLASH_ATTR copyString(const char *string)
{
  int length = os_strlen(string) + 1;
  char *copy = (char *)os_zalloc(length);

  if (copy)
  {
    os_memcpy(copy, string, length);
  }

  return copy;
}

char *ICACHE_FLASH_ATTR bufferToHex(unsigned char *buffer, char *output)
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
}

int strcasecmp(const char *string1, const char *string2)
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

void zerofill(unsigned char *stream, int amount)
{
  unsigned char *pointer = stream;
  for (int i = 0; i < amount; i++)
  {
    *(pointer + i) = 0;
  }
}

String::String()
{
  buffer = (uint8_t *)os_zalloc(1024);
  buffer[0] = '\0';
}

uint8_t *String::getBuffer()
{
  return buffer - length;
}

void String::append(String string) { append((uint8_t *)string.getBuffer()); }
void String::append(const char *chars) { append((uint8_t *)chars); }
void String::append(const uint8_t *chars) { append((uint8_t *)chars); }
void String::append(uint8_t *chars) { append(chars, os_strlen((const char *)chars)); }

void String::append(uint8_t *chars, int charsLength)
{
  if (length + charsLength > bufferLength)
  {
    bufferLength += 1024;
    buffer = (uint8 *)os_realloc(buffer, bufferLength);
  }

  os_strncpy((char *)buffer, (char *)chars, charsLength);
  buffer += charsLength;
  length += charsLength;
  buffer[length] = '\0';
}

#endif
