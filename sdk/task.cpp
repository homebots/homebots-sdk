#ifdef __cplusplus
extern "C" {
#endif

#include "task.h"

#include "c_types.h"
#include "osapi.h"
#include "user_interface.h"

static int queueSize = 0;
static os_event_t executionQueue[MAX_QUEUE_SIZE];
static bool delayGuard = false;

void ICACHE_FLASH_ATTR tick() {
  if (!delayGuard) {
    system_os_post(USER_TASK_PRIO_0, 0, 0);
  }
}

void task_schedule(os_task_t task) {
  queueSize++;
  system_os_task(task, USER_TASK_PRIO_0, executionQueue, queueSize);
}

void ICACHE_FLASH_ATTR delay(uint16_t time) {
  delayGuard = true;

  if (time < MAX_SAFE_DELAY) {
    os_delay_us(time);
  } else {
    while (time > MAX_SAFE_DELAY) {
      os_delay_us(MAX_SAFE_DELAY);
      system_soft_wdt_feed();

      time -= MAX_SAFE_DELAY;
    }

    os_delay_us(time);
  }

  delayGuard = false;
  tick();
}

void delayMicroseconds(uint32_t time) {
  delayGuard = true;
  os_delay_us(time);
  delayGuard = false;
}

#ifdef __cplusplus
}
#endif
