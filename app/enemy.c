#include "enemy.h"

float get_size_multiplier(enemySize size)
{
    switch (size)
    {
    case SMALL:
        return 2.f;
    case MEDIUM:
    case FIXED:
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
        return MINE_BIG_RADIUS * multiplier;

    case FIREBALL:
        return multiplier;

    case MINELAYER:
        return 11.7 * multiplier; // 11.7 = distance origin to furthest point from origin of figure
    }
    return 0.f;
}

float get_small_size(enemySize size, enemyType type)
{
    float multiplier = get_size_multiplier(size);

    switch (type)
    {
    case FLOATING:
        return FLOATING_SMALL_RADIUS * multiplier;

    case FIREBALL_MINE:
        return FIREBALL_MINE_SMALL_RADIUS * multiplier;

    case MAGNETIC:
        return MAGNETIC_SMALL_RADIUS * multiplier;

    case MAGNET_FIRE:
        return MAGNET_FIRE_SMALL_RADIUS * multiplier;

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
    enemy.status = NONE;
    enemy.speed = 0.f;
    enemy.type = type;

    switch (type)
    {
    case MINELAYER:
    case FIREBALL:
        enemy.size = FIXED;
        break;

    default:
        enemy.size = size;
        break;
    }

    switch (type)
    {
    case FLOATING:
        enemy.nbPoints = 6;
        break;

    case FIREBALL_MINE:
    case MAGNETIC:
    case MAGNET_FIRE:
        enemy.nbPoints = 8;
        break;

    case FIREBALL:
        enemy.nbPoints = 12;
        break;

    case MINELAYER:
        enemy.nbPoints = 9;
        break;
    
    default:
        enemy.nbPoints = 0;
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