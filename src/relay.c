#include "relay.h"

void relay_init(char port, uint8_t pin) {
    dio_init(port, pin, Output);
}

void relay_control(char port, uint8_t pin) {
    uint8_t current_state = dio_readpin(port, pin);
    
    if (current_state == 1) {
        dio_writepin(port, pin, 0);
    } else {
        dio_writepin(port, pin, 1);
    }
}
