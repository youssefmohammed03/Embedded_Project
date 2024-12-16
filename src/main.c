#include "relay.h"
#include "DIO.h"

void Delay(uint32_t time) {  
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

// A2 physical button lamp
// A3 input to relay for the lamp
// B0 for physical button plug
// B1 input to relay for the plug
// B2 for Door
int main(void) {
    char receivedChar;
    uint8_t appButtonForLamp = 0;
    uint8_t physicalButtonForLamp = 0;
    uint8_t xor_result_For_Lamp = 0;
    uint8_t appButtonForPlug = 0;
    uint8_t physicalButtonForPlug = 0;
    uint8_t xor_result_For_Plug = 0;
    uint8_t DoorStatus = 0;
    
    UART_Init(); 
    
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
        
        appButtonForLamp = getAppButton(appButtonForLamp);
        appButtonForPlug = getAppButton(appButtonForPlug);
  
        xor_result_For_Lamp = physicalButtonForLamp ^ appButtonForLamp;
        xor_result_For_Plug = physicalButtonForPlug ^ appButtonForPlug;
        
        relay_control('A',Pin3, xor_result_For_Lamp);
        relay_control('B',Pin1, xor_result_For_Plug);
        DoorStatus = dio_readpin('B', Pin2);
        Delay(500);
    }

    return 0;
}