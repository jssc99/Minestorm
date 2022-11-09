#include "enemy.h"
#include "draw_enemy.c"

// draw enemy depending on its type & size (see enemyType & enemySize)
int draw_enemy(Enemy enemy)
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
        draw_fireball((Point2)enemy.location.origin, 10, get_size_multiplicator(MEDIUM), CV_COL32_WHITE);
        return 0;

    case MINELAYER:
        draw_minelayer(enemy);
        return 0;
    }
    return -1;
}

Enemy init_enemy(float x, float y, enemyType type, enemySize size)
{
    Enemy enemy;
    enemy.location = (Axis2){{x, y}, {1.f, 0.f}, {0.f, 1.f}};
    enemy.isBorn = false;
    enemy.speed = 0.f;
    enemy.type = type;
    enemy.size = size;
    return enemy;
}