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

void loop_game()
{
}
