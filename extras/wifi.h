#ifndef _ESP_WIFI_H_
#define _ESP_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdk.h"

class Wifi {
  public:
    void connectTo(const char* ssid, const char *password);
    bool isConnected();
    void printStatus();
    uint8_t getStatus();
    void initSntp();
};

void Wifi::connectTo(const char* ssid, const char *password) {
  station_config wifiConfiguration;
  os_memcpy(wifiConfiguration.ssid, ssid, 32);
  os_memcpy(wifiConfiguration.password, password, 64);

  wifi_set_opmode(STATION_MODE);
  wifiConfiguration.bssid[0] = 1;
  wifi_station_set_config(&wifiConfiguration);
  wifi_station_connect();
}

uint8_t Wifi::getStatus() {
  return wifi_station_get_connect_status();
}

bool Wifi::isConnected() {
  return wifi_station_get_connect_status() == STATION_GOT_IP;
}

void Wifi::printStatus() {
  switch (wifi_station_get_connect_status()) {
    case STATION_CONNECTING:
      os_printf("WIFI: Connecting\n");
      break;
    case STATION_WRONG_PASSWORD:
      os_printf("WIFI: Wrong pasword\n");
      break;
    case STATION_NO_AP_FOUND:
      os_printf("WIFI: AP not found yet\n");
      break;
    case STATION_CONNECT_FAIL:
      os_printf("WIFI: Failed\n");
      break;
    case STATION_GOT_IP:
      os_printf("WIFI: Connected\n");
      break;
  }
}

#ifdef __cplusplus
}
#endif

#endif
