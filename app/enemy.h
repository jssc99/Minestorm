#pragma once

#include "Tools/axis.h"
#include <stdbool.h>
#include <math.h>
//#include <canvas.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"

#define MINE_BIG_RADIUS 6.0F
#define FLOATING_SMALL_RADIUS 2.0F
#define FIREBALL_MINE_SMALL_RADIUS 3.5F
#define MAGNETIC_SMALL_RADIUS 2.5F
#define MAGNET_FIRE_SMALL_RADIUS 4.5F

#define NB_POINTS_FIREBALL 12
#define MINELAYER_LENGTH_X 36.f
#define MINELAYER_LENGTH_Y 14.f

typedef enum enemyType
{
    FLOATING,
    FIREBALL_MINE,
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
    FIXED
} enemySize;

typedef enum Status
{
    BABY,
    CHILD,
    ADULT,
    DEAD
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

Enemy init_enemy(Vector2 pPos, enemyType type, enemySize size);
void create_minefield(Enemy e[], int nbEnemy, int width, int height);

void age_enemy(Enemy *e);
void size_enemy(Enemy *e);
void set_pos_enemy(Enemy *e, float x, float y);

float get_size_multiplier(enemySize size);
float get_max_size(enemySize size, enemyType type);
float get_small_size(enemySize size, enemyType type);

// Update func
void update_pos_any_enemy(Enemy *e, Vector2 posPlayer);

// Draw function
void draw_any_enemy(Enemy *e);