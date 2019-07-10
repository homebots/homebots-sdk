#ifndef _ESP_NTP_H_
#define _ESP_NTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SNTP_INTERVAL
#define SNTP_INTERVAL 6000000
#endif

#include "sdk.h"
#include "sntp.h"

char ntpServer1[] = "0.pool.ntp.org";
char ntpServer2[] = "1.pool.ntp.org";
char ntpServer3[] = "2.pool.ntp.org";

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
  sntp_setservername(0, (char*)ntpServer1);
  sntp_setservername(1, (char*)ntpServer2);
  sntp_setservername(2, (char*)ntpServer3);
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
