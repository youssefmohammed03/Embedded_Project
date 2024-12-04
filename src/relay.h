#ifndef RELAY_H
#define RELAY_H

#include "DIO.h"

#define RELAY_ON  1
#define RELAY_OFF 0

void relay_init(char port, uint8_t pin);
void relay_control(char port, uint8_t pin);

#endif
