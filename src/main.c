#include <embedded_tuner/include/yin.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/tuner.h"
#include <stdio.h>

#include <math.h>

/* processing buffers*/
float hann[SAMPLE_LENGTH];
int16_t (*data_array)[SAMPLE_LENGTH];

void _hw_init(void)
{
    /* Configure flash to support 48MHz master clock */
    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    init_timer();
    init_microphone();
    init_dma();
    init_display();

    ADC14_enableConversion();
}

int main(void)
{
    _hw_init();

    // Initialize Hann Window
    int n;
    for(n = 0; n < SAMPLE_LENGTH; n++)
    {
        hann[n] = 0.5f - 0.5f * cosf((2 * M_PI * n) / (SAMPLE_LENGTH - 1));
    }

    /*******************/
    /* PITCH DETECTION */
    /*******************/

    double pitch;

    while(1)
    {
        PCM_gotoLPM0();
        int i = 0;

        /* Computer real FFT using the completed data buffer */
        for(i = 0; i < SAMPLE_LENGTH; i++)
        {
            (*data_array)[i] = (int16_t)(hann[i] * (*data_array)[i]);
        }

        Yin yin;
        Yin_init(&yin, SAMPLE_LENGTH, SAMPLE_FREQUENCY, 0.05);
        pitch = Yin_getPitch(&yin, *data_array);

        if (pitch == -1) continue;

        char note[3];
        note_name(pitch, note);

        char string[20];
        sprintf(string, "%s", note);

        Graphics_clearDisplay(&g_sContext);
        Graphics_drawStringCentered(&g_sContext, (int8_t *) string, 3, 48, 70, OPAQUE_TEXT);
    }
}
