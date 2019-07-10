#ifndef _ESP_TIME_H_
#define _ESP_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "user_interface.h"
#include "c_types.h"

#define MAX_SAFE_DELAY 1000

uint32_t ICACHE_RAM_ATTR now() {
  return system_get_time();
}

uint32_t ICACHE_RAM_ATTR micros() {
  return system_get_time();
}

uint32_t ICACHE_RAM_ATTR millis() {
  return system_get_time() / 1000UL;
}

void delay(uint32_t time) {
  unsigned quotient = time / MAX_SAFE_DELAY;
  unsigned remainder = time % MAX_SAFE_DELAY;

  for (unsigned i = 0; i <= quotient; i++) {
    if (i == quotient) {
      os_delay_us(remainder * 1000);
    } else {
      os_delay_us(MAX_SAFE_DELAY * 1000);
    }

    system_soft_wdt_feed();
  }
}

void delayMicroseconds(uint32_t time) {
  os_delay_us(time);
}

#ifdef __cplusplus
}
#endif
