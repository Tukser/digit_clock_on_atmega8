/*
 * clock.c
 *
 * Created: 12.04.2018 23:14:57
 * Author : aidar
 */ 
#include "main.h"
	uint8_t i;
	uint8_t r1,r2,r3,r4;
	uint8_t second, minute;
	int hour;
enum 
{
	ZERO = 0xC0,
	ONE = 0xF9,
	TWO = 0xA4,
	THREE = 0xB0,
	FOUR = 0x99,
	FIVE = 0x92,
	SIX = 0x82,
	SEVEN = 0xF8,
	EIGHT = 0x80,
	NINE = 0x90
};

void main (void)
{
	cli();
	init_Pin();
	init_Timer_light();
	init_Timer();
	init_button_int_hour();
	init_button_int_minute();	
	sei();
	while (1) {
		convert_number_hour();	
	}
}

void init_Pin(void) 
{
	DDRD |= 0xFF;
	PORTD |= 0x00;
	DDRC |= (1<<PC3);
	PORTC &= ~(1<<PC3);
	//This is tact button minute and hour
	DDRD &= ~(1<<PD2);
	PORTD &=~(1<<PD2);
	DDRD &= ~(1<<PD3);
	PORTD &=~(1<<PD3);
}	

void init_Timer_light (void) 
{
	SFIOR |= (1<<PSR10);
	TIMSK |= (1<<TOIE0) ;
	TCNT0 = 0x9c;
	TCCR0 |= (1<<CS01);		
}

void init_Timer(void) 
{
	TIMSK |=(1<<OCIE1A);
	TCCR1B |= (1<<WGM12);
	TCCR1B |=(1<<CS12);
	OCR1A = 0xC34;
	TCNT1 = 0;
}

void output_shift_register(uint8_t buffer) 
{
	uint8_t counter;
	PORTD |= (1<<PD7);
	for (counter=0; counter<8; counter++) 
	{
		if ((buffer & 0x80) != 0)
			PORTD |= (1<<PD5);
		else
			PORTD &= ~(1<<PD5);
		PORTD |= (1<<PD7);
		buffer = buffer<<1;
		PORTD &= ~(1<<PD6);
		PORTD |= (1<<PD6);
	}
}

uint8_t change_digit(uint8_t *buffer) {
	if (*buffer == 1)
		return ONE;
	else if (*buffer == 2)
		return TWO;
	else if (*buffer == 3)
		return THREE;
	else if (*buffer == 4)
		return FOUR;
	else if (*buffer == 5)
		return FIVE;
	else if (*buffer == 6)
		return SIX;
	else if (*buffer == 7)
		return SEVEN;
	else if (*buffer == 8)
		return EIGHT;
	else if (*buffer == 9)
		return NINE;
	else if (*buffer == 0)
		return ZERO;
}

void convert_number_hour() 
{	
	r4 = hour/1000;
	r3 = hour/100%10;
	r2 = minute/10%10;
	r1 = minute%10;
}

ISR (TIMER0_OVF_vect)
{
	if (i==0)
	{
		PORTD &= ~(1<<PD0);
		PORTD &= ~(1<<PD1);
		PORTC &= ~(1<<PC3);
		PORTD |= (1<<PD4);
		output_shift_register(change_digit(&r1));
	}
	if (i==1)
	{
		PORTD &= ~(1<<PD0);
		PORTD &= ~(1<<PD1);
		PORTD &= ~(1<<PD4);
		PORTC |= (1<<PC3);
		output_shift_register(change_digit(&r2));
	}
	if (i==2)
	{
		PORTD &= ~(1<<PD0);
		PORTC &= ~(1<<PC3);
		PORTD &= ~(1<<PD4);
		PORTD |= (1<<PD1);
		output_shift_register(change_digit(&r3));
	}
	if (i==3)
	{
		PORTD &= ~(1<<PD1);
		PORTC &= ~(1<<PC3);
		PORTD &= ~(1<<PD4);
		PORTD |= (1<<PD0);
		output_shift_register(change_digit(&r4));
	}
	i++;
	if (i>3) i=0;
}
ISR (TIMER1_COMPA_vect)
{ 
	++second;
	if (second>59)
	{
		second=0;
		++minute;
	}	if (minute>59)
	{
		minute=0;
		hour+=100;
	}
	if (hour>2300) hour=0;
}
void init_button_int_minute()
{
	GICR |= (1<<INTF0);
	MCUCR |= (1<<ISC01);
}

ISR (INT0_vect)
{
	++minute;
}

void init_button_int_hour()
{
	GICR|=(1<<INTF1);
	MCUCR|=(1<<ISC11);
}

ISR (INT1_vect)
{
	hour+=100;
}