#ifndef _ESP_SDK_
#define _ESP_SDK_

#ifndef DEBUG
#define DEBUG(...)
#endif

#define print(a) os_printf("%s\n", a)

#include "c_types.h"
#include "gpio.h"
#include "pins.h"
#include "osapi.h"
#include "mem.h"
#include "time.h"
#include "user_interface.h"
#include "string-extras.h"
#include "task.h"

typedef unsigned char byte;

#endif
