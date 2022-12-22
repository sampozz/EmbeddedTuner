#include "embedded_tuner/include/yin.h"
#include "embedded_tuner/include/tuner.h"
#include "embedded_tuner/include/peripherals.h"
#include <math.h>

const char notes[12][3] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
float hann[SAMPLE_LENGTH];
double reference_pitch = 440.0;

void init_hann_window(void)
{
    int n;
    for(n = 0; n < SAMPLE_LENGTH; n++)
        hann[n] = 0.5f - 0.5f * cosf((2 * M_PI * n) / (SAMPLE_LENGTH - 1));
}

double pitch_detection(int16_t (*data_array)[])
{
    /* Apply Hann window */
    int i = 0;
    for(i = 0; i < SAMPLE_LENGTH; i++)
        (*data_array)[i] = (int16_t)(hann[i] * (*data_array)[i]);

    Yin yin;
    Yin_init(&yin, SAMPLE_LENGTH, SAMPLE_FREQUENCY, 0.05);
    return Yin_getPitch(&yin, *data_array);
}

double note_pitch(int note_number)
{
    return pow(2, (double)note_number / 12.0) * reference_pitch;
}

int note_number(double pitch)
{
    double index = 12.0 * log2(pitch / reference_pitch);
    if (index < 0)
        return index - 0.5;
    return index + 0.5;
}

void play_buzzer(int note_number)
{
    set_buzzer(note_pitch(note_number));
}

void stop_buzzer()
{
    set_buzzer(0);
}

int note_index(double pitch)
{
    double index = 12.0 * log2(pitch / reference_pitch);
    int int_index = index + 120.5;
    return int_index % 12;
}

void note_name(double pitch, char name[])
{
    int i = note_index(pitch);
    strcpy(name, notes[i]);
}

void note_pitch_range(double pitch, double *max_pitch, double *min_pitch)
{
    int note = note_number(pitch);
    double correct_pitch = note_pitch(note);

    int prev_note = note - 1;
    int next_note = note + 1;

    *min_pitch = (note_pitch(prev_note) + correct_pitch) / 2.0;
    *max_pitch = (note_pitch(next_note) + correct_pitch) / 2.0;
}
