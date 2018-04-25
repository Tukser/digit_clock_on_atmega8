/*
 * main.h
 *
 * Created: 12.04.2018 23:21:10
 *  Author: aidar
 */ 


#ifndef MAIN_H_
#define MAIN_H_


#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>


void init_Pin(void);
void init_Timer (void);
void init_Timer_light(void);

void output_shift_register(uint8_t buffer);
uint8_t change_digit(uint8_t *buffer);
void convert_number_hour();

void init_button_int_minute(void);
void init_button_int_hour(void);

#endif /* MAIN_H_ */