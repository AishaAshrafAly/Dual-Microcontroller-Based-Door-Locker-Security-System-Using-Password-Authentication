/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#include "pir.h"
#include "timer.h"
#include "avr/io.h"
#include "util/delay.h"
#include "common_macros.h"
#include "std_types.h"
#include "led.h"
#include "buzzer.h"
#include "motor.h"
#include "eeprom.h"
#include "uart.h"

#define BAUD_RATE 9600  // UART communication baud rate

// Status flags for UART communication
#define UART_SUCCESS 1
#define UART_FAILURE 0
#define PASSWORD_MISMATCH 1
#define PASSWORD_MATCH 0

// Constants
#define TIMER_FINISH 1

uint16 g_timerCounter = 0;  // Global timer counter for motor control

/* Motor control callback function for stopping the motor */
void MotorControlCallback(void) {
    g_timerCounter++;

    if (g_timerCounter == 7324) {
        PORTA ^= (1 << 0);  // Toggle PORTA pin 0
        Motor_Rotate(Stop, 0);  // Stop motor rotation
        UART_sendByte(TIMER_FINISH);  // Send finish signal via UART
        Timer_deInit(TIMER2);  // De-initialize timer
    }
}

int main(void) {
    DDRA |= (1 << 0);  // Set PORTA pin 0 as output

    uint8 receivedValue = 0, unlockCommand;
    uint8 initialPassword[5], confirmPassword[5], inputPassword[5];
    uint8 isFirstAttempt = 0;
    uint8 i = 0, errorCount = 0, isError = 0, isPasswordIncorrect = 0;

    UART_ConfigType uartConfig = {eight_bit, Disabled, one_bit, BAUD_RATE};
    Timer_ConfigType timerConfig = {0, 0, TIMER2, F_CPU_64, NORMAL_mode};

    // Initialize modules
    PIR_init();
    Motor_Init();
    Buzzer_init();
    Timer_setCallBack(&MotorControlCallback, TIMER2);
    UART_init(&uartConfig);

    while (1) {
        if (isFirstAttempt == 0) {
            // Receive first-time password setup from user
            for (i = 0; i < 5; i++) {
                initialPassword[i] = UART_receiveByte();
            }

            // Receive password confirmation from user
            for (i = 0; i < 5; i++) {
                confirmPassword[i] = UART_receiveByte();
            }

            // Check if the two passwords match
            for (i = 0; i < 5; i++) {
                if (initialPassword[i] != confirmPassword[i]) {
                    isError = 1;
                    UART_sendByte(PASSWORD_MISMATCH);  // Notify mismatch
                    break;
                }
            }

            // Retry if passwords mismatch
            if (isError == 1) {
                isError = 0;
                continue;
            }
            UART_sendByte(PASSWORD_MATCH);  // Passwords matched

            // Save password in EEPROM
            for (i = 0; i < 5; i++) {
                EEPROM_writeByte((0x0300 + i), initialPassword[i]);
                _delay_ms(10);
            }

            // Receive input password to verify against saved one
            for (i = 0; i < 5; i++) {
                inputPassword[i] = UART_receiveByte();
            }

            // Verify input password with EEPROM data
            for (i = 0; i < 5; i++) {
                EEPROM_readByte((0x0300 + i), &receivedValue);
                if (inputPassword[i] != receivedValue) {
                    UART_sendByte(UART_FAILURE);
                    isPasswordIncorrect = 1;
                    break;
                }
            }

            if (isPasswordIncorrect == 1) {
                isPasswordIncorrect = 0;
                errorCount++;
                isFirstAttempt = 1;
                continue;
            }
            UART_sendByte(UART_SUCCESS);  // Password verified successfully

            // Check for unlock command
            unlockCommand = UART_receiveByte();

            if (unlockCommand == 1) {
                // Initialize timer and rotate motor clockwise
                Timer_init(&timerConfig);
                Motor_Rotate(CW, 100);

                // Wait for PIR sensor activation to open the door
                while (PIR_getState() == 0);
                UART_sendByte(OPEN_DOOR);

                // Wait for PIR sensor deactivation to close the door
                while (PIR_getState() == 1);
                UART_sendByte(LOCK_DOOR);

                // Rotate motor counterclockwise to lock the door
                Timer_init(&timerConfig);
                Motor_Rotate(CCW, 100);
                isFirstAttempt = 1;
            } else if (unlockCommand == 0) {
                continue;
            }
        } else {
            // Prompt user to re-enter password if not the first attempt
            for (i = 0; i < 5; i++) {
                inputPassword[i] = UART_receiveByte();
            }

            // Check entered password with saved password in EEPROM
            for (uint8 j = 0; j < 5; j++) {
                EEPROM_readByte((0x0300 + j), &receivedValue);
                if (inputPassword[j] != receivedValue) {
                    UART_sendByte(UART_FAILURE);
                    isPasswordIncorrect = 1;
                    break;
                }
            }

            if (isPasswordIncorrect == 1) {
                errorCount++;
                if (errorCount == 3) {
                    // Trigger alarm on three incorrect attempts
                    if (UART_receiveByte() == 1) {
                        Buzzer_on();
                        while (UART_receiveByte() != 0);
                        Buzzer_off();
                    }
                    errorCount = 0;
                }
                isPasswordIncorrect = 0;
                continue;
            }
            errorCount = 0;
            UART_sendByte(UART_SUCCESS);

            // Verify unlock command again
            unlockCommand = UART_receiveByte();
            if (unlockCommand == 1) {
                Timer_init(&timerConfig);
                Motor_Rotate(CW, 100);

                while (PIR_getState() == 0);
                UART_sendByte(OPEN_DOOR);

                while (PIR_getState() == 1);
                UART_sendByte(LOCK_DOOR);

                Timer_init(&timerConfig);
                Motor_Rotate(CCW, 100);
            } else if (unlockCommand == 0) {
                isFirstAttempt = 0;
                continue;
            }
        }
    }
}
