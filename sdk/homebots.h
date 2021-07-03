#ifndef _HOMEBOTS_
#define _HOMEBOTS_

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 115200
#endif

#define USE_US_TIMER

extern void setup();

#ifdef __cplusplus
extern "C"
{
#endif

#include "sdk/sdk.h"

  void ICACHE_FLASH_ATTR user_init()
  {
    uart_div_modify(0, UART_CLK_FREQ / SERIAL_SPEED);
    gpio_init();

    // 160MHz
    system_update_cpu_freq(SYS_CPU_160MHZ);

    setup();
  }

#ifdef __cplusplus
}
#endif

#endif
