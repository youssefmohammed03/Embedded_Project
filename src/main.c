#include "relay.h"
#include "DIO.h"
#include "uart.h"
#include "Buzzer.h"

// Simple delay function
void Delay(uint32_t time) {  
    while (time--) {  
        for (volatile int i = 0; i < 1000; i++) { }  
    }  
}

// Converts a 16-bit integer to a string for UART communication
void distribute_uint16_to_chars(uint16_t value, char *result) {
    // Use sprintf to convert the integer into a string
    sprintf(result, "%u", value);
}

// A2 physical button lamp
// A3 input to relay for the lamp
// B0 for physical button plug
// B1 input to relay for the plug
// B2 for Door
int main(void) {
    // Define and initialize variables
    uint8_t appButtonForLamp = 0;
    uint8_t physicalButtonForLamp = 0;
    uint8_t xor_result_For_Lamp = 0;
    uint8_t appButtonForPlug = 0;
    uint8_t physicalButtonForPlug = 0;
    uint8_t DoorStatus = 0;
    char receivedChar;
    uint16_t adcValue;
    float temperature;
    
    // Initialize peripherals
    UART_Init(); 
    Buzzer_Init();
    ADC_Init_PE2();
    
    // Enable GPIO for Port A
    SYSCTL_RCGCGPIO_R |= (1U << ('A' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('A' - 'A'))) == 0);
    
    // Enable GPIO for Port B
    SYSCTL_RCGCGPIO_R |= (1U << ('B' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('B' - 'A'))) == 0);

    dio_init('A', Pin2, Input); //physical button lamp
    dio_init('B', Pin2, Input); //Door Status
    dio_init('B', Pin0, Input); //physical button plug
    relay_init('B',Pin1,Output); //input to relay for the plug
    relay_init('A',Pin3,Output); //input to relay for the lamp

    // Main loop
    while (1) {
        // Read the state of physical buttons
        physicalButtonForLamp = dio_readpin('A', Pin2);
        physicalButtonForPlug = dio_readpin('B', Pin0);

        // Read and handle UART input
        receivedChar = UART_ReadChar();
        if(receivedChar == 'P'){appButtonForPlug = 1;} // Turn on the plug relay via app
        else if(receivedChar == 'p'){appButtonForPlug = 0;} // Turn off the plug relay via app
        else if(receivedChar == 'L'){ // Toggle the lamp relay via app
          if(appButtonForLamp){appButtonForLamp = 0;}
          else{appButtonForLamp = 1;}
        }else{}
        receivedChar = 0; // Clear received character
        
        // XOR logic to determine lamp relay state
        xor_result_For_Lamp = physicalButtonForLamp ^ appButtonForLamp;
        
        relay_control('A',Pin3, xor_result_For_Lamp); // Control lamp relay

        // Logic for plug relay
        if(physicalButtonForPlug){
          if(appButtonForPlug){
            relay_control('B',Pin1, 1); // Turn on plug relay
          } else{
            relay_control('B',Pin1, 0); // Turn off plug relay
          }
        } else{
          relay_control('B',Pin1, 0); // Ensure plug relay is off
        }

        // Read door status
        DoorStatus = dio_readpin('B', Pin2);
        if(DoorStatus){
          UART_SendChar('D'); // Send 'D' if door is open
        } else{
          UART_SendChar('d'); // Send 'd' if door is closed
        }
        
        // Read ADC value and calculate temperature
        adcValue = ADC_Read_PE2();
        temperature = (adcValue * 3.3) / 4096.0; // Convert ADC value to voltage
        temperature = temperature / 0.01; // Convert voltage to temperature

        // Convert ADC value to string and send via UART
        char digits[6]; 
        distribute_uint16_to_chars(adcValue, digits);
        
        UART_SendChar('T'); // Indicate temperature data
        
        for (int i = 0; digits[i] != '\0'; i++) {
          UART_SendChar(digits[i]); // Send each digit
        }
        
        UART_SendChar('\n'); // Digits ended
        
        // Check ADC value and trigger alarm if necessary
        CheckAdcValueAndTriggerAlarm(adcValue);

        // Delay
        Delay(500);
    }
}