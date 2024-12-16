#ifndef UART_H
#define UART_H

void UART_Init(void);
char UART_ReadChar(void);
void UART_SendChar(char c);

#endif // UART_H
