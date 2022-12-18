#ifndef _TUNER_H_
#define _TUNER_H_

#include <ti/devices/msp432p4xx/inc/msp.h>

/**
 * Initialize the hann window array to perform Hann smoothing on the audio signal
 */
void init_hann_window(void);

/**
 * Apply Hann window (it needs to be initialized before) and apply Yin algorithm on the audio signal
 * @param data_array Pointer to the array containing the audio signal 
 * @return Pitch detected from the audio signal
 */
double pitch_detection(int16_t (*data_array)[]);

/**
 * Return the exact pitch of the note idetified by note_number, based on the reference pitch,
 * where A4 = 0, A# = 1, G# = -1, etc...
 * @param note_number The number of the note to compute the correct pitch 
 * @return The correct pitch from the note number, based on the reference frame 
 */
double note_pitch(int note_number);

/**
 * Search for the note number nearest to the pitch parameter
 * @param pitch The pitch of the note to be found
 * @return Note number from the pitch 
 */
int note_number(double pitch);

/**
 * Starts the buzzer, to play the frequency from the note number
 * @param note_number The number of the note to play the sound
*/
void play_buzzer(int note_number);

/**
 * Stop the buzzer by setting the playing frequency to zero
 */
void stop_buzzer(void);

/**
 * Return the index of the nearest note to pitch, used to match the index on the notes (names) array
 * @param pitch The pitch of the note to be found
 * @return the index of the note from 0 to 12
 */
int note_index(double pitch);

/**
 * Copy the name of the nearest note to pitch into name buffer, uses note_index()
 * @param pitch The pitch of the note to be found
 * @param name The name buffer in which to put the note name
 */
void note_name(double pitch, char name[]);

/**
 * Compute the minimum and maximum pitch of the nearest note to pitch paramater
 * @param pitch The pitch of the note to be found
 * @param max_pitch The maximum pitch of the same note
 * @param min_pitch The minimum pitch of the same note
 */
void note_pitch_range(double pitch, double *max_pitch, double *min_pitch);

#endif /* _TUNER_H_ */
