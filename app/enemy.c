#include "enemy.h"
#include "draw_enemy.c"
#include "move_enemy.c"

float current_enemy_size(float size, enemyType type)
{
    switch (type)
    {
    case FLOATING:
    case FIREBALL_MINE:
    case MAGNETIC:
    case MAGNET_FIRE:
        return MAX_RADIUS_MINES * size;

    case FIREBALL:
        return size;

    case MINELAYER:
        return 11.7 * size; // 11.7 = distance origin to furthest point from origin of figure
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

    switch (enemy.size)
    {
    case SMALL:
        enemy.kSize = 2.f;
        break;
    case MEDIUM:
        enemy.kSize = 4.f;
        break;
    case BIG:
        enemy.kSize = 6.f;
        break;

    default:
        enemy.kSize = 0.f;
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

// draw enemy depending on its type & size (see enemyType & enemySize)
int draw_any_enemy(Enemy enemy)
{
    switch (enemy.type)
    {
    case FLOATING:
        draw_floating_mine(enemy);
        return 0;

    case FIREBALL_MINE:
        draw_fireball_mine(enemy);
        return 0;

    case MAGNETIC:
        draw_magnetic_mine(enemy);
        return 0;

    case MAGNET_FIRE:
        draw_magnet_fire_mine(enemy);
        return 0;

    case FIREBALL:
        draw_fireball((Point2)enemy.location.origin, 10, 2.f * enemy.kSize, CV_COL32_WHITE);
        return 0;

    case MINELAYER:
        draw_minelayer(enemy);
        return 0;
    }
    return -1;
}

// move enemy depending on its type & size (see enemyType & enemySize)
int move_any_enemy(Enemy enemy)
{
    switch (enemy.type)
    {
    case FLOATING:
        move_floating_mine(enemy);
        return 0;

    case FIREBALL_MINE:
        move_fireball_mine(enemy);
        return 0;

    case MAGNETIC:
        move_magnetic_mine(enemy);
        return 0;

    case MAGNET_FIRE:
        move_magnet_fire_mine(enemy);
        return 0;

    case FIREBALL:
        move_fireball(enemy);
        return 0;

    case MINELAYER:
        move_minelayer(enemy);
        return 0;
    }
    return -1;
}