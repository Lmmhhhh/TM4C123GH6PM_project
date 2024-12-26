#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "timer.h"
#include "ultrasonic_sensor.h"
#include "servo_motor.h"

#define base_duty 200
#define GAIN 2.5

extern uint32_t distance_arr[3];

uint32_t left_duty = base_duty;
uint32_t right_duty = base_duty;
uint32_t count_left = 0;
uint32_t count_right = 0;


void DC_Motor_Init(void) {volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000001; // port A CLOCK 활성화
    delay	= SYSCTL_RCGC2_R;

	
    GPIO_PORTA_AMSEL_R &= ~0x50; // PA4, 6 analog 비활성화
    GPIO_PORTA_PCTL_R &= ~0x0F0F0000; // configure PA4,6 as GPIO
    GPIO_PORTA_DIR_R |= 0x50; // PA4, 6 out으로 설정
    GPIO_PORTA_DR8R_R |= 0x50; // enable 8 mA drive on PA4, 6
    GPIO_PORTA_AFSEL_R &= ~0x50; // alt funct on PA4, 6 비활성화
    GPIO_PORTA_DEN_R |= 0x50; // PA4, 6 디지털 핀으로 설정
    GPIO_PORTA_DATA_R &= ~0x50; // 초기값 0
}

void Left_DC_Motor_Control(uint32_t duty) {
    if (count_left >= 400) { // 1ms 주기
        count_left = 0;
    }

    if (count_left < duty) {
        GPIO_PORTA_DATA_R |= 0x40; // 모터 핀 ON
    } else {
        GPIO_PORTA_DATA_R &= ~0x40; // 모터 핀 OFF
    }
}

void Right_DC_Motor_Control(uint32_t duty) {
    if (count_right >= 400) { // 1ms 주기
        count_right = 0;
    }

    if (count_right < duty) {
        GPIO_PORTA_DATA_R |= 0x10; // 모터 핀 ON
    } else {
        GPIO_PORTA_DATA_R &= ~0x10; // 모터 핀 OFF
    }
}

void Change_duty_cycle (void) {
	uint32_t left_D, middle_D, right_D;   // 좌, 중앙, 우 방향의 거리 저장 변수
	signed long diff, diff_duty;              // 좌/우 거리 차이와 이를 기반으로 한 duty 조정 값
	
	left_D = distance_arr[0];               // 좌측 거리값 저장
	middle_D = distance_arr[1];         // 중앙 거리값 저장
	right_D = distance_arr[2];            // 우측 거리값 저장
	
	diff = left_D - right_D;    // 좌우 거리 차이 계산
	
	if (middle_D <= 31) { // 장애물이 가까운 경우 (중앙 거리 30cm 이하)
			if (diff > 0) {  // 좌측 장애물이 더 멀 경우
				left_duty = base_duty;               // 좌측 바퀴는 기본 duty
				right_duty = base_duty + 130;   // 우측 바퀴는 속도 증가
			} else if (diff < 0) { // 우측 장애물이 더 멀 경우
				left_duty = base_duty + 130;     // 좌측 바퀴는 속도 증가
				right_duty = base_duty;           // 우측 바퀴는 기본 duty
			} else {  // 좌우 거리가 같은 경우
				left_duty = base_duty;             // 양 바퀴 기본 duty
				right_duty = base_duty;
			}
	} else {  // 장애물이 멀리 있는 경우 (중앙 거리 30cm 초과)
		if ((left_D > 20) & (right_D > 20)) { // 양측 장애물이 충분히 멀 경우
			left_duty = base_duty + 10;  
			right_duty = base_duty + 10;
		} else {
			diff_duty = (signed long)(diff * GAIN); // 좌우 거리 차이를 기반으로 duty 조정
			
			if (diff_duty < -70) { // duty 값이 과도하게 낮아지는 것을 방지
				diff_duty = -70;
			} else if (diff_duty > 70) {  // duty 값이 과도하게 높아지는 것을 방지
				diff_duty = 70;
			}
	
			left_duty = base_duty - diff_duty; // 좌측 바퀴의 duty 조정
			right_duty = base_duty;              // 우측 바퀴는 기본 duty 유지
			}
    }
 }