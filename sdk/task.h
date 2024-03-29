#ifndef _ESP_TASK_H_
#define _ESP_TASK_H_

#include "c_types.h"
#include "user_interface.h"

#define MAX_SAFE_DELAY 1000UL
#define MAX_QUEUE_SIZE 3

void system_tick();
void task_schedule(os_task_t task);
void delay(uint16_t time);
void delayMicroseconds(uint32_t time);

////

#include "c_types.h"
#include "osapi.h"
#include "user_interface.h"

static int queueSize = 0;
static os_event_t executionQueue[MAX_QUEUE_SIZE];
static bool delayGuard = false;

void MOVE_TO_FLASH system_tick()
{
  if (!delayGuard)
  {
    system_os_post(USER_TASK_PRIO_0, 0, 0);
  }
}

void MOVE_TO_FLASH task_schedule(os_task_t task)
{
  queueSize++;
  system_os_task(task, USER_TASK_PRIO_0, executionQueue, queueSize);
}

void MOVE_TO_FLASH delay(uint16_t time)
{
  delayGuard = true;

  if (time < MAX_SAFE_DELAY)
  {
    os_delay_us(time);
  }
  else
  {
    while (time > MAX_SAFE_DELAY)
    {
      os_delay_us(MAX_SAFE_DELAY);
      system_soft_wdt_feed();

      time -= MAX_SAFE_DELAY;
    }

    os_delay_us(time);
  }

  delayGuard = false;
  system_tick();
}

void MOVE_TO_FLASH delayMicroseconds(uint32_t time)
{
  delayGuard = true;
  os_delay_us(time);
  delayGuard = false;
}

#endif
