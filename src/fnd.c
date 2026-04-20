#include "fnd.h"
#include "xparameters.h"

// FND 모듈 내부에서 사용하는 변수들
static u8 seg[] = { 0x3f,0x06,0x5b,0x4f,0x66, 0x6d,0x7d,0x07,0x7f,0x6f }; // 숫자 0~9
static u8 anode[] = {0xE, 0xD, 0xB, 0x7}; // 1~4번 자리
static volatile u8 digit[4] = {0};
static volatile u8 current_digit = 0;

// "STOP" 글자 데이터
static u8 seg_s = 0x6d, seg_t = 0x78, seg_o = 0x5c, seg_p = 0x73;
volatile u8 fnd_is_stop_mode = 0; // 0: 숫자 표시, 1: STOP 표시


// FND 초기화
void fnd_init()
{
    XGpio_Initialize(&gpio_fnd, XPAR_AXI_GPIO_1_BASEADDR);
    XGpio_SetDataDirection(&gpio_fnd, 1, 0x0); 
    XGpio_SetDataDirection(&gpio_fnd, 2, 0x0);
}

void fnd_set_stop_mode(u8 is_stop)
{
    fnd_is_stop_mode = is_stop;
}

// 숫자를 4자리로 분리하여 FND에 표시할 준비
void split(u32 val)
{
    if(val > 9999) val = 9999;
    digit[0] = val % 10;
    digit[1] = (val / 10) % 10;
    digit[2] = (val / 100) % 10;
    digit[3] = (val / 1000) % 10;
}

// FND의 한 자리 표시
void fnd_display_one_digit()
{
    if (fnd_is_stop_mode) 
    {
        // "STOP" 표시
        u8 seg_to_display;
        switch(current_digit) 
        {
            case 3: seg_to_display = seg_s; break;
            case 2: seg_to_display = seg_t; break;
            case 1: seg_to_display = seg_o; break;
            case 0: seg_to_display = seg_p; break;
            default: seg_to_display = 0; break;
        }
        XGpio_DiscreteWrite(&gpio_fnd, 1, anode[current_digit]);
        XGpio_DiscreteWrite(&gpio_fnd, 2, ~seg_to_display & 0xFF);
    } 
    else 
    {
        // 숫자 표시
        XGpio_DiscreteWrite(&gpio_fnd, 1, anode[current_digit]);
        XGpio_DiscreteWrite(&gpio_fnd, 2, ~seg[digit[current_digit]] & 0xFF);
    }
    current_digit = (current_digit + 1) % 4;
}

// FND를 갱신하며 대기하는 딜레이 함수
void delay_with_fnd(u32 us)
{
    while(us >= 250)
    {
        fnd_display_one_digit();
        usleep(250);
        us -= 250;
    }

    if(us) usleep(us);
}