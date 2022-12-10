#include "embedded_tuner/include/tuner.h"
#include <math.h>

const char notes[12][3] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

double note_pitch(int note_number)
{
    return pow(2, (double)note_number / 12.0) * 440.0;
}

int note_number(double pitch)
{
    double index = 12.0 * log2(pitch / 440.0);
    if (index < 0)
        return index - 0.5;
    return index + 0.5;
}

int note_index(double pitch)
{
    double index = 12.0 * log2(pitch / 440.0);
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
