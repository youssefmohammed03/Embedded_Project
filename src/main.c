/*#include "relay.h"
#include "DIO.h"*/




/*void Delay(uint32_t time) {  
    while (time--) {  
        for (volatile int i = 0; i < 1000; i++) { }  
    }  
}

uint8_t getAppButton(uint8_t appButton){
  if(appButton){
    return 1;
  } else{
    return 0;
  }
}

char uart_buffer[10];
uint8_t lamp_state = 0, plug_state = 0;

void UART0_IRQHandler(void) {
    if (UART0_FR_R & UART_FR_RXFE) return; // Exit if FIFO empty
    
    char c = UART0_DR_R; // Read character from UART
    static int index = 0;

    if (c == '\n' || c == '\r') {
        uart_buffer[index] = '\0'; // Null-terminate string
        sscanf(uart_buffer, "%hhu,%hhu", &lamp_state, &plug_state);
        index = 0; // Reset index for next input
    } else if (index < sizeof(uart_buffer) - 1) {
        uart_buffer[index++] = c; // Store character in buffer
    }
}

// A2 for the physical button (lamp)
// A3 for the relay (lamp)
// B0 for phyical buttin plug
// B1 for the relay (plug)
// B2 for Door
int main(void) {
  
    UART0_Init();      // Initialize UART0
    SYSCTL_RCGCGPIO_R |= (1 << 5); // Enable clock for GPIOF
    UART0_SendString("MCU Ready\r\n");
    uint8_t index = 0;
    char c;
    
    
    while ((SYSCTL_PRGPIO_R & (1 << 5)) == 0);

    GPIO_PORTF_DIR_R |= (1 << 1) | (1 << 2) | (1 << 3); // Set LEDs as output
    GPIO_PORTF_DEN_R |= (1 << 1) | (1 << 2) | (1 << 3); // Digital enable for LEDs
    
    UART0_TransmitString("IoT Home Automation System Ready!\r\n");
    
    uint8_t appButtonForLamp = 0;
    uint8_t physicalButtonForLamp = 0;
    uint8_t xor_result_For_Lamp = 0;
    
    uint8_t appButtonForPlug = 0;
    uint8_t physicalButtonForPlug = 0;
    uint8_t xor_result_For_Plug = 0;
    
    uint8_t DoorStatus = 0;
    
    SYSCTL_RCGCGPIO_R |= (1U << ('A' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('A' - 'A'))) == 0);
    
    SYSCTL_RCGCGPIO_R |= (1U << ('B' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('B' - 'A'))) == 0);

    dio_init('A', Pin2, Input);
    relay_init('A',Pin3,Output);
    
    dio_init('B', Pin0, Input);
    relay_init('B',Pin1,Output);
    
    dio_init('B', Pin2, Input);
    

    while (1) {
        
        physicalButtonForLamp = dio_readpin('A', Pin2);
        
        physicalButtonForPlug = dio_readpin('B', Pin0);
        
        appButtonForLamp = getAppButton(appButtonForLamp);
        appButtonForPlug = getAppButton(appButtonForPlug);

        xor_result_For_Lamp = physicalButtonForLamp ^ appButtonForLamp;
        xor_result_For_Plug = physicalButtonForPlug ^ appButtonForPlug;

        relay_control_lamp('A',Pin3, xor_result_For_Lamp);
        relay_control_lamp('B',Pin1, xor_result_For_Plug);
        DoorStatus = dio_readpin('B', Pin2);
        Delay(500);
    }

    return 0;
}
*/

#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"

// Function prototypes
void UART0_Init(void);
void UART0_SendString(char *str);
char UART0_ReadChar(void);

// Global variables
char uart_buffer[10];   // Buffer for received commands
uint8_t lamp_state = 0; // Variable to store lamp state
uint8_t plug_state = 0; // Variable to store plug state
// Global or local variable declaration
uint8_t index = 0; // Declare index variable

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;   // Enable UART0
    SYSCTL_RCGCGPIO_R |= 0x01;   // Enable clock to GPIOA
    
    UART0_CTL_R &= ~0x01;        // Disable UART0
    UART0_IBRD_R = 104;          // Set baud rate to 9600 (16 MHz Clock)
    UART0_FBRD_R = 11;
    UART0_LCRH_R = 0x60;         // 8-bit, no parity, 1-stop bit
    UART0_CTL_R |= 0x01;         // Enable UART0

    GPIO_PORTA_AFSEL_R |= 0x03;  // Enable alternate function for PA0, PA1
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF) | 0x11;
    GPIO_PORTA_DEN_R |= 0x03;    // Enable digital for PA0, PA1
}

void UART0_SendString(char *str) {
    while (*str) {
        while (UART0_FR_R & UART_FR_TXFF); // Wait if TX FIFO is full
        UART0_DR_R = *str++;              // Send character
    }
}

char UART0_ReadChar(void) {
    while (UART0_FR_R & UART_FR_RXFE);    // Wait until RX FIFO is not empty
    return UART0_DR_R;                   // Read the received character
}

void processCommand(char c) {
    if (c == '\n' || c == '\r') {
        uart_buffer[index] = '\0'; // Null-terminate string
        if (sscanf(uart_buffer, "%hhu,%hhu", &lamp_state, &plug_state) == 2) {
            UART0_SendString("Lamp and Plug states updated!\r\n");
        } else {
            UART0_SendString("Invalid command!\r\n");
        }
        index = 0; // Reset buffer index
    } else if (index < sizeof(uart_buffer) - 1) {
        uart_buffer[index++] = c; // Store character in buffer
    }
}


int main(void) {
    UART0_Init();  // Initialize UART0
    UART0_SendString("MCU Ready\r\n");

    uint8_t index = 0;
    char c;

    while (1) {
        c = UART0_ReadChar();  // Read characters into buffer

        if (c == '\n' || c == '\r') {  // End of command
            uart_buffer[index] = '\0'; // Null-terminate string
            processCommand(c);          // Process the received command
            index = 0;                 // Reset buffer index
        } else if (index < sizeof(uart_buffer) - 1) {
            uart_buffer[index++] = c;  // Store character in buffer
        }
    }
}


