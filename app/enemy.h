#pragma once

#include "Tools/axis.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MINE_BIG_RADIUS 6.0F
#define FLOATING_SMALL_RADIUS 2.0F
#define FIREBALL_MINE_SMALL_RADIUS 3.5F
#define MAGNETIC_SMALL_RADIUS 2.5F
#define MAGNET_FIRE_SMALL_RADIUS 4.5F

#define NB_POINTS_FIREBALL 12   // for draw function
#define MINELAYER_LENGTH_X 36.f // furthest x point from origin
#define MINELAYER_LENGTH_Y 14.f // furthest y point from origin

#define MAX_ENEMY 36            // max enemy possible
#define MIN_ON_SCREEN 3         // max on screen at once

typedef enum enemyType
{
    FLOATING,
    FIREBALL_MINE,  // mine with fireball in it, not to confuse with actual fireball
    MAGNETIC,
    MAGNET_FIRE,
    FIREBALL,
    MINELAYER
} enemyType;

typedef enum enemySize
{
    SMALL,
    MEDIUM,
    BIG,
    FIXED // used for minelayer and fireball enemy
} enemySize;

typedef enum Status
{
    BABY,  // created but not placed
    CHILD, // placed but not moving
    ADULT, // moving around
    DEAD   // DEAD
} Status;

typedef struct Enemy
{
    Axis2 location;
    Status status;
    enemyType type;
    enemySize size;
    float angle;
    int nbPoints;
    Point2 points[10];
    int deathScore;
} Enemy;

// creation functions

void create_minefield(Enemy e[], int nbEnemy, int width, int height);
Enemy init_enemy(Point2 origin, enemyType type, enemySize size);

// game needed functions

bool is_any_enemy_alive(Enemy e[], int nbEnemies);
int how_many_e_adult(Enemy e[], int nbEnemies);
int how_many_e_child(Enemy e[], int nbEnemies);

// debug (primarely) functions

void age_enemy(Enemy *e);
void size_enemy(Enemy *e);
void set_pos_enemy(Enemy *e, float x, float y);

// size functions

float get_size_multiplier(enemySize size);
float get_max_size(enemySize size, enemyType type);
float get_small_size(enemySize size, enemyType type);

// update functions

void update_pos_any_enemy(Enemy *e, Vector2 posPlayer);
void update_pos_all_enemy(Enemy e[], int size, Vector2 posPlayer);

// draw functions

void draw_any_enemy(Enemy *e);
void draw_minefield(Enemy *e, int size);
void draw_all_enemy(Enemy e[], int size);