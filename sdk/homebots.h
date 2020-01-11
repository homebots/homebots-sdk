#ifndef _HOMEBOTS_
#define _HOMEBOTS_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 115200
#endif

#define USE_US_TIMER

#include "sdk.h"
#include "missing-includes.h"

extern void setup();

void ICACHE_FLASH_ATTR
user_init() {
  uart_div_modify(0, UART_CLK_FREQ / SERIAL_SPEED);
  gpio_init();

  // 160MHz
  // *((volatile uint32_t *)(0x3FF00014)) = 1;

  setup();
}

#ifdef __cplusplus
}
#endif

#endif
