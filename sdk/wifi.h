#ifndef _ESP_WIFI_H_
#define _ESP_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"

class Wifi {
  public:
    void connectTo(const char* ssid, const char* password);
    void connectTo(const char* ssid, const char* password, bool storeCredentials);
    void startAccessPoint();
    void startAccessPoint(const char* ssid);
    void startAccessPoint(const char* ssid, const char* password);
    void stopAccessPoint();
    void useStatusLed(uint8_t ioPin);
    bool isConnected();
    void printStatus();
    void printStationStatus();
    void printMacAddress();
    void disconnect();
    void getMacAddress(char* address);

    uint8_t getStatus();
};

#ifdef __cplusplus
}
#endif

#endif
