
#ifndef _ESP_TCP_H_
#define _ESP_TCP_H_

#include "c_types.h"
#include "ip_addr.h"
#include "espconn.h"
#include "osapi.h"
#include "serial-debug.h"
#include "user_interface.h"

typedef void (*TcpConnectCallback)(TCP *connection);
typedef void (*TcpDisconnectCallback)(TCP *connection);
typedef void (*TcpErrorCallback)(TCP *connection, signed char error);
typedef void (*TcpReceiveCallback)(TCP *connection, char *data, uint16_t len);

typedef struct
{
  int port;
  struct espconn esp;
  esp_tcp tcp;
  TcpReceiveCallback onReceive;
  TcpConnectCallback onConnect;
  TcpDisconnectCallback onDisconnect;
  TcpErrorCallback onError;
} TCP;

static void MOVE_TO_FLASH _onReceive(void *arg, char *data,
                                     uint16_t length)
{
  struct espconn *esp = (struct espconn *)arg;
  TCP *connection = (TCP *)esp->reverse;

  LOG("Received %d bytes from " IPSTR " \n", length,
      IP2STR(connection.tcp->remote_ip));

  if (connection->onReceive != NULL)
  {
    connection->onReceive(connection, data, length);
  }
}

static void MOVE_TO_FLASH _onSendCompleted(void *arg)
{
  struct espconn *esp = (struct espconn *)arg;
  TCP *connection = (TCP *)esp->reverse;
  tcp_close(connection);
}

static void MOVE_TO_FLASH _onConnect(void *arg)
{
  struct espconn *esp = (struct espconn *)arg;
  TCP *connection = (TCP *)esp->reverse;

  LOG("connected\n");
  espconn_regist_recvcb(esp, (espconn_recv_callback)_onReceive);

  if (connection->onConnect)
  {
    connection->onConnect(connection);
  }
}

static void MOVE_TO_FLASH _onDisconnect(void *arg)
{
  struct espconn *esp = (struct espconn *)arg;
  TCP *connection = (TCP *)esp->reverse;

  LOG("disconnected\n");
  if (connection->onDisconnect)
  {
    connection->onDisconnect(connection);
  }
}

void MOVE_TO_FLASH tcp_connect(TCP *connection, int port)
{
  LOG("\n\nConnecting...\n");
  connection.tcp.local_port = port;
  espconn *esp = connection->esp;
  esp.state = ESPCONN_NONE;
  esp.type = ESPCONN_TCP;
  esp.proto.tcp = &tcp;
  esp.reverse = (void *)this;

  espconn_regist_connectcb(&connection, (espconn_connect_callback)_onConnect);
  espconn_regist_disconcb(&connection, (espconn_connect_callback)_onDisconnect);
  espconn_regist_sentcb(&connection, (espconn_sent_callback)_onSendCompleted);
  espconn_accept(&connection);

  LOG("Listening on %d\n", port);
}

int MOVE_TO_FLASH tcp_send(TCP *connection, uint8_t *data)
{
  return send(data, os_strlen((const char *)data));
}

int MOVE_TO_FLASH tcp_send(TCP *connection, uint8_t *data, uint16 length)
{
  if (isConnected())
  {
    LOG(">> Sending %d bytes\n%s\n", length, data);
    return espconn_sent(&connection, data, length);
  }

  LOG("Can't write, server is disconnected!\n");
  return 0;
}

bool MOVE_TO_FLASH tcp_isConnected(TCP *connection)
{
  return connection.state != ESPCONN_CLOSE && connection.state != ESPCONN_NONE;
}

void MOVE_TO_FLASH tcp_close(TCP *connection)
{
  espconn_disconnect(&connection);
}

uint8 MOVE_TO_FLASH tcp_getState(TCP *connection)
{
  return connection.state;
}

#endif
