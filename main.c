#include "main.h"
	uint8_t number_rank;
	uint8_t digit_rank_one,digit_rank_two,digit_rank_tree,digit_rank_thou;
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
	init_pin();
	init_timer_light();
	init_timer();
	init_timer_button();	
	sei();
	while (1) {
		convert_number_clock();	
	}
}

void init_pin(void) 
{
	PIND_OUT;
	PIND_LOW;
	PINC_OUT;
	PINC_LOW;
	//This is tact button minute and hour
	TACT_BUTTON_OUT;
	TACT_BUTTON_LOW;
}	

void init_timer_light(void) 
{
	SFIOR |= (1<<PSR10);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 0x9c;
	TCCR0 |= (1<<CS01);		
}

void init_timer(void) 
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

void convert_number_clock() 
{	
	digit_rank_thou = hour/1000;
	digit_rank_tree = hour/100%10;
	digit_rank_two = minute/10%10;
	digit_rank_one = minute%10;
}

ISR (TIMER0_OVF_vect)
{
	if (number_rank==0)
	{
		ONE_DIGIT_OFF;
		TWO_DIGIT_OFF;
		THREE_DIGIT_OFF;
		THOU_DIGIT_ON;
		output_shift_register(change_digit(&digit_rank_one));
	}
	if (number_rank==1)
	{
		ONE_DIGIT_OFF;
		TWO_DIGIT_OFF;
		THOU_DIGIT_OFF;
		THREE_DIGIT_ON;
		output_shift_register(change_digit(&digit_rank_two));
	}
	if (number_rank==2)
	{
		ONE_DIGIT_OFF;
		THREE_DIGIT_OFF;
		THOU_DIGIT_OFF;
		TWO_DIGIT_ON;
		output_shift_register(change_digit(&digit_rank_tree));
	}
	if (number_rank==3)
	{
		TWO_DIGIT_OFF;
		THREE_DIGIT_OFF;
		THOU_DIGIT_OFF;
		ONE_DIGIT_ON;
		output_shift_register(change_digit(&digit_rank_thou));
	}
	number_rank++;
	if (number_rank>3) number_rank=0;
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
void init_timer_button()
{
	GICR |= (1<<INTF0);
	MCUCR |= (1<<ISC01);
	
	GICR|=(1<<INTF1);
	MCUCR|=(1<<ISC11);
}

ISR (INT0_vect)
{
	++minute;
}

ISR (INT1_vect)
{
	hour+=100;
}