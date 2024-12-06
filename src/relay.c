#include "relay.h"

void relay_init(char port, uint8_t pins, uint8_t direction) {
    dio_init(port, pins, direction);
}

void relay_control_lamp(char port, uint8_t pin) {
    uint8_t current_state = dio_readpin(port, pin);
    
    if (current_state == 1) {
        dio_writepin(port, pin, 0);
    } else {
        dio_writepin(port, pin, 1);
    }
}

void relay_control_plug(char port, uint8_t pin, uint8_t state) {
    if (state == 1) { 
        dio_writepin(port, pin, 1);
    } else { 
        dio_writepin(port, pin, 0);
    }
}