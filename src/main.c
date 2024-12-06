#include <stdint.h>
#include "relay.h"
#include "DIO.h"

int main(void) {
    uint8_t a = 1; // Variable initialized to 1 or 0

    // Initialize PD2 as input
    dio_init('D', 2, 0);

    // Initialize PE1 as output
    relay_init('E', 1);

    while (1) {
        // Read the current state of PD2
        uint8_t pd2_state = dio_readpin('D', 2);

        // Perform XOR operation between PD2 state and variable `a`
        uint8_t xor_result = pd2_state ^ a;

        // Send the XOR result to control the relay lamp on PE1
        relay_control_lamp('E', xor_result);
    }

    return 0;
}
