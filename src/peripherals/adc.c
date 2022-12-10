#include "embedded_tuner/include/peripherals.h"


void init_adc(void)
{
    /* Initializing ADC (MCLK/1/1) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    /* Use timer_a0_base pwm as reference */
    ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    /* Configuring ADC Memory */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

    /* Microphone memory */
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A10, ADC_NONDIFFERENTIAL_INPUTS);

    /* Joystick Y-axis memory */
    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Set ADC result format to signed binary */
    ADC14_setResultFormat(ADC_SIGNED_BINARY);

    /* Setting up the sample timer to automatically step through the sequence convert */
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Interrupt for joystick */
    ADC14_enableInterrupt(ADC_INT1);
}
