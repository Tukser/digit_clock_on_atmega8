#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//Переключение сегментов по разряду
#define ONE_DIGIT_OFF (PORTD &= ~(1<<PD0))
#define ONE_DIGIT_ON (PORTD |= (1<<PD0))
#define TWO_DIGIT_OFF (PORTD &= ~(1<<PD1))
#define TWO_DIGIT_ON (PORTD |= (1<<PD1))
#define THREE_DIGIT_OFF (PORTC &= ~(1<<PC3))
#define THREE_DIGIT_ON (PORTC |= (1<<PC3))
#define THOU_DIGIT_OFF (PORTD &= ~(1<<PD4))
#define THOU_DIGIT_ON (PORTD |= (1<<PD4))
//Настройка портов
#define PIND_OUT (DDRD |= 0xFF)
#define	PIND_LOW (PORTD |= 0x00)
#define	PINC_OUT (DDRC |= 0xFF)
#define	PINC_LOW (PORTC |= 0x00)
#define	TACT_BUTTON_OUT (DDRD &= ~(1<<PD2)) && (DDRD &= ~(1<<PD3))
#define	TACT_BUTTON_LOW (PORTD &= ~(1<<PD2)) && (PORTD &= ~(1<<PD3))

void init_pin(void); //Инициализация пинов
void init_timer(void); //Таймер часов
void init_timer_light(void); //Таймер светодиодов
void output_shift_register(uint8_t buffer);//Передача через сдвиговый регистр данные
uint8_t change_digit(uint8_t *buffer);//Перевод числа в код
void convert_number_clock();//Поразрядное деление числа
void init_timer_button(void);//Инициализация таймера прерывания для тактовых кнопок

#endif /* MAIN_H_ */