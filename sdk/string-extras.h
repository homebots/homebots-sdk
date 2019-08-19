#ifndef _STRING_EXTRAS_H_
#define _STRING_EXTRAS_H_

#include "c_types.h"

char* ICACHE_FLASH_ATTR copyString(const char *string);

char* ICACHE_FLASH_ATTR bufferToHex(unsigned char* buffer, char* output);

void zerofill(unsigned char* stream, int amount);

#endif
