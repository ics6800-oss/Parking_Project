#include "servo.h"
#include "xparameters.h"
#include "fnd.h"

// 서보 게이트 상태
typedef enum 
{ 
    SERVO_CLOSED = 0, 
    SERVO_OPEN = 1 
}ServoState;

static volatile ServoState servo_state = SERVO_CLOSED;
static u32 servo_current_angle = 0;


// 각도를 서보 PWM 펄스 폭(us)으로 변환
static u32 angle_to_pulse(u32 angle)
{
    if(angle > 180) angle = 180;
    return 600 + (angle * 1800 / 180);
}


// 서보 PWM 신호 1주기(20ms) 전송
static void servo_pulse_once(u32 pulse_us)
{
    XGpio_DiscreteWrite(&gpio_servo, 1, 1);
    delay_with_fnd(pulse_us);

    XGpio_DiscreteWrite(&gpio_servo, 1, 0);
    delay_with_fnd(20000 - pulse_us);
}


// 지정된 각도로 서보 위치 고정
static void servo_hold(u32 angle, u32 cycles)
{
    u32 pulse = angle_to_pulse(angle);
    for(u32 i = 0; i < cycles; i++) 
    {
        servo_pulse_once(pulse);
    }
}


// 서보 모터 부드럽게 이동
static void servo_move_smooth(u32 from_angle, u32 to_angle)
{
    if (from_angle == to_angle) return;
    int step = (from_angle < to_angle) ? 3 : -3;
    int a = (int)from_angle;
    int target = (int)to_angle;
    while(1)
    {
        if((step > 0 && a >= target) || (step < 0 && a <= target))
        {
            servo_hold((u32)target, 50);
            break;
        }
        servo_pulse_once(angle_to_pulse((u32)a));
        a += step;
    }
}


// 서보 초기화
void servo_init()
{
    XGpio_Initialize(&gpio_servo, XPAR_AXI_GPIO_3_BASEADDR);
    XGpio_DiscreteWrite(&gpio_servo, 1, 0);

    // 서보 초기 위치 설정
    servo_hold(0, 30);
    servo_current_angle = 0;
    servo_state = SERVO_CLOSED;
}


// 거리에 따라 차단기 제어
void control_servo(u32 distance)
{
    if(distance > 0 && distance <= 10) 
    {   
        // 10cm 이하: 열림
        if(servo_state == SERVO_CLOSED) 
        {
            servo_move_smooth(servo_current_angle, 120);
            servo_current_angle = 120;
            servo_state = SERVO_OPEN;
        }
    }
    else if(distance >= 15) 
    {   
        // 15cm 이상: 닫힘
        if(servo_state == SERVO_OPEN) 
        {
            servo_move_smooth(servo_current_angle, 0);
            servo_current_angle = 0;
            servo_state = SERVO_CLOSED;
        }
    }
}