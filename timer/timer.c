#include "tm4c123gh6pm.h"
#include "dc_motor.h"
#include <stdint.h>
#include "PLL.h"
#include "servo_motor.h"
#include "ultrasonic_sensor.h" 

unsigned long delay;

void Timer0A_Init(void) {  // initialize echo timer
    SYSCTL_RCGCTIMER_R |= 0x01;          // enable clock to Timer0 
    delay	= SYSCTL_RCGCTIMER_R;

    TIMER0_CTL_R &= ~0x01;               // disable
    TIMER0_CFG_R = 0x04;                 // 16-bit timer mode 
    TIMER0_TAMR_R = 0x17;                // up-count, edge-time, capture mode 
    TIMER0_CTL_R |= 0x0C;                // capture the both edges to start
    TIMER0_CTL_R |= 0x01;                // enable
}

void Timer1A_Init(void) {  // initialize trigger pulse timer
    SYSCTL_RCGCTIMER_R |= 0x02;             // enable clock to Timer1 
    delay = SYSCTL_RCGCTIMER_R;

    TIMER1_CTL_R = 0x00;                    // disable 
    TIMER1_CFG_R = 0x00;                    // 32-bit option 
    TIMER1_TAMR_R = 0x01;                   // down-counter, one-shot mode 
}

void Timer1A_Wait(unsigned long period) { 
    TIMER1_TAILR_R = period - 1;            // Timer A interval load value register
    TIMER1_ICR_R = 0x01;                    // clear the TimerA timeout flag
    TIMER1_CTL_R |= 0x01;                   // enable Timer A after initialization
    while ((TIMER1_RIS_R & 0x01) == 0);     // wait for TimerA timeout flag to set
}

void Timer1A_Wait_1us(int us) { 
    TIMER1_TAILR_R = 80 * us - 1;           // Timer A interval load value register
    TIMER1_ICR_R = 0x01;                    // clear the TimerA timeout flag
    TIMER1_CTL_R |= 0x01;                   // enable Timer A after initialization
    while ((TIMER1_RIS_R & 0x01) == 0);     // wait for TimerA timeout flag to set
}

void SysTick_Init(void) { 
    NVIC_ST_CTRL_R = 0;                     // SysTick 비활성화
    NVIC_ST_RELOAD_R = 200 - 1;             // Reload 값 설정 ---> 2.5us
    NVIC_ST_CURRENT_R = 0;                  // 현재 값 초기화
    NVIC_ST_CTRL_R = 0x07;                  // SysTick 활성화, 인터럽트 사용
}

void SysTick_Handler(void) { 
    extern uint32_t count_left, count_right;
    extern uint32_t left_duty, right_duty;

    count_left++; 
    count_right++;
    Left_DC_Motor_Control(left_duty);
    Right_DC_Motor_Control(right_duty);
}