#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void Timer0A_Init(void);
void Timer1A_Init(void);
void Timer1A_Wait(unsigned long period);
void Timer1A_Wait_1us(int us);
void SysTick_Init(void);

#endif // TIMER_H