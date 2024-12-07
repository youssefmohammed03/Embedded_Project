#include "relay.h"
#include "DIO.h"

void Delay(uint32_t time) {  
    while (time--) {  
        for (volatile int i = 0; i < 1000; i++) { }  
    }  
}

uint8_t getAppButton(uint8_t appButton){
  if(appButton){
    return 0;
  } else{
    return 1;
  }
}

// A2 for the physical button (lamp)
// A3 for the relay (lamp)
int main(void) {
    uint8_t appButton = 0;
    uint8_t physicalButton = 0;
    uint8_t xor_result = 0;

    dio_init('A', Pin2, Input);
    relay_init('A',Pin3,Output);

    while (1) {
        physicalButton = dio_readpin('A', Pin2);
        
        appButton = getAppButton(appButton);

        xor_result = physicalButton ^ appButton;

        relay_control_lamp('A',Pin3, xor_result);
        Delay(500);
    }

    return 0;
}

