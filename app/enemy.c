#include "enemy.h"

#include <canvas.h>

#define COLOR (255, 255, 255, 255)

void draw_floating_mine(Enemy *enemy)
{
}

void draw_fireball_mine(Enemy *enemy)
{
}

void draw_magnetic_mine(Enemy *enemy)
{
}

void draw_magnet_fire_mine(Enemy *enemy)
{
}

void draw_fireball(Enemy *enemy)
{
}

void draw_minelayer(Enemy *enemy)
{
}

// draw enemy (see Type for ennemy types)
int draw_enemy(Enemy *enemy)
{
    switch (enemy->type)
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
        printf("enemy type error, can't draw\n\n");
        return -1;
        break;
    }
    printf("typeless enemy, can't draw\n\n");
    return -1;
}

Enemy *init_enemy(float x, float y, enemySize size, enemyType type)
{
    Enemy *enemy;
    enemy->isBorn = 0;
    enemy->type = type;
    enemy->location = (Axis2){{x, y}, {1.f, 0.f}, {0.f, 1.f}};
    enemy->size = size;
    enemy->speed = 0.f;
    return enemy;
}