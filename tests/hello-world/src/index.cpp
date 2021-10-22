#include "homebots.h"

static os_timer_t timer;

void sayHi()
{
  os_printf("Hello world!\n");
}

// This function is called after boot
// Add your program here...
void setup()
{
  os_timer_setfn(&timer, (os_timer_func_t *)sayHi, NULL);
  os_timer_arm(&timer, 1000, 1);
}
