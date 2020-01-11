#ifndef _ESP_PINS_
#define _ESP_PINS_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "ets_sys.h"
#include "gpio.h"

#define PIN_0   0
#define PIN_TX  1
#define PIN_2   2
#define PIN_RX  3

#define LOW     0
#define HIGH    1

#ifndef NUMBER_OF_PINS
#define NUMBER_OF_PINS 4
#endif

typedef void InterruptCallback();
typedef enum {
  Disabled  = 0,
  Falling   = 1,
  Rising    = 2,
  Changing  = 3,
  Low       = 4,  // triggers for as long as pin is GND
  High      = 5,  // not working
} InterruptMode;

typedef enum {
  PinInput        = 0,
  PinOutput       = 1,
  PinOpenDrain    = 3,
  PinInputPullUp  = 4,
} PinMode;

bool pinRead(uint8_t pin);
void pinWrite(uint8_t pin, bool value);
void pinMode(uint8_t pin, PinMode mode);
void pinType(uint8_t pin, uint8_t mode);
bool isHigh(uint8_t pin);
bool isLow(uint8_t pin);

void attachInterrupt(uint8_t pin, InterruptCallback* callback, InterruptMode mode);
void detachInterrupt(uint8_t pin);
void armInterrupts();

#define GPIO_INTERRUPT_CONFIG GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) | \
                       GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) | \
                       GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE)

#ifdef __cplusplus
}
#endif

#endif
