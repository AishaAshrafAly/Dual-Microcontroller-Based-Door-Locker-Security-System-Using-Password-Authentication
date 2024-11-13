/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_PORT_ID PORTC_ID
#define BUZZER_PIN_ID   PIN7_ID

void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */
