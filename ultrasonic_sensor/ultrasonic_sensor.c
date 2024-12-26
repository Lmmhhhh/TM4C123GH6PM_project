#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "timer.h"
#include "dc_motor.h"
#include "servo_motor.h" 

extern uint32_t left_degree;
extern uint32_t middle_degree;
extern uint32_t right_degree;

uint32_t distance_arr[3]; // [0] : left, [1] : middle, [2] : right



void Sensor_Init(void) {volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x02; // enable clock to PORTB
	  delay = SYSCTL_RCGC2_R;

    GPIO_PORTB_DIR_R &= ~0x40; // make PB6 an input pin
    GPIO_PORTB_DEN_R |= 0x40; // make PB6 as digital pin
    GPIO_PORTB_AFSEL_R |= 0x40; // use PB6 alternate function
    GPIO_PORTB_PCTL_R &= ~0x0F000000; // configure PB6 for T0CCP0
    GPIO_PORTB_PCTL_R |= 0x07000000;

    SYSCTL_RCGC2_R |= 0x10; // Port E clock
    delay = SYSCTL_RCGC2_R;

    GPIO_PORTE_DIR_R |= 0x02; // PE1 output
    GPIO_PORTE_AFSEL_R &= ~0x02; // not alternative
    GPIO_PORTE_AMSEL_R &= ~0x02; // no analog
    GPIO_PORTE_DEN_R |= 0x02; // enable PE1
    GPIO_PORTE_DATA_R &= ~0x02; // Start Trigger Low
}

void Trigger_Pulse(void) { // Send Trigger Pulse
    Timer1A_Wait_1us(10000); // Wait 10ms
    GPIO_PORTE_DATA_R |= 0x02; // Start Trigger High
    Timer1A_Wait_1us(10); // Wait 10 us
    GPIO_PORTE_DATA_R &= ~0x02; // Set Trigger Low
}


uint32_t Calculate_Distance(void) {
    uint32_t risingEdge, fallingEdge, period, distance;

    TIMER0_ICR_R = 0x04; // clear timer0A capture flag
    while ((TIMER0_RIS_R & 0x04) == 0); // wait till captured
    risingEdge = TIMER0_TAR_R; // save the timestamp

    TIMER0_ICR_R = 0x04; // clear timer0A capture flag
    while ((TIMER0_RIS_R & 0x04) == 0); // wait till captured
    fallingEdge = TIMER0_TAR_R; // save the timestamp

    if (fallingEdge > risingEdge) { // time difference
        period = fallingEdge - risingEdge;
    } else {
        period = (0xFFFF - risingEdge) + fallingEdge; // 타이머가 오버플로우한 경우 처리
    }

    distance = (uint32_t)((period * 0.034 / 200) * 19 / 15 + 0.5); // Calculate Distance in cm, 타이머 1MHz

    return (distance > 35) ? 35 : distance; // 최대 35cm로 제한
}


void Measure_Distance(unsigned long degree){
	uint32_t distance,i;
	uint32_t min_distance = 100;
	
	for (i = 0; i < 10; i++){
		Trigger_Pulse();
		distance = Calculate_Distance(); 
		if (distance < min_distance) {
			min_distance = distance;
		}
	}
	
	if (degree == left_degree){
		distance_arr[0] = distance;
	}else if (degree == right_degree){
		distance_arr[2] = distance;
	} else if (degree == middle_degree){
		distance_arr[1] = distance;
	}
}