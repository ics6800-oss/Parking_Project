#ifndef SERVO_H_
#define SERVO_H_

#include "xgpio.h"
#include "sleep.h"

extern XGpio gpio_servo;

void servo_init();
void control_servo(u32 distance);

#endif