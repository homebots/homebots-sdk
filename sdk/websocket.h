/* Websocket client implementation
 *
 * Copyright (c) 2016 Luís Fonseca <miguelluisfonseca@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _WEBSOCKET_H_
#define _WEBSOCKET_H_

#define WS_OPCODE_CONTINUATION 0x0
#define WS_OPCODE_TEXT         0x1
#define WS_OPCODE_BINARY       0x2
#define WS_OPCODE_CLOSE        0x8
#define WS_OPCODE_PING         0x9
#define WS_OPCODE_PONG         0xA

#define CS_INITIAL    0
#define CS_CLOSED     1
#define CS_CONNECTING 2
#define CS_CONNECTED  3
#define CS_CLOSING    4

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "ip_addr.h"
#include "espconn.h"
#include "limits.h"
#include "mem.h"
#include "osapi.h"
#include "serial-debug.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string-extras.h"
#include "user_interface.h"

struct ws_info;

typedef void (*ws_onConnectionCallback)(struct ws_info *wsInfo);
typedef void (*ws_onReceiveCallback)(struct ws_info *wsInfo, int len,
                                     char *message, int opCode);
typedef void (*ws_onFailureCallback)(struct ws_info *wsInfo, int errorCode);

typedef struct {
  const char *key;
  const char *value;
} header_t;

typedef struct ws_info {
  int connectionState;

  bool isSecure;
  char *hostname;
  int port;
  char *path;
  char *expectedSecKey;
  header_t *extraHeaders;

  struct espconn *conn;
  void *reservedData;
  int knownFailureCode;

  char *frameBuffer;
  int frameBufferLen;

  char *payloadBuffer;
  int payloadBufferLen;
  int payloadOriginalOpCode;

  os_timer_t timeoutTimer;
  int unhealthyPoints;

  ws_onConnectionCallback onConnection;
  ws_onReceiveCallback onReceive;
  ws_onFailureCallback onFailure;
} ws_info;

void ICACHE_FLASH_ATTR ws_connect(ws_info *wsInfo, const char *url);
void ICACHE_FLASH_ATTR ws_send(ws_info *wsInfo, int opCode, const char *message,
                               unsigned short length);
void ICACHE_FLASH_ATTR ws_close(ws_info *wsInfo);

#ifdef __cplusplus
}
#endif

#endif  // _WEBSOCKET_H_
