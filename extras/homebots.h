#ifndef _ESP_OPEN_SDK_
#define _ESP_OPEN_SDK_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 115200
#endif

#include "sdk.h"

#define loopTaskQueueLength     1

extern void loop();
extern void setup();

os_event_t executionQueue[loopTaskQueueLength];

void tick() {
  system_os_post(USER_TASK_PRIO_0, 0, 0);
}

static void ICACHE_FLASH_ATTR
user_loop(os_event_t *events) {
  loop();
  tick();
}

void ICACHE_FLASH_ATTR
user_init() {
  uart_div_modify(0, UART_CLK_FREQ / SERIAL_SPEED);
  gpio_init();
  setup();

  system_os_task(user_loop, USER_TASK_PRIO_0, executionQueue, loopTaskQueueLength);
  tick();
}

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

#ifdef __cplusplus
}
#endif

#endif
