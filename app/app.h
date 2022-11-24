#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "game.h"

typedef struct App
{
    CvTexture backg;
    CvTexture logo;
    Point2 maxScreen;
    ImFont *font;
    int width;
    int height;
    bool closeApp;
    bool debugMenu;
    bool movePointE;
    bool hideSAT;
    int id;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
