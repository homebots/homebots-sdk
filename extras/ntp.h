#ifndef _ESP_NTP_H_
#define _ESP_NTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SNTP_INTERVAL
#define SNTP_INTERVAL 6000000
#endif

#include "sntp.h"

#ifndef NTP_SERVER_1
#define NTP_SERVER_1 (char*)"0.pool.ntp.org"
#define NTP_SERVER_2 (char*)"1.pool.ntp.org"
#define NTP_SERVER_3 (char*)"2.pool.ntp.org"
#endif

class NTP {
  bool stopped;

  public:
    uint32_t lastTimestamp;
    uint32_t interval;
    void start();
    void stop();
    void sync();
    bool isReady();
};

void NTP::start() {
  this->stopped = false;
  sntp_init();
  sntp_setservername(0, NTP_SERVER_1);
  sntp_setservername(1, NTP_SERVER_2);
  sntp_setservername(2, NTP_SERVER_3);
}

void NTP::stop() {
  this->stopped = true;
  sntp_stop();
}

void NTP::sync() {
  if (this->stopped) {
    return;
  }

  if (this->interval < SNTP_INTERVAL) {
    return;
  }

  uint32_t systemTime = system_get_time();
  this->lastTimestamp = sntp_get_current_timestamp();
  this->interval = this->lastTimestamp - systemTime;
  // os_delay_us(1);
}

bool NTP::isReady() {
  return this->interval == 0;
}

#ifdef __cplusplus
}
#endif

#endif
