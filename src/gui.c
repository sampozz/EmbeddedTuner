#include "embedded_tuner/include/gui.h"

void draw_tuner_lines(void)
{
    int i = 0;
    draw_line_v(0, 120, 72, WHITE);
    for (i = 1; i < 64; i++)
    {
        if (i % 4 == 0)
        {
            draw_line_v(i, 112 - (70 - i) / 4, 80 + (70 - i) / 4, BLACK);
            draw_line_v(i, 112, 112 - (70 - i) / 4 + 1, WHITE);
            draw_line_v(i,  80, 80 + (70 - i) / 4 - 1, WHITE);
        }
        else
        {
            draw_line_v(i, 112, 80, WHITE);
        }
    }

    draw_line_v(64, 120, 72, BLACK);

    for (i = 65; i < 128; i++)
    {
        if (i % 4 == 0)
        {
            draw_line_v(i, 112 + (60 - i) / 4, 80 - (60 - i) / 4, BLACK);
            draw_line_v(i, 112, 112 - (i - 60) / 4 + 1, WHITE);
            draw_line_v(i,  80, 80 + (i - 60) / 4 - 1, WHITE);
        }
        else
        {
            draw_line_v(i, 112, 80, WHITE);
        }
    }
}

void draw_tuner_cursor(int32_t position)
{
    int i = 0;
    for (i = -2; i < 3; i++)
    {
        draw_line_v(position + i, 102, 90, BLUE);
    }
}

void draw_reference_pitch(int32_t pitch)
{
    char string[20];
    sprintf(string, "%d Hz", (int)pitch);
    draw_string(string, 10, 10, BLACK, 0, 0);
}


void draw_note(char note[])
{
    draw_string(note, 64, 44, BLACK, 1, 1);
}
