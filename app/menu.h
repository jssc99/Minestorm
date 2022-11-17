#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

typedef enum Menu
{
    PAUSE,
    GAME,
    GAMEOVER,
    PLAY,
    OTHER
} Menu;

void draw_menu(Menu menu);
