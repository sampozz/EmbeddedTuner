/**
 * @file gui.h
 *
 * @brief High level graphics functions that could be used on any display
 *
 * @author Samuele Pozzani
 *
 * @date 20/12/2022
 */

#ifndef _GUI_H_
#define _GUI_H_

#include "embedded_tuner/include/peripherals.h"

/* Colors defined as alphaRGB */
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define BLUE 0x000000FF

/**
 * Draw the guidelines used by the tuner cursor on the screen 
 */
void draw_tuner_lines(void);

/**
 * Draw the cursor that shows how much the pitch is distant from the correct value
 * @param position Position of the cursor on the x axis of the screen
*/
void draw_tuner_cursor(int32_t position);

/**
 * Writes the value of the reference pitch on the top left of the screen
 * @param pitch The value of the reference pitch
 */
void draw_reference_pitch(int32_t pitch);

/**
 * Writes the name of the detected note on the screen as a big letter
 * @param note The name (string) of the note
 */
void draw_note(char note[]);

/**
 * Draw the string "Playing buzzer"
 * @param note The note being played by the buzzer
 */
void draw_buzzer_mode(char note[]);


#endif /* _GUI_H_ */
