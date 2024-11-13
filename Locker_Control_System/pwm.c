/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#include <avr/io.h>
#include "pwm.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	DDRB  = DDRB | (1<<PB3);
	TCNT0 = 0; // Set Timer Initial Value to 0

	switch(duty_cycle)
	{
	case 0:
		OCR0 = 0;
	break;
	case 25:
		OCR0 = 63;
	break;
	case 50:
		OCR0 = 127;
	break;
	case 75:
		OCR0 = 191;
	break;
	case 100:
		OCR0 = 255;
	break;
	}

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01) | (1<<CS00) ;
}
