#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART0_Init(void);
void UART0_TransmitChar(char c);
void processCommand(char c);
void displayLastCommand(void);
void UART0_SendString(char *str);
void processCommand(char c);


extern volatile char received_char;

#endif
