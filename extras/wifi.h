#ifndef _ESP_WIFI_H_
#define _ESP_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"

class Wifi {
  public:
    void connectTo(const char* ssid, const char *password);
    bool isConnected();
    void printStatus();
    void startAccessPoint();
    uint8_t getStatus();
    void initSntp();
};

#ifdef __cplusplus
}
#endif

#endif
