#include "embedded_tuner/include/peripherals.h"

void init_gpio(void)
{
    /* Configuring Microphone GPIO 4.3 A10 */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring user button S1 */
    GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    /* Configuring user button S2 */
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);

    /* Configuring joystick button */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);

    /* Configures P2.7 to PM_TA0.4 for using Timer PWM to control the buzzer */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

void set_buzzer(int freq)
{
    TIMER_A0->CCR[0] = BUZZER_CLK / freq;
}

void init_display(void)
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);

    /* Draw Title, x-axis, gradation & labels */
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}

void draw_line_v(int32_t x, int32_t y1, int32_t y2, int32_t color)
{
    Graphics_setForegroundColor(&g_sContext, color);
    Graphics_drawLineV(&g_sContext, x, y1, y2);
}

void draw_circle(int32_t x, int32_t y, int32_t r, int32_t color)
{
    Graphics_setForegroundColor(&g_sContext, color);
    Graphics_fillCircle(&g_sContext, x, y, r);
}

void draw_string(char* string, int32_t x, int32_t y, int32_t color, int16_t centered, int16_t big)
{
    // Clear overlapping strings
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_Rectangle rect = {x - 24, y - 24, x + 24, y + 24};
    Graphics_fillRectangle(&g_sContext, &rect);
    Graphics_setForegroundColor(&g_sContext, color);

    if (big)
    {
        GrContextFontSet(&g_sContext, &g_sFontCmss24);
    }
    else
    {
        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    }
    if (centered)
    {
        Graphics_drawStringCentered(&g_sContext, (int8_t *) string, AUTO_STRING_LENGTH, x, y, OPAQUE_TEXT);
    }
    else
    {
        Graphics_drawString(&g_sContext, (int8_t *) string, AUTO_STRING_LENGTH, x, y, OPAQUE_TEXT);
    }
}




