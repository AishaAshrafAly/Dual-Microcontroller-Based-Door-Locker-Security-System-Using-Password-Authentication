/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#ifndef LED_H_
#define LED_H_

#define LEDS_PORT_ID PORTB_ID

typedef enum
{
	RED_led=5 , GREEN_led , BLUE_led
}LED_ID;

void LEDS_init(void);

void LED_on(LED_ID id);

void LED_off(LED_ID id);

#endif /* LED_H_ */
