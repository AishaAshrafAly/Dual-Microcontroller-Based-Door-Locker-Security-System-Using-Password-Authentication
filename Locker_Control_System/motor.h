

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

#define MOTOR_PORT_ID PORTD_ID
#define MOTOR_IN1_PIN_ID PIN6_ID
#define MOTOR_IN2_PIN_ID PIN7_ID

typedef enum
{
	CW, CCW, Stop
}Motor_State;

void Motor_Init(void);
void Motor_Rotate(Motor_State state, uint8 speed);

#endif /* MOTOR_H_ */
