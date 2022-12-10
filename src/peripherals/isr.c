#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/yin.h"

extern int16_t (*data_array)[];
extern int16_t data_array1[];
extern int16_t data_array2[];

extern uint16_t resultsBuffer;
uint16_t mode;

/* Completion interrupt for ADC14 MEM0 */
void DMA_INT1_IRQHandler(void)
{
    /* Switch between primary and alternate bufferes with DMA's PingPong mode */
    if (DMA_getChannelAttribute(7) & UDMA_ATTR_ALTSELECT)
    {
        DMA_setChannelControl(
                UDMA_PRI_SELECT | DMA_CH7_ADC14,
                UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH7_ADC14,
        UDMA_MODE_PINGPONG,
                               (void*) &ADC14->MEM[0], data_array1,
                               SAMPLE_LENGTH);
        data_array = &data_array1;
    }
    else
    {
        DMA_setChannelControl(
                UDMA_ALT_SELECT | DMA_CH7_ADC14,
                UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
        UDMA_MODE_PINGPONG,
                               (void*) &ADC14->MEM[0], data_array2,
                               SAMPLE_LENGTH);
        data_array = &data_array2;
    }
}

void PORT5_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN1)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        switch (mode)
        {
        case 0:
            mode = 1;
            break;
        case 1:
            mode = 0;
            break;
        }
    }
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if (status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer = ADC14_getResult(ADC_MEM1);
    }
}
