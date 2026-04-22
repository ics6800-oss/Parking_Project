#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "stdlib.h"

// 각 모듈의 헤더 파일 포함
#include "fnd.h"
#include "led.h"
#include "servo.h"
#include "ultrasonic.h"

// 전역 드라이버 인스턴스 정의
XGpio gpio_led, gpio_fnd, gpio_ultra, gpio_servo;
XTmrCtr timer_ultra;

int main()
{
    init_platform();

    // 각 모듈 초기화
    fnd_init();
    led_init();
    servo_init();
    ultrasonic_init();

    u32 dist = 0;
    
    while(1)
    {
        // 거리 측정
        dist = get_filtered_distance();

        // FND 모드 결정
        if (dist > 0 && dist <= 5) 
        {
            fnd_set_stop_mode(1);
        } 
        else 
        {
            fnd_set_stop_mode(0);
            split(dist);
        }

        // LED 및 서보 제어
        if(dist > 0) 
        {
            control_led(dist);
            control_servo(dist);
        } 
        else 
        {
            control_led(0);
        }

        // FND 갱신 및 대기
        for(int i = 0; i < 200; i++) 
        {
            fnd_display_one_digit();
            usleep(250);
        }
    }

    cleanup_platform();
    return 0;
}
