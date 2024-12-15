#include "alarm.h"
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define ALARM_PORT GPIO_PORTF_BASE
#define ALARM_PIN GPIO_PIN_2

void AlarmInit(void)
{
     // Enable the GPIO peripheral for Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait until the peripheral is ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Set the pin as output
    GPIOPinTypeGPIOOutput(ALARM_PORT, ALARM_PIN);

    // Ensure the alarm is initially off
    DeactivateAlarm();
}

void ActivateAlarm(void)
{
    GPIOPinWrite(ALARM_PORT, ALARM_PIN, ALARM_PIN);
}

void DeactivateAlarm(void)
{
    GPIOPinWrite(ALARM_PORT, ALARM_PIN, 0);
}

void ToggleAlarm(void)
{
    uint8_t state = GPIOPinRead(ALARM_PORT, ALARM_PIN);
    GPIOPinWrite(ALARM_PORT, ALARM_PIN, state ^ ALARM_PIN);
}