/* Module to interface with speaker. Generate sounds. */

#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "speaker.h"

#define PB1 (*((volatile uint32_t *)0x40005008))
#define HIGH 1
#define LOW 0

/* Period should be 40000. */
#define BUS_FREQ 80000000
#define SPEAKER_FREQ 2000
#define PERIOD BUS_FREQ / SPEAKER_FREQ

void Speaker_Init()
{
    /* Initialize PB1 for wave signal generation for. */
    SYSCTL_RCGCGPIO_R |= 0x0002;     // activate clock for Port B
    while((SYSCTL_PRGPIO_R&0x02) != 0x02){};// wait for clocks to stabilize

    GPIO_PORTB_AFSEL_R &= ~0x02;     // SRDY is PB1, J2.19
    GPIO_PORTB_AMSEL_R &= ~0x02;     // disable analog functionality on PB1
    GPIO_PORTB_DEN_R |= 0x02;        // enable digital I/O on PB1
    GPIO_PORTB_PUR_R |= 0x02;        // enable pullup on PB1
    GPIO_PORTB_DIR_R |= 0x02;       // output on PB1
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF0F);
}

void SysTick_Handler()
{
    PB1 ^= 0x02;
}

void Speaker_Enable(uint8_t on)
{
    uint8_t previously_on = NVIC_ST_CTRL_R == 0x07;
    /* Detect edge when enabling speaker (off -> on). */
    if(on & !previously_on)
    {
        NVIC_ST_CTRL_R = 0;             // Disable Systick.
        NVIC_ST_RELOAD_R = PERIOD - 1;  // reload value
        NVIC_ST_CURRENT_R = 0;		    // any write clears it
        NVIC_SYS_PRI3_R	= (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x40000000; //priority 2															
        NVIC_ST_CTRL_R	=	0x00000007;//	enable	with	core	clock	and	interrupts
    }
    /* If off, disable SysTick. */
    else if(!on)
    {
        PB1 = 0x00;
        NVIC_ST_CTRL_R = 0;
    }

    /*If remaining on, do nothing. */
}