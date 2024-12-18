#ifndef UART_H
#define UART_H
#include "DIO.h"

void UART_Init(void);
char UART_ReadChar(void);
void UART_SendChar(char c);
void UART_SendFloat(float temperature);
void UART_SendUInt16(uint16_t value, int is_big_endian);
#endif // UART_H
