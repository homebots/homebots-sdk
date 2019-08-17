#include "limits.h"
#include "string-extras.h"
#include "osapi.h"
#include "mem.h"
#include "c_types.h"

unsigned long ICACHE_FLASH_ATTR strtoul(const char *nptr, char **endptr, int base) {
  const unsigned char *s = (const unsigned char *)nptr;
  unsigned long acc;
  int c;
  unsigned long cutoff;
  int neg = 0, any, cutlim;

  do {
    c = *s++;
  } while (isspace(c));

  if (c == '-') {
    neg = 1;
    c = *s++;
  } else if (c == '+'){
    c = *s++;
  }

  if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
    c = s[1];
    s += 2;
    base = 16;
  }

  if (base == 0) {
    base = c == '0' ? 8 : 10;
  }

  cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
  cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;

  for (acc = 0, any = 0;; c = *s++) {
    if (isdigit(c)){
      c -= '0';
    } else if (isalpha(c)){
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    } else
      break;

    if (c >= base) break;

    if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)){
      any = -1;
    } else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }

  if (any < 0) {
    acc = ULONG_MAX;
  } else if (neg) {
    acc = -acc;
  }

  if (endptr != 0) {
    *endptr = (char *)(any ? (char *)s - 1 : nptr);
  }
  return (acc);
}

char* ICACHE_FLASH_ATTR copyString(const char *string) {
  int length = os_strlen(string) + 1;
  char *copy = (char*)os_malloc(length);

  if (copy) {
    os_memcpy(copy, string, length);
  }

  return copy;
}

char* ICACHE_FLASH_ATTR bufferToHex(unsigned char* buffer, char* output) {
  int length = strlen(buffer);

  unsigned char* input = buffer;
  const char* hex = "0123456789ABCDEF";
  int i = 0;

  for(; i < length; i++){
    *output++ = hex[(*input>>4)&0xF];
    *output++ = hex[(*input++)&0xF];
  }

  *output = 0;
}

