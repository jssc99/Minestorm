#include "enemy.h"
//#include "draw_enemy.c"
//#include "move_enemy.c"

float get_size_multiplier(enemySize size)
{
    switch (size)
    {
    case SMALL:
        return 2.f;
    case MEDIUM:
        return 4.f;
    case BIG:
        return 6.f;

    default:
        return 0.f;
    }
}

float get_max_size(enemySize size, enemyType type)
{
    float multiplier = get_size_multiplier(size);

    switch (type)
    {
    case FLOATING:
    case FIREBALL_MINE:
    case MAGNETIC:
    case MAGNET_FIRE:
        return MAX_RADIUS_MINES * multiplier;

    case FIREBALL:
        return multiplier;

    case MINELAYER:
        return 11.7 * multiplier; // 11.7 = distance origin to furthest point from origin of figure
    }
    return 0.f;
}

Enemy init_enemy(float x, float y, enemyType type, enemySize size)
{
    Enemy enemy;
    enemy.location = (Axis2){{x, y}, {1.f, 0.f}, {0.f, 1.f}};
    enemy.isBorn = false;
    enemy.speed = 0.f;
    enemy.type = type;

    switch (type)
    {
    case MINELAYER:
        enemy.size = MEDIUM;
        break;
    case FIREBALL:
        enemy.size = SMALL;
        break;

    default:
        enemy.size = size;
        break;
    }

    if (type == FLOATING || type == FIREBALL_MINE)
    {
        srand(time(NULL));
        enemy.angle = ((rand() % 360) / M_PI);
    } // ADD ? IF TYPE = MAGNET, ANGLE = PLAYER POS
    else
        enemy.angle = 0.f;
    return enemy;
}