#include "embedded_tuner/include/peripherals.h"

extern int16_t data_array1[];
extern int16_t data_array2[];

void init_adc(void)
{
    /* Initializing ADC (MCLK/1/1) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE3, false);

    /* Configuring ADC Memory */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A10, false);

    /* Set ADC result format to signed binary */
    ADC14_setResultFormat(ADC_SIGNED_BINARY);
}

void set_adc_microphone(void)
{
    ADC14_disableConversion();
    while(!ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A10, false));
    ADC14_enableConversion();
}

void set_adc_jack(void)
{
    ADC14_disableConversion();
    while(!ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A0, false));
    ADC14_enableConversion();
}
