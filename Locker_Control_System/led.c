/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */
#include "led.h"
#include "gpio.h"



void LEDS_init(void)
{
	GPIO_setupPinDirection(LEDS_PORT_ID,RED_led,PIN_OUTPUT);
	GPIO_setupPinDirection(LEDS_PORT_ID,GREEN_led,PIN_OUTPUT);
	GPIO_setupPinDirection(LEDS_PORT_ID,BLUE_led,PIN_OUTPUT);

	GPIO_writePin(LEDS_PORT_ID,RED_led,LOGIC_LOW);
	GPIO_writePin(LEDS_PORT_ID,GREEN_led,LOGIC_LOW);
	GPIO_writePin(LEDS_PORT_ID,BLUE_led,LOGIC_LOW);
}

void LED_on(LED_ID id)
{
	switch(id)
	{
	case RED_led:
		GPIO_writePin(LEDS_PORT_ID,RED_led,LOGIC_HIGH);
	break;
	case GREEN_led:
		GPIO_writePin(LEDS_PORT_ID,GREEN_led,LOGIC_HIGH);
	break;
	case BLUE_led:
		GPIO_writePin(LEDS_PORT_ID,BLUE_led,LOGIC_HIGH);
	break;
	}
}

void LED_off(LED_ID id)
{
	switch(id)
	{
	case RED_led:
		GPIO_writePin(LEDS_PORT_ID,RED_led,LOGIC_LOW);
	break;
	case GREEN_led:
		GPIO_writePin(LEDS_PORT_ID,GREEN_led,LOGIC_LOW);
	break;
	case BLUE_led:
		GPIO_writePin(LEDS_PORT_ID,BLUE_led,LOGIC_LOW);
	break;
	}
}
