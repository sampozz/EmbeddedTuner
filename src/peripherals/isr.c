#include <embedded_tuner/include/yin.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/tuner.h"

extern int16_t (*data_array)[];
extern int16_t data_array1[];
extern int16_t data_array2[];

extern int mode; // 0: tuner, 1: buzzer
extern double reference_pitch;
extern int buzzer_note_number;

/* Completion interrupt for ADC14 MEM0 */
void DMA_INT1_IRQHandler(void)
{
    /* Switch between primary and alternate bufferes with DMA's PingPong mode */
    if(DMA_getChannelAttribute(7) & UDMA_ATTR_ALTSELECT)
    {
        DMA_setChannelControl(
            UDMA_PRI_SELECT | DMA_CH7_ADC14,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
            UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH7_ADC14,
                               UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
                               data_array1, SAMPLE_LENGTH);
        data_array = &data_array1;
    }
    else
    {
        DMA_setChannelControl(
            UDMA_ALT_SELECT | DMA_CH7_ADC14,
            UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
            UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
                               UDMA_MODE_PINGPONG, (void*) &ADC14->MEM[0],
                               data_array2, SAMPLE_LENGTH);
        data_array = &data_array2;
    }
}

void PORT3_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    if (status & GPIO_PIN5) {
        /* Decrement reference frequency */
        if (mode == 0) reference_pitch--;
        /* Lower pitch buzzer */
        else
        {
            buzzer_note_number--;
            play_buzzer();
        }
    }
}

void PORT4_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    if (status & GPIO_PIN1) {
        /* Toggle buzzer */
        if (mode == 0)
        {
            mode = 1;
            buzzer_note_number = 0;
            play_buzzer();
        }
        else
        {
            mode = 0;
            stop_buzzer();
        }
    }
}

void PORT5_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN1) {
        /* Increment reference frequency */
        if (mode == 0) reference_pitch++;
        /* Higher pitch buzzer */
        else
        {
            buzzer_note_number++;
            play_buzzer();
        }
    }
}
