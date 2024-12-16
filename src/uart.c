/*#include "tm4c123gh6pm.h"
#include "uart.h"
#include <stdio.h>

volatile char received_char = 0;
volatile char last_command = '\0';  // Holds the last received command
char uart_buffer[10];  // Buffer for storing received commands
uint8_t lamp_state = 0; // Variable to store lamp state
uint8_t plug_state = 0; // Variable to store plug state

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= (1 << 0);
    SYSCTL_RCGCGPIO_R |= (1 << 0);

    GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);
    GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4);
    GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);

    UART0_CTL_R &= ~(1 << 0);
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = (0x3 << 5);
    UART0_CC_R = 0x0;
    UART0_IM_R |= (1 << 4);
    NVIC_EN0_R |= (1 << 5);
    UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9);
}

void UART0_TransmitChar(char c) {
    while (UART0_FR_R & (1 << 5));
    UART0_DR_R = c;
}

void UART0_TransmitString(const char *str) {
    while (*str) {
        UART0_TransmitChar(*str++);
    }
}

void UART0_Handler(void) {
    if (UART0_MIS_R & (1 << 4)) {
        received_char = (char)(UART0_DR_R & 0xFF);
        UART0_ICR_R |= (1 << 4);
    }
}

void displayLastCommand(void) {
    UART0_TransmitString("Last Command: ");
    if (last_command != '\0') {
        UART0_TransmitChar(last_command);
    } else {
        UART0_TransmitString("No command received yet");
    }
    UART0_TransmitString("\r\n");
}*/



