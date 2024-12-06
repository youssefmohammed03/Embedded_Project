#include <stdint.h>
#include "relay.h"
#include "DIO.h"

void Delay(uint32_t time) {  
    while (time--) {  
        for (volatile int i = 0; i < 1000; i++) { }  
    }  
}

int main(void) {
    uint8_t a = 0; // Variable initialized to 1 or 0

    // Initialize PD2 as input
    dio_init('D', 2, 0);
    relay_init('D',1,1);

    while (1) {
        // Read the current state of PD2
        uint8_t pd2_state = dio_readpin('D', 2);

        // Perform XOR operation between PD2 state and variable `a`
        volatile uint8_t xor_result = pd2_state ^ a;

        // Send the XOR result to control the relay lamp on PE1
        relay_control_lamp('D',1, xor_result);
        Delay(500);
    }

    return 0;
}
