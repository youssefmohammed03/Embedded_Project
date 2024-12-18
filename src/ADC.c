#include "ADC.h"
#include "tm4c123gh6pm.h"

void ADC_Init_PE2(void) {
    // Enable GPIO Port E and ADC0 clocks
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // Enable GPIO Port E clock
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;  // Enable ADC0 clock
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R4) == 0); // Wait for GPIO E ready

    // Configure PE2 as analog input
    GPIO_PORTE_AFSEL_R |= 0x04;  // Enable alternate function for PE2
    GPIO_PORTE_DEN_R &= ~0x04;   // Disable digital function for PE2
    GPIO_PORTE_AMSEL_R |= 0x04;  // Enable analog functionality for PE2

    // Configure ADC0 Sequencer 3 for AIN1 (PE2)
    ADC0_ACTSS_R &= ~0x08;       // Disable SS3 for configuration
    ADC0_EMUX_R &= ~0xF000;      // Set SS3 to use software trigger
    ADC0_SSMUX3_R = 1;           // Select AIN1 (PE2)
    ADC0_SSCTL3_R = 0x06;        // Enable interrupt and end-of-sequence for SS3
    ADC0_ACTSS_R |= 0x08;        // Enable SS3
}

// Read the ADC value from PE2
uint16_t ADC_Read_PE2(void) {
    ADC0_PSSI_R = 0x08;          // Start ADC conversion on SS3
    while ((ADC0_RIS_R & 0x08) == 0); // Wait for conversion to complete
    uint16_t result = ADC0_SSFIFO3_R & 0xFFF; // Read 12-bit result
    ADC0_ISC_R = 0x08;           // Clear completion flag
    return result;
}
