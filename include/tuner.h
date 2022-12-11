#ifndef _TUNER_H_
#define _TUNER_H_

#include <ti/devices/msp432p4xx/inc/msp.h>

void init_hann_window(void);

double pitch_detection(int16_t (*data_array)[]);

/**
 * Return the exact pitch of the note idetified by note_number,
 * where A4 = 0, A# = 1, G# = -1
 */
double note_pitch(int note_number);

int note_number(double pitch);

void play_buzzer();

void stop_buzzer();

/**
 * Return the index of the nearest note to pitch
 */
int note_index(double pitch);

/**
 * Copy the name of the nearest note to pitch into name buffer
 */
void note_name(double pitch, char name[]);

void note_pitch_range(double pitch, double *max_pitch, double *min_pitch);

#endif /* _TUNER_H_ */
