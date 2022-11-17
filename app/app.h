#pragma once

#include "player.h"
#include "enemy.h"
#include "menu.h"

typedef struct Game
{
    Menu menu;
    Enemy enemies[6];
    Player player1;
} Game;

typedef struct App
{
    bool closeGame;
    bool debugMenu;
    int width;
    int height;

    // enemy debug
    unsigned int id;
    bool movePoint;
    bool rotate;
    float angle;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
