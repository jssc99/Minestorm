#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "game.h"

typedef struct App
{
    bool closeGame;
    bool debugMenu;
    int width;
    int height;
    Point2 maxScreen;
    ImFont *font;

    // enemy debug
    unsigned int id;
    bool movePointE;
    bool movePointP;
    bool rotate;
    float angle;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
