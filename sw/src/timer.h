#include <stdint.h>

/* Functions for Timer. */
void Timer_Init(uint8_t h, uint8_t m, uint8_t s);

void Timer_SetAlarm(uint8_t, uint8_t, uint8_t);

void Timer_GetAlarm(uint8_t *, uint8_t *, uint8_t *);

void Timer_SetTime(uint8_t, uint8_t, uint8_t);

void Timer_GetTime(uint8_t *, uint8_t *, uint8_t *);

void Timer_Start(void);

uint8_t Timer_TriggerAlarm();

void set_time(uint8_t h, uint8_t m, uint8_t s);