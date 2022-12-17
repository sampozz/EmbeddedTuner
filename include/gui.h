#ifndef _GUI_H_
#define _GUI_H_

#include "embedded_tuner/include/peripherals.h"

#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define BLUE 0x000000FF

void draw_tuner_lines(void);

void draw_tuner_cursor(int32_t position);

void draw_reference_pitch(int32_t pitch);

void draw_note(char* note);


#endif /* _GUI_H_ */
