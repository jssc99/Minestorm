#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "game.h"
#include "audio.h"
#include "../include/miniaudio.h"

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
    ma_result result;
    ma_engine engine;
} App;

typedef struct Music
{
    ma_sound* current;
    ma_sound menu;
    ma_sound game;
    ma_sound lose;
    ma_sound win;
} Music;

void music_stop(Music* music);
void music_play(Music *music);
void music_change(ma_sound* toPlay, Music *music);

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
