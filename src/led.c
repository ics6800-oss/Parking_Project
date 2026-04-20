#include "led.h"
#include "xparameters.h"

// LED 초기화
void led_init()
{
    XGpio_Initialize(&gpio_led, XPAR_AXI_GPIO_0_BASEADDR);
    XGpio_SetDataDirection(&gpio_led, 1, 0x0);
}

// 거리에 따라 LED 제어
void control_led(u32 distance)
{
    u16 led_value = 0;
    if     (distance < 10) led_value = 0xFFFF;
    else if(distance < 15) led_value = 0x3FFF;
    else if(distance < 20) led_value = 0x0FFF;
    else if(distance < 25) led_value = 0x01FF;
    else if(distance < 30) led_value = 0x003F;
    else if(distance < 40) led_value = 0x0007;
    XGpio_DiscreteWrite(&gpio_led, 1, led_value);
}