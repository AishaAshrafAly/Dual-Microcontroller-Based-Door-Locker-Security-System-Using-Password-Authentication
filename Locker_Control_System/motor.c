
#include "motor.h"
#include "pwm.h"
#include "gpio.h"

void Motor_Init(void)
{
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID,PIN_OUTPUT);

	GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
}

void Motor_Rotate(Motor_State state, uint8 speed)
{
	PWM_Timer0_Start(speed);
	switch(state)
	{
	case CW :
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
	break;
	case CCW :
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_HIGH);
	break;
	case Stop :
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
	break;
	}


}
