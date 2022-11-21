#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

typedef enum Menu
{
    PAUSE,
    IN_GAME,
    GAMEOVER,
    PLAY,
    OTHER
} Menu;

void draw_menu(Menu menu, ImFont *font, int score, bool p2, int p1_lives, int p2_lives);
