#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "game.h"

typedef struct App
{
    CvTexture backg;
    Point2 maxScreen;
    ImFont *font;
    int width;
    int height;
    bool closeApp;
    bool debugMenu;

    // enemy debug    
    bool movePointE;
    bool movePointP;
    int id;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
