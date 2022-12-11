#include "embedded_tuner/include/peripherals.h"

/* Timer_A PWM Configuration Parameter */
const Timer_A_PWMConfig adc_pwmConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    (SMCLK_FREQUENCY / SAMPLE_FREQUENCY),
    TIMER_A_CAPTURECOMPARE_REGISTER_1,
    TIMER_A_OUTPUTMODE_SET_RESET,
    (SMCLK_FREQUENCY / SAMPLE_FREQUENCY) / 2
};

const Timer_A_PWMConfig buzzer_pwmConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,     // SMCLK = 48 MHz
    TIMER_A_CLOCKSOURCE_DIVIDER_64, // SMCLK / 64 = 750 kHz
    0,
    TIMER_A_CAPTURECOMPARE_REGISTER_4,
    TIMER_A_OUTPUTMODE_TOGGLE_SET,
    500
};

void init_timer(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* ADC trigger */
    Timer_A_generatePWM(TIMER_A1_BASE, &adc_pwmConfig);

    /* Buzzer */
    Timer_A_generatePWM(TIMER_A0_BASE, &buzzer_pwmConfig);
}

