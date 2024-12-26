#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "timer.h"
#include "dc_motor.h"
#include "ultrasonic_sensor.h"

unsigned long H, L;


void ServoMotor_Init(void) { // output from PC4, SysTick
	  volatile unsigned long delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC; // Port C clock enable (bit 2)
    delay = SYSCTL_RCGC2_R;

    GPIO_PORTC_AMSEL_R &= ~0x10; // Disable analog functionality on PC4
    GPIO_PORTC_PCTL_R &= ~0x000F0000; // Configure PC4 as GPIO
    GPIO_PORTC_DIR_R |= 0x10; // PC4 output (motor)
    GPIO_PORTC_DR8R_R |= 0x10; // enable 8 mA drive on PC4
    GPIO_PORTC_AFSEL_R &= ~0x10; // Disable alternate functions on PC4
    GPIO_PORTC_DEN_R |= 0x10;
    GPIO_PORTC_DATA_R &= ~0x10; // start Low
}

void Servo_Angle(unsigned long degree) { // total = 1600000(80M Hz 기준) --> 20ms
    H = 200000 - (80000 / 90) * degree;
    L = 1600000 - H;

    GPIO_PORTC_DATA_R |= 0x10; // PC4 High
    Timer1A_Wait(H);
    GPIO_PORTC_DATA_R &= ~0x10; // PC4 Low
    Timer1A_Wait(L);
}