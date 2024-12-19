#include "relay.h"
#include "DIO.h"
#include "uart.h"
#include "Buzzer.h"

void Delay(uint32_t time) {  
    while (time--) {  
        for (volatile int i = 0; i < 1000; i++) { }  
    }  
}

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
    uint8_t appButtonForLamp = 0;
    uint8_t physicalButtonForLamp = 0;
    uint8_t xor_result_For_Lamp = 0;
    uint8_t appButtonForPlug = 0;
    uint8_t physicalButtonForPlug = 0;
    uint8_t DoorStatus = 0;
    char receivedChar;
    uint16_t adcValue;
    float temperature;
    
    UART_Init(); 
    Buzzer_Init();
    ADC_Init_PE2();
     
    SYSCTL_RCGCGPIO_R |= (1U << ('A' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('A' - 'A'))) == 0);
    
    SYSCTL_RCGCGPIO_R |= (1U << ('B' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('B' - 'A'))) == 0);

    dio_init('A', Pin2, Input); //physical button lamp
    dio_init('B', Pin2, Input); //Door Status
    dio_init('B', Pin0, Input); //physical button plug
    relay_init('B',Pin1,Output); //input to relay for the plug
    relay_init('A',Pin3,Output); //input to relay for the lamp

    while (1) {
        physicalButtonForLamp = dio_readpin('A', Pin2);
        physicalButtonForPlug = dio_readpin('B', Pin0);

        
        receivedChar = UART_ReadChar();
        if(receivedChar == 'P'){appButtonForPlug = 1;}
        else if(receivedChar == 'p'){appButtonForPlug = 0;}
        else if(receivedChar == 'L'){
          if(appButtonForLamp){appButtonForLamp = 0;}
          else{appButtonForLamp = 1;}
        }else{}
        receivedChar = 0;
        
        xor_result_For_Lamp = physicalButtonForLamp ^ appButtonForLamp;
        
        relay_control('A',Pin3, xor_result_For_Lamp);
        if(physicalButtonForPlug){
          if(appButtonForPlug){
            relay_control('B',Pin1, 1);
          } else{
            relay_control('B',Pin1, 0);
          }
        } else{
          relay_control('B',Pin1, 0);
        }
        DoorStatus = dio_readpin('B', Pin2);
        
        if(DoorStatus){
          UART_SendChar('D');
        } else{
          UART_SendChar('d');
        }
        
        adcValue = ADC_Read_PE2();
        temperature = (adcValue * 3.3) / 4096.0;
        temperature = temperature / 0.01; 
        char digits[6]; 

        distribute_uint16_to_chars(adcValue, digits);
        
        UART_SendChar('T');
        
        for (int i = 0; digits[i] != '\0'; i++) {
          UART_SendChar(digits[i]);
        }
        
        UART_SendChar('\n');
        
        CheckAdcValueAndTriggerAlarm(adcValue);
        
        
        Delay(500);
    }
}