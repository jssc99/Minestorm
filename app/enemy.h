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
    float kSize;
    float angle;
} Enemy;

Enemy init_enemy(float x, float y, enemyType type, enemySize size);
float current_enemy_size(float size, enemyType type);
int draw_any_enemy(Enemy enemy);
int move_any_enemy(Enemy enemy);