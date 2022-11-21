#include "game.h"

#include "player.h"
#include "enemy.h"

// begin
// display menu
// chose game 1p / 2p
// // if 1p
// init player pos central
// enemy array size x
// minefield enemies
// // if 2p
// init player1 pos central left
// init player2 pos central right
// enemy array size x
// minefield enemies

// run
// display menu
// update player(s)
// update mines
// // if magnet follows p1 if placement array (i) even, else p2 (if p2 && i % 2)
// until lives p1 p2 == 0 || every mine == DEAD
// display game over
// exit / restart

// enemies
// if floating / magnetic dead / minelayer / fireball == dead
// if fireball_mine / magnet_fire dead = dies and create fireball (in same array pos)
// on death score += points

void asign_role(Enemy e[], int level)
{
    for (int i = 0; i < MAX_ENEMY - 1; i += 5)
    {
        e[i + 0] = init_enemy(e[i + 0].location.origin, FLOATING, BIG);
        e[i + 1] = init_enemy(e[i + 1].location.origin, FLOATING, MEDIUM);
        e[i + 2] = init_enemy(e[i + 2].location.origin, FLOATING, MEDIUM);
        e[i + 3] = init_enemy(e[i + 3].location.origin, FLOATING, SMALL);
        e[i + 4] = init_enemy(e[i + 4].location.origin, FLOATING, SMALL);
    }
    e[MAX_ENEMY - 1] = init_enemy(e[MAX_ENEMY - 1].location.origin, MINELAYER, FIXED);
}

// p2 = NULL if 1 player
void init_game(Player *p1, Player *p2, Enemy *e, int level)
{
    if (p2)
    {
        *p1 = player_init(250, 400, 2.f);
        *p2 = player_init(450, 400, 2.f);
    }
    else
        *p1 = player_init(350, 400, 2.f);

    create_minefield(e, MAX_ENEMY, 700, 800);
    asign_role(e, level);
}

void game_update(Enemy e[], Player *p1, Player *p2, float deltaTime, int nbEnemy)
{
    if (p1->lives)
        update_player(p1, deltaTime, (Point2){700, 800}, 0);
    if (p2 && p2->lives)
    {
        update_player(p2, deltaTime, (Point2){700, 800}, 1);
        Enemy split[(nbEnemy / 2)];
        for (int i = 0, j = 0; i < nbEnemy; j++, i += 2) // pas efficace mais bon..
            split[j] = e[i];
        update_pos_all_enemy(e, nbEnemy, p2->axis.origin);
        update_pos_all_enemy(split, nbEnemy / 2, p1->axis.origin);
    }
    else
        update_pos_all_enemy(e, nbEnemy, p2->axis.origin);
}
