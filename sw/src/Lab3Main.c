// Lab3Main.c
// Runs on LM4F120/TM4C123
// Uses ST7735.c to display the clock
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD
// Mark McDermott, Daniel Valvano and Jonathan Valvano
// Lab solution, do not post
// August 3,2024

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2024

 Copyright 2024 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// Specify your hardware connections, feel free to change
// PD0 is squarewave output to speaker
// PE0 is mode select
// PE1 is left
// PE2 is right 
// PE3 is up
// PE4 is down
// if alarm is sounding, any button will quiet the alarm

#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "switch.h"
#include "timer.h"
#include "speaker.h"
#include "lcd.h"

#define NUM_CHILD_MENUS 4
//#include "Lab3.h"
// ---------- Prototypes   -------------------------
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

uint8_t main_menu_sm(unsigned int *selector_ptr)
{
  /* Load Main Menu Image. */
  switch(*selector_ptr)
  {
    case 0:
      /* Set Circle to Analog Clk*/
			break;
    case 1:
      /* Set Circle to Digital Clk*/
			break;
    case 2:
      /* Set Circle to Set Time. */
			break;
    case 3:
      /* Set Circle to Set Alarm. */
			break;
    default:
      /* Set Circle to Analog Clk. */
			break;
  }

  if(Switch_Mode())
  {
    return *selector_ptr + 1;
  }
  else if(Switch_Down())
  {
    *selector_ptr = (*selector_ptr + 1) % NUM_CHILD_MENUS;
  }
  else if(Switch_Up())
  {
    *selector_ptr = (*selector_ptr - 1) % NUM_CHILD_MENUS;
  }
  return 0;
}

uint8_t analog_clock_sm(uint8_t h, uint8_t m, uint8_t s)
{
  /* Load Analog Clock Image. */
  return 1;
}

uint8_t digital_clock_sm(uint8_t h, uint8_t m, uint8_t s)
{
  /* Load Digital Clock Image. */
  return 2;
}

uint8_t alarm_sm(uint8_t *alarm_mode)
{
  /* Load Alarm Image. */
  return 3;
}

uint8_t time_sm(uint8_t *time_mode)
{
  /* Load Time Image. */
  return 4;
}

int main(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz);    // bus clock at 80 MHz
  Timer_Init();
  Switch_Init();
  Speaker_Init();
  EnableInterrupts();
	init_lcd(2,0,30,0, 0);

  unsigned int current_mode = 0;
  unsigned int selector_ptr = 0;
	uint8_t alarm_mode = 0;
  uint8_t time_mode = 0;

  uint8_t curr_h = 0;
  uint8_t curr_m = 0;
  uint8_t curr_s = 0;
  while(1){
      // write this
      uint32_t time_left = Timer_TimeLeft(&curr_h, &curr_m, &curr_s);

      switch(current_mode)
      {
        case 0:
          current_mode = main_menu_sm(&selector_ptr);
          break;
        case 1:
          current_mode = time_sm(&time_mode);
          break;
        case 2:
          current_mode = alarm_sm(&alarm_mode);
          break;
        case 3:
          current_mode = digital_clock_sm(curr_h, curr_m, curr_s);
          break;
        case 4:
          current_mode = analog_clock_sm(curr_h, curr_m, curr_s);
          break;
      }

      if(time_left == 0 && alarm_mode)
      {
        if(Switch_Up())
        {
          Speaker_Enable(0);
        }
        else{
          Speaker_Enable(1);
        }
      }
      else
      {
        Speaker_Enable(0);
      }
			
			

  }
}

