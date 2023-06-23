#ifndef _ESP_WIFI_H_
#define _ESP_WIFI_H_

#include "c_types.h"
#include "mem.h"
#include "osapi.h"
#include "gpio.h"
#include "serial-debug.h"
#include "user_interface.h"

class Wifi
{
public:
  void connectTo(const char *ssid, const char *password);
  void connectTo(const char *ssid, const char *password, bool storeCredentials);
  void startAccessPoint();
  void startAccessPoint(const char *ssid);
  void startAccessPoint(const char *ssid, const char *password);
  void stopAccessPoint();
  void useStatusLed(uint8_t ioPin);
  bool isConnected();
  void printStatus();
  void printStationStatus();
  void printMacAddress();
  void disconnect();
  void getMacAddress(char *address);
  void scan();
  bss_info *getWifiList();

  uint8_t getStatus();
};

////

void _getMacAddress(char *address)
{
  uint8_t mac[6];
  wifi_get_macaddr(STATION_IF, mac);
  os_sprintf(address, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

LOCAL bss_info *lastScan = 0;

void _onScanCompleted(bss_info *result, STATUS status)
{
  if (status != OK)
  {
    return;
  }

  int count = 0;

  for (bss_info *pointer = result; pointer; pointer = STAILQ_NEXT(pointer, next), ++count)
    ;

  if (count == 0)
  {
    return;
  }

  if (lastScan != 0)
  {
    os_free(lastScan);
  }

  lastScan = (bss_info *)os_zalloc(count * sizeof(bss_info));
  bss_info *copy = lastScan;

  os_printf("Copy %d %ul \n", count, copy);
  count = 0;

  for (bss_info *current = result; current; current = STAILQ_NEXT(current, next), ++count)
  {
    os_memcpy(copy + count, (void *)current, sizeof(bss_info));
    os_printf(".. %s\n", *(copy + count)->ssid);
  }
}

void MOVE_TO_FLASH Wifi::connectTo(const char *ssid, const char *password)
{
  connectTo(ssid, password, false);
}

void MOVE_TO_FLASH Wifi::connectTo(const char *ssid, const char *password, bool storeCredentials)
{
  station_config wifiConfiguration;
  os_memcpy(wifiConfiguration.ssid, ssid, 32);
  os_memcpy(wifiConfiguration.password, password, 64);

  wifi_set_opmode(STATIONAP_MODE);
  wifiConfiguration.bssid[0] = 1;

  if (storeCredentials)
  {
    wifi_station_set_config(&wifiConfiguration);
  }
  else
  {
    wifi_station_set_config_current(&wifiConfiguration);
  }

  wifi_station_connect();
}

void MOVE_TO_FLASH Wifi::disconnect()
{
  station_config wifiConfiguration;
  wifiConfiguration.ssid[0] = '\0';
  wifiConfiguration.password[0] = '\0';
  wifiConfiguration.bssid[0] = 1;

  wifi_set_opmode(STATIONAP_MODE);
  wifi_station_set_config(&wifiConfiguration);
  wifi_station_set_config_current(&wifiConfiguration);
}

uint8_t MOVE_TO_FLASH Wifi::getStatus()
{
  return wifi_station_get_connect_status();
}

bool MOVE_TO_FLASH Wifi::isConnected()
{
  return wifi_station_get_connect_status() == STATION_GOT_IP;
}

void MOVE_TO_FLASH Wifi::printStationStatus()
{
  uint8 clients = wifi_softap_get_station_num();
  LOG("Clients: %d\n", clients);

  if (!clients)
  {
    wifi_softap_free_station_info();
    return;
  }

  struct station_info *info = wifi_softap_get_station_info();

  union
  {
    uint32_t address;
    uint8 octets[4];
  } ip;

  while (info != NULL)
  {
    LOG("SSID: %d\n", info->bssid);
    LOG("IP: ");
    ip.address = info->ip.addr;

    LOG("%d.%d.%d.%d\n", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3]);
    info = STAILQ_NEXT(info, next);
  }

  wifi_softap_free_station_info();
}

void MOVE_TO_FLASH Wifi::getMacAddress(char *mac)
{
  _getMacAddress(mac);
}

void MOVE_TO_FLASH Wifi::printMacAddress()
{
  char mac[18] = {0};
  _getMacAddress(mac);
  LOG("MAC: %s\n", mac);
}

void MOVE_TO_FLASH Wifi::printStatus()
{
  LOG("WIFI:");

  switch (wifi_station_get_connect_status())
  {
  case STATION_CONNECTING:
    LOG("Connecting\n");
    break;
  case STATION_WRONG_PASSWORD:
    LOG("Wrong pasword\n");
    break;
  case STATION_NO_AP_FOUND:
    LOG("AP not found yet\n");
    break;
  case STATION_CONNECT_FAIL:
    LOG("Failed\n");
    break;
  case STATION_GOT_IP:
    LOG("Connected\n");
    break;
  }
}

void MOVE_TO_FLASH Wifi::startAccessPoint()
{
  startAccessPoint(NULL, NULL);
}

void MOVE_TO_FLASH Wifi::startAccessPoint(const char *ssid)
{
  startAccessPoint(ssid, NULL);
}

void MOVE_TO_FLASH Wifi::startAccessPoint(const char *ssid, const char *password)
{
  LOG("\nAccess point as %s %s\n", ssid, password);
  softap_config config;

  if (ssid == NULL)
  {
    ssid = (const char *)"Homebots_AP";
  }

  os_strcpy((char *)config.ssid, ssid);

  LOG("SSID is '%s'\n", config.ssid);

  config.ssid_len = strlen(ssid);
  config.channel = 6;

  if (password != NULL)
  {
    config.authmode = AUTH_WPA2_PSK;
    os_strcpy((char *)config.password, password);
    LOG("PWD is '%s'\n", config.password);
  }
  else
  {
    config.authmode = AUTH_OPEN;
    *config.password = 0;
  }

  config.ssid_hidden = 0;
  config.max_connection = 32;
  config.beacon_interval = 100;

  ETS_UART_INTR_DISABLE();
  wifi_set_opmode(STATIONAP_MODE);
  wifi_softap_set_config_current(&config);
  ETS_UART_INTR_ENABLE();

  if (wifi_softap_dhcps_status() != DHCP_STARTED)
  {
    wifi_softap_dhcps_start();
  }
}

void MOVE_TO_FLASH Wifi::stopAccessPoint()
{
  struct softap_config conf;

  *conf.ssid = 0;
  *conf.password = 0;
  conf.authmode = AUTH_OPEN;

  ETS_UART_INTR_DISABLE();
  wifi_softap_set_config(&conf);
  wifi_softap_set_config_current(&conf);
  ETS_UART_INTR_ENABLE();
}

void MOVE_TO_FLASH Wifi::useStatusLed(uint8_t ioPin)
{
  switch (ioPin)
  {
  case 0:
    wifi_status_led_install(0, PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    break;
  case 2:
    wifi_status_led_install(2, PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    break;
  }
}

void MOVE_TO_FLASH Wifi::scan()
{
  wifi_station_scan(NULL, (scan_done_cb_t)_onScanCompleted);
}

bss_info *MOVE_TO_FLASH Wifi::getWifiList()
{
  return lastScan;
}

#endif
