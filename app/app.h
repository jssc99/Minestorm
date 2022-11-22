#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "game.h"

typedef struct App
{
    bool closeApp;
    bool debugMenu;
    int width;
    int height;
    Point2 maxScreen;
    ImFont *font;

    // enemy debug
    unsigned int id;
    bool movePointE;
    bool movePointP;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
