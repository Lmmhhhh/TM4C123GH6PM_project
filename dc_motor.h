#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <stdint.h>

void DC_Motor_Init(void);
void Left_DC_Motor_Control(uint32_t duty);
void Right_DC_Motor_Control(uint32_t duty);


#endif // DC_MOTOR_H