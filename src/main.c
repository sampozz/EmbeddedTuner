#include <embedded_tuner/include/yin.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/gui.h"
#include "embedded_tuner/include/tuner.h"
#include <stdio.h>
#include <math.h>

/* processing buffer */
int16_t (*data_array)[SAMPLE_LENGTH];

int mode = 0; // 0: tuner, 1: buzzer

extern double reference_pitch;

void _hw_init(void)
{
    /* Configure flash to support 48MHz master clock */
    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    init_timer();
    init_gpio();
    init_adc();
    init_dma();
    init_display();

    /* Interrupts enable */
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_DMA_INT1);
    Interrupt_enableMaster();

    ADC14_enableConversion();
}

int main(void)
{
    _hw_init();
    init_hann_window();

    while(1)
    {
        PCM_gotoLPM0();

        double pitch = pitch_detection(data_array);

        draw_tuner_lines();
        char string[20], note[3];

        /* Display reference pitch */

        draw_reference_pitch(reference_pitch);

        /* Display note */

        if (pitch == -1) continue;
        note_name(pitch, note);
        draw_note(note);

        /* Display tuning bar */

        double max_pitch, min_pitch;
        note_pitch_range(pitch, &max_pitch, &min_pitch);
        int cursor_pos = 128 * (pitch - min_pitch) / (max_pitch - min_pitch);
        draw_tuner_cursor(cursor_pos);
    }
}
