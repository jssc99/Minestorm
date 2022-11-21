#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

typedef enum Menu
{
    PLAY,
    IN_GAME,
    PAUSE,
    GAMEOVER
} Menu;

void draw_menu(Menu menu, ImFont *font, int score, bool p2, int p1_lives, int p2_lives);
