#ifndef _ESP_PINS_
#define _ESP_PINS_

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_0   0
#define PIN_TX  1
#define PIN_2   2
#define PIN_RX  3

#define LOW     0
#define HIGH    1

#define INPUT   0
#define OUTPUT  1

#include "c_types.h"

int pinRead(uint8_t pin);
void pinWrite(uint8_t pin, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
