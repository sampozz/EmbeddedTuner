#include <embedded_tuner/include/yin.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "embedded_tuner/include/peripherals.h"
#include "embedded_tuner/include/gui.h"
#include "embedded_tuner/include/tuner.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/* processing buffer */
int16_t (*data_array)[SAMPLE_LENGTH];
int16_t mode = 0; // 0: tuner, 1: buzzer
int16_t input = 0; // 0: mic, 1: jack
int16_t display_updated = 1;
int16_t mode_changed = 1;
int16_t buzzer_note_number = 0;

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
    Interrupt_enableInterrupt(INT_PORT1);
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

    while (1)
    {
        PCM_gotoLPM0();

        char note[3];

        if (mode_changed)
        {
            mode_changed = 0;
            clear_screen();
        }

        /* GUI updates on display */
        if (display_updated)
        {
            display_updated = 0;

            if (mode == 1)
            {
                /* Display playing note name */
                double pitch = note_pitch(buzzer_note_number);
                note_name(pitch, note);
                draw_buzzer_mode(note);
            }
            else
            {
                /* Display input mode */
                if (input == 0)
                    draw_input_mode("Mic");
                else
                    draw_input_mode("Jack");
            }

            draw_reference_pitch(reference_pitch);
        }

        /* Note detection and display */
        if (mode == 0) // Tuner mode
        {
            /* Find Pitch */
            double pitch = pitch_detection(data_array);

            draw_tuner_lines();

            if (pitch == -1 || pitch > 148 && pitch < 148.8)
            {
                /* Do not display note */
                strcpy(note, " ");
            }
            else
            {
                /* Display tuning cursor */
                double max_pitch, min_pitch;
                note_pitch_range(pitch, &max_pitch, &min_pitch);
                int cursor_pos = 128 * (pitch - min_pitch)
                        / (max_pitch - min_pitch);
                draw_tuner_cursor(cursor_pos);

                /* Get detected note name */
                note_name(pitch, note);
            }

            draw_note(note);
        }
    }
}
