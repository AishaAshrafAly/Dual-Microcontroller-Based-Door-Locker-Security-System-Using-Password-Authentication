/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include "uart.h"
#include "util/delay.h"
#include "timer.h"

#define BAUD_RATE 9600

#define UN_LOCK   1
#define LOCK      0

#define BUZZER_ON  1
#define BUZZER_OFF 0

uint16 count=0;

uint8 first_time = 0 ;
void lock_control(void)
{
	count++;
	if(count==29297)
	{
		UART_sendByte(BUZZER_OFF);
		Timer_deInit(TIMER2);
		first_time=1;
	}
}
int main(void)
{
	UART_ConfigType uart_setting={eight_bit,Disabled,one_bit,BAUD_RATE};

	Timer_ConfigType timer_setting={0,0,TIMER2,F_CPU_64,NORMAL_mode};

	uint8 pass_arr[5] , counter = 0 , error=0 , choice=0 , state=0 , motion=0;

	uint8 press_button=0 , door_status = 0;

	uint8 wrong = 0 ;

	LCD_init();

	LCD_displayString("Pass, Please :)");
	LCD_moveCursor(1,0);

	Timer_setCallBack(&lock_control,TIMER2);

	UART_init(&uart_setting);

	while(1)
	{
		if(first_time==0)
		{
			/* first create password */
			for(counter=0;counter<5;counter++)
			{
				/* insert password number by number */
				pass_arr[counter] = KEYPAD_getPressedKey();
				/* send the pass to Control */
				UART_sendByte(pass_arr[counter]);
				/* display pass as a (*) */
				LCD_displayCharacter('*');
			}
			/* enter '=' button to continue */
			press_button = KEYPAD_getPressedKey();
			while(press_button!='=');

			/* clear screen */
			LCD_clearScreen();

			LCD_displayString("Retype");
			LCD_moveCursor(1,0);
			LCD_displayString("Password :) ");

			/* to confirm password */
			for(counter=0;counter<5;counter++)
			{
				/* again insert pass number by number */
				pass_arr[counter] = KEYPAD_getPressedKey();
				/* send it to the control */
				UART_sendByte(pass_arr[counter]);
				/* display it as a (*) */
				LCD_displayCharacter('*');
			}
			/* enter '=' button to continue */
			press_button = KEYPAD_getPressedKey();
			while(press_button!='=');

			error = UART_recieveByte();

			if( error == 1 )
			{
				LCD_clearScreen();
				LCD_displayString("Pass, Please :)");
				LCD_moveCursor(1,0);

				error=0;
				/* return to start of while loop */
				continue;
			}
			else
			{
				LCD_clearScreen();
				LCD_displayString("+ : Open Door ");
				LCD_moveCursor(1,0);
				LCD_displayString("- : Change Pass ");

				/* ask user to choose open door or change pass */
				choice = KEYPAD_getPressedKey();
				if(choice=='+')
				{
					LCD_clearScreen();
					LCD_displayString("Pass, Please :)");
					LCD_moveCursor(1,0);
					for(counter=0;counter<5;counter++)
					{
						pass_arr[counter] = KEYPAD_getPressedKey();
						UART_sendByte(pass_arr[counter]);
						LCD_displayCharacter('*');
					}
					/* enter button to continue */
					press_button = KEYPAD_getPressedKey();
					while(press_button!='=');


					/* after entering the pass we will wait to know if it as saved in EEprom or not */
					state = UART_recieveByte();

					if(state==1)
					{
						/* inform control that we are in open door state */
						UART_sendByte(UN_LOCK);

						LCD_clearScreen();
						LCD_moveCursor(0,4);
						LCD_displayString("Door is");
						LCD_moveCursor(1,3);
						LCD_displayString("Unlocking");
					}

					else if(state==0)
					{
						do
						{
							wrong++;
							if(wrong==3)
							{
								LCD_clearScreen();
								LCD_displayString("Wait");
								UART_sendByte(BUZZER_ON);
								Timer_init(&timer_setting);
								wrong=0;
								while(first_time==0);
								break;
							}
							else
							{
								LCD_clearScreen();
								LCD_displayString("Password ");
								LCD_moveCursor(1,0);
								LCD_displayString("again :) ");
								for(counter=0;counter<5;counter++)
								{
									pass_arr[counter] = KEYPAD_getPressedKey();
									UART_sendByte(pass_arr[counter]);
									LCD_displayCharacter('*');
								}
								/* enter button to continue */
								press_button = KEYPAD_getPressedKey();
								while(press_button!='=');


								/* after entering the pass we will wait to know if it as saved in EEprom or not */
								state = UART_recieveByte();
							}
						}while(state==0);
						if(state==0)
						{
							continue;
						}
						else
						{
							/* inform control that we are in open door state */
							UART_sendByte(UN_LOCK);

							LCD_clearScreen();
							LCD_moveCursor(0,4);
							LCD_displayString("Door is");
							LCD_moveCursor(1,3);
							LCD_displayString("Unlocking");
							wrong=0;
						}
					}

					/* wait for motor to rotate completely */
					while(UART_recieveByte()==0);

					/* motion detected by PIR sensor */
					motion = UART_recieveByte();

					LCD_clearScreen();
					if(motion==1)
					{
						LCD_displayString("Wait for people");
						LCD_moveCursor(1,3);
						LCD_displayString("To Enter");
					}

					while(UART_recieveByte()!=0);

					LCD_clearScreen();
					LCD_moveCursor(0,4);
					LCD_displayString("Door is");
					LCD_moveCursor(1,4);
					LCD_displayString("locking");

					/* wait for motor to rotate completely */
					while(door_status == 0)
					{
						door_status = UART_recieveByte();
					}
					door_status = 0;

					first_time=1;
				}
				else if(choice=='-')
				{
					LCD_clearScreen();
					LCD_displayString("Pass, Please :)");
					LCD_moveCursor(1,0);
					for(counter=0;counter<5;counter++)
					{
						pass_arr[counter] = KEYPAD_getPressedKey();
						UART_sendByte(pass_arr[counter]);
						LCD_displayCharacter('*');
					}
					/* enter button to continue */
					press_button = KEYPAD_getPressedKey();
					while(press_button!='=');

					state = UART_recieveByte();
					if(state==1)
					{
						/* inform control that we are in chg_pass state */
						UART_sendByte(LOCK);

						LCD_clearScreen();
						LCD_displayString("Enter new");
						LCD_moveCursor(1,0);
						LCD_displayString("pass: ");

						continue;
					}
					else if(state==0)
					{
						do
						{
							wrong++;
							if(wrong==3)
							{
								LCD_clearScreen();
								LCD_displayString("Wait");
								UART_sendByte(BUZZER_ON);
								Timer_init(&timer_setting);
								wrong=0;
								while(first_time==0);
								break;
							}
							else
							{
								LCD_clearScreen();
								LCD_displayString("Password ");
								LCD_moveCursor(1,0);
								LCD_displayString("again :) ");
								for(counter=0;counter<5;counter++)
								{
									pass_arr[counter] = KEYPAD_getPressedKey();
									UART_sendByte(pass_arr[counter]);
									LCD_displayCharacter('*');
								}
								/* enter button to continue */
								press_button = KEYPAD_getPressedKey();
								while(press_button!='=');


								/* after entering the pass we will wait to know if it as saved in EEprom or not */
								state = UART_recieveByte();
							}
						}while(state==0);
						if(state==0)
						{
							continue;
						}
						else
						{

							/* inform control that we are in chg_pass state */
							UART_sendByte(LOCK);

							LCD_clearScreen();
							LCD_displayString("Enter new");
							LCD_moveCursor(1,0);
							LCD_displayString("pass: ");
							wrong=0;
							continue;

						}

					}

				}
			}

		}
		else
		{
			LCD_clearScreen();
			LCD_displayString("+ : Open Door ");
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass ");

			choice = KEYPAD_getPressedKey();
			if(choice=='+')
			{
				LCD_clearScreen();
				LCD_displayString("Pass, Please :)");
				LCD_moveCursor(1,0);
				for(counter=0;counter<5;counter++)
				{
					pass_arr[counter] = KEYPAD_getPressedKey();
					UART_sendByte(pass_arr[counter]);
					LCD_displayCharacter('*');
				}
				/* enter button to continue */
				press_button = KEYPAD_getPressedKey();
				while(press_button!='=');


				/* after entering the pass we will wait to know if it as saved in EEprom or not */
				state = UART_recieveByte();

				if(state==1)
				{
					/* inform control that we are in open door state */
					UART_sendByte(UN_LOCK);

					LCD_clearScreen();
					LCD_moveCursor(0,4);
					LCD_displayString("Door is");
					LCD_moveCursor(1,3);
					LCD_displayString("Unlocking");
				}
				/* !!! */
				else if(state==0)
				{
					do
					{
						wrong++;
						if(wrong==3)
						{
							LCD_clearScreen();
							LCD_displayString("Wait");
							UART_sendByte(BUZZER_ON);
							Timer_init(&timer_setting);
							wrong=0;
							first_time=0;
							while(first_time==0);
							break;
						}
						else
						{
							LCD_clearScreen();
							LCD_displayString("Password ");
							LCD_moveCursor(1,0);
							LCD_displayString("again :) ");
							for(counter=0;counter<5;counter++)
							{
								pass_arr[counter] = KEYPAD_getPressedKey();
								UART_sendByte(pass_arr[counter]);
								LCD_displayCharacter('*');
							}
							/* enter button to continue */
							press_button = KEYPAD_getPressedKey();
							while(press_button!='=');


							/* after entering the pass we will wait to know if it as saved in EEprom or not */
							state = UART_recieveByte();
						}
					}while(state==0);
					if(state==0)
					{
						continue;
					}
					else
					{
						/* inform control that we are in open door state */
						UART_sendByte(UN_LOCK);

						LCD_clearScreen();
						LCD_moveCursor(0,4);
						LCD_displayString("Door is");
						LCD_moveCursor(1,3);
						LCD_displayString("Unlocking");
						wrong=0;
					}
				}

				/* wait for motor to rotate completely */
				while(UART_recieveByte()==0);

				/* motion detected by PIR sensor */
				motion = UART_recieveByte();

				LCD_clearScreen();
				if(motion==1)
				{
					LCD_displayString("Wait for people");
					LCD_moveCursor(1,3);
					LCD_displayString("To Enter");
				}

				/* motion detected by PIR sensor */
				motion = UART_recieveByte();

				if(motion==0)
				LCD_clearScreen();
				LCD_moveCursor(0,4);
				LCD_displayString("Door is");
				LCD_moveCursor(1,4);
				LCD_displayString("locking");

				/* wait for motor to rotate completely */
				while(door_status == 0)
				{
					door_status = UART_recieveByte();
				}
				door_status = 0;
			}
			else if(choice=='-')
			{
				LCD_clearScreen();
				LCD_displayString("Pass, Please :)");
				LCD_moveCursor(1,0);
				for(counter=0;counter<5;counter++)
				{
					pass_arr[counter] = KEYPAD_getPressedKey();
					UART_sendByte(pass_arr[counter]);
					LCD_displayCharacter('*');
				}
				/* enter button to continue */
				press_button = KEYPAD_getPressedKey();
				while(press_button!='=');

				state = UART_recieveByte();
				if(state==1)
				{
					/* to inform control that we are in chg_pass state */
					UART_sendByte(LOCK);

					LCD_clearScreen();
					LCD_displayString("Enter new");
					LCD_moveCursor(1,0);
					LCD_displayString("pass: ");

					first_time=0;
					continue;
				}
				else if(state==0)
				{
					do
					{
						wrong++;
						if(wrong==3)
						{
							LCD_clearScreen();
							LCD_displayString("Wait");
							UART_sendByte(BUZZER_ON);
							Timer_init(&timer_setting);
							wrong=0;
							first_time=0;
							while(first_time==0);
							break;
						}
						else
						{
							LCD_clearScreen();
							LCD_displayString("Password ");
							LCD_moveCursor(1,0);
							LCD_displayString("again :) ");
							for(counter=0;counter<5;counter++)
							{
								pass_arr[counter] = KEYPAD_getPressedKey();
								UART_sendByte(pass_arr[counter]);
								LCD_displayCharacter('*');
							}
							/* enter button to continue */
							press_button = KEYPAD_getPressedKey();
							while(press_button!='=');


							/* after entering the pass we will wait to know if it as saved in EEprom or not */
							state = UART_recieveByte();
						}
					}while(state==0);
					if(state==0)
					{
						continue;
					}
					else
					{

						/* inform control that we are in chg_pass state */
						UART_sendByte(LOCK);

						LCD_clearScreen();
						LCD_displayString("Enter new");
						LCD_moveCursor(1,0);
						LCD_displayString("pass: ");
						wrong=0;
						first_time=0;
						continue;

					}
				}

			}
		}
	}
}



