#ifndef RELAY_H
#define RELAY_H

#include "DIO.h"

#define RELAY_ON  1
#define RELAY_OFF 0

void relay_init(char port, uint8_t pins, uint8_t direction);
void relay_control_lamp(char port, uint8_t pin);

#endif
