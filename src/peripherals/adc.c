#include "embedded_tuner/include/peripherals.h"

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];

int16_t data_array1[SAMPLE_LENGTH];
int16_t data_array2[SAMPLE_LENGTH];

void init_adc(void)
{
    /* Initializing ADC (MCLK/1/1) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE3, false);

    /* Configuring ADC Memory with microphone */
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A10, false);

    /* Set ADC result format to signed binary */
    ADC14_setResultFormat(ADC_SIGNED_BINARY);
}

void init_dma(void)
{
    /* Configuring DMA module */
    DMA_enableModule();
    DMA_setControlBase(MSP_EXP432P401RLP_DMAControlTable);

    /* Select alternate control structure, high priority and mask interrupts */
    DMA_disableChannelAttribute(DMA_CH7_ADC14,
    UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
    UDMA_ATTR_HIGH_PRIORITY |
    UDMA_ATTR_REQMASK);

    /* Setting Control Indexes. In this case we will set the source of the
     * DMA transfer to ADC14 Memory 0
     *  and the destination to the
     * destination data array. */
    DMA_setChannelControl(
    UDMA_PRI_SELECT | DMA_CH7_ADC14,
                          UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
                          UDMA_DST_INC_16 | UDMA_ARB_1);
    DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH7_ADC14,
    UDMA_MODE_PINGPONG,
                           (void*) &ADC14->MEM[0], data_array1, SAMPLE_LENGTH);

    DMA_setChannelControl(
    UDMA_ALT_SELECT | DMA_CH7_ADC14,
                          UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
                          UDMA_DST_INC_16 | UDMA_ARB_1);
    DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
    UDMA_MODE_PINGPONG,
                           (void*) &ADC14->MEM[0], data_array2, SAMPLE_LENGTH);

    /* Assigning/Enabling Interrupts */
    DMA_assignInterrupt(DMA_INT1, 7);
    DMA_assignChannel(DMA_CH7_ADC14);
    DMA_clearInterruptFlag(7);

    /* Now that the DMA is primed and setup, enabling the channels. The ADC14
     * hardware should take over and transfer/receive all bytes */
    DMA_enableChannel(7);
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
