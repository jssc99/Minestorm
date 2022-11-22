#pragma once

#include "player.h"
#include "enemy.h"
#include "menu.h"

#define MAX_ENEMY 36
#define MIN_ON_SCREEN 3

typedef struct Game
{
    Menu menu;
    bool is_p2;
    Enemy enemy[MAX_ENEMY];
    Player player1;
    Player player2;
    int score;
    int level;
    float cptDelta;
} Game;

void init_game(Player *p1, Player *p2, Enemy *e, int level);
void update_game(Enemy e[], Player *p1, Player *p2, float deltaTime, float ctpDeltaTime, int nbEnemy, int *score);
void start_minelayer(Enemy *e);
