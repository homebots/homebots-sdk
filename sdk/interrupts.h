#ifndef _ESP_INTERRUPTS_
#define _ESP_INTERRUPTS_

#include "c_types.h"
#include "ets_sys.h"
#include "gpio.h"
#include "pins.h"

typedef void (*interruptCallbackHandler)(void *arg, uint8_t pin);

void MOVE_TO_FLASH attachPinInterrupt(uint8_t pin, interruptCallbackHandler callback, void *arg, GPIO_INT_TYPE mode);
void MOVE_TO_FLASH detachPinInterrupt(uint8_t pin);
void MOVE_TO_FLASH armInterrupts();

#define GPIO_INTERRUPT_CONFIG                            \
  GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) |         \
      GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) | \
      GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE)

static interruptCallbackHandler interruptCallbacks[NUMBER_OF_PINS];
static void *interruptArgs[NUMBER_OF_PINS];

void _onInterruptTrigger();
void armInterrupts()
{
  ETS_GPIO_INTR_ATTACH((ets_isr_t)_onInterruptTrigger, NULL);
  ETS_GPIO_INTR_ENABLE();
}

void disarmInterrupts()
{
  ETS_GPIO_INTR_ATTACH((ets_isr_t)NULL, NULL);
}

void _onInterruptTrigger()
{
  uint8_t pin = 0;
  uint32_t gpioStatus = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
  interruptCallbackHandler handler;

  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpioStatus);

  for (pin = 0; pin < NUMBER_OF_PINS; pin++)
  {
    if ((gpioStatus & BIT(pin)) && interruptCallbacks[pin] != 0)
    {
      handler = interruptCallbacks[pin];
      handler(interruptArgs[pin], pin);
    }
  }
}

void MOVE_TO_FLASH detachPinInterrupt(uint8_t pin)
{
  interruptCallbacks[pin] = 0;
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin));
  gpio_pin_intr_state_set(pin, GPIO_PIN_INTR_DISABLE);
}

void MOVE_TO_FLASH attachPinInterrupt(uint8_t pin, interruptCallbackHandler callback, void *arg, GPIO_INT_TYPE mode)
{
  interruptCallbacks[pin] = callback;
  interruptArgs[pin] = arg;

  GPIO_DIS_OUTPUT(GPIO_ID_PIN(pin));
  ETS_GPIO_INTR_DISABLE();
  gpio_pin_intr_state_set(GPIO_ID_PIN(pin), (GPIO_INT_TYPE)mode);
  ETS_GPIO_INTR_ENABLE();
}

class InterruptLock
{
public:
  InterruptLock() { ets_intr_lock(); }

  ~InterruptLock() { ets_intr_unlock(); }
};

#endif
