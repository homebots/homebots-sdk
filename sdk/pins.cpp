#ifdef __cplusplus
extern "C" {
#endif

#include "pins.h"
#include "eagle_soc.h"
#include "gpio.h"
#include "osapi.h"
#include "c_types.h"
#include "ets_sys.h"
#include "missing-includes.h"

/**
 * @see https://www.espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf
 *
 * GPIO_ENABLE_W1TS = BIT(x)    enable output in pin(x)
 * GPIO_ENABLE_W1TC = BIT(x)    disable output in pin(x)
 * y = bit(x) & GPIO_ENABLE     is pin(x) in output mode
 *
 * GPIO_OUT_W1TS    = BIT(x)    set output high
 * GPIO_OUT_W1TC    = BIT(x)    set output low
 * y = bit(x) & GPIO_OUT        read pin(x) status
 *
 * GPIO_STATUS = bit(x)         enable pin(x) interrupt
 * GPIO_STATUS_W1TC = bit(x)    disable interrupt in pin(x)
 *
 * write 1 to pin(x):
 *    GPIO_OUTPUT_SET(GPIO_ID_PIN(x), 1);
 *
 * enable IO pull up
 *    PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);
 *
 * disable pull up
 *    PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTDI_U);
 *
 * disable input
 *    GPIO_DIS_OUTPUT(GPIO_ID_PIN(x));
 *
 * read pin status
 *    uint8 level = GPIO_INPUT_GET(GPIO_ID_PIN(x))
 *
 * attach interrupt to pin(x)
 *    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO2);
 *    GPIO_DIS_OUTPUT(GPIO_ID_PIN(2));
 *    ETS_GPIO_INTR_DISABLE();
 *    ETS_GPIO_INTR_ATTACH(GPIO_INTERRUPT, NULL);
 *    gpio_pin_intr_state_set(GPIO_ID_PIN(2), GPIO_PIN_INTR_NEGEDGE);
 *    ETS_GPIO_INTR_ENABLE();
 *
 * read and clear interrupt status
 *    static uint8_t pin = 2;
 *    void handleInterrupt() {
 *      uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
 *      uint8_t pinLevel = 0;
 *      GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
 *
 *      if (gpio_status && BIT(pin)) {
 *        level = GPIO_INPUT_GET(pin);
 *      }
 *    }
 */

bool pinRead(uint8_t pin) {
  bool value = GPIO_INPUT_GET(pin);

  if (value) {
    return HIGH;
  }

  return LOW;
}

void pinWrite(uint8_t pin, bool value) {
  GPIO_OUTPUT_SET(pin, value & 0x01);
}

void pinType(uint8_t pin, uint8_t mode) {
  PIN_FUNC_SELECT(PERIPHS_IO_MUX + (pin * 4), mode);
}

uint32_t _pinName(uint8_t pin) {
  switch (pin) {
    case 0:
      return PERIPHS_IO_MUX_GPIO0_U;

    case 1:
      return PERIPHS_IO_MUX_U0RXD_U;

    case 2:
      return PERIPHS_IO_MUX_GPIO2_U;

    case 3:
      return PERIPHS_IO_MUX_U0TXD_U;
  }
}

void pinMode(uint8_t pin, PinMode mode) {
  uint32_t pinRegister = GPIO_PIN_ADDR(GPIO_ID_PIN(pin));

  switch (mode) {
    case PinInput:
    case PinInputPullUp:
      gpio_output_set(0, 0, 0, BIT(pin));
      break;

    case PinOutput:
      GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | BIT(pin));
      break;
  }

  if (mode == PinOpenDrain) {
    GPIO_REG_WRITE(pinRegister, GPIO_REG_READ(pinRegister) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
  }

  if (mode == PinInputPullUp) {
    PIN_PULLUP_EN(_pinName(pin));
  } else {
    PIN_PULLUP_DIS(_pinName(pin));
  }
}

bool isHigh(uint8_t pin) {
  return (gpio_input_get() >> pin) & BIT0;
}

bool isLow(uint8_t pin) {
  return !isHigh(pin);
}

volatile static InterruptCallback* interruptCallbacks[NUMBER_OF_PINS];

void onInterrupt() {
  uint8_t pin = NUMBER_OF_PINS;
  uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  bool satisfied = false;

  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

  while(pin--) {
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

void attachInterrupt(uint8_t pin, InterruptCallback* callback, InterruptMode mode) {
  interruptCallbacks[pin] = callback;
  GPIO_DIS_OUTPUT(GPIO_ID_PIN(pin));
  ETS_GPIO_INTR_DISABLE();
  gpio_pin_intr_state_set(GPIO_ID_PIN(pin), (GPIO_INT_TYPE)mode);
  ETS_GPIO_INTR_ENABLE();
}

void armInterrupts() {
  ETS_GPIO_INTR_ATTACH((ets_isr_t)onInterrupt, NULL);
}

class InterruptLock {
  public:
    InterruptLock() {
      ets_intr_lock();
    }

    ~InterruptLock() {
      ets_intr_unlock();
    }
};


#ifdef __cplusplus
}
#endif
