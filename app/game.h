#pragma once

#include "player.h"
#include "enemy.h"
#include "menu.h"

#define MAX_ENEMY 31
#define MAX_ON_SCREEN 6

typedef struct Game
{
    Menu menu;
    Enemy enemy[MAX_ENEMY];
    Player player1;
    bool is_p2;
    Player player2;
    int score;
    int level;
} Game;

void init_game(Player *p1, Player *p2, Enemy *e, int level);