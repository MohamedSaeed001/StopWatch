/*
=====================================================================
Name       : Mini_Project2.c
Created on : 10 February 2022
Auther     : Mohamed Saeed
Description: Stop Watch
Diploma    : Number 61
=====================================================================
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char sec = 0;
unsigned char min = 0;
unsigned char hour = 0;

ISR(TIMER1_COMPA_vect)
{
	sec++;
	if(sec == 60)
	{
		sec = 0;
		min++;
	}
	else if(min == 60)
	{
		sec = 0;
		min = 0;
		hour++;
	}
	else if(hour == 12)
	{
		sec  = 0;
		min  = 0;
		hour = 0;
	}

}

ISR(INT0_vect)
{
	sec  = 0;
	min  = 0;
	hour = 0;
}

ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS11) & ~(1<<CS10);
}

ISR(INT2_vect)
{
	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
}

void Timer1_CTC_Init(void)
{
	TCNT1  = 0;
	OCR1A  = 15625;
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);

}

void INT0_Init_Reset(void)
{
	MCUCR |= (1<<ISC01);
	GICR  |= (1<<INT0);
}

void INT1_Init_Pause(void)
{
	MCUCR = (1<<ISC11) | (1<<ISC10);
	GICR |= (1<<INT1);
}

void INT2_init_Resume(void)
{
	MCUCSR &= ~(1<<ISC2);
	GICR   |= (1<<INT2);

}

int main(void)
{
	DDRA |= 0x3F;
	DDRB &= 0xFB;
	DDRC |= 0x0F;
	DDRD &= 0xF3;

	PORTA &= 0xC0;
	PORTB |= 0x04;
	PORTC &= 0xF0;
	PORTD |= 0x04;
	PORTD &= 0xF7;

	Timer1_CTC_Init();
	INT0_Init_Reset();
	INT1_Init_Pause();
	INT2_init_Resume();
	SREG |= (1<<7);


	while(1)
	{
		PORTA = (1<<PA0);
		PORTC = sec % 10;
		_delay_us(5);

		PORTA = (1<<PA1);
		PORTC = sec / 10;
		_delay_us(5);

		PORTA = (1<<PA2);
		PORTC = min % 10;
		_delay_us(5);

		PORTA = (1<<PA3);
		PORTC = min / 10;
		_delay_us(5);

		PORTA = (1<<PA4);
		PORTC = hour % 10;
		_delay_us(5);

		PORTA = (1<<PA5);
		PORTC = hour / 10;
		_delay_us(5);
	}
}
