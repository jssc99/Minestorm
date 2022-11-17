#include "enemy.h"

void move_floating_mine(Enemy enemy)
{
    
}

void move_fireball_mine(Enemy enemy)
{
    
}

void move_magnetic_mine(Enemy enemy)
{
    
}

void move_magnet_fire_mine(Enemy enemy)
{
    
}

void move_fireball(Enemy enemy)
{
    
}

void move_minelayer(Enemy enemy)
{
    
}

// move enemy depending on its type & size (see enemyType & enemySize)
void move_any_enemy(Enemy enemy)
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