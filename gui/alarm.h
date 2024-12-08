#ifndef ALARM_H
#define ALARM_H
#include <stdint.h>
#include <stdbool.h>

// Function prototypes
void AlarmInit(void);        // Initialize the alarm
void ActivateAlarm(void);    // Turn the alarm ON
void DeactivateAlarm(void);  // Turn the alarm OFF
void ToggleAlarm(void);      // Toggle the alarm 


#endif // ALARM_H
