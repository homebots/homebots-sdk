#ifndef _HOMEBOTS_
#define _HOMEBOTS_

#ifndef SERIAL_SPEED
#define SERIAL_SPEED 115200
#endif

#define USE_US_TIMER
#define FLASH_SIZE_MAP 2
#define SYSTEM_PARTITION_OTA_SIZE 0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR 0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR 0x7b000
#define SYSTEM_PARTITION_PHY_DATA_ADDR 0x7c000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR 0x7d000

extern void setup();

#ifdef __cplusplus
extern "C"
{
#endif

#include "sdk.h"

  void ICACHE_FLASH_ATTR user_init()
  {
    uart_div_modify(0, UART_CLK_FREQ / SERIAL_SPEED);
    gpio_init();

    // 160MHz
    system_update_cpu_freq(SYS_CPU_160MHZ);

    setup();
  }

  void ICACHE_FLASH_ATTR user_pre_init(void)
  {
    const partition_item_t partitions[] = {
        {SYSTEM_PARTITION_BOOTLOADER, 0x0, 0x1000},
        {SYSTEM_PARTITION_OTA_1, 0x1000, SYSTEM_PARTITION_OTA_SIZE},
        {SYSTEM_PARTITION_OTA_2, SYSTEM_PARTITION_OTA_2_ADDR, SYSTEM_PARTITION_OTA_SIZE},
        {SYSTEM_PARTITION_RF_CAL, SYSTEM_PARTITION_RF_CAL_ADDR, 0x1000},
        {SYSTEM_PARTITION_PHY_DATA, SYSTEM_PARTITION_PHY_DATA_ADDR, 0x1000},
        {SYSTEM_PARTITION_SYSTEM_PARAMETER, SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 0x3000},
    };

    if (!system_partition_table_regist(partitions, sizeof(partitions) / sizeof(partitions[0]), FLASH_SIZE_MAP))
    {
      os_printf("system partition failed\n");
      while (1)
        ;
    }
  }

#ifdef __cplusplus
}
#endif

#endif
