#include "embedded_tuner/include/tuner.h"
#include <math.h>

const char notes[12][3] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

double note_pitch(int note_index)
{
    return pow(2, (double)note_index / 12.0) * 440.0;
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
