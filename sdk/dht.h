#ifndef __DHT__
#define __DHT__

// #ifdef __cplusplus
// extern "C" {
// #endif

// float DHT_readTemperature(bool force);

// #define clockCyclesPerMicrosecond() ( 260L ) //260 is Clock Cycle of LinkIt ONE in MHz
// #define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
// #define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

// #ifdef __cplusplus
// }
// #endif

#include "dht.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "pins.h"

#define MIN_INTERVAL 2000
#define TIMEOUT -1

static uint8_t data[5];
static uint8_t _pin = 0;
static uint8_t _maxCycles = microsecondsToClockCycles(1000);
static uint32_t _lastreadtime = 0;
static uint32_t _lastresult = 0;
static uint32_t pullTime = 55;

/*float DHT_readTemperature(bool force) {
  float f = 0;

  if (read(force)) {
    f = data[2];
    if (data[3] & 0x80) {
      f = -1 - f;
    }

    f += (data[3] & 0x0f) * 0.1;
  }

  return f;
}

float DHT_readHumidity(bool force) {
  float f = 0;

  if (read(force)) {
    f = data[0] + data[1] * 0.1;
  }

  return f;
}


bool DHT_read(bool force) {
  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  uint32_t currenttime = NOW() / 1000UL;

  if (!force && ((currenttime - _lastreadtime) < MIN_INTERVAL)) {
    return _lastresult; // return last correct measurement
  }
  _lastreadtime = currenttime;

  // Reset 40 bits of received data to zero.
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // First set data line low for a period according to sensor type
  // pinMode(_pin, OUTPUT);
  GPIO_OUTPUT_SET(_pin, 0);
  os_delay(20);

  uint32_t cycles[80];
  {
    // End the start signal by setting data line high for 40 microseconds.
    pinMode(_pin, PinInputPullUp);

    // Delay a moment to let sensor pull data line low.
    os_delay_us(pullTime);

    // Now start reading the data line to get the value from the DHT sensor.

    // Turn off interrupts temporarily because the next sections
    // are timing critical and we don't want any interruptions.
    // InterruptLock lock;

    // First expect a low signal for ~80 microseconds followed by a high signal
    // for ~80 microseconds again.
    if (expectPulse(LOW) == TIMEOUT) {
      _lastresult = false;
      return _lastresult;
    }

    if (expectPulse(HIGH) == TIMEOUT) {
      _lastresult = false;
      return _lastresult;
    }

    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed
    // all the pulses are read into a array and then examined in a later step.
    for (int i = 0; i < 80; i += 2) {
      cycles[i] = expectPulse(LOW);
      cycles[i + 1] = expectPulse(HIGH);
    }
  } // Timing critical code is now complete.

  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i = 0; i < 40; ++i) {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[2 * i + 1];

    if ((lowCycles == TIMEOUT) || (highCycles == TIMEOUT)) {
      DEBUG_PRINTLN(F("DHT timeout waiting for pulse."));
      _lastresult = false;
      return _lastresult;
    }

    data[i / 8] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.

    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      data[i / 8] |= 1;
    }

    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }

  // Check we read 40 bits and that the checksum matches.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  } else {
    _lastresult = false;
    return _lastresult;
  }
}

uint32_t DHT_expectPulse(bool level) {
  uint32_t count = 0;

  while ((gpio_input_get() >> _pin) == level) {
    if (count++ >= _maxCycles) {
      return TIMEOUT;
    }
  }

  return count;
}
*/

#endif

