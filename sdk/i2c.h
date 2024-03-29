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

#include "pins.h"
#include "c_types.h"
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"
#include "serial-debug.h"

static uint8_t clockPin = 0;
static uint8_t dataPin = 2;

#define dataHighClockHigh() gpio_output_set(1 << dataPin | 1 << clockPin, 0, 1 << dataPin | 1 << clockPin, 0)
#define dataHighClockLow() gpio_output_set(1 << dataPin, 1 << clockPin, 1 << dataPin | 1 << clockPin, 0)
#define dataLowClockHigh() gpio_output_set(1 << clockPin, 1 << dataPin, 1 << dataPin | 1 << clockPin, 0)
#define dataLowClockLow() gpio_output_set(0, 1 << dataPin | 1 << clockPin, 1 << dataPin | 1 << clockPin, 0)

static uint8_t m_nLastSDA;
static uint8_t m_nLastSCL;

static void MOVE_TO_FLASH i2c_setDataAndClock(uint8_t SDA, uint8_t SCL)
{
  SDA &= 0x01;
  SCL &= 0x01;
  m_nLastSDA = SDA;
  m_nLastSCL = SCL;

  if ((0 == SDA) && (0 == SCL))
  {
    dataLowClockLow();
  }
  else if ((0 == SDA) && (1 == SCL))
  {
    dataLowClockHigh();
  }
  else if ((1 == SDA) && (0 == SCL))
  {
    dataHighClockLow();
  }
  else
  {
    dataHighClockHigh();
  }

  os_delay_us(5);
}

uint8_t MOVE_TO_FLASH i2c_getDC()
{
  return GPIO_INPUT_GET(GPIO_ID_PIN(dataPin));
}

void MOVE_TO_FLASH i2c_start()
{
  i2c_setDataAndClock(1, m_nLastSCL);
  i2c_setDataAndClock(1, 1);
  i2c_setDataAndClock(0, 1);
}

void MOVE_TO_FLASH i2c_stop()
{
  i2c_setDataAndClock(0, m_nLastSCL);
  i2c_setDataAndClock(0, 1);
  i2c_setDataAndClock(1, 1);
}

void MOVE_TO_FLASH i2c_init()
{
  uint8_t i;

  i2c_setDataAndClock(1, 0);

  // when SCL = 0, toggle SDA to clear up
  i2c_setDataAndClock(0, 0);
  i2c_setDataAndClock(1, 0);

  // set data_cnt to max value
  for (i = 0; i < 28; i++)
  {
    i2c_setDataAndClock(1, 0);
    i2c_setDataAndClock(1, 1);
  }

  i2c_stop();
}

void MOVE_TO_FLASH i2c_setup(uint8_t data, uint8_t clock)
{
  ETS_GPIO_INTR_DISABLE();
  clockPin = clock;
  dataPin = data;

  if (clockPin % 2 == 0)
  {
    pinType(clockPin, 0);
  }
  else
  {
    pinType(clockPin, 3);
  }

  if (dataPin % 2 == 0)
  {
    pinType(dataPin, 0);
  }
  else
  {
    pinType(dataPin, 3);
  }

  pinMode(dataPin, PinOpenDrain);
  pinMode(dataPin, PinOutput);
  pinMode(clockPin, PinOpenDrain);
  pinMode(clock, PinOutput);

  dataHighClockHigh();

  ETS_GPIO_INTR_ENABLE();

  i2c_init();
}

void MOVE_TO_FLASH i2c_setAck(uint8_t level)
{
  i2c_setDataAndClock(m_nLastSDA, 0);
  i2c_setDataAndClock(level, 0);
  i2c_setDataAndClock(level, 1);
  os_delay_us(8);
  i2c_setDataAndClock(level, 0);
  i2c_setDataAndClock(1, 0);
}

uint8_t MOVE_TO_FLASH i2c_getAck(void)
{
  uint8_t retVal;
  i2c_setDataAndClock(m_nLastSDA, 0);
  i2c_setDataAndClock(1, 0);
  i2c_setDataAndClock(1, 1);
  retVal = i2c_getDC();
  i2c_setDataAndClock(1, 0);

  return retVal;
}

bool MOVE_TO_FLASH i2c_checkAck()
{
  if (i2c_getAck())
  {
    return 0;
  }

  return 1;
}

void MOVE_TO_FLASH i2c_sendAck()
{
  i2c_setAck(0x0);
}

void MOVE_TO_FLASH i2c_sendNack()
{
  i2c_setAck(0x1);
}

uint8_t MOVE_TO_FLASH i2c_readByte()
{
  uint8 retVal = 0;
  uint8 k, i;

  i2c_setDataAndClock(m_nLastSDA, 0);

  for (i = 0; i < 8; i++)
  {
    i2c_setDataAndClock(1, 0);
    i2c_setDataAndClock(1, 1);

    k = i2c_getDC();
    if (i == 7)
    {
      os_delay_us(3);
    }

    k <<= (7 - i);
    retVal |= k;
  }

  i2c_setDataAndClock(1, 0);

  return retVal;
}

void MOVE_TO_FLASH i2c_writeByte(uint8_t byte)
{
  uint8_t bit;
  sint8_t i;

  i2c_setDataAndClock(m_nLastSDA, 0);

  for (i = 7; i >= 0; i--)
  {
    bit = byte >> i;
    i2c_setDataAndClock(bit, 0);
    i2c_setDataAndClock(bit, 1);

    if (i == 0)
    {
      os_delay_us(3);
    }

    i2c_setDataAndClock(bit, 0);
  }
}

void MOVE_TO_FLASH i2c_writeBytes(uint8_t *bytes, int length)
{
  uint8_t index = 0;
  uint8_t bit;
  sint8_t i;

  i2c_setDataAndClock(m_nLastSDA, 0);
  while (index < length)
  {
    auto byte = bytes[index];
    for (i = 7; i >= 0; i--)
    {
      bit = byte >> i;
      i2c_setDataAndClock(bit, 0);
      i2c_setDataAndClock(bit, 1);

      if (i == 0)
      {
        os_delay_us(3);
      }

      i2c_setDataAndClock(bit, 0);
    }

    index++;
  }
}

bool MOVE_TO_FLASH i2c_writeByteAndAck(uint8_t byte)
{
  i2c_writeByte(byte);
  return i2c_checkAck();
}

void MOVE_TO_FLASH i2c_findDevices(uint8_t *devices)
{
  uint8_t i;

  for (i = 0; i < 255; i++)
  {
    i2c_start();
    i2c_writeByte(i);
    devices[i] = i2c_checkAck();
    i2c_stop();
  }
}

uint8_t MOVE_TO_FLASH i2c_findDevice()
{
  uint8_t i;

  for (i = 0; i < 255; i++)
  {
    i2c_start();
    i2c_writeByte(i);

    if (i2c_checkAck())
    {
      return i;
    }

    i2c_stop();
  }

  return 0;
}

#endif
