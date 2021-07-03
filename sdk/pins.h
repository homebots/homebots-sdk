#ifndef _ESP_PINS_
#define _ESP_PINS_

#include "c_types.h"
#include "missing-includes.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "gpio.h"
#include "missing-includes.h"
#include "osapi.h"

#define PIN_0 0
#define PIN_TX 1
#define PIN_2 2
#define PIN_RX 3

#define LOW 0
#define HIGH 1

#ifndef NUMBER_OF_PINS
#define NUMBER_OF_PINS 4
#endif

typedef enum
{
  PinInput = 0,
  PinOutput = 1,
  PinOpenDrain = 2,
  PinInputPullUp = 3,
} PinMode;

bool pinRead(uint8_t pin);
void pinWrite(uint8_t pin, bool value);
void pinMode(uint8_t pin, PinMode mode);
void pinType(uint8_t pin, uint8_t mode);
bool isHigh(uint8_t pin);
bool isLow(uint8_t pin);

uint32_t _pinName(uint8_t pin)
{
  switch (pin)
  {
  case 0:
    return PERIPHS_IO_MUX_GPIO0_U;

  case 1:
    return PERIPHS_IO_MUX_U0TXD_U;

  case 2:
    return PERIPHS_IO_MUX_GPIO2_U;

  case 3:
    return PERIPHS_IO_MUX_U0RXD_U;
  }
}

/**
 * @see
 * https://www.espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf
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

bool pinRead(uint8_t pin)
{
  bool value = GPIO_INPUT_GET(pin);

  if (value)
  {
    return HIGH;
  }

  return LOW;
}

void pinWrite(uint8_t pin, bool value)
{
  GPIO_OUTPUT_SET(pin, value & 0x01);
}

void pinType(uint8_t pin, uint8_t type)
{
  PIN_FUNC_SELECT(_pinName(pin), type);
}

void pinMode(uint8_t pin, PinMode mode)
{
  uint32_t pinRegister = GPIO_PIN_ADDR(GPIO_ID_PIN(pin));

  switch (mode)
  {
  case PinInput:
  case PinInputPullUp:
    gpio_output_set(0, 0, 0, BIT(pin));
    break;

  case PinOutput:
    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS,
                   GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | BIT(pin));
    break;
  }

  if (mode == PinOpenDrain)
  {
    GPIO_REG_WRITE(pinRegister,
                   GPIO_REG_READ(pinRegister) |
                       GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));
  }

  if (mode == PinInputPullUp)
  {
    PIN_PULLUP_EN(_pinName(pin));
  }
  else
  {
    PIN_PULLUP_DIS(_pinName(pin));
  }
}

bool isHigh(uint8_t pin) { return (gpio_input_get() >> pin) & BIT0; }

bool isLow(uint8_t pin) { return !isHigh(pin); }

#endif
