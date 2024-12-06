#include "relay.h"

void relay_init(char port, uint8_t pins, uint8_t direction) {
    dio_init(port, pins, direction);
}

void relay_control_lamp(char port, uint8_t pin, uint8_t value) {
        dio_writepin(port, pin, value);
}