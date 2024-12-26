#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <stdint.h>

void ServoMotor_Init(void);
void Servo_Angle(unsigned long degree);

#endif // SERVO_MOTOR_H