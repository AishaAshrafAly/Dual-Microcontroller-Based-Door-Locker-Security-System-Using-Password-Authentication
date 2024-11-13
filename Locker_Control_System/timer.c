/*
 *  Created on: Nov 1, 2024
 *      Author: Aisha
 */
#include "timer.h"
#include "avr/io.h"
#include "common_macros.h"
#include <avr/interrupt.h>

volatile extern uint16 g_timerCounter;
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

// Interrupt Service Routine for Timer0 Overflow
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

// Interrupt Service Routine for Timer0 Compare Match
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

// Interrupt Service Routine for Timer1 Overflow
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

// Interrupt Service Routine for Timer1 Compare Match
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

// Interrupt Service Routine for Timer2 Overflow
ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

// Interrupt Service Routine for Timer2 Compare Match
ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)(); // Execute the callback function if set
	}
}

/* Initializes the timer with the specified configuration */
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	SREG |= (1<<7); // Enable global interrupts

	if((Config_Ptr->timer_mode) == TIMER0)
	{
		TCNT0 = Config_Ptr->timer_InitialValue;
		OCR0 = Config_Ptr->timer_compare_MatchValue;

		if((Config_Ptr->timer_mode) == NORMAL_mode)
		{
			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
		}
		else if((Config_Ptr->timer_mode) == CTC_mode)
		{
			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt
		}

		TCCR0 |= (1<<FOC0); // Non-PWM mode
		TCCR0 = (TCCR0 & 0xF7) | ((Config_Ptr->timer_mode) << 3);
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock);
	}
	else if((Config_Ptr->timer_mode) == TIMER1)
	{
		TCNT1 = Config_Ptr->timer_InitialValue;
		OCR1A = Config_Ptr->timer_compare_MatchValue;

		if((Config_Ptr->timer_mode) == NORMAL_mode)
		{
			TIMSK |= (1<<TOIE1); // Enable Timer1 Overflow Interrupt
		}
		else if((Config_Ptr->timer_mode) == CTC_mode)
		{
			TIMSK |= (1<<OCIE1A); // Enable Timer1 Compare Interrupt
		}

		TCCR1A = (1<<FOC1A); // Non-PWM mode
		TCCR1B = (TCCR1B & 0xF7) | ((Config_Ptr->timer_mode) << 3);
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock);
	}
	else if((Config_Ptr->timer_mode) == TIMER2)
	{
		TCNT2 = Config_Ptr->timer_InitialValue;
		OCR2 = Config_Ptr->timer_compare_MatchValue;

		if((Config_Ptr->timer_mode) == NORMAL_mode)
		{
			TIMSK |= (1<<TOIE2); // Enable Timer2 Overflow Interrupt
		}
		else if((Config_Ptr->timer_mode) == CTC_mode)
		{
			TIMSK |= (1<<OCIE2); // Enable Timer2 Compare Interrupt
		}

		TCCR2 = (1<<FOC2); // Non-PWM mode
		TCCR2 = (TCCR2 & 0xF7) | ((Config_Ptr->timer_mode) << 3);
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock);
	}
}

/* Deinitializes the timer, disabling its function */
void Timer_deInit(Timer_ID_Type timer_type)
{
	SREG &= (~(1<<7)); // Disable global interrupts
	g_timerCounter = 0;

	switch(timer_type)
	{
	case TIMER0:
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0  = 0;
		TIMSK = 0;
		break;
	case TIMER1:
		TCCR1B = 0;
		TCCR1A = 0;
		TCNT1  = 0;
		OCR1A  = 0;
		OCR1B  = 0;
		TIMSK = 0;
		break;
	case TIMER2:
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2  = 0;
		TIMSK = 0;
		break;
	}
}

/* Assigns a callback function for the specified timer */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
	g_callBackPtr = a_ptr; // Store callback function address
	Timer_deInit(a_timer_ID); // Deinitialize specified timer
}
