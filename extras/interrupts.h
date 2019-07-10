
static inline unsigned getClockCount(void) {
  unsigned r;
  asm volatile ("rsr %0, ccount" : "=r"(r));
  return r;
}

// https://github.com/espressif/ESP8266_NONOS_SDK/blob/master/examples/wps/include/driver/key.h
// LOCAL void key_intr_handler(void *arg);

// LOCAL void key_intr_handler(void *arg) {
//   uint8 i;
//   uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
//   struct keys_param *keys = (struct keys_param *)arg;

//   for (i = 0; i < keys->key_num; i++) {
//     if (gpio_status & BIT(keys->single_key[i]->gpio_id)) {
//       //disable interrupt
//       gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_DISABLE);

//       //clear interrupt status
//       GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(keys->single_key[i]->gpio_id));

//       if (keys->single_key[i]->key_level == 1) {
//         // 5s, restart & enter softap mode
//         os_timer_disarm(&keys->single_key[i]->key_5s);
//         os_timer_setfn(&keys->single_key[i]->key_5s, (os_timer_func_t *)key_5s_cb, keys->single_key[i]);
//         os_timer_arm(&keys->single_key[i]->key_5s, 5000, 0);
//         keys->single_key[i]->key_level = 0;
//         gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_POSEDGE);
//       } else {
//         // 50ms, check if this is a real key up
//         os_timer_disarm(&keys->single_key[i]->key_50ms);
//         os_timer_setfn(&keys->single_key[i]->key_50ms, (os_timer_func_t *)key_50ms_cb, keys->single_key[i]);
//         os_timer_arm(&keys->single_key[i]->key_50ms, 50, 0);
//       }
//     }
//   }
// }

// void attachInterrupt(uint8_t pin) {
//   ETS_GPIO_INTR_ATTACH(key_intr_handler, keys);

//   ETS_GPIO_INTR_DISABLE();

//   for (i = 0; i < keys->key_num; i++) {
//       keys->single_key[i]->key_level = 1;

//       PIN_FUNC_SELECT(keys->single_key[i]->gpio_name, keys->single_key[i]->gpio_func);

//       gpio_output_set(0, 0, 0, GPIO_ID_PIN(keys->single_key[i]->gpio_id));

//       gpio_register_set(GPIO_PIN_ADDR(keys->single_key[i]->gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
//                         | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
//                         | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

//       //clear gpio14 status
//       GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(keys->single_key[i]->gpio_id));

//       //enable interrupt
//       gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_NEGEDGE);
//   }

//   ETS_GPIO_INTR_ENABLE();
// }
