#include "game.h"

#include "player.h"
#include "enemy.h"

// enemies
// if floating / magnetic dead / minelayer / fireball == dead
// if fireball_mine / magnet_fire dead = dies and create fireball (in same array pos)
// on death score += points

void asign_role(Enemy e[], int level)
{
    for (int i = 0; i < MAX_ENEMY - 1; i += 5)
    {
        float type = 0;
        if ((level + i) > 35)
            type++;
        if ((level + i) > 40)
            type++;
        if ((level + i) > 45)
            type++;
        e[i + 0] = init_enemy(e[i + 0].location.origin, MINELAYER, BIG);
        e[i + 1] = init_enemy(e[i + 1].location.origin, type, MEDIUM);
        e[i + 2] = init_enemy(e[i + 2].location.origin, type, MEDIUM);
        e[i + 3] = init_enemy(e[i + 3].location.origin, type, SMALL);
        e[i + 4] = init_enemy(e[i + 4].location.origin, type, SMALL);
    }
    e[MAX_ENEMY - 1] = init_enemy(e[MAX_ENEMY - 1].location.origin, MINELAYER, FIXED);
}

// p2 = NULL if 1 player
void init_game(Player *p1, Player *p2, Enemy *e, int level)
{
    if (p2)
    {
        *p1 = player_init(250, 400, 25.f);
        *p2 = player_init(450, 400, 25.f);
    }
    else
        *p1 = player_init(350, 400, 25.f);

    create_minefield(e, MAX_ENEMY, 700, 800);
    asign_role(e, level);
}

void update_game(Enemy e[], Player *p1, Player *p2, float deltaTime, float cptDeltaTime, int nbEnemy)
{
    if (p1->lives)
        update_player(p1, deltaTime, (Point2){700, 800}, 0);
    for (int i = 0; i < MAX_ENEMY; i++)
        player_collision_enemy(p1, e + i);

    if (p2 && (p2->lives > 0))
    {
        update_player(p2, deltaTime, (Point2){700, 800}, 1);
        Enemy split[(nbEnemy / 2)], split2[(nbEnemy / 2)];
        for (int i = 0, j = 0; i < nbEnemy; j++, i += 2)
        { // pas super efficace mais bon.
            split[j] = e[i];
            split2[j] = e[i + 1];
        }
        update_pos_all_enemy(split, nbEnemy / 2, p1->axis.origin);
        update_pos_all_enemy(split2, nbEnemy / 2, p2->axis.origin);
    }
    else
        update_pos_all_enemy(e, nbEnemy, p1->axis.origin);

    int nbEnAdult = 0;
    if (cptDeltaTime > 5.f && (nbEnAdult = how_many_e_adult(e, MAX_ENEMY - 1)) < MAX_ON_SCREEN)
    {
        nbEnAdult = MAX_ON_SCREEN - nbEnAdult;
        for (int i = 0; i < MAX_ENEMY - 1; i += 5)
            if (nbEnAdult && e[i].size == BIG && e[i].status == CHILD)
            {
                e[i].status = ADULT;
                nbEnAdult--;
            }
    }
}
