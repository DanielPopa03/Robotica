#ifndef USART_H
#define USART_H

#include <avr/io.h>

void USART0_init(unsigned int baud_rate);
void USART_transmit(unsigned char data);
void USART0_print(const char *str);

#endif // USART_H