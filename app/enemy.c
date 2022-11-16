#include "enemy.h"

float get_size_multiplier(enemySize size)
{
    switch (size)
    {
    case SMALL:
        return 2.f;
    case MEDIUM:
        return 4.f;
    case BIG:
    case FIXED:
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

    case MINELAYER:
        enemy.nbPoints = 9;
        break;

    case FIREBALL:
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

void update_pos_basic_mine(Enemy *e)
{
    Point2 center = e->location.origin;
    float radius_big = get_max_size(e->size, e->type);
    float radius_small = get_small_size(e->size, e->type);

    float angle = e->angle;
    float rotation = M_PI / ((float)e->nbPoints / 2.f);

    for (int i = 0; i < e->nbPoints; i += 2)
    {
        e->points[i] = rotatePoint2(center, (Point2){center.x, center.y + radius_big}, angle);
        angle += rotation;
        (e->points[i + 1]) = rotatePoint2(center, (Point2){center.x, center.y + radius_small}, angle);
        angle += rotation;
    }
}

void update_pos_magnet_fire_mine(Enemy *e)
{
    Point2 center = e->location.origin;
    float sizeTop = get_max_size(e->size, e->type);
    float size = get_small_size(e->size, e->type);

    for (int i = 0; i < 8; i += 2)
    {
        float angle = M_PI * i / 4.f + e->angle;
        e->points[i] = rotatePoint2(center, (Point2){center.x + sizeTop, center.y}, angle);
        e->points[i + 1] = rotatePoint2(center, (Point2){center.x + size, center.y}, angle);
    }
}

void update_pos_minelayer(Enemy *e)
{
    float x = e->location.origin.x;
    float y = e->location.origin.y;

    Point2 point[9] = {
        {x - 26.f, y + 10.f},
        {x - 36.f, y + 14.f},
        {x - 30.f, y},
        {x - 12.f, y},
        {x /*  */, y - 12.f},
        {x + 12.f, y},
        {x + 30.f, y},
        {x + 36.f, y + 14.f},
        {x + 26.f, y + 10.f}};

    for (int i = 0; i < 9; i++)
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
}

void update_pos_any_mine(Enemy *e)
{
    if (e->type == FLOATING || e->type == FIREBALL_MINE || e->type == MAGNETIC)
        update_pos_basic_mine(e);
    if (e->type == MAGNET_FIRE)
        update_pos_magnet_fire_mine(e);
    if (e->type == MINELAYER)
        update_pos_minelayer(e);
}
