#include <embedded_tuner/include/yin.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/tuner.h"
#include <stdio.h>

#include <math.h>

/* processing buffers*/
int16_t (*data_array)[SAMPLE_LENGTH];
extern uint16_t resultsBuffer;

void _hw_init(void)
{
    /* Configure flash to support 48MHz master clock */
    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    init_adc();

    init_gpio();
    init_timer();
    init_dma();
    init_display();

    /* Enabling Interrupts */
//    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_DMA_INT1);
    Interrupt_enableMaster();

    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
}

int main(void)
{
    _hw_init();
    dsp_init();

    while (1)
    {
        PCM_gotoLPM0();

        double pitch = pitch_detection(data_array);

        /* Update display */

        char string[20];

        if (pitch == -1)
            continue;

        char note[3];
        note_name(pitch, note);

        double max_pitch, min_pitch;
        note_pitch_range(pitch, &max_pitch, &min_pitch);
        int cursor_pos = 128 * (pitch - min_pitch) / (max_pitch - min_pitch);

        sprintf(string, "%s", note);

        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) string, 5, 48, 50,
        OPAQUE_TEXT);

        int i;
        for (i = 0; i < 10; i++)
        {
            Graphics_drawPixel(&g_sContext, cursor_pos, 80 + i);
            Graphics_drawPixel(&g_sContext, cursor_pos + 1, 80 + i);
        }

        sprintf(string, "Y: %5d", resultsBuffer);
        Graphics_drawStringCentered(&g_sContext, (int8_t*) string, 8, 64, 70,
        OPAQUE_TEXT);

    }
}




