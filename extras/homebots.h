#ifndef _ESP_OPEN_SDK_
#define _ESP_OPEN_SDK_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef SERIAL_SPEED
#define SERIAL_SPEED 115200
#endif

#include "sdk.h"

extern void setup();

// static void ICACHE_FLASH_ATTR
// user_loop(os_event_t *events) {
//   loop();
//   tick();
// }

void ICACHE_FLASH_ATTR
user_init() {
  uart_div_modify(0, UART_CLK_FREQ / SERIAL_SPEED);
  gpio_init();
  setup();
  tick();
}

#ifdef __cplusplus
}
#endif

#endif
