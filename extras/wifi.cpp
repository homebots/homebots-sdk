#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "mem.h"
#include "osapi.h"
#include "serial-debug.h"
#include "user_interface.h"
#include "wifi.h"

// #ifndef DEBUG
// #define DEBUG(...) os_printf(__VA_ARGS__)
// #define DEBUG(...)
// #endif

void Wifi::connectTo(const char* ssid, const char *password) {
  station_config wifiConfiguration;
  os_memcpy(wifiConfiguration.ssid, ssid, 32);
  os_memcpy(wifiConfiguration.password, password, 64);

  wifi_set_opmode(STATION_MODE);
  wifiConfiguration.bssid[0] = 1;
  wifi_station_set_config(&wifiConfiguration);
  wifi_station_connect();
}

uint8_t  Wifi::getStatus() {
  return wifi_station_get_connect_status();
}

bool  Wifi::isConnected() {
  return wifi_station_get_connect_status() == STATION_GOT_IP;
}

void  Wifi::printStatus() {
  switch (wifi_station_get_connect_status()) {
    case STATION_CONNECTING:
      DEBUG("WIFI: Connecting\n");
      break;
    case STATION_WRONG_PASSWORD:
      DEBUG("WIFI: Wrong pasword\n");
      break;
    case STATION_NO_AP_FOUND:
      DEBUG("WIFI: AP not found yet\n");
      break;
    case STATION_CONNECT_FAIL:
      DEBUG("WIFI: Failed\n");
      break;
    case STATION_GOT_IP:
      DEBUG("WIFI: Connected\n");
      break;
  }
}

void  Wifi::startAccessPoint() {
  // softap_config config;
  // char* ssid = os_sprintf("HomeBots %s", (char)os_random());

  // config.ssid = ssid;
  // config.password = NULL;
  // config.ssid_len = sizeof(ssid);
  // config.channel = 6;
  // config.authmode = AUTH_WPA2_PSK;
  // config.ssid_hidden = 0;
  // config.max_connection = 1;
  // config.beacon_interval = 100;

  // wifi_softap_set_config(config);
}

#ifdef __cplusplus
}
#endif
