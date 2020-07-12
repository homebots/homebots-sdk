
#ifndef _ESP_TCP_H_
#define _ESP_TCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "ip_addr.h"
#include "espconn.h"
#include "osapi.h"
#include "serial-debug.h"
#include "user_interface.h"

class TcpServer;

typedef void (*TcpConnectCallback)(TcpServer* server);
typedef void (*TcpDisconnectCallback)(TcpServer* server);
typedef void (*TcpErrorCallback)(TcpServer* server, signed char error);
typedef void (*TcpReceiveCallback)(TcpServer* server, char* data, uint16_t len);

class TcpServer {
 public:
  TcpReceiveCallback onReceive;
  TcpConnectCallback onConnect;
  TcpDisconnectCallback onDisconnect;
  TcpErrorCallback onError;

  TcpServer();
  void connect(int port);
  int send(uint8_t* data);
  int send(uint8_t* data, uint16 length);
  void close();
  bool isConnected();
  uint8 getState();

 private:
  int port;
  struct espconn connection;
  esp_tcp tcp;
};

#ifdef __cplusplus
}
#endif

#endif
