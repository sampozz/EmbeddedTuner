#include <embedded_tuner/include/yin.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/tuner.h"

extern int16_t (*data_array)[];
extern int16_t data_array1[];
extern int16_t data_array2[];

extern int16_t mode; // 0: tuner, 1: buzzer
extern int16_t input; // 0: mic, 1: jack
extern double reference_pitch;
extern int16_t buzzer_note_number;
extern int16_t display_updated;
extern int16_t mode_changed;

/**
 * This interrupt is called whenever ADC completes the conversion of SAMPLE_LENGTH samples.
 * Switch primary to alternate buffers of DMA (and vice-versa) in pingpong mode and set data_array pointer
 */
void DMA_INT1_IRQHandler(void)
{
    /* Switch between primary and alternate buffers with DMA's PingPong mode */
    if (DMA_getChannelAttribute(7) & UDMA_ATTR_ALTSELECT)
    {
        DMA_setChannelControl(
        UDMA_PRI_SELECT | DMA_CH7_ADC14,
                              UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
                              UDMA_DST_INC_16 | UDMA_ARB_1);
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
                              UDMA_SIZE_16 | UDMA_SRC_INC_NONE |
                              UDMA_DST_INC_16 | UDMA_ARB_1);
        DMA_setChannelTransfer(UDMA_ALT_SELECT | DMA_CH7_ADC14,
        UDMA_MODE_PINGPONG,
                               (void*) &ADC14->MEM[0], data_array2,
                               SAMPLE_LENGTH);
        data_array = &data_array2;
    }
}

/**
 * This interrupt is called whenever button S2 is pressed.
 * Depending on current mode, decrease pitch reference or note on buzzer.
 */
void PORT3_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
    if (is_debouncing())
        return;

    if (status & GPIO_PIN5)
    {
        debounce_button();
        display_updated = 1;
        if (mode == 0)
        {
            /* Decrement reference frequency */
            reference_pitch--;
        }
        else
        {
            /* Lower pitch buzzer */
            if (--buzzer_note_number == -21)
                buzzer_note_number = -20;
            play_buzzer(buzzer_note_number);
        }
    }
}

/**
 * This interrupt is called whenever the joystick button is pressed.
 * Toggle mode from tuner to buzzer and vice-versa.
 */
void PORT4_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);
    if (is_debouncing())
        return;

    if (status & GPIO_PIN1)
    {
        debounce_button();
        mode_changed = 1;
        display_updated = 1;
        /* Toggle buzzer */
        if (mode == 0)
        {
            mode = 1;
            buzzer_note_number = 0;
            play_buzzer(buzzer_note_number);
        }
        else
        {
            mode = 0;
            stop_buzzer();
        }
    }
}

/**
 * This interrupt is called whenever button S1 is pressed.
 * Depending on current mode, increase pitch reference or note on buzzer.
 */
void PORT5_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    if (is_debouncing())
        return;

    if (status & GPIO_PIN1)
    {
        debounce_button();
        display_updated = 1;
        if (mode == 0)
        {
            /* Increment reference frequency */
            reference_pitch++;
        }
        else
        {
            /* Higher pitch buzzer */
            if (++buzzer_note_number == 21)
                buzzer_note_number = 20;
            play_buzzer(buzzer_note_number);
        }
    }
}

/**
 * This interrupt is called whenever the left button on the board is pressed.
 * Toggle tuner mode from microphone to jack and vice-versa.
 */
void PORT1_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    if (is_debouncing())
        return;

    if (status & GPIO_PIN1)
    {
        debounce_button();
        display_updated = 1;
        if (input == 0)
        {
            input = 1;
            /* Enable jack */
            set_adc_jack();
        }
        else
        {
            input = 0;
            /* Enable Mic */
            set_adc_microphone();
        }
    }
}
