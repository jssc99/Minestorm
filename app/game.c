#include "game.h"

#include "player.h"
#include "enemy.h"

void asign_role(Enemy en[], int level)
{
    for (int i = 0; i < MAX_ENEMY - 1; i += 7)
    {
        float type = 0;
        if ((level + i) > 35)
            type++;
        if ((level + i) > 40)
            type++;
        if ((level + i) > 45)
            type++;
        en[i + 0] = init_enemy(en[i + 0].location.origin, type, BIG);
        en[i + 1] = init_enemy(en[i + 1].location.origin, type, MEDIUM);
        en[i + 2] = init_enemy(en[i + 2].location.origin, type, SMALL);
        en[i + 3] = init_enemy(en[i + 3].location.origin, type, SMALL);
        en[i + 4] = init_enemy(en[i + 4].location.origin, type, MEDIUM);
        en[i + 3] = init_enemy(en[i + 5].location.origin, type, SMALL);
        en[i + 4] = init_enemy(en[i + 6].location.origin, type, SMALL);
    }
    en[MAX_ENEMY - 1] = init_enemy(en[MAX_ENEMY - 1].location.origin, MINELAYER, FIXED);
}

// p2 = NULL if 1 player
void init_game(Player *p1, Player *p2, Enemy *en, int level)
{
    if (p2)
    {
        *p1 = player_init(250, 400, 25.f);
        *p2 = player_init(450, 400, 25.f);
    }
    else
        *p1 = player_init(350, 400, 25.f);

    create_minefield(en, MAX_ENEMY, 700, 800);
    asign_role(en, level);
}

void test_all_collision(Enemy en[], Player *p1, Player *p2, int *score)
{

    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (player_collision_enemy(p1, &en[i]))
        {
            player_spawn(p1, p1->spawnPoint.x, p1->spawnPoint.y);
            p1->lives--;
            en[i].status = DEAD;
            if (en[i].size == BIG)
            {
                en[i + 1].status = ADULT;
                en[i + 4].status = ADULT;
            }
            else if (en[i].size == MEDIUM)
            {
                en[i + 1].status = ADULT;
                en[i + 2].status = ADULT;
            }
            else if (en[i].size == SMALL)
            {
            }
            else if (en[i].size == FIXED)
            {
            }
        }
        if (p2 && player_collision_enemy(p1, &en[i]))
        {
            player_spawn(p2, p2->spawnPoint.x, p2->spawnPoint.y);
            p2->lives--;
            en[i].status = DEAD;
            if (en[i].size == BIG)
            {
                en[i + 1].status = ADULT;
                en[i + 4].status = ADULT;
            }
            else if (en[i].size == MEDIUM)
            {
                en[i + 1].status = ADULT;
                en[i + 2].status = ADULT;
            }
            else if (en[i].size == SMALL)
            {
            }
            else if (en[i].size == FIXED)
            {
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (p1->bullets[i].lifespan > 0 && bullet_collision_enemy(&p1->bullets[i], &en[i]))
            {
                score += en[i].deathScore;
                if (en[i].size == BIG)
                {
                    en[i + 1].status = ADULT;
                    en[i + 4].status = ADULT;
                }
                else if (en[i].size == MEDIUM)
                {
                    en[i + 1].status = ADULT;
                    en[i + 2].status = ADULT;
                }
                else if (en[i].size == SMALL)
                {
                }
                else if (en[i].size == FIXED)
                {
                }
                if (en[i].type == FIREBALL_MINE || en[i].type == MAGNET_FIRE)
                {
                    en[i].type = FIREBALL;
                    en[i].size = FIXED;
                    en[i].status = ADULT;
                    en[i].nbPoints = 0;
                }
            }
            if (p2 && p2->bullets[i].lifespan > 0 && bullet_collision_enemy(&p2->bullets[i], &en[i]))
            {
            }
            if (/*bullet collision player*/ 0)
            {
            }
        }
    }
}

void update_game(Enemy en[], Player *p1, Player *p2, float deltaTime, float cptDeltaTime, int *score)
{
    if (p1->lives)
        update_player(p1, deltaTime, (Point2){700, 800}, 0);

    test_all_collision(en, p1, p2, score);

    if (p2 && (p2->lives > 0))
    {
        update_player(p2, deltaTime, (Point2){700, 800}, 1);
        Enemy split[50], split2[50];
        for (int i = 0, j = 0; i < (MAX_ENEMY - 1); j++, i += 2)
        {
            split[j] = en[i];
            split2[j] = en[i + 1];
        }
        update_pos_all_enemy(split, (MAX_ENEMY - 1) / 2, p1->axis.origin);
        update_pos_all_enemy(split2, (MAX_ENEMY - 1) / 2, p2->axis.origin);
    }
    else
        update_pos_all_enemy(en, MAX_ENEMY, p1->axis.origin);

    int nbEnAdult = 0;
    if (cptDeltaTime > 5.f && (nbEnAdult = how_many_e_adult(en, MAX_ENEMY - 1)) < MIN_ON_SCREEN)
    {
        nbEnAdult = MIN_ON_SCREEN - nbEnAdult;
        for (int i = 0; i < MAX_ENEMY - 1; i += 7)
            if (nbEnAdult && en[i].size == BIG && en[i].status == CHILD)
            {
                en[i].status = ADULT;
                nbEnAdult--;
            }
    }
}
