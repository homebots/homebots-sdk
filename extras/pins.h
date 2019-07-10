#ifndef _ESP_PINS_
#define _ESP_PINS_

#ifdef __cplusplus
extern "C" {
#endif

#include "eagle_soc.h"
// #include "./esp8266_peri.h"
#include "stdint.h"
#include "gpio.h"
#include "osapi.h"

#define PIN_0   0
#define PIN_TX  1
#define PIN_2   2
#define PIN_RX  3

#define LOW     0
#define HIGH    1

#define INPUT             0x00
#define OUTPUT            0x01

#define ESP8266_REG(addr)       *((volatile uint32_t *)(0x60000000+(addr)))

//GPIO (0-15) Control Registers
#define GPIO_O    ESP8266_REG(0x300) //GPIO_OUT R/W (Output Level)
#define GPIO_OUTPUT_SET         ESP8266_REG(0x304) //GPIO_OUT_SET WO
#define GPIO_OUTPUT_CLEAR       ESP8266_REG(0x308) //GPIO_OUT_CLR WO
#define GPIO_E    ESP8266_REG(0x30C) //GPIO_ENABLE R/W (Enable)
#define GPIO_ES   ESP8266_REG(0x310) //GPIO_ENABLE_SET WO
#define GPIO_ENABLE_CLEAR       ESP8266_REG(0x314) //GPIO_ENABLE_CLR WO
#define GPIO_INPUT              ESP8266_REG(0x318) //GPIO_IN RO (Read Input Level)
#define GPIO_IE   ESP8266_REG(0x31C) //GPIO_STATUS R/W (Interrupt Enable)
#define GPIO_IES  ESP8266_REG(0x320) //GPIO_STATUS_SET WO
#define GPIO_IEC  ESP8266_REG(0x324) //GPIO_STATUS_CLR WO

uint8_t esp8266_gpioToFn[16] = {0x34, 0x18, 0x38, 0x14, 0x3C, 0x40, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x04, 0x08, 0x0C, 0x10};

typedef struct {
  uint8_t   id;
  uint32_t  multiplexer;
  uint8_t   gpioFunction;
} DigitalPin;

static const DigitalPin Pins[4] = {
  { PIN_0,   PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0 },
  { PIN_TX,  PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1 },
  { PIN_2,   PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2 },
  { PIN_RX,  PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3 }
};

int pinRead(uint8_t pin) {
  bool value = (GPIO_INPUT & (1UL << (pin & 0xF))) != 0;
  // GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1UL << pin)) > 0 ? HIGH : LOW;
  // uint8_t value = ((uint8_t) GPIO_REG_READ(GPIO_OUT_ADDRESS) & (1UL << pin)) > 0 ? HIGH : LOW;
  // os_printf("-- READ GPIO %d: %ld \r\n", pin, value);
  return value ? HIGH : LOW;
}

void pinWrite(uint8_t pin, uint8_t value) {
  uint32_t bits = 1UL << pin;

  switch (value) {
    case LOW:
      GPIO_OUTPUT_CLEAR = bits;
      // gpio_output_set(0, bits, bits, 0);
      break;

    case HIGH:
      GPIO_OUTPUT_SET = bits;
      // gpio_output_set(bits, 0, bits, 0);
      break;
  }

  os_printf("-- WRITE GPIO %d: %d \r\n", pin, value);
}

void pinPullUp(uint8_t pin) {
  PIN_PULLUP_EN((Pins[pin].multiplexer));
}

void pinPullDown(uint8_t pin) {
  PIN_PULLUP_DIS((Pins[pin].multiplexer));
}

void pinMode(uint8_t pin, uint8_t mode) {
  PIN_FUNC_SELECT(Pins[pin].multiplexer, Pins[pin].gpioFunction);
  uint32_t bits = 1UL << pin;

  switch (mode) {
    case OUTPUT:
      gpio_output_set(0, bits, bits, 0);
      break;

    case INPUT:
      // gpio_output_set(0, 0, 0, bits);
      GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
      GPIO_ENABLE_CLEAR = (1 << pin); //Disable
      GPC(pin) = (GPC(pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
      // if(mode == INPUT_PULLUP) {
      //     GPF(pin) |= (1 << GPFPU);  // Enable  Pullup
      // }
      break;
  }
}

void interruptDisable() {
  ets_isr_mask(1UL << 4);
}

void interruptEnable() {
  ets_isr_unmask(1UL << 4);
}

typedef void (*function)(void);

static function interruptHandler = NULL;
static uint32_t interruptRegister = 0;

void interruptAttach(uint8_t pin, function handler, GPIO_INT_TYPE mode) {
  interruptHandler = handler;
  gpio_pin_intr_state_set(pin, mode);
  ets_isr_attach(ETS_GPIO_INUM, handler, &interruptRegister);
  gpio_register_set(GPIO_PIN_ADDR(pin),
                       GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)  |
                       GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) |
                       GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin));
}

void interruptHandle() {
  if (interruptHandler == NULL || interruptRegister == 0) return;

  interruptDisable();
  uint32_t savedPS = xt_rsil(15);
  interruptHandler();
  xt_wsr_ps(savedPS);
}

char* printRegister(uint32_t value) {
  char index, k;
  char output[33];

  for (index = 31; index >= 0; index--) {
    k = value >> index;
    output[31 - index] = (k & 1) ? '1' : '0';
  }

  output[32] = '\0';

  return &output;
}

#ifdef __cplusplus
}
#endif

#endif