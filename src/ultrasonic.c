#include "ultrasonic.h"
#include "sleep.h"
#include "xparameters.h"
#include "fnd.h"


// 초음파 필터링
#define FILTER_SIZE 3
u32 distance_buffer[FILTER_SIZE] = {0};
u8  buffer_index = 0, buffer_full = 0;


// 초음파 거리 1회 측정 (raw data)
static u32 get_distance_raw()
{
    // Trig 펄스 생성
    XGpio_DiscreteWrite(&gpio_ultra, 1, 1);
    delay_with_fnd(15);
    XGpio_DiscreteWrite(&gpio_ultra, 1, 0);


    // Echo HIGH 대기
    u32 timeout = 0;
    while(XGpio_DiscreteRead(&gpio_ultra, 2) == 0 && timeout < 400) 
    {
        fnd_display_one_digit();
        usleep(250);
        timeout++;
    }
    if(timeout >= 400) return 0;


    // Echo 펄스 폭 측정
    XTmrCtr_Reset(&timer_ultra, 0);
    XTmrCtr_Start(&timer_ultra, 0);

    while(XGpio_DiscreteRead(&gpio_ultra, 2) == 1 && timeout < 1000) 
    {
        fnd_display_one_digit();
        usleep(100);
        timeout++;
    }
    
    XTmrCtr_Stop(&timer_ultra, 0);
    if(timeout >= 1000) return 0;


    // 시간(us)을 거리(cm)로 변환
    u32 time_us = XTmrCtr_GetValue(&timer_ultra, 0) / 100;
    return (time_us > 58 && time_us < 35000) ? time_us / 58 : 0;
}


// 5회 측정하여 중간값 반환 (튀는 값 제거)
u32 get_median_distance()
{
    u32 samples[5], temp;
    int valid_count = 0;
    for (int i = 0; i < 5; i++) 
    {
        samples[i] = get_distance_raw();
        if(samples[i] > 0 && samples[i] < 400) valid_count++;
         delay_with_fnd(20000);
    }
    if(valid_count < 3) return 0;


    for(int i = 0; i < 4; i++) // 버블 정렬
        for(int j = 0; j < 4-i; j++)
            if(samples[j] > samples[j+1])
            { temp=samples[j]; samples[j]=samples[j+1]; samples[j+1]=temp; }

    return samples[2]; // 중간값
}


// 초음파 초기화
void ultrasonic_init()
{
    XGpio_Initialize(&gpio_ultra, XPAR_AXI_GPIO_2_BASEADDR);
    XGpio_SetDataDirection(&gpio_ultra, 1, 0x0); 
    XGpio_SetDataDirection(&gpio_ultra, 2, 0xFFFFFFFF);

    XTmrCtr_Initialize(&timer_ultra, XPAR_XTMRCTR_0_BASEADDR);
}


// 이동 평균 필터로 값을 부드럽게 만듦
u32 get_filtered_distance()
{
    u32 current = get_median_distance();
    if(current == 0) return 0;


    distance_buffer[buffer_index] = current;
    buffer_index = (buffer_index + 1) % FILTER_SIZE;
    if(!buffer_full && buffer_index == 0) buffer_full = 1;

    
    u32 sum = 0;
    u8 count = buffer_full ? FILTER_SIZE : buffer_index;

    for (u8 i = 0; i < count; i++) {
        sum += distance_buffer[i];
    }

    return sum / count;
}