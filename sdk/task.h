#ifndef _ESP_TASK_H_
#define _ESP_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "user_interface.h"
#include "c_types.h"

#define MAX_SAFE_DELAY    1000UL
#define MAX_QUEUE_SIZE    3

class TaskQueueHelper {
  uint8_t queueSize;

  public:
    void schedule(os_task_t task);
};

void tick();
void ICACHE_FLASH_ATTR delay(uint16_t time);
void delayMicroseconds(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif
