#include <stdio.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "timer.h"
#include "lcd.h"

volatile uint32_t seconds;
volatile uint32_t minutes;
volatile uint32_t hours;
volatile uint32_t tenth_seconds;

volatile uint32_t hours_alarm;
volatile uint32_t minutes_alarm;
volatile uint32_t seconds_alarm;

void counter_task()
{
    tenth_seconds++;
    if(tenth_seconds <= 10)
    {
        return;
    }
    
    if(seconds == 59){
        if(minutes == 59){
            minutes = 0;
            if(hours == 12){
                hours = 1;
            }
            else hours += 1;  
        }
        else minutes += 1 % 60;
        seconds = 0;
    } 
    else seconds += 1;

    clock_update(hours, minutes,  seconds);
    tenth_seconds = 0;
    
}

void Timer_Init(uint8_t h, uint8_t m, uint8_t s)
{
    tenth_seconds = 0;
    seconds = s;
    minutes = m;
    hours = h;
    Timer0A_Init(&counter_task, 8000000, 2);
}

void Timer_SetAlarm(uint8_t _hours, uint8_t _minutes, uint8_t _seconds)
{
    Timer0A_Stop();
    hours_alarm = _hours;
    minutes_alarm = _minutes;
    seconds_alarm = _seconds;
    Timer_Start();
}

void Timer_GetAlarm(uint8_t *_hours, uint8_t *_minutes, uint8_t *_seconds)
{
    *_hours = hours_alarm;
    *_minutes = minutes_alarm;
    *_seconds = seconds_alarm;
}

void Timer_SetTime(uint8_t _hours, uint8_t _minutes, uint8_t _seconds)
{
    Timer0A_Stop();
    hours = _hours;
    minutes = _minutes;
    seconds = _seconds;
    Timer_Start();
}

void Timer_GetTime(uint8_t *_hours, uint8_t *_minutes, uint8_t *_seconds)
{
    *_hours = hours;
    *_minutes = minutes;
    *_seconds = seconds;
}

void Timer_SnoozeAlarm()
{
    if(Timer_TriggerAlarm())
    {
        if(minutes_alarm + 10 > 59)
        {
            hours_alarm = (hours_alarm + 1) % 24;
            minutes_alarm = (minutes_alarm + 10) % 60;
        }
        else
        {
            minutes_alarm = minutes_alarm + 10;
        }
    }
}

#define NVIC_EN0_INT19 0x00080000  // Interrupt 19 enable
void Timer_Start()
{
    NVIC_EN0_R = NVIC_EN0_INT19;     // 9) enable interrupt 19 in NVIC
    TIMER0_CTL_R |= 0x00000001;      // 10) enable timer0A
}

uint8_t Timer_TriggerAlarm()
{
    if((hours_alarm - hours) == 0 && (minutes_alarm - minutes) == 0 && (seconds_alarm - seconds) == 0)
    {
        return 1;
    }   
    else
    {
        return 0;
    }
}