/**
 * @file peripherals.h
 *
 * @brief Functions used to initialize and control the MSP432 hardware
 *
 * @author Samuele Pozzani
 *
 * @date 20/12/2022
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

/**
 * Halt WDT, initialize clock system,
 * set timer_a1 for ADC and timer_a0 for buzzer,
 * set timer32 for buttons debouncing
 */
void init_timer(void);

/**
 * Start timer32 for debouncing (duration of 250ms)
 */
void debounce_button(void);

/**
 * Check whether timer32 has finished counting
 * @return False if timer32 value == 0
 */
bool is_debouncing(void);

/* GPIO */

/**
 * Configure GPIO for microphone, buzzer, S1, S2 and joystick buttons
 */
void init_gpio(void);

/**
 * Set buzzer speed to the desired frequency
 * @param freq The desired frequency
 */
void set_buzzer(int freq);

/* ADC */

/**
 * Set ADC trigger and memory location
 */
void init_adc(void);

/**
 * Set microphone ADC port to ADC MEM0
 */
void set_adc_microphone(void);

/**
 * Set A0 ADC port to ADC MEM0
 */
void set_adc_jack(void);

/* DMA */

#define SAMPLE_LENGTH    1024

/**
 * Init DMA control table, enable DMA module,
 * set channel control and transfer to PingPong mode to match ADC memory location
 */
void init_dma(void);

/* Graphic library */

Graphics_Context g_sContext;

/**
 * Initialize context and clear display
 */
void init_display(void);

/**
 * Draw a straight vertical line on the display
 * @param x The x position of the line
 * @param y1 The starting y position of the line
 * @param y2 The final y position of the line
 * @param color The color of the line in the format aRGB (32 bit)
 */
void draw_line_v(int32_t x, int32_t y1, int32_t y2, int32_t color);

/**
 * Draw a circle on the display
 * @param x The x center position
 * @param y The y center position
 * @param r The radius of the circle
 * @param color The color of the circle (filled) in the format aRGB (32 bit)
 */
void draw_circle(int32_t x, int32_t y, int32_t r, int32_t color);

/**
 * Draw a string on the display
 * @param string The string buffer
 * @param x The x starting position
 * @param y The y starting position
 * @param color The color of the string in the format aRGB (32 bit)
 * @param centered If true, the params (x, y) represent the center of the string, otherwise the top left corner
 * @param big If True, set a big font
 */
void draw_string(char* string, int32_t x, int32_t y, int32_t color, int16_t centered, int16_t big);

#endif /* _PERIPHERALS_H_ */
