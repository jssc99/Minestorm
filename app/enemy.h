#pragma once

#include "Tools/axis.h"
#include <stdbool.h>
#include <math.h>
#include <canvas.h>
#include <stdlib.h>
#include <time.h>

#define MAX_RADIUS_MINES 6.0F
#define FLOATING_SMALL_RADIUS 2.0F
#define FIREBALL_MINE_SMALL_RADIUS 3.0F
#define MAGNETIC_SMALL_RADIUS 2.5F
#define MAGNET_FIRE_SMALL_RADIUS 4.5F

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
    BIG
} enemySize;

typedef struct Enemy
{
    Axis2 location;
    bool isBorn;
    float speed;
    enemyType type;
    enemySize size;
    float angle;
} Enemy;

Enemy init_enemy(float x, float y, enemyType type, enemySize size);

float get_size_multiplier(enemySize size);
float get_max_size(enemySize size, enemyType type);

// Move function
int move_any_enemy(Enemy enemy);

// Draw function
int draw_any_enemy(Enemy enemy);