#include "Usart.h" 

void USART0_init(unsigned int baud_rate) {
  /* seteaza baud rate */
  UBRR0H = (unsigned char)(baud_rate>>8);
  UBRR0L = (unsigned char)baud_rate;
 
  /* porneste transmitatorul */
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
 
  /* seteaza formatul frame-ului: 8 biti de date, 2 bit de stop, fara paritate */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void USART_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the data
    UDR0 = data;
}

void USART0_print(const char *str) {
    // Iterate through the string and send each character
    while (*str) {
        USART_transmit(*str);
        str++;  // Move to the next character in the string
    }
}