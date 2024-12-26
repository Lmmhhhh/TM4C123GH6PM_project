#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <stdint.h>

void Sensor_Init(void); 
void Trigger_Pulse(void);
uint32_t Calculate_Distance(void);
void Measure_Distance(unsigned long degree);
void Change_duty_cycle(void);

#endif // ULTRASONIC_SENSOR_H