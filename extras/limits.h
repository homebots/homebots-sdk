// Copied from Arduino esp8266 project:
// https://raw.githubusercontent.com/esp8266/Arduino/master/tools/sdk/libc/xtensa-lx106-elf/include/limits.h

#ifndef _LIMITS_H
#define _LIMITS_H

#undef LONG_MAX
#define LONG_MAX    2147483647L

#undef LONG_MIN
#define LONG_MIN    (-LONG_MAX-1)

#undef ULONG_MAX
#define ULONG_MAX   (LONG_MAX * 2UL + 1)

#endif
