
/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#include "pir.h"
#include "gpio.h"
#include "uart.h"
#include "util/delay.h"



void PIR_init(void)
{
	GPIO_setupPinDirection(PIR_PORT_ID,PIR_PIN_ID,PIN_INPUT);
}

uint8 PIR_getState(void)
{
	return GPIO_readPin(PIR_PORT_ID,PIR_PIN_ID);
}
