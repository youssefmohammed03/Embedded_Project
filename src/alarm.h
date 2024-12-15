#ifndef ALARM_H
#define ALARM_H
#include <stdint.h>
#include <stdbool.h>

// cplusplus makes the code be read in C when called in a c++ file
#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
void AlarmInit(void);        // Initialize the alarm
void ActivateAlarm(void);    // Turn the alarm ON
void DeactivateAlarm(void);  // Turn the alarm OFF
void ToggleAlarm(void);      // Toggle the alarm 

#ifdef __cplusplus
}
#endif
#endif // ALARM_H
