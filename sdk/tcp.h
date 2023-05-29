
#ifndef _ESP_TCP_H_
#define _ESP_TCP_H_

#include "c_types.h"
#include "ip_addr.h"
#include "espconn.h"
#include "osapi.h"
#include "serial-debug.h"
#include "user_interface.h"

class TcpServer;

typedef void (*TcpConnectCallback)(TcpServer *server);
typedef void (*TcpDisconnectCallback)(TcpServer *server);
typedef void (*TcpErrorCallback)(TcpServer *server, signed char error);
typedef void (*TcpReceiveCallback)(TcpServer *server, char *data, uint16_t len);

class TcpServer
{
public:
  TcpReceiveCallback onReceive;
  TcpConnectCallback onConnect;
  TcpDisconnectCallback onDisconnect;
  TcpErrorCallback onError;

  TcpServer();
  void connect(int port);
  int send(uint8_t *data);
  int send(uint8_t *data, uint16 length);
  void close();
  bool isConnected();
  uint8 getState();

private:
  int port;
  struct espconn connection;
  esp_tcp tcp;
};

////

LOCAL void ICACHE_FLASH_ATTR _onReceive(void *arg, char *data,
                                        uint16_t length)
{
  struct espconn *connection = (struct espconn *)arg;
  TcpServer *server = (TcpServer *)connection->reverse;

  LOG("Received %d bytes from " IPSTR " \n", length,
      IP2STR(connection->proto.tcp->remote_ip));

  if (server->onReceive)
  {
    server->onReceive(server, data, length);
  }
}

LOCAL void ICACHE_FLASH_ATTR _onSendCompleted(void *arg)
{
  struct espconn *connection = (struct espconn *)arg;
  TcpServer *server = (TcpServer *)connection->reverse;
  server->close();
}

LOCAL void ICACHE_FLASH_ATTR _onConnect(void *arg)
{
  struct espconn *connection = (struct espconn *)arg;
  TcpServer *server = (TcpServer *)connection->reverse;

  LOG("connected\n");
  espconn_regist_recvcb(connection, (espconn_recv_callback)_onReceive);

  if (server->onConnect)
  {
    server->onConnect(server);
  }
}

LOCAL void ICACHE_FLASH_ATTR _onDisconnect(void *arg)
{
  struct espconn *connection = (struct espconn *)arg;
  TcpServer *server = (TcpServer *)connection->reverse;

  LOG("disconnected\n");
  if (server->onDisconnect)
  {
    server->onDisconnect(server);
  }
}

TcpServer::TcpServer()
{
  onReceive = 0;
  onConnect = 0;
  onDisconnect = 0;
  onError = 0;
}

void TcpServer::connect(int port_)
{
  LOG("\n\nConnecting...\n");
  port = port_;
  tcp.local_port = port_;
  connection.state = ESPCONN_NONE;
  connection.type = ESPCONN_TCP;
  connection.proto.tcp = &tcp;
  connection.reverse = (void *)this;

  espconn_regist_connectcb(&connection, (espconn_connect_callback)_onConnect);
  espconn_regist_disconcb(&connection, (espconn_connect_callback)_onDisconnect);
  espconn_regist_sentcb(&connection, (espconn_sent_callback)_onSendCompleted);
  espconn_accept(&connection);

  LOG("Listening on %d\n", port);
}

int TcpServer::send(uint8_t *data)
{
  return send(data, os_strlen((const char *)data));
}

int TcpServer::send(uint8_t *data, uint16 length)
{
  if (isConnected())
  {
    LOG(">> Sending %d bytes\n%s\n", length, data);
    return espconn_sent(&connection, data, length);
  }

  LOG("Can't write, server is disconnected!\n");
  return 0;
}

bool TcpServer::isConnected()
{
  return connection.state != ESPCONN_CLOSE && connection.state != ESPCONN_NONE;
}

void TcpServer::close()
{
  espconn_disconnect(&connection);
}

uint8 TcpServer::getState()
{
  return connection.state;
}

#endif
