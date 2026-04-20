#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "xgpio.h"
#include "xtmrctr.h"

extern XGpio gpio_ultra;
extern XTmrCtr timer_ultra;

void ultrasonic_init();
u32 get_filtered_distance();

#endif