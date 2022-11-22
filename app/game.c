#include "game.h"

#include "player.h"
#include "enemy.h"

// gives out roles to enemies (type depends on level)
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
        en[i + 5] = init_enemy(en[i + 5].location.origin, type, SMALL);
        en[i + 6] = init_enemy(en[i + 6].location.origin, type, SMALL);
    }
    en[MAX_ENEMY - 1] = init_enemy(en[MAX_ENEMY - 1].location.origin, MINELAYER, FIXED);
}

// initialises player(s) and enemies
// p2 = NULL if 1 player game
void init_game(Player *p1, Player *p2, Enemy *en, int level)
{
    if (level == 1)
    {
        if (p2)
        {
            *p1 = player_init(250, 400, 25.f);
            *p2 = player_init(450, 400, 25.f);
        }
        else
            *p1 = player_init(350, 400, 25.f);
    }
    else
    {
        if (p2)
        {
            player_spawn(p1, 250, 400);
            player_spawn(p2, 450, 400);
        }
        else
            player_spawn(p1, 350, 400);
    }
    create_minefield(en, MAX_ENEMY, 700, 800);
    asign_role(en, level);
}

// if a player collisions an enemy
void p_col_en(Player *p, Enemy *en)
{
    player_spawn(p, p->spawnPoint.x, p->spawnPoint.y);
    p->lives--;
    en->status = DEAD;
    if (en->size == BIG)
    {
        (en + 1)->status = ADULT;
        (en + 4)->status = ADULT;
    }
    else if (en->size == MEDIUM)
    {
        (en + 1)->status = ADULT;
        (en + 2)->status = ADULT;
    } /*
     else if (en->size == SMALL)
     {
     }
     else if (en->size == FIXED)
     {
     }*/
}

// if a bullet collisions an enemy
void b_col_en(Bullet *b, Enemy *en, int *score)
{
    score += en->deathScore;
    if (en->size == BIG)
    {
        (en + 1)->status = ADULT;
        (en + 4)->status = ADULT;
    }
    else if (en->size == MEDIUM)
    {
        (en + 1)->status = ADULT;
        (en + 2)->status = ADULT;
    } /*
     else if (en->size == SMALL)
     {
     }
     else if (en->size == FIXED)
     {
     }*/
    if (en->type == FIREBALL_MINE || en->type == MAGNET_FIRE)
    {
        en->type = FIREBALL;
        en->size = FIXED;
        en->status = ADULT;
        en->nbPoints = 0;
    }
    else
        en->status = DEAD;
}

// tries all the collision possible in the game
void test_all_collision(Enemy en[], Player *p1, Player *p2, int *score)
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (player_collision_enemy(p1, &en[i]))
            p_col_en(p1, &en[i]);

        for (int j = 0; j < MAX_BULLETS; j++)
            if (p1->bullets[j].lifespan > 0 && bullet_collision_enemy(&p1->bullets[j], &en[i]))
                b_col_en(&p1->bullets[j], &en[i], score);
    }
    if (p2)
    {
        for (int i = 0; i < MAX_ENEMY; i++)
        {
            if (p2 && player_collision_enemy(p1, &en[i]))
                p_col_en(p2, &en[i]);

            for (int j = 0; j < MAX_BULLETS; j++)
                if (p2 && p2->bullets[j].lifespan > 0 && bullet_collision_enemy(&p2->bullets[j], &en[i]))
                    b_col_en(&p2->bullets[j], &en[i], score);
        }
        if (/*bullet collision player*/ 0)
        {
        }
    }
}

// all update functions needed for the game
void update_game(Enemy en[], Player *p1, Player *p2, float deltaTime, float cptDeltaTime, int *score)
{
    test_all_collision(en, p1, p2, score);

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

    if (p1->lives > 0)
        update_player(p1, deltaTime, (Point2){700, 800}, 0, en);
    if (p2 && (p2->lives > 0))
    {
        update_player(p2, deltaTime, (Point2){700, 800}, 1, en);
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
}

// all usefull draw function for the game
void draw_loop(Enemy en[], Player *p1, Player *p2)
{
    draw_all_enemy(en, MAX_ENEMY);
    if (p1->lives > 0)
    {
        draw_player(*p1, 1);
        draw_players_bullets(p1);
    }
    if (p2 && (p2->lives > 0))
    {
        draw_player(*p2, 2);
        draw_players_bullets(p2);
    }
}