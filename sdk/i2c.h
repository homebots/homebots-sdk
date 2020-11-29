/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in
 * which case, it is free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef __I2C_MASTER_H__
#define __I2C_MASTER_H__

#include "c_types.h"
#include "missing-includes.h"
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"
#include "serial-debug.h"

#define I2C_MASTER_SDA_MUX PERIPHS_IO_MUX_GPIO2_U
#define I2C_MASTER_SCL_MUX PERIPHS_IO_MUX_GPIO0_U
#define I2C_MASTER_SDA_GPIO 2
#define I2C_MASTER_SCL_GPIO 0
#define I2C_MASTER_SDA_FUNC FUNC_GPIO2
#define I2C_MASTER_SCL_FUNC FUNC_GPIO0

#define I2C_MASTER_SDA_HIGH_SCL_HIGH()                                    \
  gpio_output_set(1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, 0, \
                  1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, 0)

#define I2C_MASTER_SDA_HIGH_SCL_LOW()                                 \
  gpio_output_set(1 << I2C_MASTER_SDA_GPIO, 1 << I2C_MASTER_SCL_GPIO, \
                  1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, 0)

#define I2C_MASTER_SDA_LOW_SCL_HIGH()                                 \
  gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 1 << I2C_MASTER_SDA_GPIO, \
                  1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, 0)

#define I2C_MASTER_SDA_LOW_SCL_LOW()                                      \
  gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, \
                  1 << I2C_MASTER_SDA_GPIO | 1 << I2C_MASTER_SCL_GPIO, 0)

#define i2c_wait os_delay_us

void i2c_gpio_init(void);
void i2c_init(void);
void i2c_stop(void);
void i2c_start(void);
void i2c_setAck(uint8 level);
uint8_t i2c_getAck(void);
uint8_t i2c_readByte(void);
void i2c_findDevices(uint8_t* devices);
uint8_t i2c_findDevice();
void i2c_writeByte(uint8_t wrdata);
void i2c_writeByteAndAck(uint8_t wrdata);
bool i2c_checkAck(void);
void i2c_send_ack(void);
void i2c_send_nack(void);

////

LOCAL uint8 m_nLastSDA;
LOCAL uint8 m_nLastSCL;

LOCAL void ICACHE_FLASH_ATTR i2c_setDataAndClock(uint8 SDA, uint8 SCL) {
  SDA &= 0x01;
  SCL &= 0x01;
  m_nLastSDA = SDA;
  m_nLastSCL = SCL;

  if ((0 == SDA) && (0 == SCL)) {
    I2C_MASTER_SDA_LOW_SCL_LOW();
  } else if ((0 == SDA) && (1 == SCL)) {
    I2C_MASTER_SDA_LOW_SCL_HIGH();
  } else if ((1 == SDA) && (0 == SCL)) {
    I2C_MASTER_SDA_HIGH_SCL_LOW();
  } else {
    I2C_MASTER_SDA_HIGH_SCL_HIGH();
  }

  i2c_wait(5);
}

LOCAL uint8 ICACHE_FLASH_ATTR i2c_getDC(void) {
  return GPIO_INPUT_GET(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO));
}

void ICACHE_FLASH_ATTR i2c_init(void) {
  uint8 i;

  i2c_setDataAndClock(1, 0);

  // when SCL = 0, toggle SDA to clear up
  i2c_setDataAndClock(0, 0);
  i2c_setDataAndClock(1, 0);

  // set data_cnt to max value
  for (i = 0; i < 28; i++) {
    i2c_setDataAndClock(1, 0);
    i2c_setDataAndClock(1, 1);
  }

  i2c_stop();
  return;
}

void ICACHE_FLASH_ATTR i2c_gpio_init(void) {
  ETS_GPIO_INTR_DISABLE();

  PIN_FUNC_SELECT(I2C_MASTER_SDA_MUX, I2C_MASTER_SDA_FUNC);
  PIN_FUNC_SELECT(I2C_MASTER_SCL_MUX, I2C_MASTER_SCL_FUNC);

  GPIO_REG_WRITE(
      GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO)),
      GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO))) |
          GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));  // open drain;
  GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) |
                                          (1 << I2C_MASTER_SDA_GPIO));
  GPIO_REG_WRITE(
      GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO)),
      GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO))) |
          GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE));  // open drain;
  GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) |
                                          (1 << I2C_MASTER_SCL_GPIO));

  I2C_MASTER_SDA_HIGH_SCL_HIGH();

  ETS_GPIO_INTR_ENABLE();

  i2c_init();
}

void ICACHE_FLASH_ATTR i2c_start(void) {
  LOG("START\n");
  i2c_setDataAndClock(1, m_nLastSCL);
  i2c_setDataAndClock(1, 1);
  i2c_setDataAndClock(0, 1);
}

void ICACHE_FLASH_ATTR i2c_stop(void) {
  LOG("STOP\n");
  i2c_setDataAndClock(0, m_nLastSCL);
  i2c_setDataAndClock(0, 1);
  i2c_setDataAndClock(1, 1);
}

void ICACHE_FLASH_ATTR i2c_setAck(uint8 level) {
  i2c_setDataAndClock(m_nLastSDA, 0);
  i2c_setDataAndClock(level, 0);
  i2c_setDataAndClock(level, 1);
  i2c_wait(8);
  i2c_setDataAndClock(level, 0);
  i2c_setDataAndClock(1, 0);
}

uint8 ICACHE_FLASH_ATTR i2c_getAck(void) {
  uint8 retVal;
  i2c_setDataAndClock(m_nLastSDA, 0);
  i2c_setDataAndClock(1, 0);
  i2c_setDataAndClock(1, 1);
  retVal = i2c_getDC();
  i2c_setDataAndClock(1, 0);

  return retVal;
}

bool ICACHE_FLASH_ATTR i2c_checkAck(void) {
  if (i2c_getAck()) {
    return 0;
  } else {
    return 1;
  }
}

void ICACHE_FLASH_ATTR i2c_send_ack(void) { i2c_setAck(0x0); }

void ICACHE_FLASH_ATTR i2c_send_nack(void) { i2c_setAck(0x1); }

uint8 ICACHE_FLASH_ATTR i2c_readByte(void) {
  uint8 retVal = 0;
  uint8 k, i;

  i2c_setDataAndClock(m_nLastSDA, 0);

  for (i = 0; i < 8; i++) {
    i2c_setDataAndClock(1, 0);
    i2c_setDataAndClock(1, 1);

    k = i2c_getDC();
    if (i == 7) {
      i2c_wait(3);
    }

    k <<= (7 - i);
    retVal |= k;
  }

  i2c_setDataAndClock(1, 0);

  return retVal;
}

void ICACHE_FLASH_ATTR i2c_writeByteAndAck(uint8 byte) {
  bool ack;

  i2c_writeByte(byte);
  ack = i2c_checkAck();
  LOG("> ACK 0x%x\n", byte, ack);
}

void ICACHE_FLASH_ATTR i2c_writeByte(uint8 byte) {
  uint8 bit;
  sint8 i;

  LOG("WRITE 0x%x\n", byte);

  i2c_setDataAndClock(m_nLastSDA, 0);

  for (i = 7; i >= 0; i--) {
    bit = byte >> i;
    i2c_setDataAndClock(bit, 0);
    i2c_setDataAndClock(bit, 1);

    if (i == 0) {
      i2c_wait(3);
    }

    i2c_setDataAndClock(bit, 0);
  }
}

void ICACHE_FLASH_ATTR i2c_findDevices(uint8_t* devices) {
  uint8_t i;

  for (i = 0; i < 255; i++) {
    i2c_start();
    i2c_writeByte(i);

    if (i2c_checkAck()) {
      devices[i] = 1;
    } else {
      devices[0] = 1;
    }

    i2c_stop();
  }
}

uint8_t ICACHE_FLASH_ATTR i2c_findDevice() {
  uint8_t i;

  for (i = 0; i < 255; i++) {
    i2c_start();
    i2c_writeByte(i);

    if (i2c_checkAck()) {
      return i;
    }

    i2c_stop();
  }

  return 0;
}

#endif
