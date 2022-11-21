#pragma once

#include "player.h"
#include "enemy.h"
#include "menu.h"

#define MAX_ENEMY 31
#define MAX_ON_SCREEN 6

typedef struct Game
{
    Menu menu;
    bool is_p2;
    Enemy enemy[MAX_ENEMY];
    Player player1;
    Player player2;
    int score;
    int level;
} Game;

void init_game(Player *p1, Player *p2, Enemy *e, int level);
void game_update(Enemy e[], Player *p1, Player *p2, float deltaTime, int nbEnemy);
