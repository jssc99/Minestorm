#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

typedef enum Menu
{
    MAIN,
    IN_GAME,
    PAUSE,
    SUCCESS,
    GAMEOVER
} Menu;

void draw_menu(Menu menu, ImFont *font, ImTextureID logoId, int score, int level, bool p2, int p1_lives, int p2_lives);