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
//#include "switch.h"
//#include "timer.h"
//#include "speaker.h"
#include "lcd.c"

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
    case 1:
      /* Set Circle to Digital Clk*/
    case 2:
      /* Set Circle to Set Time. */
    case 3:
      /* Set Circle to Set Alarm. */
    default:
      /* Set Circle to Analog Clk. */
  }

  /*if(Switch_Mode())
  {
    return *selector_ptr + 1;
  }
  else if(Switch_Down())
  {
    *selector_ptr = (*selector_ptr + 1) % NUM_CHILD_MENUS;
  }
  else if(Switch_Down())
  {
    *selector_ptr = (*selector_ptr - 1) % NUM_CHILD_MENUS;
  } */
  return 0;
}

uint8_t analog_clock_sm()
{
  /* Load Analog Clock Image. */
  return 1;
}

uint8_t digital_clock_sm()
{
  /* Load Digital Clock Image. */
  return 2;
}

uint8_t alarm_sm()
{
  /* Load Alarm Image. */
  return 3;
}

uint8_t time_sm()
{
  /* Load Time Image. */
  return 4;
}

int main(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz);    // bus clock at 80 MHz
  //Timer_Init();
  //Switch_Init();
  //Speaker_Init();
  EnableInterrupts();
	init_lcd(9,41,0,0, 0);

  unsigned int current_mode = 0;
  unsigned int selector_ptr = 0;
  while(1){
      // write this
      switch(current_mode)
      {
        case 0:
          current_mode = main_menu_sm(&selector_ptr);
          break;
        case 1:
          current_mode = time_sm();
          break;
        case 2:
          current_mode = alarm_sm();
          break;
        case 3:
          current_mode = digital_clock_sm();
          break;
        case 4:
          current_mode = analog_clock_sm();
          break;
      }

  }
}

