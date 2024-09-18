#include <stdint.h>

/* Functions for Timer. */
void Timer_Init(void);

void Timer_Set(uint8_t, uint8_t, uint8_t);

void Timer_Start(void):

void Timer_Stop(void);

uint32_t Timer_TimeLeft(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);