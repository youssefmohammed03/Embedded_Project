#include "uart.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

// UART Initialization
void UART_Init(void) {
    // 1. Enable clocks for UART0 and GPIO Port A
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;  // Enable UART0 clock
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;  // Enable GPIO Port A clock

    // 2. Configure GPIO Pins PA0 and PA1 for UART
    GPIO_PORTA_AFSEL_R |= 0x03;      // Enable alternate function for PA0 and PA1
    GPIO_PORTA_PCTL_R |= 0x00000011; // Configure PA0 and PA1 for UART
    GPIO_PORTA_DEN_R |= 0x03;        // Enable digital function for PA0 and PA1

    // 3. Configure UART0
    UART0_CTL_R &= ~UART_CTL_UARTEN; // Disable UART0 for configuration
    UART0_IBRD_R = 104;              // Integer part of baud rate (16 MHz / (16 * 9600))
    UART0_FBRD_R = 11;               // Fractional part of baud rate
    UART0_LCRH_R = UART_LCRH_WLEN_8; // 8-bit data, no parity, 1 stop bit
    UART0_CTL_R = UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE; // Enable UART0
}

// UART Send Character
void UART_SendChar(char c) {
    while (UART0_FR_R & UART_FR_TXFF); // Wait until TX buffer is not full
    UART0_DR_R = c;                    // Write character to data register
}

char UART_ReadChar(void) {
    uint32_t timeout = 100000;
    
    while ((UART0_FR_R & UART_FR_RXFE) && timeout > 0) {
        timeout--;
    }
    
    if (timeout == 0) {
        return '\0'; 
    }
    
    return (char)(UART0_DR_R & 0xFF);
}





