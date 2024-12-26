#include "tm4c123gh6pm.h"
#include "dc_motor.h"
#include "servo_motor.h"
#include "ultrasonic_sensor.h"
#include "timer.h"
#include "PLL.h"

#include <stdint.h>


void DisableInterrupts(void);
void EnableInterrupts(void);


// Global Variables
uint32_t left_degree = 30;
uint32_t middle_degree = 90;
uint32_t right_degree = 150;

int main(void) {
		PLL_Init();         // 클럭 초기화
		Timer0A_Init();     // Echo Timer
		Timer1A_Init();     // Trigger Timer
		SysTick_Init();     // SysTick 초기화
		DC_Motor_Init();    // DC 모터 초기화
		ServoMotor_Init();  // 서보 모터 초기화
		Sensor_Init();      // 초음파 센서 초기화

    while (1) {
		Servo_Angle(middle_degree); 
		Measure_Distance(middle_degree); 
		Timer1A_Wait_1us(10000);
			
		Servo_Angle(left_degree); 
		Measure_Distance(left_degree);
		Timer1A_Wait_1us(10000);
			
		Servo_Angle(middle_degree);
		Measure_Distance(middle_degree); 
		Timer1A_Wait_1us(10000);
			
		Servo_Angle(right_degree);     
		Measure_Distance(right_degree); 
		Timer1A_Wait_1us(10000);
			
		Change_duty_cycle();
    }
} 