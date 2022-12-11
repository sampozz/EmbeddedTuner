/*
 * peripherals.h
 */

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "embedded_tuner/include/LcdDriver/Crystalfontz128x128_ST7735.h"


/* Timers */

#define SMCLK_FREQUENCY     48000000
#define SAMPLE_FREQUENCY    16000
#define BUZZER_CLK (SMCLK_FREQUENCY / 64)

void init_timer(void);
void debounce_button(void);
bool is_debouncing(void);

/* GPIO */

void init_gpio(void);
void set_buzzer(int freq);

/* ADC */

void init_adc(void);


/* DMA */

#define SAMPLE_LENGTH    1024

void init_dma(void);


/* Graphic library */

Graphics_Context g_sContext;

void init_display(void);

#endif /* _PERIPHERALS_H_ */
