#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "uart.h"
#include "gpio.h"

int main(void) {
    UART0_Init();      // Initialize UART0
    SYSCTL_RCGCGPIO_R |= (1 << 5); // Enable clock for GPIOF
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0);

    GPIO_PORTF_DIR_R |= (1 << 1) | (1 << 2) | (1 << 3); // Set LEDs as output
    GPIO_PORTF_DEN_R |= (1 << 1) | (1 << 2) | (1 << 3); // Digital enable for LEDs
    
    UART0_TransmitString("IoT Home Automation System Ready!\r\n");

    while (1) {
        if (received_char != 0) {
            processCommand(received_char);
            received_char = 0; // Clear the received character
        }
    }
}
