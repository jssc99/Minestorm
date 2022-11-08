#include "enemy.h"

#include <canvas.h>

#define COLOR (255, 255, 255, 255)

void draw_floating_mine(Enemy enemy)
{
    Axis2 axi = (Axis2){{0, 0}, {0, 0}, {0, 0}};
    Axis2 axe = rotateAxis2(axi, 2.0);
}

void draw_fireball_mine(Enemy enemy)
{
}

void draw_magnetic_mine(Enemy enemy)
{
}

void draw_magnet_fire_mine(Enemy enemy)
{
}

void draw_fireball(Enemy enemy)
{
}

void draw_minelayer(Enemy enemy)
{
}

// draw enemy depending on its type & size (see enemyType & enemySize)
int draw_enemy(Enemy enemy)
{
    switch (enemy.type)
    {
    case FLOATING:
        draw_floating_mine(enemy);
        return 0;
        break;

    case FIREBALL_MINE:
        draw_fireball_mine(enemy);
        return 0;
        break;

    case MAGNETIC:
        draw_magnetic_mine(enemy);
        return 0;
        break;

    case MAGNET_FIRE:
        draw_magnet_fire_mine(enemy);
        return 0;
        break;

    case FIREBALL:
        draw_fireball(enemy);
        return 0;
        break;

    case MINELAYER:
        draw_minelayer(enemy);
        return 0;
        break;

    default:
        return -1;
        break;
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