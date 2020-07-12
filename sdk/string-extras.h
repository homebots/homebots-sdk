#ifndef _STRING_EXTRAS_H_
#define _STRING_EXTRAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>

#include "c_types.h"
#include "mem.h"
#include "osapi.h"

char* ICACHE_FLASH_ATTR copyString(const char* string);
char* ICACHE_FLASH_ATTR bufferToHex(unsigned char* buffer, char* output);

// LOCAL char *ICACHE_FLASH_ATTR strcpy(char *dst, char *src) {
//   while (*dst++ = *src++)
//     ;
//   return dst - 1;
// }

void zerofill(unsigned char* stream, int amount);
int strcasecmp(const char* s1, const char* s2);

class String {
 public:
  int length = 0;

  String();

  void append(String string);
  void append(const char* chars);
  void append(const uint8_t* chars);
  void append(uint8_t* chars);
  uint8_t* getBuffer();

 private:
  int bufferLength = 1024;
  uint8_t* buffer;
};

#ifdef __cplusplus
}
#endif

#endif
