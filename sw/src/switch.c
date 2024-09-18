#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "switch.h"

#define MODE_PE0   (*((volatile uint32_t *)0x40024004))    // Mode Select Switch PE0
#define LEFT_PE1   (*((volatile uint32_t *)0x40024008))    // Left Arrow Switch PE1
#define RIGHT_PE2  (*((volatile uint32_t *)0x40024010))    // Right Arrow Switch PE2
#define UP_PE3     (*((volatile uint32_t *)0x40024020))    // Up Arrow Switch PE3
#define DOWN_PE4   (*((volatile uint32_t *)0x40024040))    // Down Arrow Switch PE4

void Switch_Init()
{
    SYSCTL_RCGCGPIO_R |= 0x0010;     // activate clock for Ports E
    while((SYSCTL_PRGPIO_R&0x10) != 0x10){};// wait for clocks to stabilize

    GPIO_PORTE_AFSEL_R &= ~0x1F;     // SRDY is PB2, J2.19
    GPIO_PORTE_AMSEL_R &= ~0x1F;     // disable analog functionality on P
    GPIO_PORTE_DEN_R |= 0x1F;        // enable digital I/O on PB2
    GPIO_PORTE_PDR_R |= 0x1F;        // enable pullup on PB2
    GPIO_PORTE_DIR_R &= ~0x1F;       // input on PB2
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFF00000);
}

typedef enum switch_type {
    MODE_SELECT = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
} switch_t;

uint8_t debounce(uint32_t val)
{
    uint8_t curr_state = val;
    uint32_t cnt = 0xFFFFFFFF;
    while(cnt > 0)
    {
        cnt--;
    }
    uint8_t new_curr_state = val;
    return (curr_state && new_curr_state);
}

uint8_t Switch_Mode()
{
    return debounce(MODE_PE0 & 0x01);
}

uint8_t Switch_Left()
{
    return debounce(LEFT_PE1 & 0x02);
}

uint8_t Switch_Right()
{
    return debounce(RIGHT_PE2 & 0x04);
}

uint8_t Switch_Up()
{
    return debounce(UP_PE3 & 0x08);
}

uint8_t Switch_Down()
{
    return debounce(DOWN_PE4 & 0x10);
}

uint8_t Switch_BitVector(switch_t sw)
{
    switch(sw)
    {
        case MODE_SELECT:
            return Switch_Mode();
        case LEFT:
            return Switch_Left();
        case RIGHT:
            return Switch_Right();
        case UP:
            return Switch_Up();
        case DOWN:
            return Switch_Down();
        default:
            return 0;
    }
}