#ifndef BUZZER
#define BUZZER

// Define buzzer-specific configurations
#define BUZZER_PORT 'B'   
#define BUZZER_PIN (1U << 7) // PB7 is used for the buzzer

// Function to read the state of the manual switch
void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void CheckAdcValueAndTriggerAlarm(uint16_t adcValue);

#endif