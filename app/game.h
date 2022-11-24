#pragma once

#include "player.h"
#include "enemy.h"
#include "menu.h"

typedef struct Game
{
    Menu menu;
    Enemy enemy[MAX_ENEMY];
    Player *player1;
    Player *player2;
    int score;
    int level;
    float cptDelta;
} Game;

void init_game(Player *p1, Player *p2, Enemy *e, int level);
void update_game(Enemy e[], Player *p1, Player *p2, float deltaTime, float ctpDeltaTime, int *score);
void draw_loop(Enemy en[], Player *p1, Player *p2);
void save_game(int score);
