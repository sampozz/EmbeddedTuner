#include "embedded_tuner/include/gui.h"

void draw_tuner_lines(void)
{
    int i = 0;
    draw_line_v(0, 120, 72, WHITE);
    for (i = 1; i < 64; i++)
    {
        if (i == 48)
        {
            draw_line_v(i, 112 - (50 - i) / 3, 80 + (50 - i) / 3, BLACK);
            draw_line_v(i, 112, 112 - (50 - i) / 3 + 1, WHITE);
            draw_line_v(i,  80, 80 + (50 - i) / 3 - 1, WHITE);
        }
        else if (i % 4 == 0)
        {
            draw_line_v(i, 112 - (70 - i) / 4, 80 + (70 - i) / 4, GRAY);
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
        if (i == 80)
        {
            draw_line_v(i, 112 + (75 - i) / 3, 80 - (75 - i) / 3, BLACK);
            draw_line_v(i, 112, 112 - (i - 75) / 3 + 1, WHITE);
            draw_line_v(i,  80, 80 + (i - 75) / 3 - 1, WHITE);
        }
        else if (i % 4 == 0)
        {
            draw_line_v(i, 112 + (60 - i) / 4, 80 - (60 - i) / 4, GRAY);
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
    draw_circle(position, 96, 6, BLUE);
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

void draw_buzzer_mode(char note[])
{
    draw_string(note, 64, 50, BLACK, 1, 1);
    draw_string("Playing buzzer", 64, 96, BLACK, 1, 0);
}

void draw_input_mode(char mode[])
{
    draw_string(mode, 96, 10, BLACK, 0, 0);
}
