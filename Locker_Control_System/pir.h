/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#ifndef PIR_H_
#define PIRH_
#include "gpio.h"
#include "std_types.h"

#define PIR_PORT_ID    PORTC_ID
#define PIR_PIN_ID     PIN2_ID

#define LOCK_DOOR   0
#define OPEN_DOOR   1

void PIR_init(void);
uint8 PIR_getState(void);

#endif /* PIR_H_ */
