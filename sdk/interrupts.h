#ifndef _ESP_INTERRUPTS_
#define _ESP_INTERRUPTS_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "missing-includes.h"
#include "ets_sys.h"
#include "gpio.h"
#include "pins.h"

typedef void InterruptCallback();
typedef enum {
  Disabled = 0,
  Falling  = 1,
  Rising   = 2,
  Changing = 3,
  Low      = 4,  // triggers for as long as pin is GND
  High     = 5,  // not working
} InterruptMode;

#define GPIO_INTERRUPT_CONFIG                            \
  GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) |         \
      GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) | \
      GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE)

volatile static InterruptCallback* interruptCallbacks[NUMBER_OF_PINS];

void onInterrupt() {
  uint8_t pin = NUMBER_OF_PINS;
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  bool satisfied = false;

  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

  while (pin--) {
    if ((gpio_status & BIT(pin)) && interruptCallbacks[pin] != 0) {
      satisfied = true;
      interruptCallbacks[pin]();
    }
  }

  if (!satisfied) {
    armInterrupts();
  }
}

void detachInterrupt(uint8_t pin) {
  interruptCallbacks[pin] = 0;
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin));
  gpio_pin_intr_state_set(pin, GPIO_PIN_INTR_DISABLE);
}

void attachInterrupt(uint8_t pin, InterruptCallback* callback,
                     InterruptMode mode) {
  interruptCallbacks[pin] = callback;
  GPIO_DIS_OUTPUT(GPIO_ID_PIN(pin));
  ETS_GPIO_INTR_DISABLE();
  gpio_pin_intr_state_set(GPIO_ID_PIN(pin), (GPIO_INT_TYPE)mode);
  ETS_GPIO_INTR_ENABLE();
}

void armInterrupts() { ETS_GPIO_INTR_ATTACH((ets_isr_t)onInterrupt, NULL); }

class InterruptLock {
 public:
  InterruptLock() { ets_intr_lock(); }

  ~InterruptLock() { ets_intr_unlock(); }
};

#ifdef __cplusplus
}
#endif

#endif
