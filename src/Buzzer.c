#include <stdbool.h>
#include <stdint.h>
#include "dio.h"
#include "bitwise_operations.h"
#include <stdio.h>
#include "Buzzer.h"

#define BUZZER_PORT 'B'    
#define BUZZER_PIN (1U << 7) // PB7 is used for the buzzer
#define ALARM_adcValue_THRESHOLD 435 

void Buzzer_Init(void) {
    // Initialize PB7 as an output for the buzzer
    SYSCTL_RCGCGPIO_R |= (1U << ('B' - 'A'));
    while ((SYSCTL_PRGPIO_R & (1U << ('B' - 'A'))) == 0);
    dio_init(BUZZER_PORT, BUZZER_PIN, Output);

    // Ensure the buzzer is initially off
    dio_writepin(BUZZER_PORT, BUZZER_PIN, 0);
}

void Buzzer_On(void) {
    // Set PB7 to high to turn on the buzzer
    dio_writepin(BUZZER_PORT, BUZZER_PIN, 1);
}

void Buzzer_Off(void) {
    // Set PB7 to low to turn off the buzzer
    dio_writepin(BUZZER_PORT, BUZZER_PIN, 0);
}

void CheckAdcValueAndTriggerAlarm(uint16_t adcValue) {
    // Check if the adcValue exceeds the threshold
  
    if (adcValue > ALARM_adcValue_THRESHOLD) {
        Buzzer_On();
    } else {
        Buzzer_Off();
    }
}
