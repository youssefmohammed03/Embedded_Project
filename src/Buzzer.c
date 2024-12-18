#include <stdbool.h>
#include <stdint.h>
#include "dio.h"
#include "bitwise_operations.h"
#include <stdio.h>
#include "Buzzer.h"

#define BUZZER_PORT 'B'    
#define BUZZER_PIN (1U << 7) // PB7 is used for the buzzer
#define ALARM_TEMP_THRESHOLD 35.0f 

void Buzzer_Init(void) {
    // Initialize PB7 as an output for the buzzer
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

void CheckTemperatureAndTriggerAlarm(float temperature) {
    // Check if the temperature exceeds the threshold
    if (temperature > ALARM_TEMP_THRESHOLD) {
        Buzzer_On();
        ActivateAlarm(); 
    } else {
        Buzzer_Off();
        DeactivateAlarm(); 
    }
}
