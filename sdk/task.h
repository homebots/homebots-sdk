#ifndef _ESP_TASK_H_
#define _ESP_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "c_types.h"
#include "user_interface.h"

#define MAX_SAFE_DELAY 1000UL
#define MAX_QUEUE_SIZE 3

void tick();
void task_schedule(os_task_t task);
void delay(uint16_t time);
void delayMicroseconds(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif
