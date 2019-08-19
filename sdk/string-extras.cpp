#ifdef __cplusplus
extern "C" {
#endif

#include "string-extras.h"
#include "osapi.h"
#include "mem.h"
#include "c_types.h"
#include <ctype.h>

char* ICACHE_FLASH_ATTR copyString(const char *string) {
  int length = os_strlen(string) + 1;
  char *copy = (char*)os_zalloc(length);

  if (copy) {
    os_memcpy(copy, string, length);
  }

  return copy;
}

char* ICACHE_FLASH_ATTR bufferToHex(unsigned char* buffer, char* output) {
  int length = os_strlen((const char*)buffer);
  unsigned char* input = buffer;
  const char* hex = "0123456789ABCDEF";
  int i = 0;

  for(; i < length; i++){
    *output++ = hex[(*input>>4)&0xF];
    *output++ = hex[(*input++)&0xF];
  }

  *output = 0;
}

void zerofill(unsigned char* stream, int amount) {
  unsigned char* pointer = stream;
  for (int i = 0; i < amount; i++) {
    *(pointer + i) = 0;
  }
}

#ifdef __cplusplus
}
#endif
