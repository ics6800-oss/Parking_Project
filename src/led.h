#ifndef LED_H_
#define LED_H_

#include "xgpio.h"

extern XGpio gpio_led;

void led_init();
void control_led(u32 distance);

#endif