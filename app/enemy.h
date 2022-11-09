#pragma once

#include "Tools/axis.h"
#include <stdbool.h>
#include <math.h>
#include <canvas.h>

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

int draw_enemy(Enemy enemy);
Enemy init_enemy(float x, float y, enemyType type, enemySize size);
