#ifndef _ESP_SDK_
#define _ESP_SDK_

#ifndef DEBUG
#define DEBUG(...)
#endif

#define print(a) os_printf("%s\n", a)

#include "c_types.h"
#include "serial-debug.h"
#include "gpio.h"
#include "pins.h"
#include "mem.h"
#include "osapi.h"
#include "string-extras.h"
#include "task.h"
#include "time.h"
#include "user_interface.h"
#include "websocket.h"
#include "wifi.h"
// #include "ntp.h"

#endif
