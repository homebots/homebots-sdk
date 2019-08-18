#ifdef __cplusplus
extern "C" {
#endif

#include "pins.h"
#include "eagle_soc.h"
#include "gpio.h"
#include "osapi.h"
#include "c_types.h"

int pinRead(uint8_t pin) {
  bool value = GPIO_INPUT_GET(pin);

  if (value) {
    return HIGH;
  }

  return LOW;
}

void pinWrite(uint8_t pin, uint8_t value) {
  GPIO_OUTPUT_SET(pin, value);
}

#ifdef __cplusplus
}
#endif
