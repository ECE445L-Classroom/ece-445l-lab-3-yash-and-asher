#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "Lab3.h"

volatile uint32_t seconds_left;

void Timer_Init()
{
    seconds_left = 0;
    Timer0A_Init(&counter_task, 80000, 2);
}

void counter_task()
{
    if(seconds_left != 0)
    {
        seconds_left--;
    }
    else
    {
        Timer0A_Stop();
    }  
}

void Timer_Set(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    Timer0A_Stop();
    seconds_left = hours * 3600 + minutes * 60 + seconds;
}

void Timer_Start()
{
    NVIC_EN0_R = NVIC_EN0_INT19;     // 9) enable interrupt 19 in NVIC
    TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
}

void Timer_Stop()
{
    Timer0A_Stop();
}

uint32_t Timer_TimeLeft(uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
{
    uint8_t h = seconds_left / 3600;
    uint8_t m = (seconds_left - h * 3600) / 60;
    uint8_t s = seconds_left - h * 3600 - m * 60;
    *hours = h;
    *minutes = m;
    *seconds = s;
    return seconds_left;
}