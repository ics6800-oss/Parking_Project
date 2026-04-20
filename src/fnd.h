#ifndef FND_H_
#define FND_H_

#include "xgpio.h"
#include "sleep.h"

extern XGpio gpio_fnd;

void fnd_init();
void fnd_set_stop_mode(u8 is_stop);
void split(u32 val);
void fnd_display_one_digit();
void delay_with_fnd(u32 us);

#endif