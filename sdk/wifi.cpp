#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "mem.h"
#include "osapi.h"
#include "gpio.h"
#include "serial-debug.h"
#include "user_interface.h"
#include "wifi.h"

void Wifi::connectTo(const char* ssid, const char* password) {
  station_config wifiConfiguration;
  os_memcpy(wifiConfiguration.ssid, ssid, 32);
  os_memcpy(wifiConfiguration.password, password, 64);

  wifi_set_opmode(STATIONAP_MODE);
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
      LOG("WIFI: Connecting\n");
      break;
    case STATION_WRONG_PASSWORD:
      LOG("WIFI: Wrong pasword\n");
      break;
    case STATION_NO_AP_FOUND:
      LOG("WIFI: AP not found yet\n");
      break;
    case STATION_CONNECT_FAIL:
      LOG("WIFI: Failed\n");
      break;
    case STATION_GOT_IP:
      LOG("WIFI: Connected\n");
      break;
  }
}

void Wifi::startAccessPoint() {
  startAccessPoint(NULL, NULL);
}

void Wifi::startAccessPoint(const char* ssid) {
  startAccessPoint(ssid, NULL);
}

void Wifi::startAccessPoint(const char* ssid, const char* password) {
  softap_config config;

  if (ssid == NULL) {
    ssid = (const char*)"Homebots AP";
  }

  os_strcpy((char*)config.ssid, ssid);
  os_strcpy((char*)config.password, password);

  config.ssid_len = sizeof(ssid);
  config.channel = 6;

  if (password != NULL) {
    config.authmode = AUTH_WPA2_PSK;
  } else {
    config.authmode = AUTH_OPEN;
  }

  config.ssid_hidden = 0;
  config.max_connection = 4;
  config.beacon_interval = 100;

  wifi_set_opmode(STATIONAP_MODE);
  // wifi_status_led_install(GPIO_ID_PIN(2), PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
  wifi_softap_set_config(&config);
}

#ifdef __cplusplus
}
#endif
