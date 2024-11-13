/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */

#include "timer.h"
#include "common_macros.h"
#include <avr/interrupt.h>
#include "avr/io.h"


// External counter variable for the timer (used elsewhere in the program)
volatile extern uint16 count;
// Pointer to store callback function for timer interrupts
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

// Timer 0 Overflow Interrupt Service Routine
ISR(TIMER0_OVF_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

// Timer 0 Compare Match Interrupt Service Routine
ISR(TIMER0_COMP_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

// Timer 1 Overflow Interrupt Service Routine
ISR(TIMER1_OVF_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

// Timer 1 Compare Match Interrupt Service Routine
ISR(TIMER1_COMPA_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

// Timer 2 Overflow Interrupt Service Routine
ISR(TIMER2_OVF_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

// Timer 2 Compare Match Interrupt Service Routine
ISR(TIMER2_COMP_vect)
{
    if (g_callBackPtr != NULL_PTR)
    {
        // Execute the callback function if it’s set
        (*g_callBackPtr)();
    }
}

/*
 * Timer initialization function
 * Sets up the timer based on the provided configuration struct.
 * Config_Ptr - Pointer to the configuration struct containing the timer parameters.
 */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
    // Enable global interrupts
    SREG |= (1 << 7);

    if ((Config_Ptr->timer_mode) == TIMER0)
    {
        TCNT0 = Config_Ptr->timer_InitialValue;    // Set initial value
        OCR0 = Config_Ptr->timer_compare_MatchValue; // Set compare value

        // Enable the appropriate interrupt for NORMAL or CTC mode
        if ((Config_Ptr->timer_mode) == NORMAL_mode)
        {
            TIMSK |= (1 << TOIE0); // Enable Timer0 Overflow Interrupt
        }
        else if ((Config_Ptr->timer_mode) == CTC_mode)
        {
            TIMSK |= (1 << OCIE0); // Enable Timer0 Compare Match Interrupt
        }

        TCCR0 |= (1 << FOC0); // Non-PWM mode (Force Output Compare)
        TCCR0 = (TCCR0 & 0xF7) | ((Config_Ptr->timer_mode) << 3); // Mode configuration
        TCCR0 = (TCCR0 & 0xF8) | ((Config_Ptr->timer_clock));     // Clock configuration
    }
    else if ((Config_Ptr->timer_mode) == TIMER1)
    {
        TCNT1 = Config_Ptr->timer_InitialValue;    // Set initial value
        OCR1A = Config_Ptr->timer_compare_MatchValue; // Set compare value

        if ((Config_Ptr->timer_mode) == NORMAL_mode)
        {
            TIMSK |= (1 << TOIE1); // Enable Timer1 Overflow Interrupt
        }
        else if ((Config_Ptr->timer_mode) == CTC_mode)
        {
            TIMSK |= (1 << OCIE1A); // Enable Timer1 Compare Match Interrupt
        }

        TCCR1A = (1 << FOC1A); // Non-PWM mode (Force Output Compare)
        TCCR1B = (TCCR1B & 0xE7) | ((Config_Ptr->timer_mode) << 3); // Mode configuration
        TCCR1B = (TCCR1B & 0xF8) | ((Config_Ptr->timer_clock));     // Clock configuration
    }
    else if ((Config_Ptr->timer_mode) == TIMER2)
    {
        TCNT2 = Config_Ptr->timer_InitialValue;    // Set initial value
        OCR2 = Config_Ptr->timer_compare_MatchValue; // Set compare value

        if ((Config_Ptr->timer_mode) == NORMAL_mode)
        {
            TIMSK |= (1 << TOIE2); // Enable Timer2 Overflow Interrupt
        }
        else if ((Config_Ptr->timer_mode) == CTC_mode)
        {
            TIMSK |= (1 << OCIE2); // Enable Timer2 Compare Match Interrupt
        }

        TCCR2 = (1 << FOC2); // Non-PWM mode (Force Output Compare)
        TCCR2 = (TCCR2 & 0xF7) | ((Config_Ptr->timer_mode) << 3); // Mode configuration
        TCCR2 = (TCCR2 & 0xF8) | ((Config_Ptr->timer_clock));     // Clock configuration
    }
}

/*
 * Timer de-initialization function
 * Disables the specified timer and resets its registers.
 * timer_type - Enum specifying which timer to disable.
 */
void Timer_deInit(Timer_ID_Type timer_type)
{
    // Disable global interrupts
    SREG &= (~(1 << 7));
    count = 0;

    switch (timer_type)
    {
        case TIMER0:
            TCCR0 = 0;
            TCNT0 = 0;
            OCR0 = 0;
            TIMSK = 0; // Disable all Timer0 interrupts
            break;

        case TIMER1:
            TCCR1B = 0;
            TCCR1A = 0;
            TCNT1 = 0;
            OCR1A = 0;
            OCR1B = 0;
            TIMSK = 0; // Disable all Timer1 interrupts
            break;

        case TIMER2:
            TCCR2 = 0;
            TCNT2 = 0;
            OCR2 = 0;
            TIMSK = 0; // Disable all Timer2 interrupts
            break;
    }
}


/*
 * Function to set the callback function for a specified timer.
 * a_ptr - Pointer to the function that should be called by the timer ISR.
 * a_timer_ID - Enum specifying which timer the callback function is for.
 */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    g_callBackPtr = a_ptr; // Store callback function pointer
    Timer_deInit(a_timer_ID); // De-initialize specified timer to apply callback function
}
